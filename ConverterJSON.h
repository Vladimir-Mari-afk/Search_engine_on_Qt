#include <iostream>
#include <algorithm>
#include <exception>
#include <QVector>
#include <QTextStream>
#include <QFile>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QString>

class ConfigFieldIsEmptyException : public std::exception
{
    const char* what() const noexcept override
    {
        return "config file is empty";
    }
};

class СonfigFileIsNotOpenException : public std::exception
{
    const char* what() const noexcept override
    {
        return "config file is missing";
    }
};

class ConverterJSON
{
    QString reqStr, docStr;
    QString data;
    QJsonDocument doc;
    QJsonObject jsonData;

public:

    ConverterJSON() {}
    QVector<QString> getFilePaths();
    int getResponsesLimit();
    ~ConverterJSON() {}
};
