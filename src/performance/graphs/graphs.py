import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
import os

# Charger les données depuis le fichier CSV
df = pd.read_csv(os.path.abspath('src/performance/performance.csv'))

# Regrouper les données par programme et nombre de threads
grouped = df.groupby(['Programme', 'Nombre de Threads']).agg(
    mean_time=('Temps d\'execution', 'mean'), # Calcul de la moyenne
    std_time=('Temps d\'execution', 'std') # Calcul de l'ecart type
).reset_index()

program_types = grouped['Programme'].unique()
color = '#A3A1D1'

# Créer un graphique pour chaque programme
for program in program_types:
    fig, ax = plt.subplots(figsize=(10, 6))

    # Filtrer les données pour le programme actuel
    program_data = grouped[grouped['Programme'] == program]
    thread_counts = sorted(program_data['Nombre de Threads'].unique())
    index = np.arange(len(thread_counts))

    # Extraire les valeurs de grouped (ecart-type et moyenne)
    mean_values = program_data.set_index('Nombre de Threads')['mean_time'].reindex(thread_counts, fill_value=0)
    std_values = program_data.set_index('Nombre de Threads')['std_time'].reindex(thread_counts, fill_value=0)

    # Dessiner les barres avec l'ecart type (en noir)
    bar_width = 0.4
    ax.bar(index, mean_values, bar_width,
           yerr=std_values, label=program,
           color=color, alpha=0.7, capsize=5)

    # Ajouter les points pour visualiser les moyennes
    ax.plot(index, mean_values, color=color, marker='o', linestyle='dotted')
    program_name = ""
    if program == "phil": program_name = " le problème des philosophes"
    if program == "prod": program_name = "le problème des producteurs-consommateurs"
    if program == "rw": program_name = "le problème des lecteurs et écrivains"

    # Parametres
    ax.set_title(f'Performance pour {program_name}')
    ax.set_xlabel('Nombre de Threads')
    ax.set_ylabel('Temps d\'execution(s)')
    ax.set_xticks(index)
    ax.set_xticklabels(thread_counts)
    ax.grid(axis='y', linestyle='-', alpha=0.7)

    # Sauver en PNG
    plt.tight_layout()
    plt.savefig(f'img/{program}_performance.png')
    plt.close()

print("Graphs saved successfully.")
