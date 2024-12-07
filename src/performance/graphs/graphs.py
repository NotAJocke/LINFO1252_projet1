import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
import os

# Load the combined CSV file
df = pd.read_csv(os.path.abspath('src/performance/performance.csv'))

# Group data for plotting
grouped = df.groupby(['Programme', 'Nombre de Threads']).agg(
    mean_time=('Temps d\'execution', 'mean'),
    std_time=('Temps d\'execution', 'std')
).reset_index()

# Unique program types
program_types = grouped['Programme'].unique()
color = '#87A878'  # Single color for the bar

# Iterate through each program type to generate a separate graph
for program in program_types:
    fig, ax = plt.subplots(figsize=(10, 6))

    # Filter data for the current program
    program_data = grouped[grouped['Programme'] == program]

    # Extract unique thread counts
    thread_counts = sorted(program_data['Nombre de Threads'].unique())
    index = np.arange(len(thread_counts))

    # Get mean and standard deviation values
    mean_values = program_data.set_index('Nombre de Threads')['mean_time'].reindex(thread_counts, fill_value=0)
    std_values = program_data.set_index('Nombre de Threads')['std_time'].reindex(thread_counts, fill_value=0)

    # Draw bars with reduced opacity
    bar_width = 0.4
    ax.bar(index, mean_values, bar_width,
           yerr=std_values, label=program,
           color=color, alpha=0.7, capsize=5)

    # Add points centered on the bars
    ax.plot(index, mean_values, color=color, marker='o', linestyle='dotted')

    # Graph settings
    ax.set_title(f'Performance de {program}')
    ax.set_xlabel('Nombre de Threads')
    ax.set_ylabel('Temps d\'execution(s)')
    ax.set_xticks(index)
    ax.set_xticklabels(thread_counts)
    ax.grid(axis='y', linestyle='-', alpha=0.7)

    # Save to PNG
    plt.tight_layout()
    plt.savefig(f'{program}_performance.png')
    plt.close()

print("Graphs saved successfully.")
