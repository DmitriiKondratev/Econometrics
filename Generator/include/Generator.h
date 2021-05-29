#ifndef GENERATOR_H
#define GENERATOR_H

#include <cstdlib>
#include <vector>
#include <string>
#include <cassert>
#include <cmath>

#include <iostream>
#include <fstream>
#include <iomanip>

#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>

using Interval = struct Interval {
    double min;
    double max;
};

// TODO: think about inner class for distribution
enum Distribution_type {
    NONE,
    NORMAL
};

using Distribution = struct Distribution {
    Distribution_type type;
    double mu;
    double sigma;
};

// TODO: think about hiding the insides of the class
class Generator {
private:
    std::vector<double> betas;
    Distribution distrib{};
    Interval interval{};

    std::vector<double> sample;
    std::vector<double> times;

public:
    Generator(std::vector<double> &_betas, Distribution &_distrib, Interval &_interval);
    ~Generator();

    void setBetas(std::vector<double> &_betas);
    void setDistribution(Distribution &_distrib);
    void setDistribution(Distribution_type _type, double _mu, double _sigma);
    void setInterval(Interval &_interval);
    void setInterval(double min, double max);

    void generate(size_t _n);
    void generate(std::vector<double> &_times);
//    void generateWithTrash(size_t _n, int _percent);
//    void generateWithTrash(std::vector<double> &_times, int _percent);

    int writeTo(const char* _fileName);
};


#endif //GENERATOR_H