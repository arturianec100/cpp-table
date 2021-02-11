#ifndef PARSERESULT_H
#define PARSERESULT_H

#include "typedefs.h"

#include <QtCore>

struct ParseResult {
    StringTable table;
    std::string output;
    bool ok = false;
    size_t tableBeginIdx = 0;
    size_t tableEndIdx = 0;
};

#endif // PARSERESULT_H
