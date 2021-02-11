#ifndef OVERLOADS_H
#define OVERLOADS_H

#include "parseresult.h"

#include <QtCore>

#include <string_view>

QTextStream &operator <<(QTextStream &stream, const StringTable &table);
QTextStream &operator <<(QTextStream &stream, const std::string_view &strv);

#endif // OVERLOADS_H
