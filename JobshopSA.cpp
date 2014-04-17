<<<<<<< HEAD
=======
// JobshopSA.cpp : Defines the entry point for the console application.
//

>>>>>>> 53ba009455e5be27018416b55f157f18276bc4e0
#include <cstdio>
#include <cctype>
#include <list>
#include "Graph.h"
#include "Schedule.h"
#define PB push_back
#define INST_BEASLEY 0
#define INST_TAILLARD 1

using namespace std;

<<<<<<< HEAD
int main(int argc, char* argv[])
{
    double modulation = 0.85;
    double initial_temperature = 50;
    double alpha_warming = 2;
    double alpha_cooling = 0.85;
    int cooling_age_length = 300;
    double warming_threshold = 0.95;
    int max_moves_without_improvement = 2000;
=======
/* 
i	argv[i]:
0:	nazwa pliku wykonywanego
1:	plik z instancja
2:	Beasley/Taillard
3:	limit dla noJobs
4:	modulation (int)
5:	initial_temperature (double)
6:      alpha_warming (double)
7: 	alpha_cooling (double)
8:	cooling_age_length (int)
9:	warming_threshold (double)
10:	max_moves_without_improvement (int)

*/
int main(int argc, char* argv[])
{
        int modulation = 1;
        double initial_temperature = 100;
        double alpha_warming = 1.0;
        double alpha_cooling = 0.85;
        int cooling_age_length = 300;
        double warming_threshold = 0.9;
        int max_moves_without_improvement = 1000;
>>>>>>> 53ba009455e5be27018416b55f157f18276bc4e0
        
	int instance_format = INST_BEASLEY;
	int limit = 0;
	double totaltime;
	// INTERPRETACJA ARGUMENTOW WIERSZA POLECEN
	/*
		WYBOR FORMATU INSTANCJI 
		jobshop inst_file - Beasley (default)
		jobshop inst_file {T|B} - Taillard/Beasley
		jobshop inst_file {T|B} n - Beasley lub Taillard z ograniczeniem
								do n pierwszych zadan
	*/
	if (argc == 1)
	{
		printf("usage: jobshop inst_file [ {T|B} {0..infinity} ]\n");
		return 0;
	}
	else
	{
		if (argc >= 3)
		{
			if (tolower(argv[2][0]) == 't') instance_format = INST_TAILLARD;
			else if (tolower(argv[2][0]) == 'b') instance_format = INST_BEASLEY;
			else printf("Niewlasciwy parametr formatu instancji - domyslnie Beasley\n");
			if (argc >= 4)
<<<<<<< HEAD
            {
				if (atoi(argv[3]) != 0) limit = atoi(argv[3]);       
            }
=======
                        {
				if (atoi(argv[3]) != 0) limit = atoi(argv[3]);
                                if (argc >= 5) if (atoi(argv[4]) != 0) modulation = atoi(argv[4]);
                                if (argc >= 6) if (atof(argv[5]) != 0) initial_temperature  = atof(argv[5]);
                                if (argc >= 7) if (atof(argv[6]) != 0) alpha_warming  = atof(argv[6]);
                                if (argc >= 8) if (atof(argv[7]) != 0) alpha_cooling  = atof(argv[7]);
                                if (argc >= 9) if (atoi(argv[8]) != 0) cooling_age_length  = atoi(argv[8]);
                                if (argc >= 10) if (atof(argv[9]) != 0) warming_threshold  = atof(argv[9]);
                                if (argc >= 11) if (atoi(argv[10]) != 0) max_moves_without_improvement  = atoi(argv[10]);
                                
                        }
>>>>>>> 53ba009455e5be27018416b55f157f18276bc4e0
		}
                
	}

	// OTWIERANIE PLIKU PODANEGO W 1 ARGUMENCIE
	FILE *source;
	if((source = fopen(argv[1], "r")) == NULL)
	{
		printf("Blad otwarcia pliku!\n");
		return 1;
	}

	//pamietamy argumenty z maina (potrzebne do testow)
	

	 // LICZBA ZADAN I MASZYN - odczytywanie 1-ego wiersza
	int noJobs, noMachines;
	long int smieci;
<<<<<<< HEAD
	int lower_bound = 0, upper_bound = 0;
=======
	int lower_bound, upper_bound;
>>>>>>> 53ba009455e5be27018416b55f157f18276bc4e0
        if (instance_format == INST_TAILLARD)
                fscanf(source, "%d %d %ld %ld %d %d", &noJobs, &noMachines, &smieci, &smieci, &lower_bound, &upper_bound);
        else
            fscanf(source, "%d %d", &noJobs, &noMachines);

	//s - uszeregowanie
	Schedule s(noMachines);
<<<<<<< HEAD
	s.lower_bound = lower_bound;
	s.upper_bound = upper_bound;
=======
>>>>>>> 53ba009455e5be27018416b55f157f18276bc4e0

	//wektory m - machines, t - times --> dla danego Joba
	vector<int> m, t;
 	// Beasley
 	if (instance_format == INST_BEASLEY)
 	{
 		// Jesli istnieje ograniczenie liczby szeregowanych zadan:
 		if (limit > 0 && limit <= noJobs) 
 			noJobs = limit;
 		int i, j;
		for(int i = 0; i < noJobs; i++)
		{
			m.clear();
			t.clear();
			//poniewaz czytamy ba biezaco czas i maszyne dla danej operacji,
			//to mozemy 2 wektory pomocnicze wykorzystac dla kazdego zadania
			//(wystarcza 2 pomocnicze)
			for(int j = 0; j < noMachines; j++)
			{
				int machine_tmp, time_tmp;
				fscanf(source, "%d %d ", &machine_tmp, &time_tmp);
				m.push_back(machine_tmp);
				t.push_back(time_tmp);
			}
			s.add_job(t, m);
		}
	}

	// Taillard
	else if (instance_format == INST_TAILLARD)
	{
		// ile zadan jest do pominiecia
		int redundant_tasks = 
			(limit > 0 && limit <= noJobs)
			? noJobs - limit
			: 0;
		
		noJobs -= redundant_tasks;

		//tablica 2d (wektor wektorow) do przechowywania czasow
		//potrzebna tylko do instancji taillarda
		//(musimy na raz zapamietac wszystkie czasy, w przeciwienstwie do beasley'a)
		vector<vector<int> > tailard_times;
		//resize: liczba wierszy to liczba jobow, 
		//liczba kolumn nie okreslona (bo robimy push_back)
		tailard_times.resize(noJobs);
		for(int i = 0; i < noJobs; i++)
		{
			tailard_times[i].resize(0);
		}

		for(int i = 0; i < noJobs; i++)
		{
			tailard_times[i].clear();
		}
<<<<<<< HEAD
		// odczytalismy juz noJobs i noMachines, teraz pomijamy smieci z pierwszego wiersza
=======
		// odczytalismy już noJobs i noMachines, teraz pomijamy smieci z pierwszego wiersza
>>>>>>> 53ba009455e5be27018416b55f157f18276bc4e0
		char skip[256];
		void *result = fgets(skip, 256, source);
		// pomijamy smieci ("Times")
		result = fgets(skip, 100, source);
		
		for(int i = 0; i < noJobs; i++)
		{			
			for(int j = 0; j < noMachines; j++)
			{
				int time_tmp;
				fscanf(source, "%d ", &time_tmp);
				tailard_times[i].push_back(time_tmp);
			}
		}
		// pomijamy reszte zadan
		for (int i = 0; i < redundant_tasks; i++)
		{
			result = fgets(skip, 256, source);
		}
		// pomijamy smieci ("Machines")
		result = fgets(skip, 100, source);
		for(int i = 0; i < noJobs; i++)
		{
			m.clear();
			//tutaj vector m tez jest wykorzystywany wiele razy
			for(int j = 0; j < noMachines; j++)
			{
				int machine_tmp;
				fscanf(source, "%d ", &machine_tmp);
				m.push_back(machine_tmp-1);
				// -1 bo w instancjach tai maszyny sa numerowane od 1
			}
			s.add_job(tailard_times[i], m);
		}
		// pomijamy reszte zadan
		for (int i = 0; i < redundant_tasks; i++)
		{
			result = fgets(skip, 256, source);
		}
	}

	if(fclose(source) != 0)
		printf("Blad zamkniecia pliku %s!\n", argv[1]);
	
	//jesli jest tylko 1 zadane, to rozwiazanie jest gotowe
	if(noJobs == 1)
	{
		s.create_graph();
		printf("%d\n", s.get_cmax());
		s.print_start_times();
		printf("\n");
	}
	//rozwiazanie metoda simulated annealing
	else 
		totaltime = s.solve_using_SA(modulation, initial_temperature, alpha_warming, alpha_cooling, cooling_age_length, warming_threshold, max_moves_without_improvement);

	//zapisywanie wynikow jakosciowych do pliku
        
<<<<<<< HEAD
=======
    //char buffer[40];
	//sprintf(buffer, "wyniki_%d_%s", noJobs, argv[1]);
>>>>>>> 53ba009455e5be27018416b55f157f18276bc4e0
	char buffer[] = "wyniki.txt";
	FILE *times;
	times = fopen(buffer, "a");
	//fprintf(stderr, "\nOtworzono plik: %s\n", buffer);
	fprintf(times, "%s\t%d\t%d\t%f\t%d", argv[1], noJobs, noMachines, totaltime, s.get_cmax());
	if(instance_format == INST_TAILLARD)
		fprintf(times, "\t%d\t%d\n", lower_bound, upper_bound);
	else 
		fprintf(times, "\n");

	if(fclose(times) != 0)
		printf("Blad zamkniecia pliku %s!\n", buffer);

<<<<<<< HEAD
	//fprintf(stderr, "przetworzono %s\n", argv[1]);
=======
	fprintf(stderr, "przetworzono %s\n", argv[1]);
>>>>>>> 53ba009455e5be27018416b55f157f18276bc4e0

	return 0;
}

