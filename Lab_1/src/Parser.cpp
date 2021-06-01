#include "../include/Parser.h"

// very unsafe
int Parser::scanSample(const char *_fileName, Sample &sample) {
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

    sample.size = N;

    sample.t.clear();
    sample.t.reserve(N);

    sample.x.clear();
    sample.x.reserve(N);

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
        double elem = 0.0;
        char arrEnd = '\0';
        do {
            in >> elem;
            sample.t.push_back(elem);
            in >> arrEnd;
        } while(arrEnd != ']');
    }

    {
        char arrStart = '\0';
        do {
            in >> arrStart;
        } while (arrStart != '[');
    }

    {
        double elem = 0.0;
        char arrEnd = '\0';
        do {
            in >> elem;
            sample.x.push_back(elem);
            in >> arrEnd;
        } while(arrEnd != ']');
    }
    in.close();
    assert(sample.t.size() == N && sample.x.size() == N);
    return 0;
}
