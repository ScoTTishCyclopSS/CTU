from blockworld import BlockWorld
from queue import PriorityQueue

class BlockWorldHeuristic(BlockWorld):

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
					while (pile_s[c_s] == pile_g[c_g]) and (c_s >= 0) and (c_g >= 0):
						same_blocks += 1
						c_s -= 1
						c_g -= 1
					# ----------------
					score += len(pile_s) - same_blocks
				else:  # there is no point even to count smt
					score += len(pile_s)

		return score


class AStar():

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
		state_from[start] = (None, None)
		actual_cost[start] = 0

		while not q.empty():
			priority, state = q.get()
			# print(f"#{priority} {state}")

			if state == goal:
				return self.reconstruct(start, goal, state_from)

			for action, neighbor in state.get_neighbors():

				cost = actual_cost[state] + state.heuristic(neighbor)
				# print(f"---> {cost}, {neighbor}")

				if neighbor not in actual_cost or (cost < actual_cost[neighbor]):
					actual_cost[neighbor] = cost
					q.put((cost, neighbor))
					state_from[neighbor] = (state, action)

				if neighbor == goal:
					return self.reconstruct(start, goal, state_from)

		return None





if __name__ == '__main__':
	# Here you can test your algorithm. You can try different N values, e.g. 6, 7.
	N = 8

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