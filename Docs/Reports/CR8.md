#Project Arimaa – CR8 : Wednesday, November 5th
Presents : Everyone, with Christian Raymond, and Nikos Parlavantzas
##Order of business :
1. Parallelization method implementation (Mikail)
2. Packages with rules implementation (Baptiste)
3. MCTS simple implementation + missing presentation (Benoit)
4. General Software architecture : diagrams (Gabriel)
5. API & I/O diagrams (Prateek & Dan)
6. Plan of the second report

##Information :
-

##Planification
Task		|						Responsible	|	Deadline
------------ | ------------- | --------
OpenMP implementation					 | 	Not decided	 | Begin after 11/05
OpenMP report part						 | 	Benoit		 | 	11/19
MPI report part						 | 	Mikail			 | 11/19
OpenAcc report part						 | 	Baptiste	 | 	11/19
First draft of the report				 | 		Everyone	 | 	11/19
Last draft of the report				 | 		Everyone	 | 	11/26
Due date for the report					 | 	Dan			 | 11/27
Finish the game application			 | 		Gabriel	 | 	12/18

##Details of the tasks
Detailed Task						|					Responsible
------------ | -------
Parallelization method  (method implement on the PC : tree, leaf, roots, others to implement)	|	Mikail	
Software architecture (work on I/O with Dan and API with Prateek) It is about program structures, general UML (containing task rules) , diagrams work			|Gabriel	
Determine I/O data (use table data format json? , handle generalization of the game, using dll library ? How handling events, board screen, clics (the MCTS program will calculate positions) Global interactions of the user with the program							|Dan		
API (Graphic interface to choose, command lines optional ?, move with mouse or keyboard,  work with Dan/Gabriel SFML						 who takes care of the screen									|Prateek		
MCTS simple (basic implementation on one computer, one thread for now In C++, we need to test it, on Tic Tac Toe for instance						|Benoit	
OpenMP implementation basic (OpenACC later) (require MCTS implementation/parallelization methods)						|Not decided
Packages with rules (a dll library ?), what to do with the screen display (Dan) How to write rules (Pascal Garcia language can be used), an abstract class implemented by Arimaa methods  and later other games (examples of possible methods : MakeMove, List of available moves		|Baptiste
Go deeper on OpenMP (references) CPU parallelization					|	Benoit
Go deeper on MPI (references) computers parallelization					|	Mikail
Go deeper on OpenAcc (references) GPU parallezation						|Baptiste

##Next meeting : 11/12
