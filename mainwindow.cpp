#include "mainwindow.h"
#include "./ui_mainwindow.h"


SearchEngine::SearchEngine(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::SearchEngine)
{
    ui->setupUi(this);
    this->setWindowTitle("Search engine");
    window = new SecondWindow();
    connect(window, &SecondWindow::backFWindow, this, &QMainWindow::show);
    ui->output->setTextInteractionFlags(Qt::TextBrowserInteraction);
    connect(ui->output, &QLabel::linkActivated, [](const QString &link)
            {
                QDesktopServices::openUrl(QUrl::fromLocalFile(link));
            });
}

SearchEngine::~SearchEngine()
{
    delete ui;
}

void SearchEngine::zeroing()
{
    futures.clear();
    watchers.clear();
    freqDict.clear();
    filePaths.clear();
    tasksCompleted = 0;
    flag = false;
}

void SearchEngine::on_search_clicked()
{
    int maxThreads = QThread::idealThreadCount();
    zeroing();

    try
    {
        filePaths = cJSON.getFilePaths();
    }
    catch (const ConfigFieldIsEmptyException& x)
    {
        qDebug() << "config file is empty";
    }
    catch (const СonfigFileIsNotOpenException& x)
    {
        qDebug() << "config file is missing";
    }
    int totalFiles = filePaths.size();
    size_t count = 0;

    while(totalFiles > 0)
    {
        int threadsToCreate = qMin(maxThreads, totalFiles);
        totalFiles -= threadsToCreate;
        for(int i = 0; i < threadsToCreate; ++i, ++count)
        {
            InvertedIndex *ii = new InvertedIndex(&freqDict, &filePaths, &flag);

            ii->e.doc_id = count;
            QFuture<void> future = (QtConcurrent::run(&InvertedIndex::getWordCount, *ii));
            futures.append(future);
            QFutureWatcher<void> *watcher = new QFutureWatcher<void>(this);
            connect(watcher, &QFutureWatcher<void>::finished, this, &SearchEngine::handleFinish);
            watcher->setFuture(future);
            watchers.append(watcher);
        }
    }
}


void SearchEngine::handleFinish()
{
    tasksCompleted++;
    QString request = ui->searchBar->text();
    QString output = "Search results for \"" + request + "\" found:";
    ui->output->setText(output);

    if (tasksCompleted == filePaths.size() && !flag)
    {
        SearchServer ss;
        QStack<QPair<float, size_t>> stack;

        int responsesLimit = cJSON.getResponsesLimit();

        auto answers = ss.search(request, responsesLimit, freqDict);

        WritingToFile write;
        QString str;

        if (answers.size() != 0)
        {
            for(auto it = answers.cbegin(); it != answers.cend(); ++it)
            {
                stack.push(qMakePair(it.key(), it.value()));
            }

            QStack<QPair<float, size_t>> tempStack = stack;

            while(!tempStack.isEmpty())
            {
                auto p = tempStack.pop();
                QString filePath = filePaths[p.second];
                QString fileName = QFileInfo(filePath).fileName();

                str += QString("<br>rank = %1: <a href=\"%2\">%3</a>").arg(p.first).arg(filePath).arg(fileName);
            }

            ui->output->setText(ui->output->text() + str);           
        }
        else
        {
            ui->output->setText("Results: 0");
        }

        write.writing(stack, filePaths, request);

    }
    else if (!flag)
    {
        ui->output->setText(ui->output->text() + "\nSearch started");
    }
    else
    {
        ui->output->setText("Search dropped");
    }
}

void SearchEngine::on_break_2_clicked()
{
    flag = true;
    for(int i = 0; i < futures.size(); ++i)
    {
        futures[i].cancel();
        futures[i].waitForFinished();
        watchers[i]->waitForFinished();
    }
}

void SearchEngine::on_history_clicked()
{
    hide();

    window->resize(800, 500);
    window->setModal(true);
    window->showHistory();


    window->exec();
}

