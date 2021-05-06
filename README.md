# checkers-bot
My implementation of a checkers robot to play against classmates without the use of true machine learning/AI.
Framework (the game rules and board itself) already exists; Player interface is given and must be implemented, which is then injencted into the framework.
Algorithm decides best moves by iteratively traverse the list of your checkers pieces and calculate its best move, which is then compared to the other pieces' best moves. 
Utilized a priority queue to handle this concept easily.
