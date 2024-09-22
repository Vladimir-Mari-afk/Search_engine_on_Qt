#ifndef SECONDWINDOW_H
#define SECONDWINDOW_H

#include "SearchServer.h"
#include <QDialog>
#include <QUrl>
#include <QFileInfo>
#include <QLineEdit>
#include <QStack>
#include <QDesktopServices>
#include <QListWidget>

namespace Ui {
class SecondWindow;
}

class SecondWindow : public QDialog
{
    Q_OBJECT

public:
    explicit SecondWindow(QWidget *parent = nullptr);

    void showHistory();

    ~SecondWindow();

signals:
    void backFWindow();

private slots:
    void on_Search_clicked();

    void on_Back_clicked();

private:
    Ui::SecondWindow *ui;
};

#endif // SECONDWINDOW_H
