from random import randint

class MyPlayer(object):
    """
    Hrac hraje random.
    """
    def __init__(self, my_color, opponent_color):
        self.name = 'random_player'
        self.my_color = my_color
        self.opponentColor = opponent_color

    def move(self, board):
        moves = [] #where we will put our available moves
        for x in range(len(board)):
            for y in range(len(board)):
                #check if the current move is available and isn't out of the board range...
                if (board[x][y] == -1) and self.valid_move([x, y], board):
                    moves.append((x, y)) #...then add it to the list
        if (len(moves) - 1) < 0: #if we don't have any available moves then that sucks, what else can I say... (((
            return None #...also return None [skip]
        return moves[randint(0, (len(moves) - 1))] #if nevertheless we have a choice - then the player will randomly choose move from the list

    def valid_move(self, move, board):
        """
        lists of (-1, 0, 1) represent all the possible combinations of the directions
        for example: [-1, -1] - to the left upper corner, [1, 1] - to the right bottom corner and etc.
        """
        x_direct = [-1, -1, -1, 0, 0, 1, 1, 1]
        y_direct = [-1, 0, 1, -1, 1, -1, 0, 1]
        #for all of the direction we check where we can move on (procide to the op_check func)
        for i in range(len(x_direct)):
            if self.op_check(move, board, x_direct[i], y_direct[i]):
                return True
        return False

    def op_check(self, move, board, x, y): #don't FORGET: move is represented as [x, y]
        """
        making local variables for checking our move
        we should "scan" all of the directions a return possible variant
        """
        x_coord = move[0] + x #adding "direction" from above to our move (for both coordinates)
        y_coord = move[1] + y
        #the main check before the cycle: our new coordinate must be in board area
        if (len(board) > x_coord >= 0) and (len(board) > y_coord >= 0):
            #the second one: we can start the cycle if the coordinate which we would check is occupied...
            if board[x_coord][y_coord] == self.opponentColor:
                while (len(board) > x_coord >= 0) and (len(board) > y_coord >= 0):
                    #...so if it is - we start again but with the next coordinate (adding another "direction")
                    x_coord += x
                    y_coord += y
                    if (len(board) > x_coord >= 0) and (len(board) > y_coord >= 0):
                        if board[x_coord][y_coord] == -1:
                            return False
                        if board[x_coord][y_coord] == self.my_color:
                            return True

        return False
