#include "integrate.h"

#include <omp.h>
#include <cmath>

double
integrate_sequential(std::function<double(double)> integrand, double a, double step_size, int step_count) {

    // Promenna kumulujici obsahy jednotlivych obdelniku
    double acc = 0.0;

    for (int i = 0; i < step_count; ++i) {
        double c = a + (2*i+1) * step_size/2.0;
        acc += integrand(c) * step_size;
    }
    
    // Celkovy obsah aproximuje hodnotu integralu funkce
    return acc;
}

double
integrate_omp_critical(std::function<double(double)> integrand, double a, double step_size, int step_count) {
    double acc = 0.0;

    //TODO
    // Rozdelte celkovy interval na podintervaly prislusici jednotlivym vlaknum
    // Identifikujte kritickou sekci, kde musi dojit k zajisteni konzistence mezi vlakny

    #pragma omp parallel
    {
        const int chunk = 1 + step_count / omp_get_num_threads();

        const int start = chunk * omp_get_thread_num();
        const int end = std::min(chunk * (omp_get_thread_num() + 1), step_count);

        for (int i = start; i < end; ++i) {
            double c = a + (2*i+1) * step_size/2.0;
            double acc_ = integrand(c) * step_size;

#pragma omp critical
            {
                acc += acc_;
            };
        }
    }

    return acc;
}

double
integrate_omp_atomic(std::function<double(double)> integrand, double a, double step_size, int step_count) {
    double acc = 0.0;

    //TODO
    // Rozdelte celkovy interval na podintervaly prislusici jednotlivym vlaknum
    // Identifikujte kritickou sekci, kde musi dojit k zajisteni konzistence mezi vlakny
#pragma omp parallel
    {
        const int chunk = 1 + step_count / omp_get_num_threads();

        const int start = chunk * omp_get_thread_num();
        const int end = std::min(chunk * (omp_get_thread_num() + 1), step_count);

        for (int i = start; i < end; ++i) {
            double c = a + (2*i+1) * step_size/2.0;
            double acc_ = integrand(c) * step_size;

            #pragma omp atomic // wait until every thread "+="
            acc += acc_;
        }
    }

    return acc;
}

double integrate_omp_reduction(std::function<double(double)> integrand, double a, double step_size, int step_count) {
    double acc = 0.0;

    //TODO
    // Rozdelte celkovy interval na podintervaly prislusici jednotlivym vlaknum
    // Identifikujte kritickou sekci, kde musi dojit k zajisteni konzistence mezi vlakny
#pragma omp parallel reduction(+:acc)
    {
        const int chunk = 1 + step_count / omp_get_num_threads();

        const int start = chunk * omp_get_thread_num();
        const int end = std::min(chunk * (omp_get_thread_num() + 1), step_count);

        for (int i = start; i < end; ++i) {
            double c = a + (2*i+1) * step_size/2.0;
            double acc_ = integrand(c) * step_size;

            acc += acc_;
        }
    }

    return acc;
}

double integrate_omp_for_static(std::function<double(double)> integrand, double a, double step_size, int step_count) {
    // Promenna kumulujici obsahy jednotlivych obdelniku
    double acc = 0.0;

    //TODO
    //rozsirte integrate_omp_reduction o staticke rozvrhovani
#pragma omp parallel for reduction(+:acc) // schedule(static)
        for (int i = 0; i < step_count; ++i) {
            double c = a + (2*i+1) * step_size/2.0;
            acc += integrand(c) * step_size;
        }
    
    // Celkovy obsah aproximuje hodnotu integralu funkce
    return acc;
}

double integrate_omp_for_dynamic(std::function<double(double)> integrand, double a, double step_size, int step_count) {
    // Promenna kumulujici obsahy jednotlivych obdelniku
    double acc = 0.0;

    //TODO
    //rozsirte integrate_omp_reduction o dynamicke rozvrhovani
#pragma omp parallel for schedule(dynamic) reduction(+:acc)
    for (int i = 0; i < step_count; ++i) {
        double c = a + (2*i+1) * step_size/2.0;
        acc += integrand(c) * step_size;
    }

    return acc;
    
    // Celkovy obsah aproximuje hodnotu integralu funkce
    return acc;
}
