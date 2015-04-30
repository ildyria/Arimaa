#Project Arimaa – CR21 : Wednesday, April, 29th
Presents : Gabriel, Benoît, Prateek, with Christian Raymond and Nikos Parlavantzas
##Order of business :
- MCTS : scalability (*Benoît*)
- MPI asynchronous voting (*Gabriel*)

##MCTS scalability

- OpenMP does its job just fine
- Performances fail to rise beyond a certain limit for MCTS (~3 times for 8 cores)

##Parallelization

- Works with asynchrounous calls
- Will soon be paired with MCTS and Connect4

##Time management

- Leave ZeroMQ
- Only try on Grid5000 if enough time


##Next meeting : 05/06
Tasks :
- Benoît :
  * Finish the implementation of the simple version of Arimaa
- Gabriel :
  * Apply MPI parallelisation on MCTS and test it
