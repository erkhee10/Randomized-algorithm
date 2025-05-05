import matplotlib.pyplot as plt
import pandas as pd
import numpy as np


def visualize_bst_performance():

    tree_sizes = [5000000, 10000000, 20000000]
    avg_nodes_visited = [22.34, 23.67, 24.89]  
    

    plt.figure(figsize=(10, 6))
    plt.plot(tree_sizes, avg_nodes_visited, 'bo-', linewidth=2, markersize=8)

    plt.title('Randomized BST Performance', fontsize=16)
    plt.xlabel('Tree Size', fontsize=14)
    plt.ylabel('Average Nodes Visited', fontsize=14)
    
    plt.grid(True, linestyle='--', alpha=0.7)
    

    for x, y in zip(tree_sizes, avg_nodes_visited):
        plt.annotate(f"{y:.2f}", 
                    (x, y),
                    textcoords="offset points", 
                    xytext=(0, 10),
                    ha='center',
                    fontsize=12)
        plt.tight_layout()
    
    plt.savefig('bst_performance.png', dpi=300)

    plt.show()

def visualize_from_csv(filename="randomized_bst_results.csv"):
    try:
        df = pd.read_csv(filename)
        
        df.columns = ['tree_size', 'avg_nodes_visited']
        
        plt.figure(figsize=(10, 6))
        plt.plot(df['tree_size'], df['avg_nodes_visited'], 'bo-', linewidth=2, markersize=8)
        
        plt.title('Randomized BST Performance (from CSV)', fontsize=16)
        plt.xlabel('Tree Size', fontsize=14)
        plt.ylabel('Average Nodes Visited', fontsize=14)
        
        plt.grid(True, linestyle='--', alpha=0.7)

        for x, y in zip(df['tree_size'], df['avg_nodes_visited']):
            plt.annotate(f"{y:.2f}", 
                        (x, y),
                        textcoords="offset points", 
                        xytext=(0, 10),
                        ha='center',
                        fontsize=12)
        
        plt.tight_layout()
    
        plt.savefig('bst_performance_from_csv.png', dpi=300)
        
        plt.show()
        
    except FileNotFoundError:
        print(f"File {filename} not found. Using example data instead.")
        visualize_bst_performance()

if __name__ == "__main__":
    visualize_from_csv()