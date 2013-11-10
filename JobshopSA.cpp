// JobshopSA.cpp : Defines the entry point for the console application.
//

#include <cstdio>
#include "Graph.h"

using namespace std;

int main(int argc, char* argv[])
{
	
	Graph g(12);
	// source=0, sink=11
	g.add_arc(0, 1, 0);
	g.add_arc(1, 2, 8);
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

	deque<int> topo = g.topological_sort();

	g.critical_path(0, 11);
		
	return 0;
}

