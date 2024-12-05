import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
import os

df = pd.read_csv(os.path.abspath('src/performance/performance.csv'))

# Fusionner les valeurs pour pas avoir 5x5x3 mais juste 5x3.
grouped = df.groupby(['Programme', 'Nombre de Threads']).agg(
    mean_time=('Temps d\'execution', 'mean'), # Moyenne.
    std_time=('Temps d\'execution', 'std') # Ecart-type.
).reset_index()

# NBR Thread et programmes (pour éviter que l'un ait pas et que rien ne marche).
pivoted = grouped.pivot(index='Nombre de Threads', columns='Programme', values='mean_time')
std_pivoted = grouped.pivot(index='Nombre de Threads', columns='Programme', values='std_time')
thread_counts = pivoted.index
programs = pivoted.columns

fig, ax = plt.subplots(figsize=(10, 6))
bar_width = 0.1
index = np.arange(len(thread_counts))

colors = ['#87A878', '#C7CCB9', '#DBF9B8'] # LE VERT <3

for i, program in enumerate(programs):
    mean_values = pivoted[program].fillna(0) # .fillna() -> empêcher d'avoir une erreur si pas de valeur
    std_values = std_pivoted[program].fillna(0)
    ax.bar(index + i * bar_width, mean_values, bar_width,
           yerr=std_values, label=program,
           color=colors[i % len(colors)], capsize=5) # Dessiner le graph

# Graph
ax.set_xlabel('Nombre de Threads') # Label du X
ax.set_ylabel('Temps d\'exécution (s)') # Label du Y
ax.set_title('Temps d\'exécution moyen par programme et nombre de threads') # Titre du graph
ax.set_xticks(index + bar_width * (len(programs) - 1) / 2)
ax.set_xticklabels(thread_counts) # Bons nombre de thread afficher
ax.set_ylim(0, pivoted.max().max() + 0.5) # Changer taille max du Y
ax.legend()
plt.tight_layout() # Plus joli
plt.show()
