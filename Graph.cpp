#include "Graph.h"
#include <cstdio>

//#define DEBUG

#ifdef DEBUG
#define debug(...) fprintf(stderr, __VA_ARGS__)
#else
#define debug(...) /*(__VA_ARGS__)*/
#endif

// Public

Graph::Graph(void)
{
}


Graph::Graph(int n)
{
	/*
		Konstruktor grafu o n wierzcholkach.
		Inicjujemy listy poprzednikow i nastepnikow - lista nastepnikow jest potrzebna do DFS,
		zas lista poprzednikow dla poszukiwania sciezek krytycznych.
	*/
	vertices_number = n;
	outgoing_arcs.resize(n);
	incoming_arcs.resize(n);
}

Graph::~Graph(void)
{
}

int Graph::get_vertices_number()
{
	/*
		Zwraca liczbe wierzcholkow w grafie.
	*/
	return vertices_number;
}


void Graph::add_arc(int from, int to, int length)
{
	/*
		Dodawanie luku z from do to o dlugosci length - zarowno do listy poprzednikow, jak i
		nastepnikow
	*/
	Arc outgoing_arc = { to, length };
	Arc incoming_arc = { from, length };
	outgoing_arcs[from].push_back(outgoing_arc);
	incoming_arcs[to].push_back(incoming_arc);
}

Arc Graph::get_arc(int from, int to)
{
	/*
		Zwraca luk (nie wskaznik do luku!) z from do to, wyszukuje w lukach wychodzacych z from.
		TODO: czy ta procedura jest w ogole potrzebna w public?
	*/
	for (Vertex::iterator it = outgoing_arcs[from].begin(); it != outgoing_arcs[from].end(); it++)
		if (it->vertex_id == to)
			return *it;
	debug("get_arc(from=%d, to=%d): arc does not exist\n", from, to);
}

int Graph::get_arc_length(int from, int to)
{
	/*
		Zwraca dlugosc luku z from do to.
	*/
	Arc a = get_arc(from, to);
	return a.length;
}

void Graph::set_arc_length(int from, int to, int length)
{
	/*
		Ustawia dlugosc luku z from do to na length. Modyfikuje obie listy sasiedztwa.
	*/
	bool ret = false;
	for (Vertex::iterator it = outgoing_arcs[from].begin(); it != outgoing_arcs[from].end(); it++)
		if (it->vertex_id == to)
		{
			it->length = length;
			ret = true;
		}
	for (Vertex::iterator it = incoming_arcs[to].begin(); it != incoming_arcs[to].end(); it++)
		if (it->vertex_id == from)
		{
			it->length = length;
			if (ret) return;
		}
	if (!ret) debug("set_arc_length(from=%d, to=%d): arc does not exist\n", from, to);
}

bool Graph::arc_exists(int from, int to)
{
	/*
		Sprawdza, czy istnieje luk z from do to
	*/
	for (Vertex::iterator it = outgoing_arcs[from].begin(); it != outgoing_arcs[from].end(); it++)
		if (it->vertex_id == to)
			return true;
	return false;
}

void Graph::delete_arc(int from, int to)
{
	/*
		Usuwa luk z from do to. Modyfikuje obie listy sasiedztwa.
	*/
	bool erased = false;
	for (Vertex::iterator it = outgoing_arcs[from].begin(); it != outgoing_arcs[from].end(); it++)
		if (it->vertex_id == to)
		{
			outgoing_arcs[from].erase(it);
			erased = true;
			break;
		}
		for (Vertex::iterator it = incoming_arcs[to].begin(); it != incoming_arcs[to].end(); it++)
			if (it->vertex_id == from)
			{
				incoming_arcs[to].erase(it);
				break;
			}
			if (erased) return;

			debug("delete_arc(from=%d, to=%d): arc does not exist\n", from, to);
}

void Graph::invert_arc(int from, int to)
{
	/*
		Odwraca luk z from do to, czyli usuwa luk z from do to i tworzy luk z to do from
		o TEJ SAMEJ dlugosci.
	*/
	int length = get_arc(from, to).length;
	delete_arc(from, to);
	add_arc(to, from, length);
}


deque<int> Graph::topological_sort() // result[i] = wierzcho³ek bêd¹cy i-tym w uszeregowaniu topologicznym
{
	/*
		Zwraca porzadek topologiczny grafu (i-ty element wyniku to numer i-tego wierzcholka
		w kolejnosci topologicznej) za pomoca metody DFS.
		Korzystamy z prywatnych wektorow dfs_already_visited (ozn. wierzcholki juz przetworzone)
		i dfs_temp_mark (wierzcholki aktualnie przetwarzane - potrzebny do ewentualnego wykrycia
		cyklu, ktory jak wiemy nie moze sie u nas znalezc).
		Jesli graf zawiera cykl, to zwracany jest pusty deque.
	*/
	topological_order.clear();
	dfs_already_visited.resize(vertices_number); // alokacja pamiêci dla wektora ju¿ odwiedzonych wierzcho³ków
	dfs_temp_mark.resize(vertices_number);
	fill(dfs_already_visited.begin(), dfs_already_visited.end(), false);
	fill(dfs_temp_mark.begin(), dfs_temp_mark.end(), false);
	cycle_flag = false;

	for (int i=0; i<vertices_number; i++)
	{
		if (!dfs_already_visited[i])
			dfs_visit_topo(i);
	}

	for (int i=0; i<topological_order.size(); i++)
		debug("%d ", topological_order[i]);
	debug("\n");

	return (!cycle_flag) ? topological_order : deque<int>();

}


