# author: @timusfed

import random, time
from collections import defaultdict
from typing import List, Dict, Callable
import numpy as np
from numpy import ndarray
from ox import Board
import ox


def check_action(action, board):
    return action in list(board.get_actions())


def calculate_depth(node: str):
    if node == "":
        return 0
    else:
        return int((len(node) - 1) / 3 + 1)


def random_simulation(state: ox.Board):
    while not state.is_terminal():
        state.apply_action(np.random.choice(list(state.get_actions())))
    return state.get_rewards()


class Bandit:  # from 9. lecture
    def total_visits(self):
        raise NotImplementedError()

    def select(self):
        raise NotImplementedError()

    def update(self, value: float):
        raise NotImplementedError()

    def best_action(self):
        raise NotImplementedError()


class UCB(Bandit):  # from 9. lab
    def __init__(self, state: ox.Board, c_uct):
        self.c_uct = c_uct
        self.actions = list(state.get_actions())  # set doesn't work

        self.last_idx = None

        self.qs = np.zeros(len(self.actions))
        self.visits = np.zeros(len(self.actions))

    def select(self):
        total_visits = self.total_visits()
        if 0 in self.visits:
            ucb = self.qs
        else:
            ucb = (self.qs + self.c_uct * np.sqrt(np.log(total_visits) / self.visits))

        self.last_idx = np.argmax(ucb)
        final_action = self.actions[self.last_idx]

        return final_action

    def update(self, value: float):
        self.visits[self.last_idx] += 1
        self.qs[self.last_idx] += (value - self.qs[self.last_idx]) / self.visits[self.last_idx]

    def best_action(self):
        return self.actions[np.argmax(self.qs)]

    def total_visits(self) -> ndarray:
        return np.sum(self.visits)


class MCTS:
    def __init__(self, bandit_factory: Callable[[ox.Board], Bandit],
                 simulate: Callable[[ox.Board], List[float]]):
        self.node_table: Dict[str, Bandit] = dict()
        self.simulate = simulate
        self.bandit_factory = bandit_factory

    def step(self, state: ox.Board):
        if not self.has_node(state):
            self.make_node(state)

        trace, last_state = self.select(state.clone())

        if last_state.is_terminal():
            values = last_state.get_rewards()
        else:
            self.expand(last_state)
            values = self.simulate(last_state.clone())

        self.back_propagate(trace, values)

    def select(self, state: ox.Board):
        trace = []
        while self.has_node(state):
            trace.append(state.clone())
            if state.is_terminal():
                break
            action = self.get_node(state).select()
            if action in state.get_actions():
                state.apply_action(action)

        last_state = state.clone()
        return trace, last_state

    def node_info(self):
        nodes = sorted(self.node_table.keys())
        node_stats = defaultdict(int)
        for node, next_node in zip(nodes, nodes[1:]):
            node_stats[calculate_depth(node)] += 1
        return node_stats

    def best_action(self, state: ox.Board):
        return self.get_node(state).best_action()

    def expand(self, state: ox.Board) -> Board:
        self.make_node(state)
        return state

    def back_propagate(self, trace, values) -> None:
        for state in trace:
            self.get_node(state).update(values[state.current_player()])

    def has_node(self, state: ox.Board) -> bool:
        return state in self.node_table

    def get_node(self, state: ox.Board) -> Bandit:
        return self.node_table[state]

    def make_node(self, state: ox.Board):
        self.node_table[state] = self.bandit_factory(state)

    def deep_node(self) -> int:
        return max(calculate_depth(node) for node in self.node_table.keys())


class MCTSBot:
    def __init__(self, play_as: int, time_limit: float):
        self.play_as = play_as
        self.time_limit = time_limit * 0.8
        self.mcts = MCTS(lambda state: UCB(state, c_uct=2), random_simulation)

    def play_action(self, board):
        start_time = time.time()
        while (time.time() - start_time) < self.time_limit:
            self.mcts.step(board)

        if check_action(self.mcts.best_action(board), board):
            return self.mcts.best_action(board)
        else:
            return random.choice(list(board.get_actions()))


if __name__ == '__main__':
    board = ox.Board(8)  # 8x8
    bots = [MCTSBot(0, 0.1), MCTSBot(1, 0.1)]

    # try your bot against itself
    while not board.is_terminal():
        current_player = board.current_player()
        current_player_mark = ox.MARKS_AS_CHAR[ox.PLAYER_TO_MARK[current_player]]

        current_bot = bots[current_player]
        a = current_bot.play_action(board)
        board.apply_action(a)

        print(f"{current_player_mark}: {a} -> \n{board}\n")
    print('game!')
