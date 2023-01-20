#include "iddfs.h"
#include <unordered_set>
#include <omp.h>

void dls(const std::shared_ptr<const state>& node, int depth);

std::shared_ptr<const state> solution = nullptr;
bool solved = false;

std::shared_ptr<const state> iddfs(std::shared_ptr<const state> root) {
    int depth = 1;
    while(!solved)
    {
        std::shared_ptr<const state> afterDls;
        #pragma omp parallel num_threads(omp_get_num_procs())
        {
            #pragma omp single
            {
                dls(root, depth++);
            }
        }
        #pragma omp taskwait
    }
    return solution;
}

void dls(const std::shared_ptr<const state>& node, int depth) {
    if (node->is_goal()) {
#pragma omp critical
        {
            if (!solved) {
                solution = node;
                solved = true;
            } else {
                solution = node->get_identifier() < solution->get_identifier() ? node : solution;
            }
        }
        return;
    }

    if (solution != nullptr) {
        if (node->current_cost() > solution->current_cost()) {
            return;
        }
    }

    if (depth > 0)
    {
        for (unsigned long long i = 0; i < node->next_states().size(); ++i)
        {
#pragma omp task // MUST BE HERe, not after checking...
            {
                if (node->get_predecessor() == nullptr || (
                        node->next_states()[i]->get_identifier() != node->get_predecessor()->get_identifier())
                        ) {
                    dls(node->next_states()[i], depth - 1);
                }
            }
        }
    }
}