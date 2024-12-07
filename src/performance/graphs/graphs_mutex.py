import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
import os

# CSV
df1 = pd.read_csv(os.path.abspath('src/performance/performance_tts.csv'))
df2 = pd.read_csv(os.path.abspath('src/performance/performance_ts.csv'))

# infos avec tts
grouped1 = df1.groupby(['Programme', 'Nombre de Threads']).agg(
    mean_time=('Temps d\'execution', 'mean'),  # Average time
    std_time=('Temps d\'execution', 'std')    # Standard deviation
).reset_index()

# infos avec ts
grouped2 = df2.groupby(['Programme', 'Nombre de Threads']).agg(
    mean_time=('Temps d\'execution', 'mean'),  # Moyenne
    std_time=('Temps d\'execution', 'std')    # Ecart type
).reset_index()

pivoted1 = grouped1.pivot(index='Nombre de Threads', columns='Programme', values='mean_time')
std_pivoted1 = grouped1.pivot(index='Nombre de Threads', columns='Programme', values='std_time')

pivoted2 = grouped2.pivot(index='Nombre de Threads', columns='Programme', values='mean_time')
std_pivoted2 = grouped2.pivot(index='Nombre de Threads', columns='Programme', values='std_time')
thread_counts = pivoted1.index

fig, ax = plt.subplots(figsize=(12, 6))
bar_width = 0.1
index = np.arange(len(thread_counts))

colors = ['#87A878', '#C7CCB9']  # VERT :3

# tts & ts (fait de sorte à avoir les 3 autres programmes)
for i, program in enumerate(pivoted1.columns):
    mean_values1 = pivoted1[program].fillna(0) # .fillna() -> empêcher d'avoir une erreur si pas de valeur
    std_values1 = std_pivoted1[program].fillna(0)
    ax.bar(index + i * bar_width, mean_values1, bar_width,
           yerr=std_values1, label=f'{program} (tts)',
           color=colors[i % len(colors)], capsize=5)

    ax.plot(index + i * bar_width, mean_values1, color=colors[i % len(colors)], marker='.', linestyle='dotted')

for i, program in enumerate(pivoted2.columns):
    mean_values2 = pivoted2[program].fillna(0) # .fillna() -> empêcher d'avoir une erreur si pas de valeur
    std_values2 = std_pivoted2[program].fillna(0)
    ax.bar(index + (len(pivoted1.columns) + i) * bar_width, mean_values2, bar_width,
           yerr=std_values2, label=f'{program} (ts)',
           color=colors[(i + len(pivoted1.columns)) % len(colors)], capsize=5)

    ax.plot(index + (len(pivoted1.columns) + i) * bar_width, mean_values2, color=colors[(i + len(pivoted1.columns)) % len(colors)], marker='.', linestyle='dotted')

# Graph
ax.set_xlabel('Nombre de Threads') # Label du X
ax.set_ylabel('Temps d\'exécution (s)') # Label du Y
ax.set_title('Comparaison des temps d\'exécution moyen par mutex et nombre de threads')
ax.set_xticks(index + bar_width * (len(pivoted1.columns) - 1) / 2)
ax.set_xticklabels(thread_counts) # Bons nombre de thread afficher
ax.set_ylim(0, max(pivoted1.max().max(), pivoted2.max().max()) + 0.5)
ax.legend()
plt.tight_layout()
plt.savefig('mutex.png')
