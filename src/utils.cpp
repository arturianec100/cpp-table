#include "utils.h"

QString escapedString(const QString &str) {
    QString res = str;
    for (auto iter = charEscapingMapping.cbegin();
         iter != charEscapingMapping.cend(); ++iter) {
        res = res.replace(iter.key(), iter.value());
    }
    return res;
}
