#Projet Arimaa – CR1 : Mercredi 17 Septembre
Présents : Toute l'équipe, Christian Raymond et un intervenant extérieur inconnu

##Ordre du jour :
- Définition du projet et des objectifs
- Choix du jeu
- Découverte des différents outils/langages possibles

##Information échangées :

- Sur quel jeu allons-nous travailler ? Proposition d'Arimaa, les dames et le morpion étant résolus, et les échecs trop stratégique, trop complexe et déjà trop étudié.
- Codage du jeu par *Gabriel* en C++
- Jouer au jeu pour mémoriser les règles et pouvoir faire des stratégies
- Quel langage utiliser ? Proposition du C++ car les applications seraient plus facile à coder, et compatible avec les cours de C++ du moment. Il sera toujours possible d'utiliser d'autres langages plus adaptés et implémenter des interfaces entre ces derniers.
- Quel logiciel utiliser pour la parallélisation ? OpenMP mais vérifier la compatibilité. *Tâche à effectuer par Baptiste et Mikail*
- Quel machine utiliser ? On prendra Grid5000, un cluster de machines virtuelles parallèles de l'IRISA pour avoir une puissance de calcul plus élevée. *Vérifier de même la compatibilité par Dan*
- Utilisation de Microsoft Project pour gérer les tâches du projet. La présentation du logiciel sera effectuée un peu plus tard dans l'année.
- Choix de la forge : Github privé. Refus d'utiliser la forge de l'INSA pour des raisons de manque de stabilité de l'ENT, utilisation du Git privé de Benoit.
- Planification de tâches à court terme : établir le cahier des charges pour le premier compte-rendu en Octobre. Le problème est que le groupe ne contient que 5 éléments avec 3 partants au deuxième semestre. Ainsi, commencer le code avant Janvier pourrait être une bonne chose. Alternative : ajout d'une sixième personne.
- Choix de l'environnement Linux
- Établir la répartition des tâches. Trouver un coordinateur. 
- But du projet : effectuer des comparaisons d'algorithmes de chercheurs pour Arimaa (Gameplaying : développer des stratégies IA)
- Techniques pour calculer le meilleur chemin pour gagner une partie :
- Technique du boulet de canon : on tente tout et n'importe quoi et on finit par avoir une trame du meilleur chemin
Montecarlo : dérouler une partie au hasard jusqu'à la fin et en tirer des statistiques en le répétant, pour pouvoir explorer en fonction des statistiques.
- Tirer profit de la parallélisation avec une heuristique
- Prix de 10000 € si l'ordinateur (un seul) peut gagner sur un ensemble de 6 parties contre un être humain.
- Technologies additionnelles à étudier : Java Scala, Akka
- État de l'art (étude de thèses faites sur le jeu Arimaa) pour partir sur des bonnes bases. *Tâche à effectuer par Benoît.*

##Planification

Tâche  | Responsable | Deadline
------------ | ------------- | --------
Initiation au jeu Arimaa | Tout le monde | aucune
Compatibilité OpenMP | Baptiste, Mikail | 24/9
Compatibilité Grid5000 | Dan | 24/9
État de l'art, thèses |  Benoît  | 24/9 extendable
Établir le cahier des charges | Chef de projet | Octobre

##Date de la prochaine réunion : 24/09

##Documents additionnels :
- Adresse du GitHub du projet : https://github.com/ildyria/Arimaa