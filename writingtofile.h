#ifndef WRITINGTOFILE_H
#define WRITINGTOFILE_H
#include "secondwindow.h"

class WritingToFile
{
    QJsonObject jsObj;
    QJsonArray jsArr;
    QJsonDocument jsDoc;

public:
    WritingToFile();

    void writing(QStack<QPair<float, size_t>> stack, QVector<QString> vec, QString request);
};

#endif // WRITINGTOFILE_H
