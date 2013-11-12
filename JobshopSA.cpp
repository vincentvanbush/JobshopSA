// JobshopSA.cpp : Defines the entry point for the console application.
//

#include <cstdio>
#include "Graph.h"
#include "Schedule.h"

using namespace std;

int main(int argc, char* argv[])
{
	printf("TWORZENIE ROZWIAZANIA POCZATKOWEGO DLA PEWNEJ INSTANCJI\n");
	//		machines	times
	// J1	0,1,2		10,8,4
	// J2	1,3,0,2		8,3,5,6
 	// J3	0,1,3		4,7,3

	Schedule s(4);
	vector<int> t1, m1, t2, m2, t3, m3;
	// J1
	t1.push_back(10);	t1.push_back(8);	t1.push_back(4);
	m1.push_back(0);	m1.push_back(1);	m1.push_back(2);
	// J2
	t2.push_back(8);	t2.push_back(3);	t2.push_back(5);	t2.push_back(6);
	m2.push_back(1);	m2.push_back(0);	m2.push_back(3);	m2.push_back(2);
	// J3
	t3.push_back(4);	t3.push_back(7);	t3.push_back(3);
	m3.push_back(0);	m3.push_back(1);	m3.push_back(3);

	s.add_job(t1, m1); // J1
	s.add_job(t2, m2); // J2
	s.add_job(t3, m3); // J3

	s.create_graph();

	printf("Cmax = %d\n", s.get_cmax());

	printf("\n\nPRZYKLADOWY GRAF DYSJUNKCYJNY\n");
	// http://i.imgur.com/mwUW5Dk.png
	Graph sch(12);

	// job1
	sch.add_arc(0, 1, 0);
	sch.add_arc(1, 2, 10);
	sch.add_arc(2, 3, 8);
	sch.add_arc(3, 11, 4);
	// job2
	sch.add_arc(0, 4, 0);
	sch.add_arc(4, 5, 8);
	sch.add_arc(5, 6, 3);
	sch.add_arc(6, 7, 5);
	sch.add_arc(7, 11, 6);
	// job3
	sch.add_arc(0, 8, 0);
	sch.add_arc(8, 9, 4);
	sch.add_arc(9, 10, 7);
	sch.add_arc(10, 11, 3);

	vector<int> v;
	vector<int> l;
	// machine1
	v.push_back(8); v.push_back(1); v.push_back(5);
	l.push_back(4); l.push_back(10); l.push_back(3);
	sch.create_acyclic_clique(v, l);
	// machine2
	v.clear(); l.clear();
	v.push_back(4); v.push_back(9); v.push_back(2);
	l.push_back(8); l.push_back(7); l.push_back(8);
	sch.create_acyclic_clique(v, l);
	// machine3
	v.clear(); l.clear();
	v.push_back(3); v.push_back(7);
	l.push_back(4); l.push_back(6);
	sch.create_acyclic_clique(v, l);
	// machine4
	v.clear(); l.clear();
	v.push_back(6); v.push_back(10);
	l.push_back(5); l.push_back(3);
	sch.create_acyclic_clique(v, l);

	sch.critical_path(0, 11);

	sch.export_dot();

	


	printf("\n\nTWORZENIE ACYKLICZNIE SKIEROWANEJ KLIKI\n");
	Graph clique(5);
	vector<int> w;
	w.push_back(4);
	w.push_back(3);
	w.push_back(2);
	w.push_back(1);
	w.push_back(0);
	vector<int> d;
	d.push_back(1); d.push_back(1); d.push_back(1); d.push_back(1); d.push_back(1);
	clique.create_acyclic_clique(w, d);
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

