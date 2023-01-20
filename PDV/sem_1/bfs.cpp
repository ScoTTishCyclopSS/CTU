#include "bfs.h"
#include <queue>
#include <unordered_set>

bool contains(std::unordered_set<unsigned long long int>& visited, unsigned long long int s)
{
    return visited.find(s) != visited.end();
}

std::shared_ptr<const state> bfs(std::shared_ptr<const state> root) {

    std::vector<std::shared_ptr<const state>> frontier;
    std::vector<std::shared_ptr<const state>> next_frontier;
    std::unordered_set<unsigned long long int> visited;
    std::shared_ptr<const state> solution;

    bool solved = false;
    frontier.push_back(root);
    visited.insert(root->get_identifier());

    while (!frontier.empty() && !solved) {
#pragma omp parallel for
        for (int i = 0; i < frontier.size(); i++) { // no auto...
            std::shared_ptr<const state> tmp;
#pragma omp critical
            {
                tmp = frontier[i];
            }
            std::vector<std::shared_ptr<const state>> neighbors = tmp->next_states();
            for (int n = 0; n < neighbors.size(); n++) {
                std::shared_ptr<const state> neighbor = neighbors[n];
                if (!contains(visited, neighbor->get_identifier())) { // not visited yet
#pragma omp critical
                    {
                        visited.insert(neighbor->get_identifier());
                        next_frontier.push_back(neighbor);

                        if (neighbor->is_goal()) {
                            if (!solved) {
                                solution = neighbor;
                                solved = true;
                            } else {
                                solution = neighbor->get_identifier() < solution->get_identifier() ? neighbor : solution;
                            }
                        }
                    }
                }
            }
        }
        frontier = next_frontier;
        next_frontier.clear();
    }
    return solution;
}
