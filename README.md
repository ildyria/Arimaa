#Project Arimaa – CR18 : Wednesday, February, 25th
Presents : Gabriel, Benoît, with Christian Raymond and Nikos Parlavantzas
##Order of business :
- MCTS : new issues / improvements (*Benoît*)
- MPI and ØMQ tests (*Gabriel*)

##MCTS
- Single array with index : +50% nodes, +50% time for pruning

##Parallelization
- Have the UCT function for the workers build fewer deeper branches
- Collection of result : proceed by voting ?
- Compare work distribution with Pachi

##Next meeting : 03/04
Tasks :
- Benoît :
  * Keep improving MCTS
  * Implement MCTS on a simpler version of Arimaa (6x6, 3 types of pawns)
- Gabriel :
  * Keep testing MPI / ØMQ
