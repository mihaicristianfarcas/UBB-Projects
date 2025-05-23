import random
import numpy as np
from enum import Enum
import time

class Encoding(Enum):
    BINARY = 1
    REAL = 2

class CrossoverType(Enum):
    # Binary crossovers
    ONE_POINT = 1
    TWO_POINT = 2
    
    # Real-valued crossovers
    ARITHMETIC = 3
    BLX_ALPHA = 4

class GeneticAlgorithm:
    def __init__(self, 
                 fitness_function, 
                 encoding_type,
                 crossover_type, 
                 domain,
                 population_size=100, 
                 crossover_rate=0.8, 
                 mutation_rate=0.1, 
                 elite_size=2,
                 max_generations=100,
                 binary_precision=16):
        """
        Initialize the Genetic Algorithm
        
        Parameters:
        - fitness_function: Function to optimize (minimize)
        - encoding_type: Type of encoding (binary or real-valued)
        - crossover_type: Type of crossover operation
        - domain: List with domain boundaries [x_min, x_max, y_min, y_max]
        - population_size: Size of the population
        - crossover_rate: Probability of crossover
        - mutation_rate: Probability of mutation
        - elite_size: Number of best individuals to keep unchanged
        - max_generations: Maximum number of generations
        - binary_precision: Precision for binary encoding (bits per variable)
        """
        self.fitness_function = fitness_function
        self.encoding_type = encoding_type
        self.crossover_type = crossover_type
        self.domain = domain
        self.population_size = population_size
        self.crossover_rate = crossover_rate
        self.mutation_rate = mutation_rate
        self.elite_size = elite_size
        self.max_generations = max_generations
        self.binary_precision = binary_precision
        
        # Validate the configuration
        self._validate_configuration()
        
        # For tracking performance
        self.best_fitness_history = []
        self.avg_fitness_history = []
        self.fitness_evaluations = 0
        
    def _validate_configuration(self):
        """Validate the GA configuration"""
        if self.encoding_type == Encoding.BINARY:
            if self.crossover_type not in [CrossoverType.ONE_POINT, CrossoverType.TWO_POINT]:
                raise ValueError("For binary encoding, crossover type must be ONE_POINT or TWO_POINT")
        elif self.encoding_type == Encoding.REAL:
            if self.crossover_type not in [CrossoverType.ARITHMETIC, CrossoverType.BLX_ALPHA]:
                raise ValueError("For real encoding, crossover type must be ARITHMETIC or BLX_ALPHA")
    
    def initialize_population(self):
        """Initialize the population based on the encoding type"""
        population = []
        
        if self.encoding_type == Encoding.BINARY:
            # For binary encoding, create random bit strings
            chromosome_length = 2 * self.binary_precision  # 2 variables, each with binary_precision bits
            for _ in range(self.population_size):
                chromosome = [random.randint(0, 1) for _ in range(chromosome_length)]
                population.append(chromosome)
        else:  # REAL encoding
            x_min, x_max, y_min, y_max = self.domain
            for _ in range(self.population_size):
                # Each individual is represented as [x, y]
                x = random.uniform(x_min, x_max)
                y = random.uniform(y_min, y_max)
                population.append([x, y])
                
        return population
    
    def decode_binary(self, chromosome):
        """Decode a binary chromosome to real values"""
        x_min, x_max, y_min, y_max = self.domain
        half_length = len(chromosome) // 2
        
        # Extract the binary strings for x and y
        x_bits = chromosome[:half_length]
        y_bits = chromosome[half_length:]
        
        # Convert binary to decimal
        x_decimal = 0
        for bit in x_bits:
            x_decimal = (x_decimal << 1) | bit
            
        y_decimal = 0
        for bit in y_bits:
            y_decimal = (y_decimal << 1) | bit
        
        # Map to the domain range
        max_decimal = 2**half_length - 1
        x = x_min + (x_max - x_min) * x_decimal / max_decimal
        y = y_min + (y_max - y_min) * y_decimal / max_decimal
        
        return x, y
    
    def calculate_fitness(self, population):
        """Calculate fitness values for the population"""
        fitness_values = []
        
        for individual in population:
            if self.encoding_type == Encoding.BINARY:
                x, y = self.decode_binary(individual)
            else:  # REAL encoding
                x, y = individual
                
            # Track the number of fitness evaluations
            self.fitness_evaluations += 1
            
            # Calculate fitness (we're minimizing, so lower is better)
            fitness = self.fitness_function(x, y)
            fitness_values.append(fitness)
            
        return fitness_values
    
    def select_parents(self, population, fitness_values):
        """Select parents using tournament selection"""
        selected_parents = []
        tournament_size = 3
        
        for _ in range(self.population_size):
            # Select tournament participants
            tournament_indices = random.sample(range(self.population_size), tournament_size)
            tournament_fitness = [fitness_values[i] for i in tournament_indices]
            
            # Find the best individual in the tournament (lowest fitness as we're minimizing)
            winner_idx = tournament_indices[tournament_fitness.index(min(tournament_fitness))]
            selected_parents.append(population[winner_idx])
            
        return selected_parents
    
    def crossover_binary_one_point(self, parent1, parent2):
        """Perform one-point crossover for binary encoding"""
        if random.random() < self.crossover_rate:
            crossover_point = random.randint(1, len(parent1) - 1)
            child1 = parent1[:crossover_point] + parent2[crossover_point:]
            child2 = parent2[:crossover_point] + parent1[crossover_point:]
            return child1, child2
        return parent1, parent2
    
    def crossover_binary_two_point(self, parent1, parent2):
        """Perform two-point crossover for binary encoding"""
        if random.random() < self.crossover_rate:
            length = len(parent1)
            point1 = random.randint(1, length - 2)
            point2 = random.randint(point1 + 1, length - 1)
            
            child1 = parent1[:point1] + parent2[point1:point2] + parent1[point2:]
            child2 = parent2[:point1] + parent1[point1:point2] + parent2[point2:]
            return child1, child2
        return parent1, parent2
    
    def crossover_real_arithmetic(self, parent1, parent2):
        """Perform arithmetic crossover for real-valued encoding"""
        if random.random() < self.crossover_rate:
            alpha = random.random()  # Blend parameter
            child1 = [alpha * parent1[0] + (1 - alpha) * parent2[0],
                      alpha * parent1[1] + (1 - alpha) * parent2[1]]
            child2 = [(1 - alpha) * parent1[0] + alpha * parent2[0],
                      (1 - alpha) * parent1[1] + alpha * parent2[1]]
            return child1, child2
        return parent1, parent2
    
    def crossover_real_blx_alpha(self, parent1, parent2):
        """Perform BLX-α crossover for real-valued encoding"""
        if random.random() < self.crossover_rate:
            alpha = 0.5  # Standard BLX-α parameter
            
            # For each dimension (x and y)
            child1, child2 = [], []
            for i in range(2):  # 2 dimensions (x and y)
                # Calculate the extended range based on parent values
                min_val = min(parent1[i], parent2[i])
                max_val = max(parent1[i], parent2[i])
                extension = alpha * (max_val - min_val)
                
                # Generate children within the extended range
                lower_bound = max(self.domain[i*2], min_val - extension)
                upper_bound = min(self.domain[i*2+1], max_val + extension)
                
                child1.append(random.uniform(lower_bound, upper_bound))
                child2.append(random.uniform(lower_bound, upper_bound))
                
            return child1, child2
        return parent1, parent2
    
    def crossover(self, parents):
        """Apply the appropriate crossover method based on encoding and crossover type"""
        children = []
        
        # Shuffle parents to create random pairs
        shuffled_parents = parents.copy()
        random.shuffle(shuffled_parents)
        
        # Perform crossover in pairs
        for i in range(0, len(shuffled_parents), 2):
            if i + 1 < len(shuffled_parents):
                parent1 = shuffled_parents[i]
                parent2 = shuffled_parents[i + 1]
                
                # Apply the appropriate crossover method
                if self.encoding_type == Encoding.BINARY:
                    if self.crossover_type == CrossoverType.ONE_POINT:
                        child1, child2 = self.crossover_binary_one_point(parent1, parent2)
                    else:  # TWO_POINT
                        child1, child2 = self.crossover_binary_two_point(parent1, parent2)
                else:  # REAL encoding
                    if self.crossover_type == CrossoverType.ARITHMETIC:
                        child1, child2 = self.crossover_real_arithmetic(parent1, parent2)
                    else:  # BLX_ALPHA
                        child1, child2 = self.crossover_real_blx_alpha(parent1, parent2)
                
                children.extend([child1, child2])
            else:
                # If there's an odd number of parents, keep the last one unchanged
                children.append(shuffled_parents[i])
        
        return children
    
    def mutate_binary(self, individual):
        """Mutate a binary-encoded individual"""
        for i in range(len(individual)):
            if random.random() < self.mutation_rate:
                individual[i] = 1 - individual[i]  # Flip the bit
        return individual
    
    def mutate_real(self, individual):
        """Mutate a real-valued individual"""
        x_min, x_max, y_min, y_max = self.domain
        
        for i in range(len(individual)):
            if random.random() < self.mutation_rate:
                # Apply Gaussian mutation
                mutation_strength = 0.1  # Control the mutation strength
                domain_range = [x_max - x_min, y_max - y_min][i]
                
                # Add random noise
                delta = random.gauss(0, mutation_strength * domain_range)
                individual[i] += delta
                
                # Ensure the value remains within the domain
                min_val = [x_min, y_min][i]
                max_val = [x_max, y_max][i]
                individual[i] = max(min_val, min(max_val, individual[i]))
                
        return individual
    
    def mutate(self, population):
        """Apply mutation based on the encoding type"""
        mutated_population = []
        
        for individual in population:
            if self.encoding_type == Encoding.BINARY:
                mutated_individual = self.mutate_binary(individual)
            else:  # REAL encoding
                mutated_individual = self.mutate_real(individual)
            
            mutated_population.append(mutated_individual)
            
        return mutated_population
    
    def elitism(self, population, fitness_values, new_population):
        """Preserve the best individuals (elitism)"""
        if self.elite_size == 0:
            return new_population
        
        # Find indices of the elite individuals (those with lowest fitness, as we're minimizing)
        elite_indices = sorted(range(len(fitness_values)), key=lambda i: fitness_values[i])[:self.elite_size]
        
        # Replace random individuals in the new population with elite individuals
        for i, elite_idx in enumerate(elite_indices):
            new_population[i] = population[elite_idx]
            
        return new_population
    
    def optimize(self, verbose=True):
        """Run the GA optimization process"""
        start_time = time.time()
        
        # Initialize population
        population = self.initialize_population()
        
        # Main optimization loop
        for generation in range(self.max_generations):
            # Calculate fitness for the current population
            fitness_values = self.calculate_fitness(population)
            
            # Track best and average fitness
            best_fitness = min(fitness_values)
            avg_fitness = sum(fitness_values) / len(fitness_values)
            
            self.best_fitness_history.append(best_fitness)
            self.avg_fitness_history.append(avg_fitness)
            
            # Print progress
            if verbose and generation % 10 == 0:
                print(f"Generation {generation}: Best Fitness = {best_fitness:.6f}, Avg Fitness = {avg_fitness:.6f}")
            
            # Select parents
            parents = self.select_parents(population, fitness_values)
            
            # Create next generation through crossover
            offspring = self.crossover(parents)
            
            # Apply mutation
            offspring = self.mutate(offspring)
            
            # Apply elitism
            population = self.elitism(population, fitness_values, offspring)
        
        # Final fitness evaluation for the last population
        fitness_values = self.calculate_fitness(population)
        
        # Find the best individual
        best_idx = fitness_values.index(min(fitness_values))
        if self.encoding_type == Encoding.BINARY:
            best_solution = self.decode_binary(population[best_idx])
        else:  # REAL encoding
            best_solution = population[best_idx]
        
        best_fitness = fitness_values[best_idx]
        
        # Print results
        if verbose:
            print("\nOptimization completed")
            print(f"Total time: {time.time() - start_time:.2f} seconds")
            print(f"Fitness evaluations: {self.fitness_evaluations}")
            print(f"Best solution: x = {best_solution[0]:.6f}, y = {best_solution[1]:.6f}")
            print(f"Best fitness: {best_fitness:.6f}")
        
        return {
            'best_solution': best_solution,
            'best_fitness': best_fitness,
            'best_fitness_history': self.best_fitness_history,
            'avg_fitness_history': self.avg_fitness_history,
            'fitness_evaluations': self.fitness_evaluations
        }

def plot_convergence(results, title):
    """Plot the convergence of the GA over generations"""
    plt.figure(figsize=(10, 6))
    plt.plot(results['best_fitness_history'], label='Best Fitness')
    plt.plot(results['avg_fitness_history'], label='Average Fitness')
    plt.xlabel('Generation')
    plt.ylabel('Fitness (lower is better)')
    plt.title(f'GA Convergence: {title}')
    plt.legend()
    plt.grid(True)
    plt.yscale('log')  # Log scale often shows convergence better
    plt.show()