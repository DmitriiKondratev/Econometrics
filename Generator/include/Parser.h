#ifndef PARSER_H
#define PARSER_H

#include <cstdlib>
#include <vector>
#include <string>
#include <cassert>

#include <iostream>
#include <fstream>

#include "Generator.h"

class Parser {
private:
    Parser() = delete;
    ~Parser() = delete;
    Parser(Parser const& parser) = delete;
    Parser& operator=(Parser const& parser) = delete;
public:
    static int scanBetas(const char* _fileName, std::vector<double> &betas);
    static int scanDistribution(const char* _fileName, Distribution &distribution);
    static int scanInterval(const char* _fileName, Interval &interval);
};


#endif // PARSER_H
