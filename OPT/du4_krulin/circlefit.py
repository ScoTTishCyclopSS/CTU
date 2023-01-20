import math

import scipy.io as sio
import numpy as np
from math import pi
from matplotlib import pyplot as plt


def quad_to_center(d, e, f):
    x0 = -d / 2
    y0 = -e / 2
    r = math.sqrt(math.pow((e / 2), 2) + math.pow((d / 2), 2) - f)
    return x0, y0, r


def fit_circle_nhom(X):
    n, m = X.shape

    A = np.ones((n, 3))
    y = np.ones((n, 1))

    for i in range(n):
        A[i][0] = X[i][0]
        A[i][1] = X[i][1]
        y[i][0] = -math.pow(X[i][0], 2) - math.pow(X[i][1], 2)

    x_ = np.linalg.lstsq(A, y, rcond=None)[0]

    d = x_[0][0]
    e = x_[1][0]
    f = x_[2][0]

    return d, e, f


def fit_circle_hom(X):

    n, m = X.shape
    A = np.ones((n, 4))

    for i in range(n):
        A[i][0] = math.pow(X[i][0], 2) + math.pow(X[i][1], 2)
        A[i][1] = X[i][0]
        A[i][2] = X[i][1]

    # w = vals, v - vecs
    w, v = np.linalg.eig(A.T@A)
    v[:, 0] = -v[:, 0]
    v = v.T
    min = np.argmin(w)

    ret = v[min, :]
    d = ret[1] / ret[0]
    e = ret[2] / ret[0]
    f = ret[3] / ret[0]

    return d, e, f


def dist(X, x0, y0, r):
    N = X.shape[0]

    ret = np.ones((1, N))

    for i in range(N):
        v = X[i]
        ret[0, i] = np.linalg.norm(np.array(x0 - v[0], y0 - v[1])) - r

    return ret


def fit_circle_ransac(X, num_iter, threshold):

    n, m = X.shape
    x0 = 0
    y0 = 0
    r = 0
    max = 0

    for _ in range(num_iter):
        # nahodne trojici
        rand = np.random.randint(n, size=3)
        d, e, f = fit_circle_nhom(X[rand])
        x0_, y0_, r_ = quad_to_center(d, e, f)
        dist_ = dist(X, x0_, y0_, r_)

        inl = 0
        n_d, m_d = dist_.shape
        for i in range(m_d):
            if threshold > abs(dist_[0][i]):
                inl = inl + 1

        if max < inl:
            max = inl
            x0 = x0_
            y0 = y0_
            r = r_

    return x0, y0, r


def plot_circle(x0, y0, r, color, label):
    t = np.arange(0, 2 * pi, 0.01)
    X = x0 + r * np.cos(t)
    Y = y0 + r * np.sin(t)
    plt.plot(X, Y, color=color, label=label)


if (__name__ == '__main__'):
    data = sio.loadmat('data.mat')
    X = data['X']  # only inliers
    A = data['A']  # X + outliers

    def_nh = fit_circle_nhom(X)
    x0y0r_nh = quad_to_center(*def_nh)
    dnh = dist(X, *x0y0r_nh)

    def_h = fit_circle_hom(X)
    x0y0r_h = quad_to_center(*def_h)
    dh = dist(X, *x0y0r_h)

    results = {'def_nh': def_nh, 'def_h': def_h,
               'x0y0r_nh': x0y0r_nh, 'x0y0r_h': x0y0r_nh,
               'dnh': dnh, 'dh': dh}

    GT = sio.loadmat('GT.mat')
    for key in results:
        print('max difference', np.amax(np.abs(results[key] - GT[key])), 'in', key)

    x = fit_circle_ransac(A, 2000, 0.1)

    plt.figure(1)
    plt.subplot(121)
    plt.scatter(X[:, 0], X[:, 1], marker='.', s=3)
    plot_circle(*x0y0r_h, 'r', 'hom')
    plot_circle(*x0y0r_nh, 'b', 'nhom')
    plt.legend()
    plt.axis('equal')
    plt.subplot(122)
    plt.scatter(A[:, 0], A[:, 1], marker='.', s=2)
    plot_circle(*x, 'y', 'ransac')
    plt.legend()
    plt.axis('equal')
    plt.show()
