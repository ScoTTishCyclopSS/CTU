from blockworld import BlockWorldEnv
import random


class QLearning():
    # don't modify the methods' signatures!
    def __init__(self, env: BlockWorldEnv):
        self.env = env
        self.reward_dict = {}  # state, valid action
        self.visited = []

    def estimate(self, goal, val):
        #print("   ESTIMATING: ", goal_)
        for action in goal.get_actions():
            new_state = goal.clone()
            new_state.apply(action)
            if new_state not in self.reward_dict:
                #print(f"      {action} -> {new_state}")
                self.reward_dict[new_state] = val
                self.estimate(new_state, val - 0.1)

    def train(self):
        state, goal = self.env.reset()  # берем рандомный (на нем env) и финальный стейт
        print(f"income: s_ = {state}, g_ = {goal}")

        self.reward_dict[goal] = 10.0

        self.estimate(goal, 9.9)


        d_view = [(v, k) for k, v in self.reward_dict.items()]
        d_view.sort(reverse=True)  # natively sort tuples by first element
        for v, k in d_view: print(v, k)

    pass


    def act(self, s):
        max = self.reward_dict[s[0]]
        fin_action = s[0].get_actions()[0]
        fin_state = s[0]

        for action in s[0].get_actions():
            new_state = s[0].clone()
            new_state.apply(action)
            print(f"   > {new_state} : {self.reward_dict[new_state]} : {new_state in self.visited}")
            if new_state not in self.visited and self.reward_dict[new_state] > max:
                max = self.reward_dict[new_state]
                fin_action = action
                fin_state = new_state

        self.visited.append(fin_state)
        #return random.choice( s[0].get_actions() )
        return fin_action


if __name__ == '__main__':
    # Here you can test your algorithm. Stick with N <= 4
    N = 3

    env = BlockWorldEnv(N)
    qlearning = QLearning(env)

    # Train
    qlearning.train()

    # Evaluate
    test_env = BlockWorldEnv(N)

    test_problems = 10
    solved = 0
    avg_steps = []

    for test_id in range(test_problems):
        s = test_env.reset()
        done = False

        print(f"\nProblem {test_id}:{s[0]} -> {s[1]}")

        for step in range(50):  # max 50 steps per problem
            a = qlearning.act(s)
            s_, r, done = test_env.step(a)

            print(f"Step #{step}: {s[0]} -- {a} -> {s_[0]}")

            s = s_

            if done:
                solved += 1
                print("SOLVED!")
                avg_steps.append(step + 1)
                break

    avg_steps = sum(avg_steps) / len(avg_steps)
    print(f"Solved {solved}/{test_problems} problems, with average number of steps {avg_steps}.")
