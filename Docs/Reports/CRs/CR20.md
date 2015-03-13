#Project Arimaa – CR20 : Wednesday, March, 11th
Presents : Gabriel, Benoît, Prateek, with Christian Raymond and Nikos Parlavantzas
##Order of business :
- MCTS : new issues / improvements (*Benoît*)
- MPI and ØMQ tests (*Gabriel*)
- Rodrigue (*Benoît*)

##MCTS
- Now able to work during the opponent's turn
- For distributed calculus, use a UCT function that prioritizes deep branches

##Parallelization
- MPI : works at the department (at last)
- ØMQ : might be installed at the department (?)
- First implementation of the Master-Worker strategy (with MPI) with dummy values

##Rodrigue
- Too old, processors have low frequency
- Bigger tree but fewer simulations

##Next meeting : 03/18
Tasks :
- Benoît :
  * Finish the implementation of the simple version of Arimaa
- Gabriel :
  * Apply distribution on MCTS and test it (at least with MPI)
