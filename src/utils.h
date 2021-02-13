#ifndef UTILS_H
#define UTILS_H

#include <QtCore>

static inline const QMap<QChar, QString> charEscapingMapping{
    {'\a', "\\a"}, {'\b', "\\b"}, {'\t', "\\t"}, {'\n', "\\n"},
    {'\v', "\\v"}, {'\f', "\\f"}, {'\r', "\\r"}, {'\e', "\\e"},
    {'\"', "\\\""}, {'\'', "\\'"}, {'\?', "\\?"}, {'\0', "\\0"},
    {'\\', "\\\\"}
};

QString escapedString(const QString &str);

#endif // UTILS_H
