// JobshopSA.cpp : Defines the entry point for the console application.
//

#include <cstdio>
#include "Graph.h"
#include "Schedule.h"
#define PB push_back
using namespace std;

int main(int argc, char* argv[])
{
	Schedule s(15);
	vector<int>
		t1, m1, 
		t2, m2,
		t3, m3,
		t4, m4,
		t5, m5,
		t6, m6,
		t7, m7,
		t8, m8,
		t9, m9,
		t10, m10,
		t11, m11,
		t12, m12,
		t13, m13,
		t14, m14,
		t15, m15;

	t1.PB(86); t1.PB(60); t1.PB(10); t1.PB(59); t1.PB(65); t1.PB(94); t1.PB(71); t1.PB(25); t1.PB(98); t1.PB(49); t1.PB(43); t1.PB(8); t1.PB(90); t1.PB(21); t1.PB(73);
	t2.PB(68); t2.PB(28); t2.PB(38); t2.PB(36); t2.PB(93); t2.PB(35); t2.PB(37); t2.PB(28); t2.PB(62); t2.PB(86); t2.PB(65); t2.PB(11); t2.PB(20); t2.PB(82); t2.PB(23);
	t3.PB(33); t3.PB(67); t3.PB(96); t3.PB(91); t3.PB(83); t3.PB(81); t3.PB(60); t3.PB(88); t3.PB(20); t3.PB(62); t3.PB(22); t3.PB(79); t3.PB(38); t3.PB(40); t3.PB(82);
	t4.PB(13); t4.PB(14); t4.PB(73); t4.PB(88); t4.PB(24); t4.PB(16); t4.PB(78); t4.PB(70); t4.PB(53); t4.PB(68); t4.PB(73); t4.PB(90); t4.PB(58); t4.PB(7); t4.PB(4);
	t5.PB(93); t5.PB(52); t5.PB(63); t5.PB(13); t5.PB(19); t5.PB(41); t5.PB(71); t5.PB(59); t5.PB(19); t5.PB(60); t5.PB(85); t5.PB(99); t5.PB(73); t5.PB(95); t5.PB(19);
	t6.PB(62); t6.PB(60); t6.PB(93); t6.PB(16); t6.PB(10); t6.PB(72); t6.PB(88); t6.PB(69); t6.PB(58); t6.PB(41); t6.PB(46); t6.PB(63); t6.PB(76); t6.PB(83); t6.PB(62);
	t7.PB(50); t7.PB(68); t7.PB(90); t7.PB(34); t7.PB(44); t7.PB(5); t7.PB(8); t7.PB(25); t7.PB(70); t7.PB(53); t7.PB(78); t7.PB(92); t7.PB(62); t7.PB(85); t7.PB(70);
	t8.PB(60); t8.PB(64); t8.PB(92); t8.PB(44); t8.PB(63); t8.PB(91); t8.PB(21); t8.PB(1); t8.PB(96); t8.PB(19); t8.PB(59); t8.PB(12); t8.PB(41); t8.PB(11); t8.PB(94);
	t9.PB(93); t9.PB(46); t9.PB(51); t9.PB(37); t9.PB(91); t9.PB(90); t9.PB(63); t9.PB(40); t9.PB(68); t9.PB(13); t9.PB(16); t9.PB(83); t9.PB(49); t9.PB(24); t9.PB(23);
	t10.PB(5); t10.PB(35); t10.PB(21); t10.PB(14); t10.PB(66); t10.PB(3); t10.PB(6); t10.PB(98); t10.PB(63); t10.PB(64); t10.PB(76); t10.PB(94); t10.PB(17); t10.PB(62); t10.PB(37);
	t11.PB(35); t11.PB(42); t11.PB(62); t11.PB(68); t11.PB(73); t11.PB(27); t11.PB(52); t11.PB(39); t11.PB(41); t11.PB(25); t11.PB(9); t11.PB(34); t11.PB(50); t11.PB(41); t11.PB(98);
	t12.PB(23); t12.PB(32); t12.PB(35); t12.PB(10); t12.PB(29); t12.PB(68); t12.PB(20); t12.PB(8); t12.PB(58); t12.PB(62); t12.PB(39); t12.PB(32); t12.PB(8); t12.PB(33); t12.PB(91);
	t13.PB(28); t13.PB(31); t13.PB(3); t13.PB(28); t13.PB(66); t13.PB(59); t13.PB(24); t13.PB(45); t13.PB(81); t13.PB(8); t13.PB(44); t13.PB(42); t13.PB(2); t13.PB(23); t13.PB(53);
	t14.PB(11); t14.PB(93); t14.PB(27); t14.PB(59); t14.PB(62); t14.PB(23); t14.PB(23); t14.PB(7); t14.PB(77); t14.PB(64); t14.PB(60); t14.PB(97); t14.PB(36); t14.PB(53); t14.PB(72);
	t15.PB(36); t15.PB(98); t15.PB(38); t15.PB(24); t15.PB(84); t15.PB(47); t15.PB(72); t15.PB(1); t15.PB(91); t15.PB(85); t15.PB(68); t15.PB(42); t15.PB(20); t15.PB(30); t15.PB(30);

	m1.PB(10); m1.PB(15); m1.PB(5); m1.PB(14); m1.PB(11); m1.PB(4); m1.PB(8); m1.PB(9); m1.PB(1); m1.PB(6); m1.PB(2); m1.PB(3); m1.PB(13); m1.PB(7); m1.PB(12);
	m2.PB(11); m2.PB(9); m2.PB(12); m2.PB(15); m2.PB(4); m2.PB(14); m2.PB(10); m2.PB(8); m2.PB(5); m2.PB(3); m2.PB(7); m2.PB(2); m2.PB(6); m2.PB(13); m2.PB(1);
	m3.PB(8); m3.PB(1); m3.PB(7); m3.PB(6); m3.PB(15); m3.PB(14); m3.PB(3); m3.PB(12); m3.PB(5); m3.PB(13); m3.PB(2); m3.PB(10); m3.PB(4); m3.PB(11); m3.PB(9);
	m4.PB(10); m4.PB(12); m4.PB(15); m4.PB(1); m4.PB(2); m4.PB(9); m4.PB(6); m4.PB(11); m4.PB(13); m4.PB(5); m4.PB(14); m4.PB(4); m4.PB(7); m4.PB(8); m4.PB(3);
	m5.PB(12); m5.PB(5); m5.PB(14); m5.PB(4); m5.PB(9); m5.PB(2); m5.PB(11); m5.PB(13); m5.PB(3); m5.PB(15); m5.PB(7); m5.PB(8); m5.PB(1); m5.PB(10); m5.PB(6);
	m6.PB(6); m6.PB(3); m6.PB(2); m6.PB(11); m6.PB(1); m6.PB(5); m6.PB(9); m6.PB(15); m6.PB(7); m6.PB(4); m6.PB(10); m6.PB(8); m6.PB(12); m6.PB(13); m6.PB(14);
	m7.PB(6); m7.PB(11); m7.PB(14); m7.PB(1); m7.PB(10); m7.PB(9); m7.PB(2); m7.PB(12); m7.PB(15); m7.PB(8); m7.PB(13); m7.PB(3); m7.PB(7); m7.PB(5); m7.PB(4);
	m8.PB(13); m8.PB(1); m8.PB(10); m8.PB(4); m8.PB(14); m8.PB(7); m8.PB(6); m8.PB(8); m8.PB(3); m8.PB(15); m8.PB(12); m8.PB(9); m8.PB(11); m8.PB(2); m8.PB(5);
	m9.PB(12); m9.PB(11); m9.PB(6); m9.PB(14); m9.PB(2); m9.PB(10); m9.PB(9); m9.PB(8); m9.PB(4); m9.PB(7); m9.PB(1); m9.PB(3); m9.PB(15); m9.PB(13); m9.PB(5);
	m10.PB(3); m10.PB(15); m10.PB(4); m10.PB(11); m10.PB(7); m10.PB(2); m10.PB(1); m10.PB(14); m10.PB(12); m10.PB(5); m10.PB(6); m10.PB(9); m10.PB(8); m10.PB(13); m10.PB(10);
	m11.PB(12); m11.PB(15); m11.PB(14); m11.PB(6); m11.PB(5); m11.PB(10); m11.PB(2); m11.PB(7); m11.PB(13); m11.PB(1); m11.PB(3); m11.PB(9); m11.PB(11); m11.PB(4); m11.PB(8);
	m12.PB(13); m12.PB(4); m12.PB(11); m12.PB(9); m12.PB(5); m12.PB(8); m12.PB(14); m12.PB(12); m12.PB(15); m12.PB(2); m12.PB(3); m12.PB(1); m12.PB(6); m12.PB(7); m12.PB(10);
	m13.PB(9); m13.PB(14); m13.PB(6); m13.PB(1); m13.PB(12); m13.PB(10); m13.PB(5); m13.PB(13); m13.PB(2); m13.PB(11); m13.PB(7); m13.PB(3); m13.PB(8); m13.PB(15); m13.PB(4);
	m14.PB(3); m14.PB(6); m14.PB(5); m14.PB(4); m14.PB(10); m14.PB(2); m14.PB(12); m14.PB(14); m14.PB(8); m14.PB(7); m14.PB(11); m14.PB(15); m14.PB(1); m14.PB(9); m14.PB(13);
	m15.PB(2); m15.PB(11); m15.PB(5); m15.PB(3); m15.PB(1); m15.PB(8); m15.PB(7); m15.PB(10); m15.PB(12); m15.PB(13); m15.PB(6); m15.PB(15); m15.PB(4); m15.PB(14); m15.PB(9);

	for (int i = 0; i < 15; i++)
	{
		m1[i]--;
		m2[i]--;
		m3[i]--;
		m4[i]--;
		m5[i]--;
		m6[i]--;
		m7[i]--;
		m8[i]--;
		m9[i]--;
		m10[i]--;
		m11[i]--;
		m12[i]--;
		m13[i]--;
		m14[i]--;
		m15[i]--;

	}

	s.add_job(t1, m1);
	s.add_job(t2, m2);
	s.add_job(t3, m3);
	s.add_job(t4, m4);
	s.add_job(t5, m5);
	s.add_job(t6, m6);
	s.add_job(t7, m7);
	s.add_job(t8, m8);
	s.add_job(t9, m9);
	s.add_job(t10, m10);
	s.add_job(t11, m11);
	s.add_job(t12, m12);
	s.add_job(t13, m13);
	s.add_job(t14, m14);
	s.add_job(t15, m15);


	s.solve_using_SA();
	getchar();
	return 0;

}

