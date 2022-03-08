//
// Created by karel on 12.2.18.
//

#include <vector>
#include <thread>
#include <mutex>
#include <atomic>
#include "decryption.h"

using namespace std;

void decrypt_sequential(const PDVCrypt &crypt, std::vector<std::pair<std::string, enc_params>> &encrypted, unsigned int) {
    // V sekvencni verzi je pocet vlaken (numThreads) ignorovany
    const auto size = static_cast<unsigned long>(encrypted.size());
    for(unsigned long i = 0 ; i < size ; i++) {
        auto & enc = encrypted[i];
        crypt.decrypt(enc.first, enc.second);
    }
}

void decrypt_openmp(const PDVCrypt &crypt, std::vector<std::pair<std::string, enc_params>> &encrypted, unsigned int numThreads) {
    const long size = static_cast<long>(encrypted.size());

    #pragma omp parallel for num_threads(numThreads)
    for(long i = 0 ; i < size ; i++) {
        auto & enc = encrypted[i];
        crypt.decrypt(enc.first, enc.second);
    }
}

void decrypt_threads_1(const PDVCrypt &crypt, std::vector<std::pair<std::string, enc_params>> &encrypted, unsigned int numThreads) {
    const unsigned long size = static_cast<unsigned long>(encrypted.size());
    unsigned long i = 0;
    
    // process je "vnorena" funkce (lambda funkce) void process(), ktera ma pristup
    // ke vsem promennym
    auto process = [&]() {
        while(i < size) {
            auto & enc = encrypted[i];
            i++;
            crypt.decrypt(enc.first, enc.second);
        }
    };

    vector<thread> threads;
    for(unsigned t = 0; t < numThreads; t++) {
        threads.emplace_back(process); // start threads (places them into vector, can call them anytime)
    }

    for (auto &thread: threads) {
        thread.join();
    }
}

void decrypt_threads_2(const PDVCrypt &crypt, std::vector<std::pair<std::string, enc_params>> &encrypted, unsigned int numThreads) {

    const unsigned long size = static_cast<unsigned long>(encrypted.size());
    unsigned long i = 0;
    mutex m;

    auto process = [&]() {
        while(i < size) {
            m.lock();
            auto & enc = encrypted[i++];
            m.unlock();
            crypt.decrypt(enc.first, enc.second);

        }
    };

    vector<thread> threads;
    for(unsigned t = 0; t < numThreads; t++) {
        threads.emplace_back(process); // start threads (places them into vector, can call them anytime)
    }

    for (auto &thread: threads) {
        thread.join();
    }
}

void decrypt_threads_3(const PDVCrypt &crypt, std::vector<std::pair<std::string, enc_params>> &encrypted, unsigned int numThreads) {
    
    atomic<int> x {0};
    const unsigned long size = static_cast<unsigned long>(encrypted.size());

    auto process = [&]() {
        while(x < size) {
            auto & enc = encrypted[x++];
            crypt.decrypt(enc.first, enc.second);
        }
    };

    vector<thread> threads;
    for(unsigned t = 0; t < numThreads; t++) {
        threads.emplace_back(process); // start threads (places them into vector, can call them anytime)
    }

    for (auto &thread: threads) {
        thread.join();
    }
}

void decrypt_threads_4(const PDVCrypt &crypt, std::vector<std::pair<std::string, enc_params>> &encrypted, unsigned int numThreads) {

    // Doplnte logiku vypoctu disjunktnich rozsahu pro jednotliva vlakna
    // Kod pro spusteni vlaken (a predani rozsahu) je jiz hotovy. Vasim
    // cilem je spocitat hodnoty promennych 'begin' a 'end' pro kazde
    // vlakno.

    const unsigned long size = static_cast<unsigned long>(encrypted.size());
    vector<thread> threads;

    for(unsigned int t = 0 ; t < numThreads ; t++) {

        // Doplnte vypocet rozsahu pro t-te vlakno zde:
        // use CEIL if threads num of cpu % data != 0
        unsigned long begin = (size / numThreads) * t;
        unsigned long end = (size / numThreads) * (t + 1);

        threads.emplace_back([&](unsigned long begin, unsigned long end) {
            for(unsigned int i = begin ; i < end ; i++) {
                auto & enc = encrypted[i];
                crypt.decrypt(enc.first, enc.second);
            }
        }, begin, end);
    }
    for(unsigned int t = 0 ; t < numThreads ; t++) {
        threads[t].join();
    }

}
