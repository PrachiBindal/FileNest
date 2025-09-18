import os
import string
import random

def setup_dirs(path):
    os.makedirs(path, exist_ok=True)
    os.chdir(path)
    for i in range(20):
        filename = f"data{i+1}.txt"
        with open(filename, "w") as file:
            random_string = ''.join(random.choices(string.ascii_uppercase + string.digits, k=1000))
            file.write(random_string)

if __name__ == "__main__":
    setup_dirs("data")