import numpy as np


def fit_wages(t: np.ndarray, M: np.ndarray):  # t- time, M - mzda
    A = np.column_stack((np.ones(len(t)), t))
    x = np.linalg.lstsq(A, M, rcond=None)[0]
    return x[0], x[1]


def quarter2_2009(x):
    # M(t) = x1 + x2*q
    return x[0] + x[1] * 2009.25


def fit_temps(t: np.ndarray, T: np.ndarray, omega):
    # T(t) = x1 + x2*t + x3*sin(omega*t) + x4*cos(omega*t)
    s = np.sin(omega*t)
    c = np.cos(omega*t)
    A = np.stack([np.ones(len(t)), t, s, c]).T
    x = np.linalg.lstsq(A, T, rcond=None)[0]
    return x


if __name__ == '__main__':
    file = open('data/teplota.txt', 'r')
    lines = file.readlines()
    a = [l.split() for l in lines]
    t_arr = []
    T_arr = []
    for split in a:
        t_arr.append(split[0])
        T_arr.append(split[1])

    t = np.array(t_arr, dtype=float)
    T = np.array(T_arr, dtype=float)
    o =(2 * np.pi) / 365
    print(fit_temps(t, T, o))

    # ------

    file = open('data/mzdy.txt', 'r')
    lines = file.readlines()
    a = [l.split() for l in lines]
    t_arr = []
    M_arr = []
    for split in a:
        t_arr.append(split[0])
        M_arr.append(split[1])

    t = np.array(t_arr, dtype=float)
    M = np.array(M_arr, dtype=float)
    x = fit_wages(t, M)
    print(quarter2_2009(x))
