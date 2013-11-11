// JobshopSA.cpp : Defines the entry point for the console application.
//

#include <cstdio>
#include "Graph.h"

using namespace std;

int main(int argc, char* argv[])
{
	printf("TWORZENIE ACYKLICZNIE SKIEROWANEJ KLIKI\n");
	Graph clique(5);
	vector<int> v;
	v.push_back(4);
	v.push_back(3);
	v.push_back(2);
	v.push_back(1);
	v.push_back(0);
	vector<int> l;
	l.push_back(1); l.push_back(1); l.push_back(1); l.push_back(1); l.push_back(1); 
	clique.create_acyclic_clique(v, l);
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

	g.critical_path(0, 11);
		
	return 0;
}

