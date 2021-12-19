import matplotlib.image as mpimg
import matplotlib.pyplot as plt
import numpy as np
from os import listdir


def plot_hist(path):
    # plt.imshow(image, cmap="gray")
    image = mpimg.imread(path)
    histogram, bin_edges = np.histogram(image, bins=256, range=(0, 256))
    plt.figure()
    plt.title(f"Histogram {path.split('.')[0]}")
    plt.xlim([0.0, 256.0])
    plt.bar(bin_edges[0:-1], histogram)
    # plt.show()
    plt.savefig(f"./histograms/{path.split('.')[0]}_histogram.png", dpi=300)

[ plot_hist(path) for path in  (['obrazy/' + filename for filename in listdir("./obrazy")] + ['rozklady/' + filename for filename in listdir("./rozklady")]) ]
