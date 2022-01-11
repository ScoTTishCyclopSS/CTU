import random


class MyPlayer(object):
    """
    The player returns random move, unless exists the corner move.
    """
    def __init__(self, my_color, opponent_color):
        self.name = 'timusfed'
        self.myColor = my_color
        self.opponentColor = opponent_color

    def move(self, board):
        """
        Make a move directly.
        :param board: for the playing field
        """
        # Define moves variable as a list of tuples, where are all of the possible moves
        moves = []
        for x in range(len(board)):
            for y in range(len(board)):
                # takes move one by one form the board and pass it into the validation function
                move = (x, y)
                if (board[x][y] == -1) and self.is_valid_move(move, board):
                    # if the move check is successful, then it is added to the possible moves
                    moves.append(move)
        if len(moves) <= 0:
            # if there is no possible move, then skip
            return None
        else:
            for move in moves:
                # takes move one by one form the possible moves, if it is on the corner - returns it
                if self.is_on_corner(move, board):
                    return move
                else:
                    # if it's not - returns randomly (unfortunately)
                    return random.choice(moves)

    def is_valid_move(self, move, board):
        """
        Checks the validation of the move.
        :param move: move, which should be checked
        :param board: for the playing field
        :return: is move valid or not
        """
        # Define direction variable as a list of tuples (all possible directions)
        direction = [
            (-1, -1),
            (-1, 0),
            (-1, 1),
            (0, -1),
            (0, 1),
            (1, -1),
            (1, 0),
            (1, 1)
        ]
        return any([self.check_direction(move, board, d) for d in direction])

    def check_direction(self, move, board, direct):
        """
        Checks all of the directions for the validation.
        :param move: move, on which will be applied the direction
        :param board: for the playing field
        :param direct: for the direction, which will be checked
        :return: is move possible or not
        """
        # define x, y variables as potential coordinates of the move
        x = move[0] + direct[0]
        y = move[1] + direct[1]
        if self.is_on_board(board, x, y) and board[x][y] == self.opponentColor:
            while self.is_on_board(board, x, y):
                if board[x][y] == -1:
                    return False
                if board[x][y] == self.myColor:
                    return True
                x += direct[0]
                y += direct[1]
        return False

    def is_on_board(self, board, x, y):
        """
        Checks if the move is on or off the field.
        :param board: for the playing field
        :param x: for the row of the field
        :param y: for the column of the field
        :return: is move on the field or not
        """
        return (len(board) > x >= 0) and (len(board) > y >= 0)

    def is_on_corner(self, move, board):
        """
        Checks if the move is on corner.
        :param move: move, which should be checked
        :param board: for the playing field
        :return: is move on the corner or not
        """
        len_board = len(board) - 1
        r = move[0]
        c = move[1]
        return (r == 0 or r == len_board) and (c == 0 or c == len_board)
