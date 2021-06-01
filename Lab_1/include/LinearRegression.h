#ifndef LINEAR_REGRESSION_H
#define LINEAR_REGRESSION_H

#include <cstdlib>
#include <cassert>
#include <algorithm>

#include <iostream>
#include <fstream>

#include <gsl/gsl_vector.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_linalg.h>

class LinearRegression {
private:
    gsl_vector* betas = nullptr;
    gsl_vector* epsilons = nullptr;
    double sigma = 0.0;

public:
    LinearRegression() = default;
    ~LinearRegression();

    void clear();

    gsl_vector* getBetas();
    gsl_vector* getEpsilons();
    double getSigma();

    void fit(const gsl_matrix* z, const gsl_vector* x);
    gsl_vector* predict(const gsl_matrix* z);

    /*
     * ( 1   1  :  1 )
     * ( a   b  :  w )
     * (... ... : ...)
     * (a^m b^m : w^m)
     */
    static gsl_matrix* createVandermondeMatrix(const gsl_vector* t, size_t degree=0);
};

void printVector(FILE* stream, gsl_vector* v);
void printMatrix(FILE* stream, gsl_matrix* m);


#endif // LINEAR_REGRESSION_H
