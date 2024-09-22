#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "writingtofile.h"
#include <QFutureWatcher>


QT_BEGIN_NAMESPACE
namespace Ui {
class SearchEngine;
}
QT_END_NAMESPACE

class SearchEngine : public QMainWindow
{
    Q_OBJECT

public:
    SearchEngine(QWidget *parent = nullptr);

    ~SearchEngine();

private slots:

    void on_search_clicked();
    void handleFinish();
    void zeroing();
    void on_break_2_clicked();

    void on_history_clicked();

private:
    Ui::SearchEngine *ui;
    SecondWindow *window;
    int tasksCompleted = 0;
    QMap<QString, QVector<Entry>> freqDict;
    QList<QFuture<void>> futures;
    QList<QFutureWatcher<void>*> watchers;
    ConverterJSON cJSON;
    QVector<QString> filePaths;
    bool flag = false;
};
#endif // MAINWINDOW_H
