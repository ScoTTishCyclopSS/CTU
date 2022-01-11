import unittest
import os
from confmat import BinaryConfusionMatrix
from utils import read_classification_from_file

SPAM_TAG = 'SPAM'
HAM_TAG = 'OK'


def __init__(self, pos_tag, neg_tag):
    self.tp = 0
    self.tn = 0
    self.fp = 0
    self.fn = 0
    self.pos_tag = pos_tag
    self.neg_tag = neg_tag


def quality_score(
    tp,
    tn,
    fp,
    fn,
    ):
    """Calculates the quality score from the passed values of the exchange matrix."""

    q = (tp + tn) / (tp + tn + 10 * fp + fn)
    return q


def compute_quality_for_corpus(corpus_dir):
    """
    Evaluates the quality of predictions (filters) based on the data
    in the! truth.txt and! prediction.txt files in the specified corpus.
    """

    new_matrix = BinaryConfusionMatrix(pos_tag='SPAM', neg_tag='OK')
    new_matrix.compute_from_dicts(dict(read_classification_from_file(os.path.join(corpus_dir,
                                  '!truth.txt'))),
                                  dict(read_classification_from_file(os.path.join(corpus_dir,
                                  '!prediction.txt'))))
    return quality_score(**new_matrix.as_dict())


if __name__ == '__main__':
    unittest.main()
