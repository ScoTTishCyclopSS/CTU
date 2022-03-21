import numpy

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

		for pile_s in self_state:
			for pile_g in goal_state:
				c_s = len(pile_s) - 1  # local counters
				c_g = len(pile_g) - 1  # first block = last element in the array

				if pile_s[c_s] == pile_g[c_g]:  # if both piles start with the same block -> start count
					blocks_in_pile = 0
					while (pile_s[c_s] == pile_g[c_g]) and (c_s >= 0) and (c_g >= 0):
						blocks_in_pile += 1
						c_s -= 1
						c_g -= 1
					score += len(pile_s) - blocks_in_pile
				else:
					score += len(pile_s)

		return score


class AStar():
	def search(self, start, goal):

		q = PriorityQueue()
		q.put((0, start, []))  # put first state into the

		while not q.empty():
			priority, state, actions = q.get()
			print("\ncurr: ", priority, state)
			if state == goal:  # the path has been found
				return actions
			for action, neighbor in state.get_neighbors():  # else -> put into q with calculated heuristic
				actions_clone = list.copy(actions)
				actions_clone.append(action)
				print("   -> ", (neighbor.heuristic(goal), neighbor))
				q.put((state.heuristic(neighbor), neighbor, actions_clone))
		return None

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