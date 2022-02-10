#pragma once
#include <iostream>
#include <string>
#include <tuple>
#include <fstream>
#include <vector>
#include <cstring>
#include <iterator>
#include <map>
#include <queue>
#include <unordered_map>
#include <iterator>
#include <unordered_set>
struct hash_pair {
	size_t operator()(const std::pair<std::string, char>& p) const
	{
		auto hash1 = std::hash<std::string>{}(p.first);
		auto hash2 = std::hash<char>{}(p.second);
		return hash1 ^ hash2;
	}
};
class AFD {
	std::vector<std::string> stari, stari2;
	std::string sigma;
	std::vector<std::string> finale;
	std::string StareInit;
	//std::vector<std::tuple<std::string, char, std::string> >delta;
	std::unordered_map<std::pair<std::string, char>, std::string, hash_pair> delta;
	int nr_tranzitii;
	friend std::istream& operator >> (std::istream& in, AFD& afd);
	friend std::ostream& operator <<(std::ostream& out, AFD afd);
	std::vector<std::vector<std::string>> tabelMini;
public:
	AFD();
	AFD(std::vector<std::string> stari1, std::string sigma1, std::vector<std::string> finale1, std::string StareInit1, int nr_tranzitii1, std::unordered_map<std::pair<std::string, char>, std::string, hash_pair> delta1);  //std::vector < std::tuple<std::string, char, std::string> >delta1
	void verificare();
	bool regula1();
	bool regula2();
	bool regula3();
	bool word_accepted(std::string word);
	std::string apply_transition(std::string stare, char character);
	void BFS();
	void createTable();
	void stars(int ind1, int ind2);
	void afisareTabel();
	void AFDmini();

};
