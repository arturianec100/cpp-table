#include "overloads.h"

QTextStream &operator <<(QTextStream &stream, const StringTable &table)
{
    bool rowComma = false;
    stream << "{\n";
    for (auto &row : table) {
        if (rowComma) {
            stream << ",\n";
        }
        rowComma = true;
        bool cellComma = false;
        stream << "  {\n";
        for (auto &cell : row) {
            if (cellComma) {
                stream << ",\n";
            }
            cellComma = true;
            stream << "    \"" << cell << "\"";
        }
        stream << "\n  }";
    }
    stream << "};\n";

    return stream;
}

QTextStream &operator <<(QTextStream &stream, const std::string_view &strv)
{
    for (auto ch : strv) {
        stream << ch;
    }
}
