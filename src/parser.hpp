#ifndef PARSER_HPP
#define PARSER_HPP

#include "parseresult.h"
#include "hwparser.h"

std::unique_ptr<ParseResult> parse_source(const char* text, const char* end) {
    HWParser parser(text, end);
    return parser.parse();
}

#endif // PARSER_HPP
