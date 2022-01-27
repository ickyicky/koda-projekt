import unittest
import matplotlib.image as mpimg
from math import log2
from collections import Counter
from os import listdir


def calculate_image_entropy(image_path):
    image = mpimg.imread(image_path)
    return _calculate_entropy(image.flatten())


def _calculate_entropy(input_data):
    freqs = Counter(input_data)
    probabilities = [float(freq) / len(input_data) for freq in freqs.values()]
    return -1.0 * sum(p * log2(p) for p in probabilities)


name_to_entropy = [
    (path.split('/')[1], calculate_image_entropy(path)) for path in (
            ['obrazy/' + filename for filename in listdir("./obrazy")] +
            ['rozklady/' + filename for filename in listdir("./rozklady")]
    )
]
with open("entropy_results.txt", 'w') as writer:
    for name, entropy in name_to_entropy:
        writer.write(f"{name}: {entropy}\n")

'''
class TestEntropy(unittest.TestCase):

    def test_entropy(self):
        self.assertEqual(_calculate_entropy("aabbccdd"), 2.0)
        self.assertEqual(_calculate_entropy("abcd"), 2.0)
        self.assertEqual(_calculate_entropy("aaa"), 0.0)
        self.assertEqual(_calculate_entropy("a"), 0.0)
'''
