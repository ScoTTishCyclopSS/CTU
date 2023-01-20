import numpy as np
from scipy import optimize


def vyhra(c, k):
    func = np.array([0, 0, 0, 0, 0, -1])
    Aub = np.array(
        [[-c[0], -c[1], 0, 0, 0, 1],
         [0, -c[1], -c[2], -c[3], 0, 1],
         [0, 0, 0, -c[3], -c[4], 1]])
    bub = np.array([0, 0, 0])
    Aeq = np.array([[1, 1, 1, 1, 1, 0]])
    beq = k
    result = optimize.linprog(c=func, A_ub=Aub, b_ub=bub, A_eq=Aeq, b_eq=beq)
    x = result.x
    return x[:-1]


def vyhra2(c, k, m):
    func = np.array([0, 0, 0, -1])
    A_ub = np.array([[-c[0], 0, 0, 1],
                     [0, -c[1], 0, 1],
                     [0, 0, -c[2], 1]])
    b_ub = np.array([0, 0, 0])
    A_equal = np.array([[1, 1, 1, 0]])
    bounds = [(m, None),
              (m, None),
              (m, None),
              (None, None)]

    result = optimize.linprog(c=func, A_ub=A_ub, b_ub=b_ub, A_eq=A_equal, b_eq=k, bounds=bounds).x[:-1]
    return result


def minimaxfit(x, y):
    n, m = x.shape
    A = np.ones((m * 2, n + 2))
    b_ub = np.zeros((m * 2, 1))

    fill = [0] * (n + 1)
    fill.append(1)
    func = np.array(fill)

    inf = float(np.Infinity)
    neg_inf = float(-np.Infinity)
    bounds = [[neg_inf, inf]] * (n + 2)

    x = x.T
    x_T_n, x_T_m = x.shape

    for i in range(x_T_n):
        for j in range(x_T_m):
            A[i][j] = x[i][j]
            A[i][-1] = -1
            A[i][-2] = 1
        b_ub[i][0] = y[0][i]

    for i in range(x_T_n):
        for j in range(x_T_m):
            A[i + m][j] = -x[i][j]
            A[i + m][-1] = -1
            A[i + m][-2] = -1
        b_ub[i + m][0] = -y[0][i]

    result = optimize.linprog(c=func, A_ub=A, b_ub=b_ub, bounds=bounds)
    a = result.x[:-2]
    b = result.x[-2]
    r = result.x[-1]

    return (a, b, r)

