// JobshopSA.cpp : Defines the entry point for the console application.
//

#include <cstdio>
#include <cctype>
#include "Graph.h"
#include "Schedule.h"
#define PB push_back
#define INST_BEASLEY 0
#define INST_TAILLARD 1

using namespace std;

int main(int argc, char* argv[])
{

	int instance_format = INST_BEASLEY;
	int limit = 0;
	
	// INTERPRETACJA ARGUMENTOW WIERSZA POLECEN
	/*
		WYBOR FORMATU INSTANCJI 
		jobshop inst_file - Beasley (default)
		jobshop inst_file {T|B} - Taillard/Beasley
		jobshop inst_file {T|B} n - Beasley lub Taillard z ograniczeniem
								do n pierwszych maszyn
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
				limit = atoi(argv[3]);
		}
	}

	// OTWIERANIE PLIKU PODANEGO W 1 ARGUMENCIE
	FILE *source;
	if((source = fopen(argv[1], "r")) == NULL)
	{
		printf("Blad otwarcia pliku!\n");
		return 1;
	}


	 // LICZBA ZADAN I MASZYN - odczytywanie 1-ego wiersza
	int noJobs, noMachines;
	fscanf(source, "%d %d\n", &noJobs, &noMachines);

	//s - uszeregowanie
	Schedule s(noMachines);

	//wektory m - machines, t - times --> dla danego Joba
	vector<int> m, t;
 	// Beasley
 	if (instance_format == INST_BEASLEY)
 	{
 		// Jesli istnieje ograniczenie liczby szeregowanych zadan:
 		if (limit > 0 && limit <= noJobs) 
 			noJobs = limit;
 		int i, j;
		for(i = 0; i < noJobs; i++)
		{
			m.clear();
			t.clear();

			for(j = 0; j < noMachines; j++)
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
		vector<vector<int> > tailard_times;
		//resize: liczba wierszy to liczba jobow, 
		//liczba kolumn nie okreslona (bo robimy push_back)
		tailard_times.resize(noJobs);
		for(i = 0; i < noJobs; i++)
			tailard_times[i].clear();
		// odczytalismy już noJobs i noMachines, teraz pomijamy smieci z pierwszego wiersza
		char skip[256];
		void *result = fgets(skip, 256, source);
		// pomijamy smieci ("Times")
		result = fgets(skip, 100, source);
		int i, j;
		for(i = 0; i < noJobs; i++)
		{			
			for(j = 0; j < noMachines; j++)
			{
				int time_tmp;
				fscanf(source, "%d ", &time_tmp);
				tailard_times[i].push_back(time_tmp);
			}
		}
		// pomijamy reszte zadan
		for (i = 0; i < redundant_tasks; i++)
		{
			result = fgets(skip, 256, source);
		}
		// pomijamy smieci ("Machines")
		result = fgets(skip, 100, source);
		for(i = 0; i < noJobs; i++)
		{
			m.clear();

			for(j = 0; j < noMachines; j++)
			{
				int machine_tmp;
				fscanf(source, "%d ", &machine_tmp);
				m.push_back(machine_tmp);
			}
			s.add_job(tailard_times[i], m);
		}
		// pomijamy reszte zadan
		for (i = 0; i < redundant_tasks; i++)
		{
			result = fgets(skip, 256, source);
		}
	}
	if(fclose(source) != 0)
		printf("Blad zamkniecia pliku %s!\n", argv[1]);
	
	s.solve_using_SA();
	return 0;

}

