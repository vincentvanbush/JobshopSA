#pragma once

#include <vector>
#include <deque>
#include <algorithm>
#include <cstdio>
#include <stack>

using namespace std;


typedef struct Arc {								// struktura opisujaca luk (zarowno dla listy poprzednikow, jak i nastepnikow)
	int vertex_id;									// numer poprzednika lub nastepnika
	int length;										// dlugosc luku
} Arc;

typedef vector<Arc> Vertex;							// element listy poprzednikow lub nastepnikow zawierajacy luki wchodzace/wychodzace
typedef vector<Vertex> Vertices;					// lista poprzednikow lub nastepnikow

class Graph
{
public:
	Graph(int n);									// konstruktor grafu o n wierzcholkach
	~Graph(void);
	int get_vertices_number();						// zwraca liczbe wierzcholkow grafu
	void add_arc(int from, int to, int length);		// dodaje luk z from do to o dlugoœci length
	Arc get_arc(int from, int to);					// zwraca dane luku z from do to
	int get_arc_length(int from, int to);
	void set_arc_length(int from, int to, int length);
	bool arc_exists(int from, int to);				// okresla, czy istnieje luk z from do to
	void delete_arc(int from, int to);				// usuwa luk z from do to
	void invert_arc(int from, int to);				// odwraca zwrot luku z from do to (tworzac luk z to do from)
	deque<int> topological_sort();					// zwraca porzadek topologiczny wierzcholkow grafu
	vector<int> max_distances(int source);			// zwraca wektor dlugosci najdluzszych sciezek z source do kazdego wierzcholka
	deque<int> critical_path(int source, int sink);	// zwraca scieżke krytyczna z source do sink

private:
	void dfs_visit_topo(int node);					// wywoluje procedure DFS dla sortowania topologicznego
	deque<int> topological_order;					// topologiczny porzadek wierzcholkow ustalony w metodzie topological_sort()
	int vertices_number;							// liczba wierzcholkow w grafie
	Vertices outgoing_arcs;							// lista nastepnikow (potrzebna do DFS)
	Vertices incoming_arcs;							// lista poprzednikow (potrzebna do znajdowania najdluzszych œciezek)
	vector<bool> dfs_already_visited;				// zawiera informacje, czy dany wierzcholek byl juz przetworzony w procedurze DFS
	vector<bool> dfs_temp_mark;						// zawiera informacje, czy dany wierzcholek jest w trakcie przetwarzania w DFS
	bool cycle_flag;								// informacja, czy wykryto cykl podczas procedury DFS
};


