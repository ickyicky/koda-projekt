import matplotlib.image as mpimg
from math import log2
from collections import Counter
from os import listdir
import numpy as np


def calculate_image_entropy(image_path, block=1):
    image = mpimg.imread(image_path)
    return _calculate_entropy(image.flatten().astype('byte'), block)


def _calculate_entropy(input_data, block=1):
    input_data = _apply_blocks(input_data, block)
    freqs = Counter(input_data)
    probabilities = [float(freq) / len(input_data) for freq in freqs.values()]
    return -1.0 * sum(p * log2(p) for p in probabilities)


def _apply_blocks(data, blocks):
    if blocks == 1:
        return data
    else:
        result = []
        for idx in range(0, len(data) - blocks + 1, 2):
            result.append(sum(data[idx:(idx + blocks)]))
        return result


def calculate_name_to_entropy(paths):
    return [(path.split('/')[1], calculate_image_entropy(path, 1), calculate_image_entropy(path, 2),calculate_image_entropy(path, 3)) for path in paths]


paths = ['obrazy/' + filename for filename in listdir("./obrazy")] + ['rozklady/' + filename for filename in listdir("./rozklady")]
name_to_entropy = calculate_name_to_entropy(paths)
with open(f"entropy_results.csv", 'w') as writer:
    writer.write("filename,1_block,2_blocks,3_blocks\n")
    for name, e1, e2, e3 in name_to_entropy:
        writer.write(f"{name},{e1},{e2},{e3}\n")