vector<int> Graph::max_distances(int source)
{
	/*
		Zwraca maksymalne odleglosci z source do kazdego wierzcholka.
		Algorytm jest zmodyfikowana wersja procedury znajdowania minimalnych odleglosci w acyklicznym
		grafie skierowanym (vide folie z wykladu) - modyfikacja polegala na zamienieniu +INFINITY
		na 0 i min na max. 
	*/
	int n = vertices_number;
	vector<int> distance;
	distance.resize(n);
	deque<int> topo = topological_sort();

	distance[topo[0]] = 0;
	for (int j=1; j<n; j++)
		distance[topo[j]] = 0;

	for (int j=1; j<n; j++)
	{
		for (int i=0; i<incoming_arcs[topo[j]].size(); i++)
		{
			int a = distance[topo[j]];
			int b = distance[incoming_arcs[topo[j]][i].vertex_id];
			Arc arc = get_arc(incoming_arcs[topo[j]][i].vertex_id, topo[j]);
			int c = arc.length;
			distance[topo[j]] = max(a, b + c);
		}
		
	}

	debug("max distances from %d to 0..%d respectively:\n", source, n-1);
	for (int i=0; i<n; i++)
		debug("%d ", distance[i]);
	debug("\n");

	return distance;
}

deque<int> Graph::critical_path(int source, int sink)
{
	/*
		Zwraca przebieg sciezki krytycznej z source do sink na podstawie wektora maksymalnych
		odleglosci z source (zrodlo: folie z wykladu).
	*/
	vector<int> d = max_distances(source);

	deque<int> path;						// STOS = []
	path.push_front(sink);					// STOS <- t
	int v = sink;							// v := t
	bool excess = false;
	while (v != source)						// while v != s do
	{										// begin
		int u;
		for (int i=0; i<incoming_arcs[v].size(); i++)
			if (d[v] == d[incoming_arcs[v][i].vertex_id] + get_arc_length(incoming_arcs[v][i].vertex_id, v)) // incoming_arcs[v][i] - "kandydat" na u
				{
					u = incoming_arcs[v][i].vertex_id;	// u := wierzcholek, dla ktorego D(v) = D(u) + A(u, v)
					break;
				}
		path.push_front(u);					// STOS <- u
		v = u;								// v := u
		if (path.size() > 1000 && !excess) { debug("Excessive path size!\n"); excess = true; }
	}										// end

	//printf("path.size() = %d\n", path.size());

	debug("critical path from %d to %d: ", source, sink);
	for (int i=0; i<path.size(); i++)
		debug("%d ", path[i]);
	debug("\n");

	
	

	return path;
}

void Graph::create_acyclic_clique(vector<int> vertices, vector<int> lengths)
{
	/*
		Metoda tworzy luki dysjunkcyjne miedzy kazda para wierzcholkow z vertices,
		przy czym wszystkie luki wychodzace z wierzcholka i maja dlugosc lengths[i],
		od razu nadajac tym lukom zwroty w sposob gwarantujacy acyklicznosc.

		Dla kazdego kolejnego wierzcholka dodajemy luk zorientowany w strone jego sasiadow,
		z ktorymi jeszcze nie jest polaczony (z pierwszego wierzcholka tworzymy luki do
		wszystkich pozostalych, z drugiego do wszystkich poza pierwszym itd.)
	*/
	for (int i=0; i<vertices.size(); i++)
	{
		int from = vertices[i];
		for (int j = i+1; j < vertices.size(); j++)
		{
			debug("adding arc from %d to %d of length %d\n", from, vertices[j], lengths[i]);
			add_arc(from, vertices[j], lengths[i]);
		}
	}

}

void Graph::export_dot(vector<vector<int> > clusters)
{
	/*
		Eksport grafu do formatu DOT (do celow diagnostycznych)
	*/
	FILE *f = fopen("graf.txt", "w");
	fprintf(f, "digraph foo {\nrankdir=\"LR\";\npencolor=\"white\";");
	for (int i=0; i<clusters.size(); i++)
	{
		fprintf(f, "subgraph cluster_%d {", i);
		for (int j=0; j<clusters[i].size(); j++)
			fprintf(f, "%d;", clusters[i][j]);
		fprintf(f, "}\n");
	}

	for (int i=0; i<vertices_number; i++)
	{
		for (int j=0; j<outgoing_arcs[i].size(); j++)
		{
			fprintf(f, "%d -> %d [label=%d];\n", i, outgoing_arcs[i][j].vertex_id, outgoing_arcs[i][j].length);
		}

	}
	fprintf(f, "}");
	fclose(f);
}

// Private

void Graph::dfs_visit_topo(int node)
{
	/*
		Glowna czesc procedury DFS dla sortowania topologicznego. Jezeli napotkamy wierzcholek, ktory
		juz oznaczylismy jako aktualnie przetwarzany, ustawiamy flage wykrytego cyklu i wychodzimy.
		(Taka sytuacja nie moze miec miejsca w praktyce, ale wykrywanie bylo potrzebne w diagnostyce)
	*/
	if (cycle_flag) return;
	if (dfs_temp_mark[node])
	{
		debug("---graph has cycle!!!\n");
		cycle_flag = true;
		return;
	}
	else if (!dfs_already_visited[node])
	{
		dfs_temp_mark[node] = true;
		for (int i=0; i<outgoing_arcs[node].size(); i++)
			dfs_visit_topo(outgoing_arcs[node][i].vertex_id);
		dfs_already_visited[node] = true;
		dfs_temp_mark[node] = false;
		topological_order.push_front(node);
	}
}
