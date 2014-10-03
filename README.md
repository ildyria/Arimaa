#Project Arimaa – CR3 : Wednesday, October 1st 
Presents : Everyone except Prateek, Christian Raymond, and Nikos Parlavantzas
##Order of business :
1. Choices of conferences
2. New incoming in our group
3. Gabriel's progress on the Arimaa game
4. "État de l'art" by Benoît and Mikail (latest developments)
5. MS Project (Baptiste)
6. Plan of first report (Dan)
7. Work on the deadlines

##Information :
- Order of conferences (In French):

1. Future car conference, between K2000 and James Bond's car
2. Medical applications for tomorrow
3. How to become an entrepreneur at the INSA de Rennes
4. Sustainable development debate/study of the agribusiness
We will attend 3 of these during the Telecoms' week (week almost only to work on the project)

- How welcome Prateek to the group ? All reports will be in English, and talks as well (this report as well). Is he interested by the project ? If(yes) then {Welcome !}
- Check with Mr Anquetil (in charge) for the notation of the English. Prateek accepted, so there would be more work, but we will learn to deal with English.
- Try to think and write in English. For the oral presentation, prepare our sentences. Avoid  learning everything by heart.
- For all reports, try to use simple sentences, not big sentences referring to each others. We will use Latex. Do not focus on the number of page of the report. Focus on the contents. Dan will look after the report, to avoid repetitions and preserve coherence of the document. 
- What's up with Gabriel's Arimaa game ? He solved some other bugs, changed icons, switch to C++11 with the SFML library (for the graphic window). One major improvement of the new C++ : null is replaced by nullptr : no possibility of nullPointerException anymore.
- There may be more problems : play the game more to find bugs. He added saving of game (file backup), and he improved the selection of icon at the beginning of the game (if it is not possible anymore to add a piece, the piece's color change). The teachers like this game, and advise us to use it for the oral presentation, to help people understand the game.
- "Etat de l'art" (latest developments of the MonteCarlo Research – MCTS) Presentation of the work of Mikail and Benoit on the parallelization techniques. The two methods (roots and trees) have pros and cons, maybe it will be good to mix the two, as a hybrid method. We will add these concepts in our December's report.
- OpenMP could be used in this project to parallelize calculations (just do a sequential scheme and OpenMP parallelize everything). But C++11 add the multithreading (but we need to think in multithreading), so what's the best ? We need to choose one of them for next week. Work on comparison of speed, will the difference between the two be negligible? Christian Raymond prefers OpenMP.
- The MPI technology operates the synchronisation between between processors, and send data for the aggregation. It becomes useless if figures changes only in the first line (one iteration). We can use C++ socket to communicate with the computer. Grid5000 won't have the same speed, so we wouldn’t be able to use MPI. We could use the library Corba in C++ or use the C library of thread. To improve our schemes, we could use the C++ library Boost.org (Standardisation and improvements of the usual C++). We could use the protocol rest to communicate between machines, with web services.
- Assessment of the Arimaa's thesis by Mikail. He made a recap of it (look at the links above). We usually use the capacity of CPU for our calculation, but it may be more interesting to use the capacity of GPU.
- Benoit presented Accelerating Parallel Kamilrochi (look at the link above). He studied the MCTS applied to the game GO. We will use the results in other reports (not the next one).
- Baptiste worked on MS Project to plan what's remaining to do (everything). We need to take notes of the name and time conceded of the task we were doing. Baptiste will show us with a tutorial what we have to do to add data in the file.

##Planification
Task		|						Responsible	|	Deadline
------------ | ------------- | --------
Mail to Marin Bertier					|	Dan	|		10/02
Organise the plan of the rapport (and main ideas)	|	Dan		|	10/08
GPU VS CPU powering our calculus		|	Baptiste	|	10/08
C++11 VS OpenMP for multithreading (for 3 solutions)	| Benoit		|	10/08
Look for a new parallelization and strategies	|	Benoit & Mikail	| 10/08
Bibliography with Zotero or Jabref to a file .bibtex		| Mikail	|		10/08
Study the C++ library Boost			|		Gabriel	|	10/08
Learn Arimaa's rules, familiarize with the project	|	Prateek	|	10/08 or more
Take notes of time and everything we have done.	|	Everyone	|	10/08 and more
Finish the application game				|	Gabriel	|	12/18

##Next meeting : 10/08
##Links (on the GitHub Arimaa) :
Arimaa\Docs\Resume des methodes de parallelisation\resume.txt is a recap of the method of parallelization
Arimaa\Docs\MCTS Generalites\AcceleratingParallel_KamilRocki.pdf is what Benoit has found on MCTS research trees.