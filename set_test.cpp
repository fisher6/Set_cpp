/*
 * set_test.cpp
 *
 *  Created on: Sep 10, 2015
 *      Author: Gal
 */

#include "mtm_set.hpp"
#include <iostream>
using namespace mtm;
using std::cout;
using std::endl;

int main()
{
	set<int> sett;
	sett.insert(2);
	sett.insert(1);
	cout << *sett.find(1) << " is in sett" << endl;
	cout << *sett.find(2) << " is in sett" << endl;
	try {
		cout << *sett.find(3) << endl;
	} catch (set<int>::ElementNotFound&) {
		cout << "ElementNotFound exception works" << endl;
	}
	set<int> set2(sett);
	try {
		cout << *set2.find(2) << " is in set2" << endl;
	} catch (set<int>::ElementNotFound&) {
		cout << "error" << endl;
	}
	set2.erase(2);
	try {
		cout << *set2.find(2) << " is in set2" << endl;
	} catch (set<int>::ElementNotFound&) {
		cout << "erase works - 2 removed from set2" << endl;
	}
	set<int> set3;
	set3 = set2;
	return 0;
}
