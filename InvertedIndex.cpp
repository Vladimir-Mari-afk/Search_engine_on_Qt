
#include "InvertedIndex.h"

QMutex mtx;

void InvertedIndex::getWordCount()
{
    QTextStream stream;
    size_t doc_id;
    QFile f;
    QString doc;
    QMap<QString, size_t> counter;

    mtx.lock();

    doc_id = e.doc_id;

    f.setFileName((*filePaths)[e.doc_id]);

    if (!f.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "What is wrong";
    }
    stream.setDevice(&f);

    while (!stream.atEnd())
    {
        size_t count = 0;
        doc = stream.readAll().toLower();
    }

    doc.removeIf([](QChar ch)
                 {
                     if (ch == ',' || ch == '.' || ch == '!' || ch == ';' || ch == ':' || ch == '?' || ch == '(' || ch == ')' || ch == '-')
                         return true;
                     return false;
                 });

    f.close();

    mtx.unlock();

    if (!stopFlag)
    {
        return;
    }

    QStringList words = doc.split(QRegularExpression("\\s+"), Qt::SkipEmptyParts);

    for(const auto& word : words)
    {
        counter[word]++;
    }

    if (!stopFlag)
    {
        return;
    }

    mtx.lock();
    e.doc_id = doc_id;

    for(auto it = counter.constBegin(); it != counter.constEnd(); ++it)
    {
        (*freqDict)[it.key()].push_back({e.doc_id, it.value()});
    }
    mtx.unlock();
}

