class MyVector: #class

    def __init__(self, setVector): #setVector - one-dimensional array
        self.vector = setVector #vector - inside in class, setVector - to outside

    def get_vector(self):
        return self.vector

    def __mul__(self, other): #skalar formula
        return sum(vi * wi for vi, wi in zip(self, other))
    """
            if isinstance(other, (int, float)):
            return MyVector([vi*other for vi in self])
        else:
            
    """

    def __str__(self):
        return str(self.vector)

    def __iter__(self): #make object iterable
        return iter(self.vector)

if __name__ == "__main__":
    vec1 = MyVector([1, 2, 3])
    vec2 = MyVector([3, 4, 5])
    print(vec1.get_vector())
    dot_product = vec1 * vec2
    print(dot_product)
