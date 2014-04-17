#pragma once
#include <vector>
#include <utility>
#include <map>
#include <cstdlib>
#include <cmath>
#include <time.h>
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
	vector<int> get_start_times(void);
	vector<int> get_job_lengths(void);
	double solve_using_SA(double modulation, double initial_temperature, double alpha_warming, double alpha_cooling, int cooling_age_length, double warming_threshold, int max_moves_without_improvement);
	bool success_chance(int cmax, int new_cmax, double temperature, double modulation);
	vector<int> select_arc(deque<int> critpath);
	void print_start_times(void);
	Graph graph; // graf dysjunkcyjny
	int lower_bound;
	int upper_bound;
private:
	Jobs jobs; // przechowuje zlecenia - kazde zlecenie ma postac pair<times, machines>
	
	int operations_number; // sumaryczna liczba operacji
	Machines machines; // wektor maszyn - kazda maszyna to wektor wierzcholkow reprez. operacje na niej wykonywane
	vector<int> vertex_weights; // zawiera wagi lukow emanujacych z danych wierzcholkow
};

