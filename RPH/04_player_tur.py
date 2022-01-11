import random
COOPERATE = False
DEFECT = True

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

    def move(self):
        if self.matrix[0][0] >= self.matrix[0][1]: #jednoducha analiza matice pro srovnání moznych bodu
            if self.olm[-1] == DEFECT:
                return DEFECT #Pokud však někdo využije dobré vůle meho hrace ,nezůstane to bez odpovědi...
            elif self.mlm[-1] == COOPERATE and self.mlm[-2] == COOPERATE:  #kdyz muj hraj byl velmi laskavy (2 - krat), tak se pokušení na podvod zvetsuje...
                r = random.random()
                if r <= 0.50: #...az do 50%
                    return DEFECT
                else:
                    return COOPERATE
            else: #můj hráč je připraven se setkat s těmi, kteří ho nebudou podvádět, ale...
                r2 = random.random()
                if r2 <= 0.9: #...všichni děláme chyby, v připadě mého hrače taková šance se rovná 10%
                    return COOPERATE
                else: #take muj hrac je schopen povedet vesely vtip :)
                    return DEFECT
        elif self.matrix[1][0] >= self.matrix[1][1]:# jeste jedna jednoducha analiza matice pro srovnání moznych bodu
            if self.olm[-1] == COOPERATE: #ted hrac hraje naopak
                return DEFECT
            if self.mlm[-1] == COOPERATE and self.mlm[-2] == DEFECT and self.olm[-1] == COOPERATE: #pokud 2 minule kroky jsou coop a defect a take posledni krok oponenta je coop, tak...
                r3 = random.random()
                if r3 <= 0.5: # tak hrac hraje 50/50
                    return COOPERATE
                else:
                    return DEFECT