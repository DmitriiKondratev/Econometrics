#include <iostream>

#include "./../include/Parser.h"
#include "./../include/LinearRegression.h"
#include "./../include/mann_whitney.h"

#define MAXIMUM_DEGREE 3
#define CHUNK_NUM_DEFAULT 5
#define SAMPLE_FILE_NAME "./../resources/sample_00.txt"

int main(int argc, char* argv[]) {
    if (argc > 2) {
        std::cout << "Wrong arg count" << std::endl;
        return 0;
    }

    size_t chunk_num = argc == 2 ? std::stoi(argv[1]) : CHUNK_NUM_DEFAULT;

    Sample sample;

    if (Parser::scanSample(SAMPLE_FILE_NAME, sample)) {
        std::cout << "Wrong init data" << std::endl;
        return 0;
    }

    // TODO: handle different chunk sizes
    size_t chunk_size = sample.size / chunk_num;
    LinearRegression lr;

    gsl_vector* t = gsl_vector_alloc(chunk_size);
    gsl_vector* x = gsl_vector_alloc(chunk_size);

    for (size_t i = 0; i < chunk_num; ++i) {
        // TODO: optimize
        for (size_t j = 0; j < chunk_size; ++j) {
            gsl_vector_set(t, j, sample.t.at(i * chunk_size + j));
            gsl_vector_set(x, j, sample.x.at(i * chunk_size + j));
        }

        std::cout << "Chunk #" << i + 1;
        std::cout << " [" << gsl_vector_get(t,0) << ", " << gsl_vector_get(t, t->size - 1) << "]" << std::endl;

//        std::cout << "x: ";
//        printVector(nullptr, x);
//        std::cout << std::endl;

        for (size_t degree = 0; degree <= MAXIMUM_DEGREE; ++degree) {
            gsl_matrix* z = LinearRegression::createVandermondeMatrix(t, degree);

//            std::cout << "z:" << std::endl;
//            printMatrix(nullptr, z);
//            std::cout << std::endl;

            lr.fit(z, x);
            gsl_vector* betas = lr.getBetas();
            gsl_vector* epsilons = lr.getEpsilons();
            double sigma = lr.getSigma();

            std::cout << "\tdegree = " << degree << std::endl;
            std::cout << "\t\tbetas: ";
            printVector(nullptr, betas);
            std::cout << std::endl;
            std::cout << "\t\tsigma: " << sigma << std::endl;

            // split
            std::vector<double> x1(epsilons->size / 2);
            std::vector<double> x2(epsilons->size / 2 + epsilons->size % 2);

            for (size_t i = 0; i < epsilons->size / 2; ++i) {
                x1.push_back(epsilons->data[i]);
                x2.push_back(epsilons->data[epsilons->size / 2 + i]);
            }
            if (epsilons->size % 2) { x2.push_back(epsilons->data[epsilons->size - 1]); }

            double pValue = MWUtest(x1, x2);
            std::cout << "pValue: " << pValue << std::endl;

//            gsl_vector* y = lr.predict(z);
//            std::cout << "y: ";
//            printVector(nullptr, y);
//            std::cout << std::endl;
//            gsl_vector_free(y);

            gsl_vector_free(betas);
            gsl_vector_free(epsilons);
            gsl_matrix_free(z);
        }

        std::cout << std::endl;
    }

    gsl_vector_free(t);
    gsl_vector_free(x);

    return 0;
}
