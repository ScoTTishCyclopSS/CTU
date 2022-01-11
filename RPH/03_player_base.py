import random

class MyPlayer:

    '''Hrac je laskavy do prvniho podvodu oponenta. Jinak příští tah bude take podvod.'''

    def __init__(self, payoff_matrix, number_of_iterations=None):
        self.matrix = payoff_matrix
        self.noi = number_of_iterations
        self.olm = [None]
        self.mlm = [None]

    def record_last_moves(self, my_last_move, opponent_last_move):
        self.mlm.append(my_last_move)
        self.olm.append(opponent_last_move)

    # Všichni děláme chyby. Vždy se však snažíme o vzájemné porozumění, vzájemný prospěch.
    # Takže můj hráč je připraven se setkat s těmi, kteří ho nebudou podvádět.
    # Pokud však někdo využije jeho dobré vůle,nezůstane to bez odpovědi...
    # Ale jak jsem řekl - všichni děláme chyby, v připadě mého hrače taková šance se rovná 10%

    def move(self):
        if self.olm[-1] == True:
            return True
        else:
            r = random.random()
            if r <= 0.9:
                return False
            else:
                return True