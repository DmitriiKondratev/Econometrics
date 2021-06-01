#include "../include/LinearRegression.h"

#define EPSILON 1e-15

void printVector(FILE* stream, gsl_vector* v) {
    assert(v);
    if (!stream) { stream = stdout; }

    if (v->size > 0) {
        fprintf(stream, "%g", v->data[0]);
        for (size_t i = 1; i < v->size; ++i) {
            fprintf(stream, ", %g", v->data[i]);
        }
    } else {
        fprintf(stream, "%s", "empty");
    }
}

void printMatrix(FILE* stream, gsl_matrix* m) {
    assert(m);
    if (!stream) { stream = stdout; }

    if (m->size1 > 0 && m->size2) {
        fprintf(stream, "%g",gsl_matrix_get(m, 0, 0));
        for (size_t j = 1; j < m->size2; ++j) {
            fprintf(stream, ", %g", gsl_matrix_get(m, 0, j));
        }

        for (size_t i = 1; i < m->size1; ++i) {
            fprintf(stream, "%c", '\n');
            fprintf(stream, "%g",gsl_matrix_get(m, i, 0));
            for (size_t j = 1; j < m->size2; ++j) {
                fprintf(stream, ", %g", gsl_matrix_get(m, i, j));
            }
        }
    } else {
        fprintf(stream, "%s", "empty");
    }
}


LinearRegression::~LinearRegression() {
    if (!betas) {
        gsl_vector_free(betas);
        betas = nullptr;
    }

    if (!epsilons) {
        gsl_vector_free(epsilons);
        epsilons = nullptr;
    }
}

void LinearRegression::clear() {
    if (!betas) {
        gsl_vector_free(betas);
        betas = nullptr;
    }
    if (!epsilons) {
        gsl_vector_free(epsilons);
        epsilons = nullptr;
    }
    sigma = 0.0;
}

gsl_vector* LinearRegression::getBetas() {
    return betas ? gsl_vector_alloc_from_vector(betas, 0, betas->size, 1) : nullptr;
}

gsl_vector* LinearRegression::getEpsilons() {
    return epsilons ? gsl_vector_alloc_from_vector(epsilons, 0, epsilons->size, 1) : nullptr;
}

double LinearRegression::getSigma() { return sigma; }

// required square matrix
static gsl_matrix* getPseudoInverseMatrix(const gsl_matrix* A) {
    if (!A) { return nullptr; }

    gsl_matrix* U = gsl_matrix_alloc(A->size1, A->size2);
    gsl_matrix_memcpy(U, A);

    gsl_vector* S = gsl_vector_alloc(A->size1);
    gsl_matrix* V = gsl_matrix_alloc(A->size1, A->size2);
    gsl_vector* workspace = gsl_vector_calloc(A->size1);

    gsl_linalg_SV_decomp(U, V, S, workspace);

//    std::cout << "U:" << std::endl;
//    printMatrix(nullptr, U);
//    std::cout << std::endl;
//    std::cout << "S:" << std::endl;
//    printVector(nullptr, S);
//    std::cout << std::endl;
//    std::cout << "V:" << std::endl;
//    printMatrix(nullptr, V);
//    std::cout << std::endl;

    gsl_vector_free(workspace);

    gsl_matrix* SInverse = gsl_matrix_calloc(S->size, S->size);
    for (size_t i = 0; i < S->size; ++i) {
        gsl_matrix_set(SInverse, i, i, std::abs(S->data[i]) > EPSILON ? 1.0 / S->data[i] : 0.0);
    }

//    std::cout << "SInverse:" << std::endl;
//    printMatrix(nullptr, SInverse);
//    std::cout << std::endl;

    gsl_matrix *VSInverse = gsl_matrix_calloc(V->size1, SInverse->size2);
    gsl_blas_dgemm(CblasNoTrans, CblasNoTrans,
                   1.0, V, SInverse,
                   0.0, VSInverse);

    gsl_matrix* res = gsl_matrix_calloc(VSInverse->size1, U->size1);
    gsl_blas_dgemm(CblasNoTrans, CblasTrans,
                   1.0, VSInverse, U,
                   0.0, res);

    // TODO: handle nullptrs
    gsl_matrix_free(U);
    gsl_vector_free(S);
    gsl_matrix_free(V);
    gsl_matrix_free(SInverse);
    gsl_matrix_free(VSInverse);

    return res;
}

static double g(const gsl_vector* betas, const gsl_vector* x) {
    assert(betas->size == x->size);
    double res = 0.0;

    for (size_t i = 0; i < betas->size; ++i) {
        res += betas->data[i] * x->data[i];
    }
    return res;
}

