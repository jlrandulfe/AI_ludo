#!/usr/bin/python3
"""
Main project executable.
"""
# Standard libraries
import matplotlib.pyplot as plt
import numpy as np
import os
# Third party libraries
# Local libraries


def format_plotting():
    plt.rcParams['figure.figsize'] = (10, 8)
    plt.rcParams['font.size'] = 22
    #    plt.rcParams['font.family'] = 'Times New Roman'
    plt.rcParams['axes.labelsize'] = plt.rcParams['font.size']
    plt.rcParams['axes.titlesize'] = 1.2 * plt.rcParams['font.size']
    plt.rcParams['legend.fontsize'] = 0.7 * plt.rcParams['font.size']
    plt.rcParams['xtick.labelsize'] = 0.6 * plt.rcParams['font.size']
    plt.rcParams['ytick.labelsize'] = 0.6 * plt.rcParams['font.size']
    plt.rcParams['savefig.dpi'] = 1000
    plt.rcParams['savefig.format'] = 'eps'
    plt.rcParams['xtick.major.size'] = 3
    plt.rcParams['xtick.minor.size'] = 3
    plt.rcParams['xtick.major.width'] = 1
    plt.rcParams['xtick.minor.width'] = 1
    plt.rcParams['ytick.major.size'] = 3
    plt.rcParams['ytick.minor.size'] = 3
    plt.rcParams['ytick.major.width'] = 1
    plt.rcParams['ytick.minor.width'] = 1
    plt.rcParams['legend.frameon'] = True
    plt.rcParams['legend.loc'] = 'lower right'
    plt.rcParams['axes.linewidth'] = 1
    plt.rcParams['lines.linewidth'] = 1
    plt.rcParams['lines.markersize'] = 3

    plt.gca().spines['right'].set_color('none')
    plt.gca().spines['top'].set_color('none')
    plt.gca().xaxis.set_ticks_position('bottom')
    plt.gca().yaxis.set_ticks_position('left')
    return


def plot_learning_rate_test():
    # Get the data from text file
    with open("../../resources/learning-rate_test", "r") as data_file:
        data = data_file.readline()
    num_data = data.split()
    results = list(map(float, num_data))
    # Create x-axis, containing the learning rates
    rate = np.array(list(range(20, 0, -1))) / 20
    ax = plt.subplot(111)
    ax.plot(rate, results)
    ax.set_ylim([0, 100])
    ax.set_xlabel('Learning rate')
    ax.set_ylabel('Win rate (%)')
    format_plotting()
    plt.title("Learning rate influence for custom R")
    script_path = os.path.dirname(os.path.realpath(__file__))
    plt.savefig('{}/tmp/learning_rate.png'.format(script_path),
                bbox_inches='tight')
    plt.show()
    return


if __name__ == "__main__":
    plot_learning_rate_test()
