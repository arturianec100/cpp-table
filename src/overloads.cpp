
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
}
