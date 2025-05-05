import pandas as pd
import matplotlib.pyplot as plt

# Load data
df = pd.read_csv('quickselect_vs_quicksort_data.csv')

# Plotting
plt.figure(figsize=(10, 6))
plt.plot(df['Size'], df['QuickSelect'], label='QuickSelect (avg of 100 trials)', marker='o')
plt.plot(df['Size'], df['QuickSort'], label='QuickSort (1 run)', marker='s')
plt.xlabel('Array Size')
plt.ylabel('Time (seconds)')
plt.title('QuickSelect vs QuickSort Benchmark')
plt.legend()
plt.grid(True)
plt.tight_layout()
plt.savefig('benchmark_plot.png')
plt.show()
