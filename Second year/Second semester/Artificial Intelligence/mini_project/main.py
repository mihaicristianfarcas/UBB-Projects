# Import necessary modules
from functions import rastrigin, ackley, visualize_function
from genetic_algorithm import GeneticAlgorithm, Encoding, CrossoverType, plot_convergence
from analysis import run_experiment, run_all_experiments, statistical_analysis
import numpy as np
import matplotlib.pyplot as plt

def main():
    # Define the functions
    functions = [rastrigin, ackley]
    function_names = ['Rastrigin', 'Ackley']
    domains = [
        [-5.12, 5.12, -5.12, 5.12],  # Rastrigin
        [-5.0, 5.0, -5.0, 5.0]        # Ackley
    ]
    
    # Visualize the functions
    print("Visualizing the benchmark functions...")
    visualize_function(rastrigin, domains[0], 'Rastrigin')
    visualize_function(ackley, domains[1], 'Ackley')
    
    # Run experiments
    print("\nRunning optimization experiments...")
    results = run_all_experiments(functions, domains, function_names, num_runs=30, max_evaluations=5000)
    
    # Perform statistical analysis
    print("\nPerforming statistical analysis...")
    df, summary = statistical_analysis(results)
    
    print("\nExperiment completed successfully!")
    return results, df, summary

if __name__ == "__main__":
    results, df, summary = main()