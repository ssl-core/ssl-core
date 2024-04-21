import sys
import numpy as np
import matplotlib.pyplot as plt
import seaborn as sns
from scipy import stats

def remove_outliers(values, threshold=3):
    z_scores = np.abs(stats.zscore(values, nan_policy='omit'))

    non_outliers = z_scores < threshold

    filtered_values = values[non_outliers]

    return filtered_values

def get_mean_and_std_deviation(values):
    print("Number of samples to get mean: ", len(values))
    return np.mean(values), np.std(values)

def get_values_and_stats_from_file(filename, removing_outliers=False):
    try:
        with open(filename, 'r') as file:
            lines = file.readlines()

            values = []

            for line in lines:
                line_values = line.strip().split(',')
                values.extend(line_values)
            
            from_index = int(len(values)/3)
            to_index = 2*from_index
            values = np.array([float(val) for val in values if val.strip()])
            values = values[from_index:to_index]
            
            filtered_values = values
            if removing_outliers: 
                filtered_values = remove_outliers(values)
                print(f'Number of outliers removed: {len(values) - len(filtered_values)}')

            mean, std = get_mean_and_std_deviation(values)
            return filtered_values, values, mean, std

    except FileNotFoundError:
        print(f'File {0} not found', filename)
        sys.exit(1)

def plot_error_bar_graph(first_filename, second_filename):
    first_values, first_column_mean, first_column_std = get_values_and_stats_from_file(first_filename)
    second_values, second_column_mean, second_column_std = get_values_and_stats_from_file(second_filename)

    if len(first_values) != len(second_values):
        print("Number of samples in the two files are not equal")
        sys.exit(1)

    print(f'Mean and standard deviation of first column: {first_column_mean}, {first_column_std}')
    print(f'Mean and standard deviation of second column: {second_column_mean}, {second_column_std}')

    means = [first_column_mean, second_column_mean]
    std_devs = [first_column_std, second_column_std]
    bars = [filename, second_filename]

    fig, ax = plt.subplots()
    ax.bar(bars, means, yerr=std_devs, capsize=5)

    ax.set_xlabel("With/Without Sync Requests")
    ax.set_ylabel("Average RTT (ms)")
    ax.set_title('Average RTT with and without sync requests (N = ' + str(len(first_values)) + ' samples)')

    plt.show()

def plot_distribution_graph(filename):
    filtered_values, original_values, mean, std = get_values_and_stats_from_file(filename)
    print(f'Mean and standard deviation of first column: {mean}, {std}')
    
    # Calculate the number of bins using Freedman-Diaconis Rule
    IQR = np.percentile(filtered_values, 75) - np.percentile(filtered_values, 25)
    bin_width = 2 * IQR / len(filtered_values) ** (1/3)
    num_bins = int(np.ceil((filtered_values.max() - filtered_values.min()) / bin_width))
    
    ax = sns.histplot(filtered_values, kde=True, bins=num_bins, color="yellowgreen")
    ax.lines[0].set_color('green')
    plt.axvline(mean, color='orange', linestyle='--', label=f'Mean = {np.round(mean, 2)} ms')
    plt.xlabel("Pipeline Latency (ms)")
    plt.ylabel("Frequency")

    n = len(original_values)
    plt.title(f'Distribution of Pipeline Latency')
    plt.legend()
    plt.savefig(f'{filename}-distribution.png')
    # tikzplotlib.save(f'distribution_{filename}.tex')

def plot_linear_graph(filename):
    filtered_values, original_values, mean, std = get_values_and_stats_from_file(filename)

    x = np.arange(0, len(filtered_values))
    sns.plot(x, filtered_values, color="yellowgreen")
    plt.axvline(mean, color='orange', linestyle='--', label='Mean')
    plt.xlabel("Measurement Iteration")
    plt.ylabel("Pipeline Latency (ms)")
    plt.title(f'Pipeline Latency')
    plt.legend()
    plt.show()


if len(sys.argv) != 2:
    print("Usage: python script.py filename")
    sys.exit(1)

filename = sys.argv[1]

plot_distribution_graph(filename)