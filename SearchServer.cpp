#include "SearchServer.h"

QMultiMap<float, size_t> SearchServer::search(QString request, const int maxResponses, const QMap<QString, QVector<Entry>> dict)
{
    QMultiMap<float, size_t> answer;

    request = request.toLower();

    request.removeIf([](QChar ch)
                 {
                     if (ch == ',' || ch == '.' || ch == '!' || ch == ';' || ch == ':' || ch == '?' || ch == '(' || ch == ')' || ch == '-')
                         return true;
                     return false;
                 });

    QStringList words = request.split(QRegularExpression("\\s+"), Qt::SkipEmptyParts);

    QMap<QString, int> uniqueList;
    for(const auto& word : words)
    {
        uniqueList[word]++;
    }

    float absFrequency = 0;
    QMap<size_t, size_t> temp;

    for (auto it = uniqueList.constBegin(); it != uniqueList.constEnd(); ++it)
    {
        for (auto& x : dict[it.key()])
        {
            temp[x.doc_id] += x.count;
            if (absFrequency < temp[x.doc_id])
            {
                absFrequency = temp[x.doc_id];
            }
        }
    }

    if (absFrequency > 0)
    {
        int counter = 0;

        for (auto it = temp.constBegin(); it != temp.constEnd() && counter < maxResponses; ++it, ++counter)
        {
            ri.doc_id = it.key();
            ri.rank = it.value() / absFrequency;
            answer.insert(ri.rank, ri.doc_id);
        }
    }

    return answer;
}

