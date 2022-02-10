#include <iterator>
#include <iostream>
#include <vector>
#include <string>
#include "AFD.h"
#include <fstream>

std::ifstream f("AFD.txt");
int main() 
{
	AFD afd;
	f >> afd;
	std::cout << afd;
	afd.BFS();
	std::cout << '\n';
	afd.createTable();
	//afd.AFDmini();
	std::cout << afd;
	return 0;
}