import os


def read_classification_from_file(file):
    """Reads email classifications from a text file."""

    dict = {}  # declare the empty dictionary
    size = os.path.getsize(file)  # get the file size by the incoming path
    if size == 0:
        return dict  # if the file is empty, then return empty dictionary
    else:
        with open(file, 'r', encoding='utf-8') as f:
            data = f.read()

            # split the data from file step by step - first by \n, then by whitespace

            for elem in data.split('\n'):
                record = elem.split()
                if len(record) == 2:
                    dict[record[0]] = record[1]  # input new "record" to the dictionary in look [mail-status]
        return dict

def write_classification_to_file(dir, dic):
    """
    Saves given prediction dictionary to file in given directory
    :param dir: directory where the prediction should be saved
    :param dic: dictionary with email classification prediction
    """
    with open(os.path.join(dir,"!prediction.txt"),'w',encoding="utf-8") as f:
        for key in dic:
            f.write(key + " " + dic[key] + "\n")

#if __name__ == '__main__':
    #read_classification_from_file('test.txt')  # i was just testing on my own file :D