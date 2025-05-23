import pandas as pd
from scipy import stats
import matplotlib.pyplot as plt
import seaborn as sns
import numpy as np
from genetic_algorithm import GeneticAlgorithm, Encoding, CrossoverType

def run_experiment(function, function_name, domain, encoding_type, crossover_type, 
                  num_runs=30, max_evaluations=10000, population_size=100):
    """
    Run multiple independent experiments with the same GA configuration
    
    Parameters:
    - function: The fitness function to optimize
    - function_name: Name of the function for reporting
    - domain: List of domain boundaries [x_min, x_max, y_min, y_max]
    - encoding_type: Type of encoding (binary or real-valued)
    - crossover_type: Type of crossover operation
    - num_runs: Number of independent runs
    - max_evaluations: Maximum number of fitness evaluations per run
    - population_size: Population size
    
    Returns:
    - Dictionary with experiment results
    """
    # Calculate max generations based on max evaluations and population size
    max_generations = max_evaluations // population_size
    
    results = {
        'function': function_name,
        'encoding': encoding_type.name,
        'crossover': crossover_type.name,
        'best_fitness': [],
        'best_solution_x': [],
        'best_solution_y': [],
        'convergence_gen': []
    }
    
    print(f"Running experiment: {function_name}, {encoding_type.name}, {crossover_type.name}")
    
    for run in range(num_runs):
        print(f"  Run {run+1}/{num_runs}")
        
        # Create and run GA
        ga = GeneticAlgorithm(
            fitness_function=function,
            encoding_type=encoding_type,
            crossover_type=crossover_type,
            domain=domain,
            population_size=population_size,
            max_generations=max_generations,
            crossover_rate=0.8,
            mutation_rate=0.1,
            elite_size=2,
            binary_precision=16
        )
        
        run_result = ga.optimize(verbose=False)
        
        # Record results
        results['best_fitness'].append(run_result['best_fitness'])
        results['best_solution_x'].append(run_result['best_solution'][0])
        results['best_solution_y'].append(run_result['best_solution'][1])
        
        # Determine convergence generation (when best fitness stops improving significantly)
        best_history = run_result['best_fitness_history']
        for gen in range(1, len(best_history)):
            if abs(best_history[gen] - best_history[gen-1]) < 1e-6:
                results['convergence_gen'].append(gen)
                break
        else:
            results['convergence_gen'].append(len(best_history))
    
    # Calculate statistics
    results['mean_fitness'] = np.mean(results['best_fitness'])
    results['std_fitness'] = np.std(results['best_fitness'])
    results['min_fitness'] = np.min(results['best_fitness'])
    results['max_fitness'] = np.max(results['best_fitness'])
    results['mean_convergence_gen'] = np.mean(results['convergence_gen'])
    
    return results

def run_all_experiments(functions, domains, function_names, num_runs=30, max_evaluations=10000):
    """Run experiments for all combinations of functions, encodings, and crossover types"""
    all_results = []
    
    for func, domain, func_name in zip(functions, domains, function_names):
        # Binary encoding with 1-point crossover
        results_b1 = run_experiment(
            func, func_name, domain, Encoding.BINARY, CrossoverType.ONE_POINT, 
            num_runs, max_evaluations
        )
        all_results.append(results_b1)
        
        # Binary encoding with 2-point crossover
        results_b2 = run_experiment(
            func, func_name, domain, Encoding.BINARY, CrossoverType.TWO_POINT, 
            num_runs, max_evaluations
        )
        all_results.append(results_b2)
        
        # Real-valued encoding with arithmetic crossover
        results_ra = run_experiment(
            func, func_name, domain, Encoding.REAL, CrossoverType.ARITHMETIC, 
            num_runs, max_evaluations
        )
        all_results.append(results_ra)
        
        # Real-valued encoding with BLX-α crossover
        results_rb = run_experiment(
            func, func_name, domain, Encoding.REAL, CrossoverType.BLX_ALPHA, 
            num_runs, max_evaluations
        )
        all_results.append(results_rb)
    
    return all_results

def statistical_analysis(all_results):
    """Perform statistical analysis on experiment results"""
    # Create a DataFrame for easier analysis
    df = pd.DataFrame()
    
    for result in all_results:
        temp_df = pd.DataFrame({
            'Function': [result['function']] * len(result['best_fitness']),
            'Encoding': [result['encoding']] * len(result['best_fitness']),
            'Crossover': [result['crossover']] * len(result['best_fitness']),
            'Best Fitness': result['best_fitness'],
            'Convergence Generation': result['convergence_gen']
        })
        df = pd.concat([df, temp_df], ignore_index=True)
    
    # Summary statistics by configuration
    summary = df.groupby(['Function', 'Encoding', 'Crossover']).agg({
        'Best Fitness': ['mean', 'std', 'min', 'max'],
        'Convergence Generation': ['mean', 'std']
    }).reset_index()
    
    print("Summary Statistics:")
    print(summary)
    
    # Statistical tests
    print("\nStatistical Tests:")
    
    # Group results by function
    for func_name in df['Function'].unique():
        func_df = df[df['Function'] == func_name]
        
        print(f"\nAnalysis for {func_name}:")
        
        # Compare encodings
        binary_results = func_df[func_df['Encoding'] == 'BINARY']['Best Fitness']
        real_results = func_df[func_df['Encoding'] == 'REAL']['Best Fitness']
        
        t_stat, p_value = stats.ttest_ind(binary_results, real_results, equal_var=False)
        print(f"  Binary vs Real encoding (t-test): t={t_stat:.4f}, p={p_value:.4f}")
        
        if p_value < 0.05:
            print("  Significant difference between encodings")
        else:
            print("  No significant difference between encodings")
        
        # Compare crossovers within each encoding
        for encoding in ['BINARY', 'REAL']:
            encoding_df = func_df[func_df['Encoding'] == encoding]
            crossover_types = encoding_df['Crossover'].unique()
            
            if len(crossover_types) > 1:
                type1_results = encoding_df[encoding_df['Crossover'] == crossover_types[0]]['Best Fitness']
                type2_results = encoding_df[encoding_df['Crossover'] == crossover_types[1]]['Best Fitness']
                
                t_stat, p_value = stats.ttest_ind(type1_results, type2_results, equal_var=False)
                print(f"  {crossover_types[0]} vs {crossover_types[1]} crossover ({encoding}): t={t_stat:.4f}, p={p_value:.4f}")
                
                if p_value < 0.05:
                    print(f"  Significant difference between crossover types for {encoding}")
                else:
                    print(f"  No significant difference between crossover types for {encoding}")
    
    # Visualize results
    plt.figure(figsize=(14, 8))
    sns.boxplot(x='Function', y='Best Fitness', hue='Encoding', data=df)
    plt.title('Comparison of Encoding Types')
    plt.yscale('log')
    plt.grid(True, alpha=0.3)
    plt.show()
    
    plt.figure(figsize=(14, 8))
    sns.boxplot(x='Encoding', y='Best Fitness', hue='Crossover', data=df)
    plt.title('Comparison of Crossover Types')
    plt.yscale('log')
    plt.grid(True, alpha=0.3)
    plt.show()
    
    return df, summary