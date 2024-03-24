import pandas as pd
import matplotlib.pyplot as plt

# Read the output file into a DataFrame
df_time = pd.read_csv('results_time.txt', sep=';')
df_samples = pd.read_csv('results_samples.txt', sep=';')

# Filter data by option type
european_data_time = df_time[df_time['Option Type'] == 'European Option']
barrier_data_time = df_time[df_time['Option Type'] == 'Barrier Option']
european_data_samples = df_samples[df_samples['Option Type'] == 'European Option']
barrier_data_samples = df_samples[df_samples['Option Type'] == 'Barrier Option']

# Create plots for time difference vs. time steps for European and Barrier Options
fig, axes = plt.subplots(2, 1, figsize=(12, 12))

# Plot time difference vs. time steps for European Option
axes[0].plot(european_data_time['Time Steps'], european_data_time['Time(non constant) - Time(Constant) in (s)'], label='European Option', color='blue', linestyle='-', marker='o')
axes[0].set_title('Time Difference vs. Time Steps')
axes[0].set_xlabel('Time Steps')
axes[0].set_ylabel('Time Difference (s)')
axes[0].legend()
axes[0].grid(True)

# Plot time difference vs. time steps for Barrier Option
axes[0].plot(barrier_data_time['Time Steps'], barrier_data_time['Time(non constant) - Time(Constant) in (s)'], label='Barrier Option', color='red', linestyle='--', marker='s')
axes[0].legend()

# Create plots for time difference vs. sample size for European and Barrier Options
# Plot time difference vs. sample size for European Option
axes[1].plot(european_data_samples['Samples'], european_data_samples['Time(non constant) - Time(Constant) in (s)'], label='European Option', color='green', linestyle='-', marker='o')
axes[1].set_title('Time Difference vs. Sample Size')
axes[1].set_xlabel('Sample Size')
axes[1].set_ylabel('Time Difference (s)')
axes[1].legend()
axes[1].grid(True)

# Plot time difference vs. sample size for Barrier Option
axes[1].plot(barrier_data_samples['Samples'], barrier_data_samples['Time(non constant) - Time(Constant) in (s)'], label='Barrier Option', color='orange', linestyle='--', marker='s')
axes[1].legend()
axes[1].set_xscale('log')  # Set x-axis to logarithmic scale

# Adjust layout and show the plots
plt.tight_layout()
plt.show()
