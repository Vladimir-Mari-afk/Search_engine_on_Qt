#include "InvertedIndex.h"
#include "ConverterJSON.h"


struct RelativeIndex {
    size_t doc_id = 0;
    float rank = 0;
};

class SearchServer {
public:
    QMultiMap<float, size_t> search(QString request, const int maxResponses, const QMap<QString, QVector<Entry>> dict);

private:
    InvertedIndex idx;
    RelativeIndex ri;
};
