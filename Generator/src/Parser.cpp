#include "../include/Parser.h"

// very unsafe
int Parser::scanBetas(const char* _fileName, std::vector<double> &betas) {
    if (!_fileName) {
        std::cout << "nullptr" << std::endl;
        return 1;
    }

    std::ifstream in;
    in.open(_fileName);
    if (!in.is_open()) {
        std::cout << "Error of open file " << _fileName << std::endl;
        return 1;
    }

    size_t N = 0;
    in >> N;

    betas.clear();
    betas.reserve(N);

    if (N == 0) {
        std::cout << "Empty vectors" << std::endl;
        return 0;
    }

    {
        char arrStart = '\0';
        do {
            in >> arrStart;
        } while (arrStart != '[');
    }

    {
        double beta = 0.0;
        char arrEnd = '\0';
        do {
            in >> beta;
            betas.push_back(beta);
            in >> arrEnd;
        } while(arrEnd != ']');
    }
    in.close();
    assert(betas.size() == N);
    return 0;
}

// very unsafe
int Parser::scanDistribution(const char* _fileName, Distribution &distribution) {
    if (!_fileName) {
        std::cout << "nullptr" << std::endl;
        return 1;
    }

    std::ifstream in;
    in.open(_fileName);
    if (!in.is_open()) {
        std::cout << "Error of open file " << _fileName << std::endl;
        return 1;
    }

    {
        char next = '\0';
        do { // IGNORE Type
            in >> next;
        } while (next != '=');
        distribution.type = Distribution_type::NORMAL;

        do {
            in >> next;
        } while (next != '=');
    }
    in >> distribution.mu;

    {
        char next = '\0';
        do {
            in >> next;
        } while (next != '=');
    }
    in >> distribution.sigma;
    in.close();
    return 0;
}

// very unsafe
int Parser::scanInterval(const char* _fileName, Interval &interval) {
    if (!_fileName) {
        std::cout << "nullptr" << std::endl;
        return 1;
    }

    std::ifstream in;
    in.open(_fileName);
    if (!in.is_open()) {
        std::cout << "Error of open file " << _fileName << std::endl;
        return 1;
    }

    in >> interval.min;
    in >> interval.max;
    in.close();

    assert(interval.min <= interval.max);
    return 0;
}
