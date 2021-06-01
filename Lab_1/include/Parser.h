#ifndef PARSER_H
#define PARSER_H

#include <cstdlib>
#include <vector>
#include <string>
#include <cassert>

#include <iostream>
#include <fstream>

using Sample = struct Sample_tag {
    size_t size;
    std::vector<double> t;
    std::vector<double> x;
};

class Parser {
private:
    Parser() = delete;
    ~Parser() = delete;
    Parser(Parser const& parser) = delete;
    Parser& operator=(Parser const& parser) = delete;
public:
    static int scanSample(const char* _fileName, Sample &sample);
};

#endif // PARSER_H
