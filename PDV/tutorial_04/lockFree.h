#ifndef LOCKFREE_H
#define LOCKFREE_H

#include <vector>
#include <iostream>
#include <atomic>

class Lockfree {
public:
    class Node {
    public:
        unsigned long long value;
        std::atomic<Node*> next { nullptr };

        Node(unsigned long long value) : value(value) {}
    };

    Node * head = new Node(999999999999UL);

    void insert(unsigned long long value) {
        Node * new_node = new Node(value); // данные для вставки
        Node * curr = head;   // нынешняя голова, старт

        while(true) {

            Node * next = curr->next; // следующий в списке после головы

            if (next == nullptr || next->value > value){
                new_node->next = next;
                if (curr->next.compare_exchange_strong(next, new_node)) return;
            }
            else{
                curr = curr->next;
            }

        }
    }

    // Pokud byste si chteli vyzkouset slozitejsi operaci se spojovym seznamem, muzete
    // si zkusit naimplementovat metodu pro odebrani prvku ze seznamu. Vzpomente si,
    // ze na prednasce jsme si rikali, ze mazani prvku probiha dvoufazove.
    //   1) Nejprve prvek oznacim za smazany. Tim zabranim ostatnim vlaknu vkladat za
    //      nej nove prvky.
    //   2) Pote ho vypojim ze seznamu
    //
    // Oznaceni prvku za smazany muzete provest pomoci atomickych operaci tak, ze ukazatel
    // na naslednika oznacite bitovym priznakem (schematicky napr., current->next | 1ULL).
    //
    // Pro jednoduchost nemusite resit dealokaci pameti po odebrani prvku.
    bool remove(unsigned long long value) {
        throw "Not implemented yet";
    }
};

#endif