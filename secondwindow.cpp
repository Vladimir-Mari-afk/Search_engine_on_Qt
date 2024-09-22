#include "secondwindow.h"
#include "ui_secondwindow.h"
#include "mainwindow.h"


SecondWindow::SecondWindow(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::SecondWindow)
{
    ui->setupUi(this);
}

SecondWindow::~SecondWindow()
{
    delete ui;
}

void SecondWindow::showHistory()
{
    QFile history("../SearchEngine2/history.json");
    QJsonDocument jsDoc;
    QJsonObject jsObj;

    if (history.open(QIODevice::ReadOnly))
    {
        QByteArray fileData = history.readAll();
        jsDoc = QJsonDocument::fromJson(fileData);
        jsObj = jsDoc.object();
        history.close();
    }
    else
    {
        qDebug() << "File is not open";
    }

    QStringList requests = jsObj.keys();

    if (!requests.isEmpty())
    {
        QString request;
        int num = 1;
        for (auto& req : requests)
        {
            // ui->window->setText(ui->window->text() + "Request #" + QString::number(num) + ": " + req + "<br>");
            request += QString("Request #%1: %2<br>").arg(num).arg(req);

            QJsonArray reqData = jsObj[req].toArray();

            for(const QJsonValue &it : reqData)
            {
                QJsonObject item = it.toObject();
                for(QString &i : item.keys())
                {
                    QString filePath = item[i].toString();
                    QString fileName = QFileInfo(filePath).fileName();
                    request += QString("rank = %1: <a href=\"%2\">%3</a><br>").arg(i).arg(filePath).arg(fileName);
                }
            }
            ++num;
        }

        ui->window->setText(request);
        ui->window->setWordWrap(true);
        ui->window->setTextInteractionFlags(Qt::TextBrowserInteraction);
        connect(ui->window, &QLabel::linkActivated, [](const QString &link)
                {
                    QDesktopServices::openUrl(QUrl::fromLocalFile(link));
                });
    }
    else
    {
        qDebug() << "What is wrong";
    }
}

void SecondWindow::on_Search_clicked()
{
    QFile history("../SearchEngine2/history.json");
    QJsonDocument jsDoc;
    QJsonObject jsObj;

    if (history.open(QIODevice::ReadOnly))
    {
        QByteArray fileData = history.readAll();
        jsDoc = QJsonDocument::fromJson(fileData);
        jsObj = jsDoc.object();
        history.close();
    }
    else
    {
        qDebug() << "File is not open";
    }

    QStringList requests = jsObj.keys();

    QString request = ui->SearchBar->text();
    ui->window->setText("Search results for \"" + request + "\" found:");

    QString str;
    for (auto& req : requests)
    {
        if (request == req)
        {
            QJsonArray reqData = jsObj[request].toArray();

            for(const QJsonValue &it : reqData)
            {
                QJsonObject item = it.toObject();
                for(QString &i : item.keys())
                {
                    QString filePath = item[i].toString();
                    QString fileName = QFileInfo(filePath).fileName();
                    str += QString("<br>rank = %1: <a href=\"%2\">%3</a>").arg(i).arg(filePath).arg(fileName);
                }
            }
        }
    }

    ui->window->setText(ui->window->text() + str);
}


void SecondWindow::on_Back_clicked()
{
    hide();

    emit backFWindow();
}

