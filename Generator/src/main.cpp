#include <iostream>
#include "./../include/Generator.h"
#include "./../include/Parser.h"

#define SAMPLE_SIZE 1000
#define FILE_NAME_BETAS "./../resources/betas_0.txt"
#define FILE_NAME_DISTRIB "./../resources/distrib_0.txt"
#define FILE_NAME_INTERVAL "./../resources/interval.txt"
#define FILE_NAME_OUTPUT "./../output.txt"

// Possible arguments:
// betas.txt distribution.txt interval.txt.txt output.txt
int main(int argc, char* argv[]) {
    if (argc > 5) {
        std::cout << "Wrong argument count";
        return 0;
    }

    std::vector<double> betas;
    Distribution distrib;
    Interval interval;

    if (Parser::scanBetas(argc >= 2 ? argv[1] : FILE_NAME_BETAS, betas) ||
        Parser::scanDistribution(argc >= 3 ? argv[2] : FILE_NAME_DISTRIB, distrib) ||
        Parser::scanInterval(argc >= 4 ? argv[3] : FILE_NAME_INTERVAL, interval)
    ) {
        std::cout << "Wrong init data" << std::endl;
        return 0;
    }

    Generator generator(betas, distrib, interval);

    generator.generate(SAMPLE_SIZE);
    generator.writeTo(argc >= 5 ? argv[4] : FILE_NAME_OUTPUT);
    return 0;
}
