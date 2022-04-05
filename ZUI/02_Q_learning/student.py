import numpy

from blockworld import BlockWorldEnv
import random
import time


class QLearning():
    # don't modify the methods' signatures!
    def __init__(self, env: BlockWorldEnv):
        self.env = env; self.Q = {}
        self.epsilon = 0.1
        self.alpha = 0.2  # learning rate
        self.gamma = 0.9  # discount factor
        self.episodes = 1000

    def mva(self, state):
        """Most-Valuable-Action, returns the action of <state>, which has the max Q."""
        return max(self.Q[state], key=self.Q[state].get)

    def mva_val(self, state):
        """Most-Valuable-Action-Value, returns the max value of <state> actions."""
        return max(self.Q[state].values())

    def put(self, state):
        self.Q[state] = {}
        for act in state[0].get_actions():
            self.Q[state][act] = 0


    def greedy(self, state):
        if numpy.random.rand() > self.epsilon:
            return self.mva(state)
        else:
            return random.choice(state[0].get_actions())

    def act(self, state):
        if state in self.Q:
            return self.mva(state)
        else:
            return random.choice(state[0].get_actions())


    def train(self):

        #print("learning...")

        while True:
        #for i in range(1, self.episodes):  # learning episodes

            #print(f"\nTRAINING {i}")

            old_state = self.env.reset() # reset environment
            if old_state not in self.Q:
                self.put(old_state)

            done = False

            while not done:

                action = self.greedy(old_state)

                new_state, reward, done = self.env.step(action)
                if new_state not in self.Q:
                    self.put(new_state)

                q_max = self.mva_val(new_state)

                # if done:
                #     self.Q[old_state][action] = 0
                # else:
                self.Q[old_state][action] += self.alpha * (reward + self.gamma * q_max - self.Q[old_state][action])

                old_state = new_state


if __name__ == '__main__':
    # Here you can test your algorithm. Stick with N <= 4
    N = 4

    env = BlockWorldEnv(N)
    qlearning = QLearning(env)

    # Train
    start = time.time()
    qlearning.train()
    #print(f"Learning time: {time.time() - start}")
    #exit(1)
    # Evaluate
    test_env = BlockWorldEnv(N)

    test_problems = 10
    solved = 0
    avg_steps = []

    for test_id in range(test_problems):
        s = test_env.reset()
        done = False

        #print(f"\nProblem {test_id}:{s[0]} -> {s[1]}")

        for step in range(50):  # max 50 steps per problem
            a = qlearning.act(s)
            s_, r, done = test_env.step(a)

            #print(f"Step #{step}: {s[0]} -- {a} -> {s_[0]}")

            s = s_

            if done:
                solved += 1
                #print("SOLVED!")
                avg_steps.append(step + 1)
                break

    avg_steps = sum(avg_steps) / len(avg_steps)
    print(f"Solved {solved}/{test_problems} problems, with average number of steps {avg_steps}.")
