#include "writingtofile.h"

WritingToFile::WritingToFile() {}

void WritingToFile::writing(QStack<QPair<float, size_t>> stack, QVector<QString> vec, QString request)
{
    QFile history("../SearchEngine2/history.json");

    if (history.open(QIODevice::ReadOnly))
    {
        QByteArray fileData = history.readAll();
        jsDoc = QJsonDocument::fromJson(fileData);
        jsObj = jsDoc.object();
        history.close();
    }

    if (!history.open(QIODevice::WriteOnly))
    {
        qWarning("File with history is not open");
        return;
    }

    if (stack.size() == 0)
    {
        QJsonObject pair;
        pair[QString::number(0)] = 0;
        jsArr.append(pair);
    }
    else
    {
        while(!stack.isEmpty())
        {
            QJsonObject pair;
            auto p = stack.pop();
            pair[QString::number(p.first)] = vec[p.second];
            jsArr.append(pair);
        }
    }

    jsObj[request] = jsArr;
    QJsonDocument jsDoc;
    jsDoc.setObject(jsObj);

    history.write(jsDoc.toJson());

    history.close();
}
