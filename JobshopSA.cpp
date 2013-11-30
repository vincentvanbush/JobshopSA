// JobshopSA.cpp : Defines the entry point for the console application.
//

#include <cstdio>
#include <cctype>
#include "Graph.h"
#include "Schedule.h"

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






	/*printf("TWORZENIE ROZWIAZANIA POCZATKOWEGO DLA PEWNEJ INSTANCJI\n");
	//		machines	times
	// J1	0,1,2		10,8,4
	// J2	1,3,0,2		8,3,5,6
 	// J3	0,1,3		4,7,3

	Schedule s(4);
	vector<int> t1, m1, t2, m2, t3, m3;
	// J1
	t1.push_back(10);	t1.push_back(8);	t1.push_back(4);
	m1.push_back(0);	m1.push_back(1);	m1.push_back(2);
	// J2
	t2.push_back(8);	t2.push_back(3);	t2.push_back(5);	t2.push_back(6);
	m2.push_back(1);	m2.push_back(0);	m2.push_back(3);	m2.push_back(2);
	// J3
	t3.push_back(4);	t3.push_back(7);	t3.push_back(3);
	m3.push_back(0);	m3.push_back(1);	m3.push_back(3);

	s.add_job(t1, m1); // J1
	s.add_job(t2, m2); // J2
	s.add_job(t3, m3); // J3

	s.create_graph();

	printf("Cmax = %d\n", s.get_cmax());

	printf("\n\nPRZYKLADOWY GRAF DYSJUNKCYJNY\n");
	// http://i.imgur.com/mwUW5Dk.png
	Graph sch(12);

	// job1
	sch.add_arc(0, 1, 0);
	sch.add_arc(1, 2, 10);
	sch.add_arc(2, 3, 8);
	sch.add_arc(3, 11, 4);
	// job2
	sch.add_arc(0, 4, 0);
	sch.add_arc(4, 5, 8);
	sch.add_arc(5, 6, 3);
	sch.add_arc(6, 7, 5);
	sch.add_arc(7, 11, 6);
	// job3
	sch.add_arc(0, 8, 0);
	sch.add_arc(8, 9, 4);
	sch.add_arc(9, 10, 7);
	sch.add_arc(10, 11, 3);

	vector<int> v;
	vector<int> l;
	// machine1
	v.push_back(8); v.push_back(1); v.push_back(5);
	l.push_back(4); l.push_back(10); l.push_back(3);
	sch.create_acyclic_clique(v, l);
	// machine2
	v.clear(); l.clear();
	v.push_back(4); v.push_back(9); v.push_back(2);
	l.push_back(8); l.push_back(7); l.push_back(8);
	sch.create_acyclic_clique(v, l);
	// machine3
	v.clear(); l.clear();
	v.push_back(3); v.push_back(7);
	l.push_back(4); l.push_back(6);
	sch.create_acyclic_clique(v, l);
	// machine4
	v.clear(); l.clear();
	v.push_back(6); v.push_back(10);
	l.push_back(5); l.push_back(3);
	sch.create_acyclic_clique(v, l);

	sch.critical_path(0, 11);

//	sch.export_dot();

	


	printf("\n\nTWORZENIE ACYKLICZNIE SKIEROWANEJ KLIKI\n");
	Graph clique(5);
	vector<int> w;
	w.push_back(4);
	w.push_back(3);
	w.push_back(2);
	w.push_back(1);
	w.push_back(0);
	vector<int> d;
	d.push_back(1); d.push_back(1); d.push_back(1); d.push_back(1); d.push_back(1);
	clique.create_acyclic_clique(w, d);
	clique.topological_sort();

	printf("\n\nSET_ARC_LENGTH\n");	


	Graph h(2);
	h.add_arc(0, 1, 5);
	printf("%d\n", h.get_arc_length(0, 1));
	h.set_arc_length(0, 1, 10);
	printf("%d\n", h.get_arc_length(0, 1));

	
	printf("\n\nCRITICAL PATH\n");	

	Graph g(12);
	// source=0, sink=11

	// conjunctive arcs
	g.add_arc(0, 1, 0);
	g.add_arc(1, 2, 10);
	g.add_arc(2, 3, 8);
	g.add_arc(3, 11, 4);

	g.add_arc(0, 4, 0);
	g.add_arc(4, 5, 8);
	g.add_arc(5, 6, 3);
	g.add_arc(6, 7, 5);
	g.add_arc(7, 11, 6);

	g.add_arc(0, 8, 0);
	g.add_arc(8, 9, 4);
	g.add_arc(9, 10, 7);
	g.add_arc(10, 11, 3);

	// disjunctive arcs
	g.add_arc(8, 1, 4);
	g.add_arc(5, 1, 3);
	g.add_arc(5, 8, 3);

	g.add_arc(4, 2, 8);
	g.add_arc(4, 9, 8);
	g.add_arc(2, 9, 8);

	g.add_arc(6, 10, 5);

	g.add_arc(3, 7, 4);

	g.critical_path(0, 11);*/
		
	return 0;
}

