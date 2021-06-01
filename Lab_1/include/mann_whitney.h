#ifndef MANN_WHITNEY_H
#define MANN_WHITNEY_H
// Compute two-sided Mann–Whitney U test between two unpaired samples.
// Code from this repo: https://github.com/dibbelab/gficf/tree/master/src

#include <cstdlib>
#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <cmath>
#include <gsl/gsl_cdf.h>

template <typename T>
std::vector<size_t> sort_indexes(std::vector<T> const &v);

std::vector<double> getRanks(std::vector<double> const &absoluteValues);

std::vector<double> getUniq(std::vector<double> u);

std::vector<double> getCounts(std::vector<double> const &valuesOrd);

double getSigma(std::vector<double>& u_counts,double n1, double n2);

double getPvalue(double z);

double avg(std::vector<double> const &v);

double MWUtest(std::vector<double> const &v1, std::vector<double> const &v2);

#endif // MANN_WHITNEY_H
