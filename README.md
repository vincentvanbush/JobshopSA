JobshopSA
=========

Job-shop scheduler using a simulated annealing heuristic.

Please use gcc/g++ to compile: Graph.cpp, Graph.h, Schedule.cpp, Schedule.h, JobshopSA.cpp into executable file. Then please run the program from console:

executable-file instance-file [ {T|B} {0..infinity} ]

where instance-file is any txt file from "instances" folder
T|B - Taillard's or Beasley's instance; Beasley's by default
0..infinity - limit for number of jobs; 0 means no limit; no limit by default
