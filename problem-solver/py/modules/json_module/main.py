from reader import Reader
from writer import Writer

if __name__ == "__main__":
    path = "data"
    reader = Reader(path)
    for instance in reader.collect():
        Writer.write(instance)
