#ifndef INVERTEDINDEX_H
#define INVERTEDINDEX_H
#include <QMap>
#include <QMultiMap>
#include <QMutex>
#include <QFuture>
#include <QtConcurrent/QtConcurrent>
#include <QList>


struct Entry {
    size_t doc_id= 0, count = 0;
};

class InvertedIndex
{
public:

    InvertedIndex() {}
    InvertedIndex(QMap<QString, QVector<Entry>>* map, QVector<QString>* i_filePaths, bool* flag) : freqDict(map), filePaths(i_filePaths), stopFlag(flag) {}

    Entry e;

    void getWordCount();
    ~InvertedIndex() {}

private:

    QVector<QString>* filePaths;
    QMap<QString, QVector<Entry>>* freqDict;
    bool* stopFlag;
};

#endif //INVERTEDINDEX_H
