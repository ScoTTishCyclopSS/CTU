#ifndef PRODUCENTCONSUMER_THREADPOOL_H
#define PRODUCENTCONSUMER_THREADPOOL_H

#include <list>
#include <thread>
#include <vector>
#include <iostream>
#include <condition_variable>

template<typename job_t, typename worker_t>
class ThreadPool {
private:
    // Fronta uloh
    std::list<job_t> queue;

    // Vlakna konzumentu zpracovavajicich ulohy
    std::vector<std::thread> threads;

    // Funkce, kterou maji konzumenti vykonavat
    const worker_t & worker;

    // Mutex
    std::mutex q_m;

    // Conditional variable
    std::condition_variable cv;

public:
    ThreadPool(const unsigned int num_threads, const worker_t & worker);
    void process(const job_t job);
    void join();
};


template<typename job_t, typename worker_t>
ThreadPool<job_t, worker_t>::ThreadPool(const unsigned int num_threads, const worker_t &worker) : worker(worker) {
    // Zde vytvorte "num_threads" vlaken konzumentu:
    //   - Po spusteni bude vlakno kontrolovat, zda je ve fronte uloh "queue" nejaka
    //     uloha ke zpracovani, tj., fronta neni prazdna - !queue.empty()
    //   - Ve chvili, kdy je ve fronte uloha ke zpracovani, tak ulohu z fronty vyjme
    //     a zavola worker(task), kde task je uloha ke zpracovani (priklad pouziti
    //     naleznete nize).
    //   - Vlakno se ukonci pokud uloha ke zpracovani je 0
    //   - Vytvorena vlakna vlozte do pole "threads

    auto work_available = [&]() {
        job_t task;
        do {
            std::unique_lock<std::mutex>lock(q_m); // lock
            cv.wait(lock, [&]{return !queue.empty();}); // release the mutex automatically

            // oh, yea, it locks again...
            task = queue.front();
            queue.pop_front();
            lock.unlock(); // unlock after

            if (task != 0) {
                worker(task);
            }
        } while(task != 0);
        cv.notify_all();
    };

    for (unsigned i = 0; i < num_threads; ++i) {
        threads.emplace_back(work_available);
    }

    // Nezapomente, ze v kriticke sekci chceme travit co nejmene casu (tj., uvolnovat zamek
    // ihned pote, co uz ho prestaneme potrebovat). Napriklad si dejte pozor na to, abyste
    // zamek uvolnili pred tim, nez spustite vypocet ulohy pomoci worker(task). Pokud by
    // totiz vypocet ulohy byl soucasti kriticke sekce, mohla by se v danou chvili pocitat
    // pouze jedna uloha - a prisli bychom o vyhody paralelizace.
}

template<typename job_t, typename worker_t>
void ThreadPool<job_t, worker_t>::process(const job_t job) {
    q_m.lock();
    queue.push_back(job);
    q_m.unlock();
    cv.notify_all();

    // Pokud timto zpusobem bude ale vice vlaken bude pristupovat ke fronte soucasne,
    // vysledek neni determinsticky. Mohlo by se nam napriklad stat, ze nam jine
    // vlakno nasi pridanou ulohu "prepise" a ta se tak ztrati.
    // Zabezpecte, ze k tomuto nemuze dojit.
}

// Tato metoda nam umozni volajici funkci v main.cpp pockat na vsechna spustena vlakna konzumentu
template<typename job_t, typename worker_t>
void ThreadPool<job_t, worker_t>::join() {
    for(unsigned int i = 0 ; i < threads.size() ; i++) threads[i].join();
}

#endif //PRODUCENTCONSUMER_THREADPOOL_H
