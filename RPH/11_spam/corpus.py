import os


class Corpus:

    """Wraps an email directory and allows to browse it easily."""

    def __init__(self, path):
        self.direct = path  # declare an incoming path

    def emails(self):
        my_dir = self.direct  # path incomes in the look of "directory"
        for file in os.listdir(my_dir):  # by function listdir - we create the list with everything that this direction has

            # to check everything PERSONALLY we must declare the full path of the file or folder and etc.
            # with this helps the join function (adds to the directory element name)

            file_path = os.path.join(my_dir, file)

            # path could have not only files but also paths and etc., so we should check it

            if os.path.isfile(file_path) and file[0] != '!':
                with open(file_path, 'r', encoding='utf-8') as f:
                    data = f.read()
                yield (file, data)


if __name__ == '__main__':

    # Create corpus from a directory

    corpus = Corpus('C:/Users/fedti/PycharmProjects/spam_filtr')
    count = 0

    # Go through all emails and print the filename and the message body

    for (fname, body) in corpus.emails():
        print(fname)
        print('----')
        print(body)
        print('-------------------------')
        count += 1
    print ('Finished: ', count, 'files processed.')
