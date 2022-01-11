class BinaryConfusionMatrix:

    """Forms the basis for evaluating the success of a filter."""

    def __init__(self, pos_tag, neg_tag):
        self.tp = 0
        self.tn = 0
        self.fp = 0
        self.fn = 0
        self.pos_tag = pos_tag
        self.neg_tag = neg_tag

    def as_dict(self):
        """Returns the confusion matrix as a dictionary."""

        return {
            'tp': self.tp,
            'tn': self.tn,
            'fp': self.fp,
            'fn': self.fn,
            }

    def update(self, truth, prediction):
        """Compares the truth with the prediction, then increments the related counter."""

        self.check_value_of(truth)
        self.check_value_of(prediction)
        if prediction == self.pos_tag:
            if truth == prediction:
                self.tp += 1
            else:
                self.fp += 1
        elif prediction == self.neg_tag:
            if truth == prediction:
                self.tn += 1
            else:
                self.fn += 1

    def check_value_of(self, truth):
        if truth not in (self.pos_tag, self.neg_tag):
            raise ValueError('The "truth" parameter can be either %s or %s.'
                              % (self.pos_tag, self.neg_tag))

    def compute_from_dicts(self, truth_dict, pred_dict):
        """
        Updates the matrix by using the corresponding values from both dictionaries.
        It is assumed that both dictionaries have the same keys.
        """

        for key in truth_dict:
            self.update(truth_dict[key], pred_dict[key])
