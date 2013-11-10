#include "Graph.h"

#ifdef DEBUG
#define debug(...) fprintf(stderr, __VA_ARGS__)
#else
#define debug(...) /*(__VA_ARGS__)*/
#endif

// Public

Graph::Graph(int n)
{
	vertices_number = n;
	outgoing_arcs.resize(n);
	incoming_arcs.resize(n);
}

Graph::~Graph(void)
{
}

int Graph::get_vertices_number()
{
	return vertices_number;
}


void Graph::add_arc(int from, int to, int length)
{
	Arc outgoing_arc = { to, length };
	Arc incoming_arc = { from, length };
	outgoing_arcs[from].push_back(outgoing_arc);
	incoming_arcs[to].push_back(incoming_arc);
}

Arc Graph::get_arc(int from, int to)
{
	for (Vertex::iterator it = outgoing_arcs[from].begin(); it != outgoing_arcs[from].end(); it++)
		if (it->vertex_id == to)
			return *it;
	debug("get_arc(from=%d, to=%d): arc does not exist\n", from, to);
}

int Graph::get_arc_length(int from, int to)
{
	Arc a = get_arc(from, to);
	return a.length;
}

bool Graph::arc_exists(int from, int to)
{
	for (Vertex::iterator it = outgoing_arcs[from].begin(); it != outgoing_arcs[from].end(); it++)
		if (it->vertex_id == to)
			return true;
	return false;
}

void Graph::delete_arc(int from, int to)
{
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
	int length = get_arc(from, to).length;
	delete_arc(from, to);
	add_arc(to, from, length);
}


deque<int> Graph::topological_sort() // result[i] = wierzcho³ek bêd¹cy i-tym w uszeregowaniu topologicznym
{
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

	return (!cycle_flag) ? topological_order : deque<int>();

}


vector<int> Graph::max_distances(int source)
{
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
	for (int i=0; i<n; i++)
		debug("%d ", distance[i]);
	debug("\n");
	return distance;
}

deque<int> Graph::critical_path(int source, int sink)
{
	vector<int> d = max_distances(source);

	deque<int> path;						// STOS = []
	path.push_front(sink);					// STOS <- t
	debug("pushing vertex %d\n", sink);
	int v = sink;							// v := t

	while (v != source)						// while v != s do
	{										// begin
		int u;
		for (int i=0; i<incoming_arcs[v].size(); i++)
			if (d[v] == d[incoming_arcs[v][i].vertex_id] + get_arc_length(incoming_arcs[v][i].vertex_id, v))
				{
					u = incoming_arcs[v][i].vertex_id;	// u := wierzcholek, dla ktorego D(v) = D(u) + A(u, v)
					debug("u=%d\n", u);
					break;
				}
				
		debug("pushing vertex %d\n", u);
		path.push_front(u);					// STOS <- u
		v = u;								// v := u
	}										// end

	debug("path.size() = %d\n", path.size());

	debug("critical path from %d to %d: ", source, sink);
	for (int i=0; i<path.size(); i++)
		debug("%d ", path[i]);
	debug("\n");

	return path;
}

// Private

void Graph::dfs_visit_topo(int node)
{
	if (cycle_flag) return;
	if (dfs_temp_mark[node])
	{
		debug("---graph has cycle!!!\n");
		cycle_flag = true;
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
