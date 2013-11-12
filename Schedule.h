#pragma once
#include <vector>
#include <utility>
#include <map>
#include "Graph.h"
using namespace std;

typedef pair<vector<int>, vector<int> > Job;
typedef vector<Job> Jobs;
typedef vector<int> Machine;
typedef vector<Machine> Machines;

class Schedule
{
public:
	Schedule(int machines); // inicjalizacja uszeregowania dla pewnej liczby maszyn
	~Schedule();
	void add_job(vector<int> times, vector<int> machines); // dodanie zlecenia 
	void create_graph(void); // utworzenie grafu na podstawie juz istniejacych zlecen
	int get_cmax(void); // zwraca Cmax
private:
	Jobs jobs; // przechowuje zlecenia - kazde zlecenie ma postac pair<times, machines>
	Graph graph; // graf dysjunkcyjny
	int operations_number; // sumaryczna liczba operacji
	Machines machines; // wektor maszyn - kazda maszyna to wektor wierzcholkow reprez. operacje na niej wykonywane
	vector<int> vertex_weights; // zawiera wagi lukow emanujacych z danych wierzcholkow
};

