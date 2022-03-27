from blockworld import BlockWorld
from queue import PriorityQueue

class BlockWorldHeuristic_f(BlockWorld):

    def __init__(self, num_blocks=5, state=None):
        BlockWorld.__init__(self, num_blocks, state)

    def heuristic(self, goal):
        self_state = self.get_state()
        goal_state = goal.get_state()

        # return len(self_state - goal_state), too simple...

        score = 0

        if self_state == goal_state:
            return score

        for pile_s in self_state:
            for pile_g in goal_state:
                c_s = len(pile_s) - 1  # local counters
                c_g = len(pile_g) - 1  # first block = last element in the array

                if pile_s[c_s] == pile_g[c_g]:  # if both piles start with the same block -> start count
                    # ----------------
                    same_blocks = 0
                    while (c_s >= 0) and (c_g >= 0):
                        if pile_s[c_s] == pile_g[c_g]:
                            same_blocks += 1
                        c_s -= 1
                        c_g -= 1
                    # ----------------
                    score += len(pile_s) - same_blocks
                else:  # there is no point even to count smt
                    score += len(pile_s)

        return score


class AStar_f():

    def reconstruct(self, start, goal, state_from):
        steps = []
        curr = goal

        while not curr == start:
            state_before, action = state_from[curr]
            steps.append(action)
            curr = state_before

        list.reverse(steps)

        return steps

    def search(self, start, goal):

        state_from = {}
        actual_cost = {}
        q = PriorityQueue()

        # define start state: into the q & dict
        q.put((0, start))
        state_from[start] = None, None
        actual_cost[start] = 0

        while not q.empty():
            priority, state = q.get()
            # print(f"#{priority} {state}")

            if state == goal:
                return self.reconstruct(start, goal, state_from)

            for action, neighbor in state.get_neighbors():

                cost = actual_cost[state] + 1
                # print(f"---> {cost}, {neighbor}")

                if neighbor not in actual_cost or cost < actual_cost[neighbor]:
                    priority = cost + neighbor.heuristic(goal)
                    actual_cost[neighbor] = cost + neighbor.heuristic(goal)
                    q.put((priority, neighbor))
                    state_from[neighbor] = state, action

                if neighbor == goal:
                    return self.reconstruct(start, goal, state_from)

        return self.reconstruct(start, goal, state_from)


class BlockWorldHeuristic(BlockWorld):
    def __init__(self, num_blocks=5, state=None):
        BlockWorld.__init__(self, num_blocks, state)

    def heuristic(self, goal):
        self_state = self.get_state()
        goal_state = goal.get_state()

        # ToDo. Implement the heuristic here.
        price = 0

        for i in goal_state:
            for j in self_state:
                if i == j:
                    price += len(i)
                    break

                if len(i) <= len(j):
                    diff = len(j) - len(i)
                    start_idx = len(j) - 1
                else:
                    diff = len(i) - len(j)
                    start_idx = len(i) - 1

                for a in range(start_idx, diff - 1, -1):
                    if len(i) <= len(j):
                        if j[a] != i[a - diff]:
                            break
                        else:
                            price += 1
                    else:
                        if i[a] != j[a - diff]:
                            break
                        else:
                            price += 1

        return price


class AStar():
    def search(self, start, goal):
        curr = start.clone()
        queue = PriorityQueue()
        dict = {}
        result = []

        if curr == goal:
            return result

        for action, neighbor in curr.get_neighbors():
            if neighbor not in dict:
                dict[neighbor] = 0
            else:
                dict[neighbor] += 1

            curr_price = neighbor.heuristic(goal)
            queue.put((10 - curr_price + dict[neighbor], neighbor, action, curr))

        while not queue.empty():
            priority, state, act, parent = queue.get()
            print(f"\n-> {priority} {state}")
            result.append(act)

            if state == goal:
                break

            for action, neighbor in state.get_neighbors():
                if neighbor not in dict:
                    dict[neighbor] = 0
                else:
                    dict[neighbor] += 1

                cost = 10 - (neighbor.heuristic(goal) + dict[neighbor])
                queue.put((cost, neighbor, action, state))
                print(f"    -< {cost} {neighbor} {action}")

        return result



if __name__ == '__main__':
    # Here you can test your algorithm. You can try different N values, e.g. 6, 7.
    N = 3

    start = BlockWorldHeuristic(N)
    goal = BlockWorldHeuristic(N)

    print("Searching for a path:")
    print(f"{start} -> {goal}")
    print()

    astar = AStar()
    path = astar.search(start, goal)

    if path is not None:
        print("Found a path:")
        print(path)

        print("\nHere's how it goes:")

        s = start.clone()
        print(s)

        for a in path:
            s.apply(a)
            print(s)

    else:
        print("No path exists.")

    print("Total expanded nodes:", BlockWorld.expanded)