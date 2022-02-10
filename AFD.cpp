#include "AFD.h"
#include <sstream>
#include <fstream>
#include <iostream>
std::istream& operator>>(std::istream& in, AFD& afd)
{
	std::string buffer, stare, sig, buff;
	std::getline(in, buffer);
	std::istringstream iss(buffer);
	while (iss >> stare)
	{
		afd.stari.push_back(stare);
		afd.stari2.push_back(stare);
	}
	buffer.clear();
	sig.clear();
	stare.clear();
	std::getline(in, buffer);
	std::istringstream is(buffer);
	while (is >> sig)
	{
		afd.sigma += sig;
	}
	buffer.clear();
	sig.clear();
	std::getline(in, buffer);
	std::istringstream isss(buffer);
	while (isss >> sig)
	{
		afd.finale.push_back(sig);
	}
	buffer.clear();
	sig.clear();
	char lit;
	in >> afd.StareInit;
	in >> afd.nr_tranzitii;
	for (int index = 0; index < afd.nr_tranzitii; index++)
	{
		in >> buffer >> lit >> stare;
		afd.delta[std::make_pair(buffer, lit)] = stare;
	}
	return in;
}

std::ostream& operator<<(std::ostream& out, AFD afd)
{
	out << "Acestea sunt starile: ";
	for (int index = 0; index < afd.stari.size(); index++)
	{
		out << afd.stari[index] << " ";
	}
	out << '\n';
	out << "Alfabetul de intrare este urmatorul: ";
	for (int index = 0; index < afd.sigma.size(); index++)
	{
		out << afd.sigma[index] << " ";
	}
	out << '\n';
	out << "Starea initiala este: " << afd.StareInit;
	out << '\n';
	out << "Multimea starilor finale este urmatoarea: ";
	for (int index = 0; index < afd.finale.size(); index++)
	{
		out << afd.finale[index] << " ";
	}
	out << '\n';
	out << "AFD-ul are urmatoarele tranzitii: ";
	out << '\n';
	int nr = 1;
	for (auto ind : afd.delta)
	{
		std::cout << nr++<<" ";
		out << (ind.first).first << " "
			<< (ind.first).second << " " << ind.second;
		out << '\n';
	}
	return out;
}

AFD::AFD()
{
	sigma = "";
	StareInit = "";
	nr_tranzitii = 0;
}

