#include "../include/Generator.h"

#define SPLITTER ','
#define PRECISION 8
#define DEFAULT_FILE_NAME "./../output.txt"

static double g(std::vector<double> &_betas, double t) {
    if (_betas.empty()) {
        return 0.0;
    }

    double x = 0.0;
    size_t size = _betas.size();
    for (unsigned int i = 0; i < size; ++i) {
        x += _betas[i] * pow(t, i);
    }
    return x;
}

Generator::Generator(std::vector<double> &_betas, Distribution &_distrib, Interval &_interval) {
    betas = _betas;
    distrib = _distrib;
    interval = _interval;
}

Generator::~Generator() = default;

void Generator::setBetas(std::vector<double> &_betas) {
    betas = _betas;
}

void Generator::setDistribution(Distribution &_distrib) {
    distrib = _distrib;
}

void Generator::setDistribution(Distribution_type _type, double _mu, double _sigma) {
    this->distrib.type = _type;
    this->distrib.mu = _mu;
    this->distrib.sigma = _sigma;
}

void Generator::setInterval(Interval &_interval) {
    assert(_interval.min < _interval.max);
    interval = _interval;
}

void Generator::setInterval(double _min, double _max) {
    assert(_min < _max);

    interval.min = _min;
    interval.max = _max;
}

void Generator::generate(size_t _n) {
    times.clear();
    times.reserve(_n);
    sample.clear();
    sample.reserve(_n);

    const gsl_rng_type * T;
    gsl_rng * r;

    gsl_rng_env_setup();

    T = gsl_rng_default;
    r = gsl_rng_alloc(T);

    double random_num = 0.0;
    for (size_t i = 0; i < _n; i++) {
        if (distrib.type == NORMAL) {
            random_num = gsl_ran_gaussian(r, distrib.sigma);
        }
        times.push_back(interval.min + i * (interval.max - interval.min) / _n);
        sample.push_back(g(betas, times[i]) + random_num);
    }

    gsl_rng_free(r);
}

void Generator::generate(std::vector<double> &_times) {
    assert(!_times.empty());

    size_t N = _times.size();
    times.clear();
    times.reserve(N);
    sample.clear();
    sample.reserve(N);

    const gsl_rng_type * T;
    gsl_rng * r;

    gsl_rng_env_setup();

    T = gsl_rng_default;
    r = gsl_rng_alloc(T);

    double random_num = 0.0;
    for (size_t i = 0; i < N; i++) {
        if (distrib.type == NORMAL) {
            random_num = gsl_ran_gaussian(r, distrib.sigma);
        }
        times.push_back(_times[i]);
        sample.push_back(g(betas, _times[i]) + random_num);
    }

    gsl_rng_free(r);
}

// TODO: in developing...
//void Generator::generateWithTrash(size_t _n, int _percent) {
//
//}
//
//void Generator::generateWithTrash(std::vector<size_t> &_times, int _percent) {
//
//}

static void printVector(std::ofstream &out, std::vector<double> &vector, char splitter) {
    out << std::fixed << std::setprecision(PRECISION);
    out << '[';
    if (!vector.empty()) {
        size_t i = 0;
        size_t size = vector.size();

        for (i = 0; i < size - 1; ++i) {
            out << vector[i] << splitter;
        }
        out << vector[i];
    }
    out << ']';
}

int Generator::writeTo(const char* _fileName) {
    std::ofstream out;
    out.open(_fileName ? _fileName : DEFAULT_FILE_NAME);
    if (!out.is_open()) {
        std::cout << "Error of open file" << std::endl;
        return 1;
    }

    out << times.size() << std::endl;

    out << "T = ";
    printVector(out, times, SPLITTER);
    out << std::endl;

    out << "X = ";
    printVector(out, sample, SPLITTER);
    out << std::endl;

    out.close();
    return 0;
}