static double S(const gsl_vector* x, const gsl_vector* betas, const gsl_matrix* z) {
    double res = 0.0;
    gsl_vector* args = gsl_vector_alloc(z->size1);
    if (!args) { return NAN; }

    double diff;
    for (size_t i = 0; i < x->size; ++i) {
        gsl_matrix_get_col(args, z, i);
        diff = x->data[i] - g(betas, args);
        res += diff * diff;
    }
    gsl_vector_free(args);
    return res;
}

static double S(const gsl_vector* epsilons) {
    double res = 0.0;
    for (size_t i = 0; i < epsilons->size; ++i) {
        res += epsilons->data[i] * epsilons->data[i];
    }
    return res;
}

// betas = (Z * Z^T)^{-1} * Z * x
void LinearRegression::fit(const gsl_matrix* z, const gsl_vector* x) {
    if (!z || !x) { return; }

    clear();

    gsl_matrix* A = gsl_matrix_calloc(z->size1, z->size1);
    gsl_blas_dgemm(CblasNoTrans, CblasTrans,
                   1.0, z, z,
                   0.0, A);

//    std::cout << "A:" << std::endl;
//    printMatrix(nullptr, A);
//    std::cout << std::endl;

    if (A->size1 == 1 && A->size2 == 1) {
        double AInverse = 1 / A->data[0];

        gsl_matrix* AInverseZ = gsl_matrix_alloc(z->size1, z->size2);
        gsl_matrix_memcpy(AInverseZ, z);

        gsl_matrix_scale(AInverseZ, AInverse);

        betas = gsl_vector_calloc(AInverseZ->size1);
        gsl_blas_dgemv(CblasNoTrans, 1.0, AInverseZ, x, 0.0, betas);
        sigma = std::sqrt(S(x, betas, z) / (x->size - betas->size));

        gsl_matrix_free(AInverseZ);
    } else {
        gsl_matrix *APseudoInverse = getPseudoInverseMatrix(A);

//        std::cout << "APseudoInverse:" << std::endl;
//        printMatrix(nullptr, APseudoInverse);
//        std::cout << std::endl;

        gsl_matrix *APseudoInverseZ = gsl_matrix_calloc(APseudoInverse->size1, z->size2);
        gsl_blas_dgemm(CblasNoTrans, CblasNoTrans,
                       1.0, APseudoInverse, z,
                       0.0, APseudoInverseZ);

//        std::cout << "APseudoInverseZ:" << std::endl;
//        printMatrix(nullptr, APseudoInverseZ);
//        std::cout << std::endl;

        betas = gsl_vector_calloc(APseudoInverseZ->size1);
        gsl_blas_dgemv(CblasNoTrans, 1.0, APseudoInverseZ, x, 0.0, betas);

        epsilons = gsl_vector_alloc(x->size);
        gsl_vector* args = gsl_vector_alloc(z->size1);
        for (size_t i = 0; i < x->size; ++i) {
            gsl_matrix_get_col(args, z, i);
            gsl_vector_set(epsilons, i, x->data[i] - g(betas, args));
        }
        gsl_vector_free(args);

        sigma = S(epsilons);

        gsl_matrix_free(APseudoInverse);
        gsl_matrix_free(APseudoInverseZ);
    }

    // TODO: handle nullptrs
    gsl_matrix_free(A);
}

gsl_vector* LinearRegression::predict(const gsl_matrix* z) {
    if (!betas || !z) { return nullptr; }
    if (betas->size != z->size1) { return nullptr; }

    gsl_vector* res = gsl_vector_calloc(z->size2);
    gsl_blas_dgemv(CblasTrans, 1.0, z, betas, 0.0, res);

    return res;
}

/*
 * m = scale
 * t = (a, b, ..., w)
 *
 * return:
 *         ( 1   1  :  1 )
 *         ( a   b  :  w )
 *         (... ... : ...)
 *         (a^m b^m : w^m)
 */
gsl_matrix* LinearRegression::createVandermondeMatrix(const gsl_vector* t, size_t degree) {
    if (!t) { return nullptr; }

    gsl_matrix* m = gsl_matrix_alloc(degree + 1, t->size);

    if (m) {
        for (size_t column = 0; column < t->size; ++column) {
            double elem = 1.0;
            gsl_matrix_set(m, 0, column, elem);
            for (size_t row = 0; row < degree; ++row) {
                elem *= gsl_vector_get(t, column);
                gsl_matrix_set(m, row + 1, column, elem);
            }
        }
    }
    return m;
}
