#include "ConverterJSON.h"
#include <QtAlgorithms>

QVector<QString>ConverterJSON::getFilePaths()
{
    QVector<QString> filePaths;
    QFile config("../SearchEngine2/config.json");
    config.open(QIODevice::ReadOnly);

    if (!config.isOpen())
    {

        throw СonfigFileIsNotOpenException();
    }

    data = config.readAll();

    config.close();

    doc = QJsonDocument::fromJson(data.toUtf8());
    jsonData = doc.object();
    QJsonValue check = jsonData["config"].toString();
    QJsonArray files = jsonData["files"].toArray();

    if (check.isNull())
    {
        throw ConfigFieldIsEmptyException();
    }

    for (int i = 0; i < files.size(); i++)
    {
        QString path = files[i].toString();

        filePaths.push_back(path);
    }

    return filePaths;
}

int ConverterJSON::getResponsesLimit()
{
    QJsonObject configObj = jsonData["config"].toObject();
    QJsonValue maxResponsesJs = configObj["max_responses"];
    int maxResponses = maxResponsesJs.toInt();


    return maxResponses;
}
