class ISBNObject:
    def __init__(self, _isbn):
        try:
            _isbn = int(_isbn)
            self.__isbn = _isbn
        except ValueError:
            print("ISBN cannot be non-integer! Please try again...")
            self.__isbn = None

    @property
    def isbn(self):
        return self.__isbn
