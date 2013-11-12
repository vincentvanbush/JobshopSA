#include "Schedule.h"
#define DEBUG

#ifdef DEBUG
#define debug(...) fprintf(stderr, __VA_ARGS__)
#else
#define debug(...) /*(__VA_ARGS__)*/
#endif

Schedule::Schedule(int machines)
{
	/*
		Inicjalizujac uszeregowanie, ustalamy liczbe operacji na zero, liczbe maszyn na machines
		i do wektora wag krawedzi emanujacych z wierzcholkow dodajemy 0, czyli wage krawedzi idacych
		z wierzcholka startowego.
	*/
	operations_number = 0;
	this->machines.resize(machines);
	this->vertex_weights.push_back(0);
}


Schedule::~Schedule()
{
}


void Schedule::add_job(vector<int> times, vector<int> machines)
{
	/*
		Dodajemy zlecenie - wektory times i machines musza byc takiej samej dlugosci.
		i-temu elementowi times odpowiada i-ty element machines.
	*/
	if (times.size() != machines.size()) // cos poszlo nie tak :)
	{
		debug("---number of times(%d) doesn't match the number of machines(%d)!\n");
		return;
	}

	int no_operations = times.size(); // liczba operacji dodawanego zlecenia

	jobs.push_back(Job(times, machines)); // dodajemy zlecenie do listy zlecen
	for (int i=0; i<no_operations; i++) // dla kazdej operacji w ramach zlecenia
	{
		operations_number++; // zwiekszamy aktualna liczbe operacji (zwroc uwage, ze symbolizuje ona tez numer wierzcholka ktory reprezentuje ta operacje)
		this->machines[machines[i]].push_back(operations_number); // zapisujemy, ze operacja jest wykonywana na i-tej maszynie
		vertex_weights.push_back(times[i]); // zapisujemy wage kazdej krawedzi emanujacej z wierzcholka
	}
}


void Schedule::create_graph(void)
{
	/*
		Tworzy graf dysjunkcyjny na podstawie juz istniejacych informacji o zleceniach, maszynach
		i wagach krawedzi.
	*/
	Graph g(operations_number + 2); // liczba operacji + zrodlo + ujscie

	int source = 0;
	int sink = operations_number + 1; // ujscie = ostatni wierzcholek

	int current_operation_vertex = 1; // zaczynamy od wierzcholka 1, bo zawiera pierwsza operacje pierwszego zlecenia

	int jobs_number = jobs.size(); // liczba zlecen
	for (int i=0; i<jobs_number; i++) // dla kazdego zlecenia
	{
		int job_operations = jobs[i].first.size(); // ile operacji jest w tym zleceniu
		g.add_arc(0, current_operation_vertex, 0); // dodajemy luk z 0 do pierwszej operacji zlecenia o dlugosci 0
		for (int j=0; j<job_operations-1; j++) // dla kazdej operacji w ramach zlecenia (poza ostatnia)
		{
			// dodajemy luk z operacji j-tej do j+1 o dlugosci rownej czasowi wykonania j-tej
			g.add_arc(current_operation_vertex, current_operation_vertex+1, jobs[i].first[j]);
			current_operation_vertex++; // przechodzimy do nastepnego wierzcholka
		}
		// dodajemy luk z ostatniej operacji do ujscia o dlugosci rownej czasowi ostatniej operacji
		g.add_arc(current_operation_vertex, sink, jobs[i].first[job_operations-1]);
		current_operation_vertex++;
	}

	for (int i=0; i<machines.size(); i++) // dla kazdej maszyny tworzymy klike
	{
		Machine m = machines[i]; // m = wierzcholki symbolizujace operacje na i-tej maszynie
		int no_ops_machine = m.size(); // liczba operacji na danej maszynie

		vector<int> l; // dlugosci operacji na i-tej maszynie

		for (int j=0; j<no_ops_machine; j++) // dla kazdej operacji
		{
			l.push_back(vertex_weights[m[j]]); // ...pobierz dlugosc lukow z niej wychodzacych
		}

		g.create_acyclic_clique(m, l); // stworz acykliczna klike

	}


	g.export_dot();

	this->graph = g;
}


int Schedule::get_cmax(void)
{
	/*
		Obliczamy dlugosc uszeregowania - sciezke krytyczna ze zrodla do ujscia grafu.
	*/
	return graph.max_distances(0).back();
}
