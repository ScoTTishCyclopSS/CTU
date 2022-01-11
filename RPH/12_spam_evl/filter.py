import re
from utils import read_classification_from_file
from corpus import Corpus

import time


class MyFilter:
    """ Naive bayes spam filter """

    def __init__(self):
        self.spam = []  # trained spam words
        self.ham = []  # trained ham words

        self.unique_spam = []
        self.unique_ham = []

        self.spamicity = {}  # dictionary of the spam words
        self.hamicity = {}  # dictionary of the ham words

        self.spam_e = 0  # counter for spam emails
        self.ham_e = 0  # counter for ham emails

    def train(self, direct):
        """ Train filter with classified data"""
        classification = read_classification_from_file(direct + "/!truth.txt")
        corp = Corpus(direct)

        for header, message in corp.emails():
            message_split = self.reg_splitter(message)
            if classification[header] == 'SPAM':
                self.spam_e += 1  # ok
            else:
                self.ham_e += 1  # ok
            for word in message_split:
                if classification[header] == 'SPAM':
                    self.spam.append(word)  # fill the list of spam or ham words
                else:
                    self.ham.append(word)

        # FULLY LOADED LISTS

        self.unique_spam = list(dict.fromkeys(self.spam))  # ok
        self.unique_ham = list(dict.fromkeys(self.ham))   # ok

        # spam/ham-icity means the probability of current word being in email or being spam or ham
        # so it's our goal to calculate these values
        # spam/ham-icity dict will have a look {'word';'%of_appearing_per_message'} (% has a look from 0 to 1)

        self.spamicity = self.create_the_icity(self.unique_spam, corp, classification, 'SPAM')
        self.hamicity = self.create_the_icity(self.unique_ham, corp, classification, 'OK')


    def test(self, direct):
        """ The main filter function """
        corp = Corpus(direct)  # getting the new corpus of data
        with open(direct + "/!prediction.txt", "wt", encoding="utf-8") as f:

            for header, message in corp.emails():
                message_split = self.reg_splitter(message)
                probs = []
                for word in message_split:
                    try:
                        prob_word_spam = self.spamicity[word]
                    except KeyError:
                        prob_word_spam = 1 / (self.spam_e + 2)

                    try:
                        prob_word_ham = self.hamicity[word]
                    except KeyError:
                        prob_word_ham = 1 / (self.ham_e + 2)

                    prob_bayes_for_word = prob_word_spam / (prob_word_spam + prob_word_ham)
                    probs.append(prob_bayes_for_word)

                stat = 1.0
                for i in probs:
                    stat += i
                stat /= len(probs)

                if stat >= 0.5:
                    f.write(header + " " + 'SPAM' + "\n")
                else:
                    f.write(header + " " + 'OK' + "\n")

    def create_the_icity(self, unique_list, corp, classification, type):
        dict_of_icity = {}
        msgs = []  # create the list of messages, it's easier to iterate over list
        for h, m in corp.emails():
            if classification[h] == type:
                msgs.append(m)

        for word in unique_list:
            emails_with_word = 0
            for ms in msgs:
                if word in ms:
                    emails_with_word += 1

            the_icity = (emails_with_word + 1) / (len(msgs) + 2)  # len(msgs) = count of spam/ham emails
            dict_of_icity[word] = the_icity

        return dict_of_icity

    def reg_splitter(self, message):

        exp = re.compile(r"[a-zA-Z0-9._-]+@[a-zA-Z0-9._-]+\.[a-zA-Z0-9]+|[a-zA-Z0-9_-]+\.[a-zA-Z0-9._-]+\.[a-zA-Z]+|[a-zA-Z0-9]+")
        split_message = re.findall(exp, message.lower())

        # we use lower() func to make all words in lower case
        # because later, when we will try to compare spam and ham
        # words with the different cases will not calculate ('X' != 'x')

        return split_message
