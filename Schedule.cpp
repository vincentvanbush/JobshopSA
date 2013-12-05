#include "Schedule.h"
#include <list>

//#define DEBUG

#ifdef DEBUG
#define debug(...) fprintf(stderr, __VA_ARGS__)
#else
#define debug(...) /*(__VA_ARGS__)*/
#endif

#define WARMING 1
#define COOLING 0

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

bool Schedule::success_chance(int cmax, int new_cmax, double temperature, double modulation)
{
	//int chance = rand() % 2;
	double chance = ((double)rand() / (RAND_MAX));
	double result = exp((cmax - new_cmax)/(temperature * modulation));
	if(chance <= result)
		return true;
	else
		return false;

}

vector<int> Schedule::select_arc(deque<int> critpath)
{
	int dice_roll = (rand() % (critpath.size() - 2)) + 1;
	vector<int> selected_arc;
	selected_arc.resize(2);
	selected_arc[0] = critpath[dice_roll];
	selected_arc[1] = critpath[dice_roll + 1];
	return selected_arc;
}

double Schedule::solve_using_SA(int modulation, double initial_temperature, double alpha_warming, double alpha_cooling, int cooling_age_length, double warming_threshold, int max_moves_without_improvement)
{
        
        srand(time(NULL));
	int mode; //ogrzewamy / oziebiamy
	int accepted_moves;
        list<bool> last_moves;
        
        vector<int> last_inverted_arc;
        last_inverted_arc.resize(2);
        
        const int last_moves_size = 500;
        last_moves.resize(last_moves_size);
        std::fill(last_moves.begin(), last_moves.end(), false);
        int accepted_moves_out_of_last_n = 0;
        
        bool halt = false;
        
	double temperature;
	int cmax;
	int best_cmax;
	create_graph();

	best_cmax = get_cmax();
	debug("begin with cmax = %d\n", best_cmax);

	deque<int> crit_path;
	mode = WARMING;
	accepted_moves = 0;
	temperature = initial_temperature;
	vector<int> random_arc;
	random_arc.resize(2);
	int new_cmax;
	bool time_exceeded;
	bool cmax_is_optimal = false; //przydaloby sie odczytac boundy z instancji i jesli trafilismy, to przerwac petlle while
	int moves_without_improvement = 0;
	int max_temperature;

	//mierzenie czasu
	struct timespec start, stop;
 	double totaltime = 0.0;
 	clock_gettime(CLOCK_REALTIME, &start);

	while (moves_without_improvement < max_moves_without_improvement && !cmax_is_optimal && !time_exceeded)
	{
                
		//debug("calculating critical path from %d to %d\n", 0, operations_number + 1);
		crit_path = graph.critical_path(0, operations_number + 1);
		cmax = get_cmax();

		int attempts = 0;
		do
		{
			random_arc = select_arc(crit_path);
                        
			++attempts;
                        if (attempts > 100000)
                        {
                            halt = true;

                            break;
                        }
		}
		while (random_arc[0] == 0 || random_arc[1] == operations_number + 1 || (random_arc[1] - random_arc[0] == 1));
		
                if (halt) break;
                
		graph.invert_arc(random_arc[0], random_arc[1]);
                last_inverted_arc = random_arc;
		graph.set_arc_length(random_arc[1], random_arc[0], vertex_weights[random_arc[1]]);

		new_cmax = get_cmax();
		debug("%d\t", new_cmax);

		if(new_cmax < cmax)
		{
                        if (mode == WARMING)
                        {
                            last_moves.push_back(true);
                            if (last_moves.front() == false)
                                accepted_moves_out_of_last_n++;
                            last_moves.pop_front();
                        }
			accepted_moves++;
			cmax = new_cmax;
			debug(" +a\t");
			if (new_cmax <= best_cmax)
			{
				moves_without_improvement = 0;
				best_cmax = new_cmax;
			}

		}
		else if (new_cmax == cmax)
		{
			accepted_moves++;
			debug("==\t");
			moves_without_improvement++;
                        if (mode == WARMING)
                        {
                            last_moves.push_back(true);
                            if (last_moves.front() == false)
                                accepted_moves_out_of_last_n++;
                            last_moves.pop_front();
                        }
		}
		else
		{
			if(success_chance(cmax, new_cmax, temperature, modulation) == true)
			{
				//failed_moves = 0;
				accepted_moves++;
				cmax = new_cmax;
				debug(" -a\t");
				moves_without_improvement++;
                                if (mode == WARMING)
                                {
                                    last_moves.push_back(true);
                                    if (last_moves.front() == false)
                                    {
                                        accepted_moves_out_of_last_n++;
                                    }
                                    last_moves.pop_front();
                                }
			}
			else // success_chance == false, brak szansy na powodzenie ruchu
			{
				debug(" -r\t");
				moves_without_improvement++;
				if(mode == WARMING)
				{
                                        if (accepted_moves > cooling_age_length/10)
                                        {
                                                accepted_moves = 0;
                                                temperature += alpha_warming * initial_temperature;
                                        }
                                        last_moves.push_back(false);
                                        if (last_moves.front() == true)
                                            accepted_moves_out_of_last_n--;
                                        last_moves.pop_front();
				}
				graph.invert_arc(random_arc[1], random_arc[0]);
				graph.set_arc_length(random_arc[0], random_arc[1], vertex_weights[random_arc[0]]);
			}
		}

		debug("%4.2f\t", temperature);
		debug(mode == WARMING ? "+" : "-");
		debug("\t%d\t%d\t%4.2f\t", accepted_moves, moves_without_improvement, (double)accepted_moves_out_of_last_n / last_moves_size);


		if (mode == WARMING && (double)accepted_moves_out_of_last_n / last_moves_size >= warming_threshold) 
		{
			accepted_moves = 0;
			mode = COOLING;
			max_temperature = temperature;
		}


		else if (mode == COOLING && accepted_moves >= cooling_age_length)
		{

			temperature *= alpha_cooling;
			accepted_moves = 0;
		}

		clock_gettime(CLOCK_REALTIME, &stop);
		totaltime += (double) (stop.tv_sec - start.tv_sec)+1.e-9*(stop.tv_nsec - start.tv_nsec);

		//jesli przekroczono okreslona liczbe sekund
		if(totaltime > 180.0)
			time_exceeded = true;

		clock_gettime(CLOCK_REALTIME, &start);

		debug("\n");
	}
#ifdef DEBUG
	debug("Stopped due to: ");
	if (!(moves_without_improvement < max_moves_without_improvement))
		debug("moves_without_improvement >= limit\n");
	else if(time_exceeded)
		debug("time exceeded\n");
#endif
/*
	vector<vector<int> > clusters;
	clusters.resize(jobs.size());
	int n = 1;
	for (int i = 0; i<clusters.size(); i++)
	for (int j = 0; j<jobs[i].first.size(); j++)
		clusters[i].push_back(n++);
	graph.export_dot(clusters);
*/
	printf("%d\n", get_cmax());
	print_start_times();
	printf("\n");
	return totaltime;
}
