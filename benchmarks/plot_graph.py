import pandas as pd
import seaborn as sns
import matplotlib.pyplot as plt
import matplotlib.ticker as mticker
import os


def plot_benchmark_results(csv_file):
    # Преобразуем относительный путь к абсолютному
    absolute_path = os.path.abspath(csv_file)
    df = pd.read_csv(absolute_path)

    plt.figure(figsize=(12, 8))
    sns.lineplot(data=df, x="Iterations", y="Time(ns)", hue="Benchmark", marker="o", linestyle="-", palette="viridis")
    plt.title("Sorting Algorithm Performance", fontsize=16)
    plt.xlabel("Number of Elements", fontsize=12)
    plt.ylabel("Time (ns)", fontsize=12)
    plt.legend(title="Algorithm", fontsize=10, title_fontsize=10)
    plt.grid(True, axis='y', linestyle='--')
    plt.xticks(rotation=45, ha='right')
    plt.gca().xaxis.set_major_formatter(mticker.EngFormatter())
    plt.gca().yaxis.set_major_formatter(mticker.EngFormatter())


    plt.tight_layout()
    plt.savefig("benchmark_results.jpg")
    plt.show()


if __name__ == "__main__":
    plot_benchmark_results("benchmark_results.csv")

#BubbleSort: O(n^2), MergeSort: O(nlogn), ShellSort: O(n^7/6) - O(n^2), HeapSort: O(nlogn)