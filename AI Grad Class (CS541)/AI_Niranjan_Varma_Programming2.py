import random
import csv


PopulationSize = 50
NumIterations = 500
MutationPct = 0.05


def CalculateFitness(board):
    non_attacking_pairs = 28
    for i in range(8):
        for j in range(i + 1, 8):
            if board[i] == board[j]  or abs(board[i] - board[j]) == abs(i - j):
                non_attacking_pairs -= 1
    return non_attacking_pairs


def SelectParents(population, fit_list):
    total_fitness = sum(fit_list)
    probability = [f / total_fitness for f in fit_list]
    parents = random.choices(population, probability, k=2)
    return parents


def Crossover(parent1, parent2):
    point = random.randint(1, 7)
    child1 = parent1[:point] + parent2[point:]
    child2 = parent2[:point] + parent1[point:]
    return child1, child2


def Mutation(child):
    if random.random() < MutationPct:
        index = random.randint(0, 7)
        new_value = random.choice([i for i in range(8) if i != child[index]])
        child[index] = new_value
    return child


def genetic_algorithm():
    population = [random.sample(range(8), 8) for _ in range(PopulationSize)]
    best_fitness = []
    average_fitness = []
    examples = {}


    for generation in range(NumIterations):
        fit_list = [CalculateFitness(individual) for individual in population]
        new_population = []
        while len(new_population) < PopulationSize:
            parent1, parent2 = SelectParents(population, fit_list)
            child1, child2 = Crossover(parent1, parent2)
            new_population.append(Mutation(child1))
            new_population.append(Mutation(child2))
        population = new_population[:PopulationSize]
        best = max(fit_list)
        average = sum(fit_list) / PopulationSize
        best_fitness.append(best)
        average_fitness.append(average)
        if generation in {0, NumIterations /4 -1,  NumIterations/2 -1, 3*NumIterations /4-1, NumIterations - 1}:
            examples[generation + 1] = population[:5]
        print(f"Generation {generation + 1}: Best Fitness = {best}, Average Fitness = {average}")
        if best == 28:
            break
    return best_fitness, average_fitness, examples


def write_to_csv(generations, best_fitness, average_fitness, filename='data.csv'):
    with open(filename, mode='w', newline='') as file:
        writer = csv.writer(file)
        writer.writerow(["Generation", "Best Fitness", "Average Fitness"])
        for generation, best, average in zip(generations, best_fitness, average_fitness):
            writer.writerow([generation, best, average])




best_fitness, average_fitness, examples = genetic_algorithm()
generations = list(range(1, len(best_fitness) + 1))
write_to_csv(generations, best_fitness, average_fitness)


for generation, individuals in examples.items():
    print(f"\nIndividuals sampled from Generation {generation}:")
    for individual in individuals:
        print(f"Board: {individual}, Fitness: {CalculateFitness(individual)}")
