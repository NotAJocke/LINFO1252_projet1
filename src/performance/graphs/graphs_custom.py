import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
import os

# Load CSV files
df_tts = pd.read_csv(os.path.abspath('src/performance/performance_tts_custom.csv'))
df_ts = pd.read_csv(os.path.abspath('src/performance/performance_ts_custom.csv'))
df_p1 = pd.read_csv(os.path.abspath('src/performance/performance_p1_custom.csv'))

# Combine dataframes with an additional column indicating mutex type
df_tts['Mutex'] = 'tts'
df_ts['Mutex'] = 'ts'
df_p1['Mutex'] = 'p1'
df = pd.concat([df_tts, df_ts, df_p1])

# Group data for plotting
grouped = df.groupby(['Programme', 'Mutex', 'Nombre de Threads']).agg(
    mean_time=('Temps d\'execution', 'mean'),
    std_time=('Temps d\'execution', 'std')
).reset_index()

# Unique program types
program_types = grouped['Programme'].unique()
mutex_types = ['tts', 'ts', 'p1']
colors = ['#87A878', '#CA3C25', '#A3A1D1']  # Colors for mutex types

# Iterate through each program type to generate a separate graph
for program in program_types:
    fig, ax = plt.subplots(figsize=(10, 6))

    # Filter data for the current program
    program_data = grouped[grouped['Programme'] == program]

    # Extract unique thread counts
    thread_counts = sorted(program_data['Nombre de Threads'].unique())
    index = np.arange(len(thread_counts))
    bar_width = 0.2

    # Plot bars for each mutex type
    for i, mutex in enumerate(mutex_types):
        mutex_data = program_data[program_data['Mutex'] == mutex]
        mean_values = mutex_data.set_index('Nombre de Threads')['mean_time'].reindex(thread_counts, fill_value=0)
        std_values = mutex_data.set_index('Nombre de Threads')['std_time'].reindex(thread_counts, fill_value=0)
        if mutex == "p1": mutex = "POSIX"
        if mutex == "tts": mutex = "Test and test and set"
        if mutex == "ts": mutex = "Test and set"
        bar_positions = index + i * bar_width
        ax.bar(bar_positions, mean_values, bar_width,
               yerr=std_values, label=mutex,
               color=colors[i], alpha=0.3, capsize=5)

        ax.plot(bar_positions, mean_values, color=colors[i], marker='.', linestyle='dotted')
    program_name = ""
    if program == "phil": program_name = " le problème des philosophes"
    if program == "prod": program_name = "le problème des producteurs-consommateurs"
    if program == "rw": program_name = "le problème des lecteurs et écrivains"
    # Graph settings
    ax.set_title(f'Performance pour {program_name}')
    ax.set_xlabel('Nombre de Threads')
    ax.set_ylabel('Temps d\'execution (s)')
    ax.set_xticks(index + bar_width)
    ax.set_xticklabels(thread_counts)
    ax.legend(title='Type de mutex')
    ax.grid(axis='y', linestyle='-', alpha=0.7)

    # Save to PNG
    plt.tight_layout()
    plt.savefig(f'img/{program}_performance_custom.png')

print("Graphs saved successfully.")