AFD::AFD(std::vector<std::string> stari1, std::string sigma1, std::vector<std::string> finale1, std::string StareInit1, int nr_tranzitii1, std::unordered_map<std::pair<std::string, char>, std::string, hash_pair> delta1) :stari{ stari1 }, sigma{ sigma1 }, finale{ finale1 }, StareInit{ StareInit1 }, nr_tranzitii{ nr_tranzitii1 }, delta{ delta1 }
{
}
bool AFD::regula1()
{
	if (find(stari.begin(), stari.end(), StareInit) == stari.end())
		return false;
	return true;
}
bool AFD::regula2()
{
	for (auto ind : finale)
	{
		if (find(stari.begin(), stari.end(), ind) == stari.end())
			return false;
	}
	return true;
}
bool AFD::regula3()
{
	for (auto ind : delta)
	{
		if (find(stari.begin(), stari.end(), ind.first.first) == stari.end() || find(stari.begin(), stari.end(), ind.second) == stari.end() || sigma.find(ind.first.second) == std::string::npos)
			return false;
	}
	return true;

}
std::string AFD::apply_transition(std::string stare, char character)
{
	for (auto index : delta)
	{
		if (index.first.first == stare && index.first.second == character)
		{
			return index.second;
		}
	}
	return "none";
}
void AFD::stars(int ind1, int ind2)
{
	for (auto ind : sigma)
	{
		std::string tranz1 = delta[{stari[ind1], ind}];
		std::string tranz2 = delta[{stari[ind2], ind}];
		auto poz1 = std::find(stari.begin(), stari.end(), tranz1);
		int pozitie = std::distance(stari.begin(), poz1);
		auto poz2 = std::find(stari.begin(), stari.end(), tranz2);
		int pozitie2 = std::distance(stari.begin(), poz2);
		if (std::find(finale.begin(), finale.end(), tranz1) != finale.end() && std::find(finale.begin(), finale.end(), tranz2) == finale.end() ||
			std::find(finale.begin(), finale.end(), tranz2) != finale.end() && std::find(finale.begin(), finale.end(), tranz1) == finale.end() ||
			(pozitie > pozitie2 && tabelMini[pozitie][pozitie2] == "*"  && pozitie != pozitie2) || (pozitie2 > pozitie && tabelMini[pozitie2][pozitie] == "*" && pozitie != pozitie2  ))
		{
			tabelMini[ind1][ind2] = "*";
			break;
		}

	}
}
void AFD::afisareTabel()
{
	for (int ind1 = 0; ind1 < tabelMini.size(); ind1++)
	{
		for (int ind2 = 0; ind2 < tabelMini[ind1].size(); ind2++)
		{
			std::cout << tabelMini[ind1][ind2] << " ";
		}
		std::cout << std::endl;
	}
}
void AFD::AFDmini()
{
	std::unordered_set<std::string> toDelete;
	std::unordered_set<std::string> toEmplace;
	std::unordered_map<std::string, std::string> elements;
	for (int ind1 = 0; ind1 < tabelMini.size(); ind1++)
	{
		for (int ind2 = 0; ind2 < tabelMini[ind1].size(); ind2++)
		{
			if (tabelMini[ind1][ind2] == "-")
			{
				toDelete.insert(stari2[ind1]);
				toEmplace.insert(stari2[ind2]);
				elements.insert({ stari2[ind1], stari2[ind2] });
			}

		}
	}
	for (auto& index : toDelete)
	{
		for (auto& ind : sigma)
		{
			std::pair<std::string, char> pair;
			pair = std::make_pair(index, ind);
			auto find = delta.find(std::pair(pair));
			if (find != delta.end())
			{
				delta.erase(find);
			}
		}
	}
	for (auto& index : toDelete)
	{
		stari.erase(std::find(stari.begin(), stari.end(), index));
		if (std::find(finale.begin(), finale.end(), index) != finale.end())
		{
			finale.erase(std::find(finale.begin(), finale.end(), index));
		}
	}
	//for (auto& index : delta)
	//{
	//	std::unordered_set<std::string>::iterator itr = toDelete.begin();
	//	std::unordered_set<std::string>::iterator itr2 = toEmplace.begin();
	//	for (int ind = 0; ind < toDelete.size(); ind++)
	//	{
	//		if (index.second == *itr)
	//		{
	//			index.second = *itr2;
	//		}
	//		itr++;
	//		itr2++;
	//	}

	//}
	for (auto& index : delta)
	{
		if (elements.find(index.second) != elements.end())
		{
			index.second = elements.find(index.second)->second;
		}
	}
	/*for (auto& i : toDelete)
	{
		std::cout << i << " ";
	}*/
	std::cout << std::endl;
	/*for (auto& j : toEmplace)
	{
		std::cout << j << " ";
	}*/
	//for (auto& index : delta)
	//{
	//	std::set<std::string>::iterator it;
	//	for (auto it : toDelete)
	//		if (index.second == it)
	//		{
	//			//index.second = toEmplace[it.];

	//		}
	//}

}
void AFD::createTable()
{
	for (int index = 0; index < stari.size(); index++)
	{
		std::vector<std::string> vect;
		for (int index1 = 0; index1 < index; index1++)
		{
			if (std::find(finale.begin(), finale.end(), stari[index]) != finale.end() && std::find(finale.begin(), finale.end(), stari[index1]) == finale.end() ||
				std::find(finale.begin(), finale.end(), stari[index1]) != finale.end() && std::find(finale.begin(), finale.end(), stari[index]) == finale.end())
			{
				vect.push_back("*");
			}
			else
				vect.push_back("-");
		}
		tabelMini.push_back(vect);
	}
	afisareTabel();
	for (int ind1 = 0; ind1 < tabelMini.size(); ind1++)
	{
		for (int ind2 = 0; ind2 < tabelMini[ind1].size(); ind2++)
		{
			std::cout << ind1<<ind2 << " ";
		}
		std::cout << std::endl;
	}
	bool flag = true;
	while (flag)
	{
		for (int ind1 = 0; ind1 < tabelMini.size(); ind1++)
		{
			for (int ind2 = 0; ind2 < tabelMini[ind1].size(); ind2++)
			{
				if (tabelMini[ind1][ind2] == "-")
				{
					stars(ind1, ind2);
					if (tabelMini[ind1][ind2] == "*")
					{
						flag = true;
					}
					else
						flag = false;
				}
			}

			afisareTabel();

		}
	}
}
void AFD::BFS()
{
	std::vector<bool> visited;
	std::queue<std::string> queue;
	std::string verif;
	for (int index = 0; index < stari.size(); index++)
		visited.push_back(false);
	queue.push(StareInit);

	while (!queue.empty())
	{
		verif = queue.front();
		queue.pop();
		for (auto& index : delta)
		{
			if (index.first.first == verif)
			{
				std::vector<std::string>::iterator poz = std::find(stari.begin(), stari.end(), index.second);
				int pozitie = std::distance(stari.begin(), poz);
				if (!visited[pozitie])
				{
					visited[pozitie] = true;
					queue.push(index.second);
				}
			}
		}
	}
	visited[0] = true;
	for (int index = 0; index < visited.size(); index++)
		if (visited[index] == false)
		{
			std::string toDelete = stari2[index];
			std::vector<std::string>::iterator poz = std::find(finale.begin(), finale.end(), stari2[index]);
			int pozitie = std::distance(finale.begin(), poz);
			finale.erase(finale.begin() + pozitie);
			stari.erase(stari.begin() + index);
			for (auto& ind : sigma)
			{
				std::pair<std::string, char> pair;
				pair = std::make_pair(toDelete, ind);
				auto find = delta.find(std::pair(pair));
				if (find != delta.end())
				{
					delta.erase(find);
				}
			}
		}
	/*std::cout << '\n';
	for (auto index : stari)
	{
		std::cout << index << " ";
	}
	std::cout << '\n';
	for (auto index1 : finale)
	{
		std::cout << index1 << " ";
	}*/
}
bool AFD::word_accepted(std::string word)
{
	std::string stare = StareInit;
	for (auto character : word)
	{
		if (word.size() == 1 && apply_transition(stare, word[0]) == "none")
		{
			std::cout << "BLOCAJ";
			return false;
		}

		if (stare != "none")
		{
			stare = apply_transition(stare, character);
		}
		if (stare == "none")
		{
			std::cout << "BLOCAJ";
			return false;
		}

	}
	if (find(finale.begin(), finale.end(), stare) == finale.end())
	{
		std::cout << "Cuvantul ales nu este acceptat";
		return false;
	}
	std::cout << "Cuvantul ales apartine automatului";
	return true;
}
void AFD::verificare()
{
	if (regula1() == false)
	{
		std::cout << "Starea initiala nu se gaseste in automat\n";
	}
	if (regula2() == false)
	{
		std::cout << "Starile finale nu se gasesc in automat\n";
	}
	if (regula3() == false)
	{
		std::cout << "Tranzitiile nu contin doar elemente ale automatului\n";
	}
	if (regula1() == true && regula2() == true && regula3() == true)
	{
		std::cout << "Automatul este ok";
	}
}


