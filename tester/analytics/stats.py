import sys
import numpy as np
import matplotlib.pyplot as plt
import seaborn as sns
from scipy import stats
from scipy.stats import ttest_ind
from scipy.stats import mannwhitneyu

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
                values.extend(eval(line))
                    
            values = np.array(values)
            from_index = int(len(values)/3)
            to_index = 2*from_index
            values = values[from_index:to_index]
            
            filtered_values = values
            if removing_outliers: 
                filtered_values = remove_outliers(values)
                print(f'Number of outliers removed: {len(values) - len(filtered_values)}')

            mean, std = get_mean_and_std_deviation(filtered_values)
            return filtered_values, values, mean, std

    except FileNotFoundError:
        print(f'File {0} not found', filename)
        sys.exit(1)

def plot_error_bar_graph(first_filename, second_filename):
    first_values, _, first_column_mean, first_column_std = get_values_and_stats_from_file(first_filename)
    second_values, _, second_column_mean, second_column_std = get_values_and_stats_from_file(second_filename)

    if len(first_values) != len(second_values):
        print("Number of samples in the two files are not equal")
        sys.exit(1)

    print(f'Mean and standard deviation of first column: {first_column_mean}, {first_column_std}')
    print(f'Mean and standard deviation of second column: {second_column_mean}, {second_column_std}')

    means = [first_column_mean, second_column_mean]
    std_devs = [first_column_std, second_column_std]
    bars = [first_filename, second_filename]

    fig, ax = plt.subplots()
    ax.bar(bars, means, yerr=std_devs, capsize=5)

    ax.set_xlabel("With/Without Sync Requests")
    ax.set_ylabel("Average RTT (ms)")
    ax.set_title('Average RTT with and without sync requests (N = ' + str(len(first_values)) + ' samples)')

    plt.show()

def plot_distribution_graph(filename):
    filtered_values, original_values, mean, std = get_values_and_stats_from_file(filename, True)
    print(f'Mean and standard deviation of first column: {mean}, {std}')
    
    # Calculate the number of bins using Freedman-Diaconis Rule
    iqr = np.percentile(filtered_values, 75) - np.percentile(filtered_values, 25)
    bin_width = 2 * iqr / len(filtered_values) ** (1/3)
    num_bins = int(np.ceil((filtered_values.max() - filtered_values.min()) / bin_width))
    
    ax = sns.histplot(filtered_values, kde=True, bins=num_bins, color="yellowgreen")
    ax.lines[0].set_color('green')
    plt.axvline(mean, color='red', linestyle='--', label=f'μ = {np.round(mean, 2)} ms')
    plt.axvline(mean - std, color='orange', linestyle=':', label=f'σ = {np.round(std, 2)} ms')
    plt.axvline(mean + std, color='orange', linestyle=':', label=f'')
    plt.xlabel("Pipeline Latency (ms)")
    plt.ylabel("Frequency")

    n = len(original_values)
    plt.title(f'Distribution of Pipeline Latency')
    plt.legend()
    plt.savefig(f'{filename}-distribution.svg')

    # Save the figure as a TikZ file
    # tkz.save(f"{filename}.tex",axis_height=f"\\{filename}H", axis_width=f"\\{filename}W", extra_groupstyle_parameters={f'vertical sep=\\{filename}VS'}, strict=True)

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


def t_test(first_filename, second_filename):
    first_filtered_values, first_values, first_mean, first_std = get_values_and_stats_from_file(first_filename, True)
    second_filtered_values, second_values, second_mean, second_std = get_values_and_stats_from_file(second_filename, True)
    t_stat, p_value = ttest_ind(first_filtered_values, second_filtered_values, equal_var=True)
    print(f'Mean and standard deviation of {first_filename}: {first_mean}, {first_std}')
    print(f'Mean and standard deviation of {second_filename}: {second_mean}, {second_std}')
    print(f'T-statistic: {t_stat}, P-value: {p_value}')
    if p_value < 0.05:
        print("The means are significantly different.")
    else:
        print("The means are not significantly different.")

    
def u_test(first_filename, second_filename):
    first_filtered_values, first_values, first_mean, first_std = get_values_and_stats_from_file(first_filename, True)
    second_filtered_values, second_values, second_mean, second_std = get_values_and_stats_from_file(second_filename, True)
    u_stat, p_value = mannwhitneyu(first_filtered_values, second_filtered_values, alternative='two-sided')

    print(f'Mean and standard deviation of {first_filename}: {first_mean}, {first_std}')
    print(f'Mean and standard deviation of {second_filename}: {second_mean}, {second_std}')
    print(f'U-statistic: {u_stat}, P-value: {p_value}')
    if p_value < 0.05:
        print("The means are significantly different.")
    else:
        print("The means are not significantly different.")

def cliffs_delta(first_filename, second_filename):
    first_filtered_values, first_values, first_mean, first_std = get_values_and_stats_from_file(first_filename, True)
    second_filtered_values, second_values, second_mean, second_std = get_values_and_stats_from_file(second_filename, True)

    # Calculate Cliff's delta
    n1 = len(first_filtered_values)
    n2 = len(second_filtered_values)
    delta = (np.sum(np.sign(np.subtract.outer(first_filtered_values, second_filtered_values))) / (n1 * n2))
    print(f'Cliff\'s delta: {delta}')
    if delta < 0.147:
        print("The effect size is negligible.")
    elif delta < 0.33:
        print("The effect size is small.")
    elif delta < 0.474:
        print("The effect size is medium.")
    else:
        print("The effect size is large.")

if len(sys.argv) == 4:
    first_filename = sys.argv[1]
    second_filename = sys.argv[2]
    method = sys.argv[3]

    valid_methods = ['t-test', 'u-test', 'cliffs-delta', 'all']
    if method not in valid_methods:
        print(f"Error; The method should be one of {valid_methods}")
        sys.exit(1)

    if method == 't-test' or method == 'all':
        print("-----------------Running t-test")
        t_test(first_filename, second_filename)
    if method == 'u-test' or method == 'all':
        print("-----------------Running u-test")
        u_test(first_filename, second_filename)
    if method == 'cliffs-delta' or method == 'all':
        print("-----------------Running Cliff's delta")
        cliffs_delta(first_filename, second_filename)
    
elif len(sys.argv) == 2:
    filename = sys.argv[1]    
    plot_distribution_graph(filename)
else:
    print("Error; The correct usage of this script is `python3 script.py filename` or `python3 script.py filename1 filename2 method`")
    sys.exit(1)
