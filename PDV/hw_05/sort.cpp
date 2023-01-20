#include "sort.h"
#include <iostream>

#define TEST
#undef TEST

using namespace std;

void radix_par(std::vector<std::string *> &vector_to_sort, const MappingFunction &mappingFunction,
               unsigned long alphabet_size, unsigned long string_lengths) {
    radix_par_std(vector_to_sort, mappingFunction, alphabet_size, string_lengths, string_lengths); // msd
}

void radix_par_std(std::vector<std::string *> &vector_to_sort, const MappingFunction &mappingFunction,
                   unsigned long alphabet_size, unsigned long string_lengths, unsigned long r) {

    if (r < 1 || vector_to_sort.empty()) return;

    vector<string *> pockets[alphabet_size];

    for (unsigned long i = 0; i < vector_to_sort.size(); ++i) {
        string *str = vector_to_sort[i];
        unsigned long map_id = mappingFunction(str->at(string_lengths - r));
        pockets[map_id].push_back(str);
    }

#pragma omp parallel for
    // traverse (don't like it)
    for (unsigned long i = 0; i < alphabet_size; i++)
        radix_par_std(pockets[i], mappingFunction, alphabet_size, string_lengths, r - 1);

#ifdef TEST

    // ------ PRINT START ARR ------
    cout << "start: \n";
    for (auto &i: vector_to_sort) cout << *i << ' ';
    cout << '\n';

    // ------ PRINT CURR POCKETS -----
    cout << "\npockets:\n";
    for (auto &pocket: pockets) {
        if (pocket.empty()) continue;
        for (auto &j: pocket) cout << *j << ' ';
        cout << '\n';
    }

#endif

    // reconstruct [take from pockets -> put into array]
    unsigned long k = 0;
    for (unsigned long i = 0; i < alphabet_size; ++i) { // for every pocket...
        for (unsigned long j = 0; j < pockets[i].size(); ++j) { // for each element in pocket...
            vector_to_sort[k++] = pockets[i][j]; // update array as sorted by putting items from pocket
        }
    }

#ifdef TEST
    // ------ PRINT FIN ARR ------
    cout << "\nsorted: \n";
    for (auto &i: vector_to_sort) cout << *i << ' ';
    cout << '\n';
    cout << "-------------\n";
#endif
}