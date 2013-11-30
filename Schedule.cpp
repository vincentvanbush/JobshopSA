#include "Schedule.h"

#define DEBUG

#ifdef DEBUG
#define debug(...) fprintf(stderr, __VA_ARGS__)
#else
#define debug(...) /*(__VA_ARGS__)*/
#endif

//na potrzeby solve_using_SA():
#define WARMING 1
#define COOLING 0
#define INITIAL_TEMPERATURE 25
#define ALPHA_WARMING 1.02
#define ALPHA_COOLING 0.995
#define TIME_EXCEEDED false
#define MODULATION 1
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

	vector<vector<int> > clusters;
	clusters.resize(jobs_number);
	int n=1;
	for (int i=0; i<jobs_number; i++)
		for (int j=0; j<jobs[i].first.size(); j++)
			clusters[i].push_back(n++);
	g.export_dot(clusters);

	this->graph = g;
}


int Schedule::get_cmax(void)
{
	/*
		Obliczamy dlugosc uszeregowania - sciezke krytyczna ze zrodla do ujscia grafu.
	*/
	return graph.max_distances(0).back();
}

vector<int> Schedule::get_start_times(void)
{
	/*
		Zwraca momenty startu wszystkich operacji - w zasadzie jest to po prostu obliczenie
		maksymalnych odleglosci w grafie ze zrodla do kazdego wierzcholka.
		Wywalamy jednak pozycje pierwsza, ktora zawsze wynosi 0
		Musza one zostac pozniej recznie oddzielone enterami na potrzeby outputu!
	*/
	vector<int> v = graph.max_distances(0);
	v.erase(v.begin(), v.begin()+1);
	v.pop_back();
	return v;
}

vector<int> Schedule::get_job_lengths(void)
{
	vector<int> v;
	for (int i=0; i<jobs.size(); i++)
		v.push_back(jobs[i].first.size());
	return v;
}

void Schedule::print_start_times(void)
{
	vector<int> times = get_start_times();
	vector<int> jlengths = get_job_lengths();
	int current_job_pos = 0;
	int current_job = 0;
	for (int i=0; i<times.size(); i++)
	{
		if (current_job_pos == jlengths[current_job])
		{
			current_job++;
			current_job_pos = 0;
			printf("\n");
		}
		printf("%d ", times[i]);
		current_job_pos++;
	}


}

bool Schedule::success_chance(int cmax, int new_cmax, double temperature)
{
	srand(time(NULL));
	int chance = rand() % 2;
	double result = exp((cmax - new_cmax)/(temperature * MODULATION));
	if((double)chance >= result)
		return true;
	else
		return false;
}

vector<int> Schedule::select_arc(deque<int> critpath)
{
	srand(time(NULL));
	int dice_roll = (rand() % (critpath.size() - 3)) + 1; 
	vector<int> selected_arc;
	selected_arc.resize(2);
	selected_arc[0] = critpath[dice_roll];
	selected_arc[1] = critpath[dice_roll + 1];
	return selected_arc;
}

void Schedule::solve_using_SA(void)
{
	int mode; //ogrzewamy / oziebiamy
	int move_acceptance;
	double temperature;
	int cmax;
	create_graph();
	
	printf("begin with cmax = %d\n", get_cmax());

	deque<int> crit_path;
	mode = WARMING;
	move_acceptance = 0;
	temperature = INITIAL_TEMPERATURE;
	vector<int> random_arc;
	random_arc.resize(2);
	int new_cmax;

	bool cold_as_ice = false; //nie udalo sie wykonac X ruchow - za niska temp
	//powyzsze mozna chyba wywalic, bo wiaze sie z tym failed_moves
	bool cmax_is_optimal = false; //przydaloby sie odczytac boundy z instancji i jesli trafilismy, to przerwac petlle while
	int failed_moves = 0;

	while(failed_moves < 1000 && !cold_as_ice && !cmax_is_optimal && !TIME_EXCEEDED)
	{
		

		//debug("calculating critical path from %d to %d\n", 0, operations_number + 1);
		crit_path = graph.critical_path(0, operations_number + 1);
		cmax = get_cmax();

		do
			random_arc = select_arc(crit_path);
		while (random_arc[0] == 0 || random_arc[1] == operations_number + 1 || (random_arc[1] - random_arc[0] == 1));
		

		graph.invert_arc(random_arc[0], random_arc[1]);
		graph.set_arc_length(random_arc[1], random_arc[0], vertex_weights[random_arc[1]]);

		new_cmax = get_cmax();
		debug("cmax = %d\t", new_cmax);

		if(new_cmax <= cmax)
		{
			move_acceptance++;
			cmax = new_cmax;
			debug(" BETTER -> ACCEPTED\t");
			failed_moves = 0;
		}
		else
		{
			if(success_chance(cmax, new_cmax, temperature) == true)
			{
				failed_moves = 0;
				move_acceptance++;
				cmax = new_cmax;
				debug(" WORSE -> ACCEPTED\t");
			}
			else // success_chance == false, brak szansy na powodzenie ruchu
			{
				debug(" WORSE -> REJECTED\t");
				failed_moves++;
				if(mode == WARMING)
				{
					move_acceptance = 0;
					temperature *= ALPHA_WARMING;
					graph.invert_arc(random_arc[1], random_arc[0]);
					graph.set_arc_length(random_arc[0], random_arc[1], vertex_weights[random_arc[0]]);
				}
			}
		}

		debug("T=%4.2f\t", temperature);
		debug(mode == WARMING ? "+" : "-");
		debug("\tacc=%d rej=%d", move_acceptance, failed_moves);
		

		if (mode == WARMING && move_acceptance >= 100)
		{
			move_acceptance = 0;
			mode = COOLING;
		}
			
			
		else if (mode == COOLING && move_acceptance >= 100)
		{
			temperature *= ALPHA_COOLING;
			move_acceptance = 0;
		}
			
		
		debug("\n");
	}

	debug("Stopped due to ");
	if (!(failed_moves < 1000))
		debug("failed_moves >= 1000");
	else if (cold_as_ice)
		debug("cold as ice");

	debug("\n%d\n", get_cmax());
	print_start_times();
}
