#ifndef DATABASEQUERIES_QUERY_H
#define DATABASEQUERIES_QUERY_H

#include <vector>
#include <functional>
#include <atomic>

template<typename row_t>
using predicate_t = std::function<bool(const row_t &)>;



template<typename row_t>
bool is_satisfied_for_all(std::vector<predicate_t<row_t>> predicates, std::vector<row_t> data_table);

template<typename row_t>
bool is_satisfied_for_any(std::vector<predicate_t<row_t>> predicates, std::vector<row_t> data_table);



template<typename row_t>
bool is_satisfied_for_all(std::vector<predicate_t<row_t>> predicates, std::vector<row_t> data_table) {
    int row_count = data_table.size();
    int pred_count = predicates.size();

    bool result = true; // all are satisfied

    #pragma omp parallel for shared(result)
    for (int i = 0; i < pred_count; ++i) {  // for every predicate...
        auto & predicate = predicates[i];
        bool  predicate_sat = false;

        for (unsigned int j = 0; j < row_count; ++j) {
            auto & row = data_table[j];

            if (predicate(row)) { // ...check if it's truly satisfied
                predicate_sat = true;
                break;
            }
        }

        if (!predicate_sat) { // so if it still false -> exists at least one record w\ satisfaction
            result = false;
            #pragma omp cancel for
        }
    }

    return result;
}

template<typename row_t>
bool is_satisfied_for_any(std::vector<predicate_t<row_t>> predicates, std::vector<row_t> data_table) {
    // Doimplementujte telo funkce, ktera rozhodne, zda je ALESPON JEDEN dilci dotaz pravdivy.
    // To znamena, ze mate zjistit, zda existuje alespon jeden predikat 'p' a jeden zaznam
    // v tabulce 'r' takovy, ze p(r) vraci true.

    // Zamyslete se nad tim, pro ktery druh dotazu je vhodny jaky druh paralelizace. Vas
    // kod optimalizujte na situaci, kdy si myslite, ze navratova hodnota funkce bude true.
    // Je pro Vas dulezitejsi rychle najit splnujici radek pro jeden vybrany predikat, nebo
    // je dulezitejsi zkouset najit takovy radek pro vice predikatu paralelne?

    int row_count = data_table.size();
    int pred_count = predicates.size();

    bool result = false; // all aren't satisfied

#pragma omp parallel for shared(result)
    for (int i = 0; i < row_count; ++i) {  // for every predicate...
        // the OPPOSITE condition, might be faster?
        auto & row = data_table[i];
        for (unsigned int j = 0; j < pred_count; ++j) {
            auto & predicate = predicates[j];
            if (predicate(row)) { // ...check if satisfied at least once
                result = true;
                break;
            }
        }

        if (result) {
#pragma omp cancel for
        }
    }
    return result;
}


#endif //DATABASEQUERIES_QUERY_H
