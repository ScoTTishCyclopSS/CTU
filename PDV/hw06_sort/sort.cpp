#include "sort.h"
#include <iostream>

using namespace std;

// implementace vaseho radiciho algoritmu. Detalnejsi popis zadani najdete v "sort.h"
void radix_par(std::vector<std::string *> &vector_to_sort, const MappingFunction &mappingFunction,
               unsigned long alphabet_size, unsigned long string_lengths) {


    vector<vector<string *>> buckets(alphabet_size, vector<string *>(0));

    //for (unsigned int i = 0; i < alphabet_size; ++i) { // for every letter in abc
        for (unsigned long n = 0; n < vector_to_sort.size(); ++n) { // for 5 first strings in vector
            string *str_p = vector_to_sort[n];
            //for (unsigned long j = 0; j < string_lengths; ++j) { // for each character
                unsigned long char_map_id = mappingFunction(str_p->at(0)); // find char map
                buckets[char_map_id].push_back(str_p);  // "this letter is represented in: <array of pointers>"
            //}
        }
    //}

        cout << "\n------------------------\n";
        for (int i = 0; i < vector_to_sort.size(); ++i) {
            cout << *vector_to_sort[i] << ' ';
        }

    cout << "\n------------------------\n";

    for (int i = 0; i < buckets.size(); ++i) {
        for (int j = 0; j < buckets[i].size(); ++j) {
            cout << *buckets[i].at(j) << " ";
        }
        cout << '\n';
    }
}