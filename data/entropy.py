import matplotlib.image as mpimg
from math import log2
from collections import Counter
from os import listdir
import numpy as np


def calculate_image_block_entropy(image_path, block=1):
    image = mpimg.imread(image_path)
    return block_entropy(image.flatten().astype('byte'), block)


def _calculate_entropy(image_path):
    image = mpimg.imread(image_path).flatten().astype('byte')
    freqs = Counter(image)
    probabilities = [float(freq) / len(image) for freq in freqs.values()]
    return -1.0 * sum(p * log2(p) for p in probabilities)


def block_entropy(data, m):
    B = np.zeros(m * [max(data) + 1])  # Generate (max(A)+1)^m-Array of zeroes for counting.
    for j in range(len(data) - m + 1):
        B[tuple(data[j:j + m])] += 1  # Do the counting by directly using the array slice for indexing.
    C = B[B > 0] / (len(data) - m + 1)  # Flatten array, take only non-zero entries, divide for probability.
    return -sum(x * np.log(x) for x in C)  # Calculate entropy


def calculate_name_to_entropy(paths):
    return [(path.split('/')[1], _calculate_entropy(path), calculate_image_block_entropy(path, 2),
             calculate_image_block_entropy(path, 3)) for path in paths]


img_paths = ['obrazy/' + filename for filename in listdir("./obrazy")] + ['rozklady/' + filename for filename in
                                                                          listdir("./rozklady")]
name_to_entropy = calculate_name_to_entropy(img_paths)
with open(f"entropy_results.csv", 'w') as writer:
    writer.write("filename,entropy,2_blocks,3_blocks\n")
    for name, e1, e2, e3 in sorted(name_to_entropy):
        writer.write(f"{name},{e1},{e2},{e3}\n")
