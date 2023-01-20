import numpy as np
import scipy.io.wavfile as wav
from matplotlib import pyplot as plt
import warnings
warnings.filterwarnings("ignore")

def ar_fit_model(y: np.ndarray, p: int) -> np.ndarray:
    M = np.ones((y.shape[0] - p, p + 1))
    b = y[p: y.shape[0]]

    for i in range(0, max(M.shape)):
        vec = y[np.arange(p - 1 + i, i - 1, -1)]
        M[i, 1:] = vec

    a = np.linalg.lstsq(M, b, rcond=None)[0]
    return a

def ar_predict(a: np.ndarray, y0: np.ndarray, N:int) -> np.ndarray:

    y_pred = np.zeros(N)
    p = y0.shape[0]
    y_pred[:p] = y0

    for i in range(p, N):
        v = np.flip(y_pred[i-p: i: 1])
        y_pred[i] = a[0] + np.dot(a[1:], v)

    return y_pred

if(__name__ == '__main__'):

    fs,y=wav.read('gong.wav')
    y = y.copy()/32767
    p = 300      # size of history considered for prediction
    N = len(y)   # length of the sequence
    K = 10000    # visualize just K first elements

    a = ar_fit_model(y, p)

    y0 = y[:p]
    y_pred = ar_predict(a, y0, N)

    wav.write('gong_predicted.wav', fs, y_pred)

    plt.plot(y[:K], 'b', label = 'original')
    plt.plot(y_pred[:K], 'r', label = 'AR model')
    plt.legend()
    plt.show()

