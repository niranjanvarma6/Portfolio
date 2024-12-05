import matplotlib.pyplot as plt
import random

# Class for the game play
class TicTacToe:
    def __init__(play):
        play.reset()
    #reset board
    def reset(play):
        play.board = [[0, 0, 0], [0, 0, 0], [0, 0, 0]]
        play.current_player = 1

    def display(play):
        symbols = {0: '-', 1: 'X', -1: 'O'}
        for row in play.board:
            print(' '.join(symbols[cell] for cell in row))
        print()
    # moves being made and switch of player
    def make_move(play, row, col):
        if play.board[row][col] != 0:
            return False
        play.board[row][col] = play.current_player
        play.current_player *= -1
        return True
    # winner checked on the board 
    def findWinner(play):
        for player in [1, -1]:
            for i in range(3):
                if all(play.board[i][j] == player for j in range(3)) or all(play.board[j][i] == player for j in range(3)):
                    return player
            if all(play.board[i][i] == player for i in range(3)) or all(play.board[i][2-i] == player for i in range(3)):
                return player
        if all(cell != 0 for row in play.board for cell in row):
            return 0  
        return None  
# Class for the Agent
class QAgent:
    def __init__(play, alpha=0.5, gamma=0.9, epsilon=0.1, agent_experience_rate=0.95):
        play.alpha = alpha
        play.gamma = gamma
        play.epsilon = epsilon
        play.agent_experience_rate = agent_experience_rate
        play.q_table = {}  
    # Examine the board from the Agent's view
    def getBoardPiece(play, board):
        return tuple(tuple(row) for row in board)
    # Returns q value for particular state/position on board
    def get_q_values(play, state):
        if state not in play.q_table:
            play.q_table[state] = [[0 for _ in range(3)] for _ in range(3)]
        return play.q_table[state]

    def AgentAction(play, state):
        # some exploration probability
        if random.uniform(0, 1) < play.epsilon:
            row, col = random.choice([(r, c) for r in range(3) for c in range(3) if state[r][c] == 0])
        # random selection from list of good actions to be taken
        else:
            q_values = play.get_q_values(play.getBoardPiece(state))
            max_value = max(q_values[r][c] for r in range(3) for c in range(3) if state[r][c] == 0)
            best_actions = [(r, c) for r in range(3) for c in range(3) if q_values[r][c] == max_value and state[r][c] == 0]
            row, col = random.choice(best_actions)
        return row, col
    # Q values updated for each t step
    def QChange(play, state, action, reward, next_state):
        state_key = play.getBoardPiece(state)
        next_state_key = play.getBoardPiece(next_state)
        q_values = play.get_q_values(state_key)
        next_q_values = play.get_q_values(next_state_key)
        row, col = action
        q_values[row][col] = q_values[row][col] + play.alpha * (reward + play.gamma * max(next_q_values[r][c] for r in range(3) for c in range(3)) - q_values[row][col])
    # epsilon decreases at a constant value with a threshold of nothing lower than 0.02
    def AgentExperience(play):
        play.epsilon = max(0.02, play.epsilon * play.agent_experience_rate)

def play_game(agent, game, train=True, human=False):
    game.reset()
    state = [row[:] for row in game.board]
    
    while True:
        # Check if the current player is the agent
        if game.current_player == 1:  
            row, col = agent.AgentAction(state)
            game.make_move(row, col)
        else:  # Human player's turn
            if human:
                game.display()
                while True:
                    try:
                        row = int(input("Enter the row (0, 1, 2): "))
                        col = int(input("Enter the column (0, 1, 2): "))
                        if game.make_move(row, col):
                            break
                        else:
                            print("Invalid move. Try again.")
                    except ValueError:
                        print("Please enter valid integers for row and column.")
            else: #random agent
                row, col = random.choice([(r, c) for r in range(3) for c in range(3) if game.board[r][c] == 0])
                game.make_move(row, col)

        # Check for winner after the move
        winner = game.findWinner()
        next_state = [row[:] for row in game.board]
        if winner is not None:
            if train:
                if winner == 1:
                    reward = 1 
                elif winner == -1:
                    reward = -1  
                else: 
                    reward = 0.5
                agent.QChange(state, (row, col), reward, next_state)
            break
        
        # Update Q-values during training
        if train and game.current_player == 1:
            agent.QChange(state, (row, col), 0, next_state) 
        state = next_state      
    return winner


agent = QAgent()
game = TicTacToe()
epochs = 100
scores = []

for epoch in range(epochs):
    agent.AgentExperience()
    # Training
    for _ in range(10000):
        play_game(agent, game)
    score = 0
    # Random agent games
    for _ in range(10):
        result = play_game(agent, game, train=False)
        if result == 1:
            score += 1
        elif result == 0:
            score += 0.5
    scores.append(score / 10)

# Plotting of scores
plt.plot(range(epochs), scores)
plt.xlabel('Epoch')
plt.ylabel('Average Score')
plt.show()

# Game against human
while True:
    result = play_game(agent, game, train=False, human=True)
    game.display()
    if result == 0:
        print("It's a draw!")
    elif result == 1:
        print("The agent wins!")
    elif result == -1:
        print("You win!")  
    play_again = input("Do you want to play again? (yes/no): ").strip().lower()
    if play_again == 'no':
        break
