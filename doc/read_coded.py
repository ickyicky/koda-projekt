import sys
import math

mapper = {
    "0": "0000",
    "1": "0001",
    "2": "0010",
    "3": "0011",
    "4": "0100",
    "5": "0101",
    "6": "0110",
    "7": "0111",
    "8": "1000",
    "9": "1001",
    "a": "1010",
    "b": "1011",
    "c": "1100",
    "d": "1101",
    "e": "1110",
    "f": "1111",
}


def bits(f):
    bytes_ = f.read()
    for b in bytes_:
        for i in range(8):
            yield (b >> i) & 1


def binary2int(binary):
    int_val = 0
    for i, a in enumerate(binary):
        int_val = int_val + int(a) * pow(2, i)
    return int_val


FNAME = "out_data.bin"

if len(sys.argv) > 1:
    FNAME = sys.argv[1]


class LZSS:
    def __init__(self, fname):
        with open(fname, "rb") as f:
            self.content = "".join([str(x) for x in bits(f)])

        self.dict_size = self.read(16)
        self.look_size = self.read(16)
        self.pos_bits = math.ceil(math.log2(self.dict_size))
        self.len_bits = math.ceil(math.log2(self.look_size))
        print(f"dict size: {self.dict_size}, look size: {self.look_size}")

        while self.content:
            self.read_next()

    def read_next(self):
        type_ = self.read(1)

        if type_ == 0:
            char = self.read(8)
            print(f"( bit: {type_}, char: {char} )")
        else:
            pos = self.read(self.pos_bits)
            lenght = self.read(self.len_bits) or self.look_size
            print(f"( bit: {type_}, pos: {pos}, len: {lenght} )")

    def read(self, amount):
        result = self.content[:amount]
        self.content = self.content[amount:]
        return binary2int(result)


LZSS(FNAME)
