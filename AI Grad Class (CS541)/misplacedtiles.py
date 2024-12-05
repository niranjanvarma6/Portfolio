import queue

N = 9  
MAX_QUEUE_SIZE = 1000
goal_state = [1, 2, 3, 4, 5, 6, 7, 8, 0]

class PuzzleState:
    def __init__(self, puzzle, heuristic, cost, parent):
        self.puzzle = puzzle
        self.heuristic = heuristic
        self.cost = cost
        self.f = cost + heuristic  
        self.parent = parent
    def __lt__(self, other):
        return self.f < other.f

def manhattan_distance(puzzle):
    distance = 0
    for i in range(N):
        value = puzzle[i]
        if value != 0:
            goal_row, goal_col = divmod(value - 1, 3)
            current_row, current_col = divmod(i, 3)
            distance += abs(current_row - goal_row) + abs(current_col - goal_col)
    return distance

def misplaced_tiles(puzzle):
    misplaced = 0
    for i in range(N):
        if puzzle[i] != goal_state[i] and puzzle[i] != 0:
            misplaced += 1
    return misplaced

def combined_heuristic(puzzle):
    misplaced = misplaced_tiles(puzzle)
    manhattan = manhattan_distance(puzzle)
    heuristic_value = misplaced + manhattan
    return heuristic_value

def generate_state(current, new_blank_pos, blank_pos, heuristic_type, is_a_star):
    new_puzzle = current.puzzle[:]
    new_puzzle[blank_pos], new_puzzle[new_blank_pos] = new_puzzle[new_blank_pos], new_puzzle[blank_pos]
    if heuristic_type == 1:
        heuristic = misplaced_tiles(new_puzzle)
    elif heuristic_type == 2:
        heuristic = manhattan_distance(new_puzzle)
    elif heuristic_type == 3:
        heuristic = combined_heuristic(new_puzzle)
    
    cost = current.cost + 1
    new_state = PuzzleState(new_puzzle, heuristic, cost, current)
    if not is_a_star:  
        new_state.f = heuristic
    
    return new_state

def check_goal_state(puzzle):
    return puzzle == goal_state
def find_blank_tile(puzzle):
    return puzzle.index(0)

def swap_tiles(puzzle, i, j):
    puzzle[i], puzzle[j] = puzzle[j], puzzle[i]

def print_puzzle(puzzle):
    for i in range(N):
        print(f"{puzzle[i]} ", end="")
        if (i + 1) % 3 == 0:
            print()
    print()

def solve_puzzle(initial_puzzle, heuristic_type, is_a_star):
    initial_state = PuzzleState(initial_puzzle, 0, 0, None)
    
    priority_queue = queue.PriorityQueue()
    priority_queue.put(initial_state)
    visited = set()
    visited.add(tuple(initial_puzzle))
    solution_found = False
    solution_cost = 0
    while not priority_queue.empty():
        current = priority_queue.get()
        if check_goal_state(current.puzzle):
            solution_found = True
            solution_cost = current.cost 
            solution_path = []
            while current:
                solution_path.append(current)
                current = current.parent
            solution_path.reverse()
            print("Path to solution:")
            for state in solution_path:
                print_puzzle(state.puzzle)
            break

        blank_pos = find_blank_tile(current.puzzle)
        
        possible_moves = []
        if blank_pos >= 3: 
            possible_moves.append(blank_pos - 3) 
        if blank_pos < 6:  
            possible_moves.append(blank_pos + 3)
        if blank_pos % 3 != 0:  
            possible_moves.append(blank_pos - 1)
        if blank_pos % 3 != 2:  
            possible_moves.append(blank_pos + 1)

        for new_blank_pos in possible_moves:
            new_state = generate_state(current, new_blank_pos, blank_pos, heuristic_type, is_a_star)
            new_state_tuple = tuple(new_state.puzzle)
            if new_state_tuple not in visited:
                priority_queue.put(new_state)
                visited.add(new_state_tuple)

    if solution_found:
        print(f"Solution cost: {solution_cost}")
    else:
        print("Solution not found.")

if __name__ == "__main__":
    initial_puzzle = [8, 7, 1, 6, 0, 2, 5, 4, 3]
    heuristic_choice = int(input("Choose heuristic:\n 1. Misplaced Tiles\n 2.Manhattan Distance\n 3. Combined Heuristic\n"))
    algorithm_choice = int(input("Choose algorithm:\n 1. Best-First Search\n 2. A* Search\n"))
    is_a_star = (algorithm_choice == 2)
    solve_puzzle(initial_puzzle, heuristic_choice, is_a_star)
