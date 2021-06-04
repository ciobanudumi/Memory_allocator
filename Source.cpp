#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <time.h>
#include <list>
#include <iterator>
#include <iomanip>
#include <windows.h>

using namespace std;

ofstream outputBest;
ofstream outputFirst;
ofstream dateOutputFirst;
ofstream dateOutputBest;

struct procese {
	int nrProces;
	int memoria;
	int inceputTimp;
	int sfarsitTimp;
};
list<procese> listProces;

struct partitie {
	bool ocupat = false;
	int nrProces = -1;
	int memorie;
};
list<partitie> Memorie;

HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
int DeltaMin , DeltaMax , Mmin , Mmax , Dmin , Dmax, DS, TotalMemory ;

int randomNumber(int Min, int Max);

void generator() {
	int inceput = 0;
	int sfarsit = 0;
	int nrProces = 0;

	while((inceput + DeltaMax) < DS) {
		procese pr;

		nrProces++;
		inceput += randomNumber(DeltaMin, DeltaMax);
		sfarsit = inceput + randomNumber(Dmin, Dmax);

		pr.nrProces = nrProces;
		pr.inceputTimp = inceput;

		sfarsit>DS ? pr.sfarsitTimp = DS: pr.sfarsitTimp = sfarsit;

		pr.memoria = randomNumber(Mmin, Mmax);
		listProces.push_back(pr);
	} 
	//cout << endl<< "Numarul de procese: " << nrProces<<endl;
	dateOutputBest << nrProces<<endl;
	dateOutputFirst << nrProces << endl;
}

void writeListProcese() {
	SetConsoleTextAttribute(hConsole, 15);
	list <procese> ::iterator it;
	for (it = listProces.begin(); it != listProces.end(); ++it) {
		cout << "NR: " << setw(3) << it->nrProces << " |";
		cout << " Memoria ocupata: "<< setw(6) << it->memoria << " |";
		cout << " Timpul de inceput: " << setw(6) << it->inceputTimp << " |";
		cout << " Timpul de Sfarsit: " << setw(6) << it->sfarsitTimp << " |" << '\n';
	}
}

bool bestFit(int nrP, int mem) {
	int memMin = TotalMemory+1;
	list<partitie>::iterator x;

	for (list<partitie>::iterator it = Memorie.begin(); it != Memorie.end(); it++) {
		if ((it->ocupat == false) && (it->memorie >= mem)) {
			if ((it->memorie < memMin)) {
				memMin = it->memorie;
				x = it;
			}
		}
	}

	if (memMin != TotalMemory+1) {
		partitie pr;
		pr.memorie = mem;
		pr.nrProces = nrP;
		pr.ocupat = true;
		Memorie.insert(x, pr);
		x->memorie -= pr.memorie;
		return true;
	}

	return false;
}

bool firstFit(int nrP,int mem) {
	for (list<partitie>::iterator it = Memorie.begin(); it != Memorie.end(); it++) {
		if ((it->ocupat==false) && (it->memorie >=mem)) {
			partitie pr;
			pr.memorie = mem;
			pr.nrProces = nrP;
			pr.ocupat = true;
			Memorie.insert(it, pr);
			it->memorie -= pr.memorie;
			return true;
		}
	}
	return false;
}

void ReconstruireaMemoriei() {
	list<partitie>::iterator i = Memorie.begin();
	while (i != Memorie.end()){
		if (i->ocupat == false) {
			if ((i._Ptr->_Next != Memorie.end()._Ptr) && (i._Ptr->_Next->_Myval.ocupat == false)) {
				int m = i->memorie;
				i._Ptr->_Next->_Myval.memorie += m;
				i=Memorie.erase(i);
			}
			else {
				i++;
			}
		}
		else {
			i++;
		}
	}
}

bool Dezalocare(int nrProces) {
	for (list<partitie>::iterator i = Memorie.begin(); i != Memorie.end(); i++) {
		if (i->nrProces == nrProces) {
			i->ocupat = false;
			i->nrProces = -1;
			ReconstruireaMemoriei();
			return true;
		}
	}
	return false;
}

void SituatieMemorie() {
	int start = 0;
	int end = 0;
	for (list<partitie>::iterator i = Memorie.begin(); i != Memorie.end(); i++) {
		if (i->ocupat == true) {
			while (i->ocupat == true && i != Memorie.end()) {
				end += i->memorie;
				i++;
			}
			--i;
			SetConsoleTextAttribute(hConsole, 12);
			cout << end - start<<" | ";
		}
		else {
			end += i->memorie;
			SetConsoleTextAttribute(hConsole, 10);
			cout << end - start << " | ";
		}
			start = end;
	}
	cout << endl << endl;
}

void SimulareFirstFit() {
	//cout << endl;
	float reusite = 0;
	float esuate = 0;
	for (int i = 0; i <= DS; i++) {
		for (list<procese>::iterator j = listProces.begin(); j != listProces.end(); j++) {
			if (i == j->inceputTimp) {
				if (firstFit(j->nrProces,j->memoria)) {
					//SetConsoleTextAttribute(hConsole, 10);
					//cout <<"Alocare   "<< j->nrProces << endl;
					//SituatieMemorie();
					reusite++;
					outputFirst << reusite+esuate << " " << reusite/(reusite + esuate) * 100<<endl;
				}
				else {
					//SetConsoleTextAttribute(hConsole, 14);
					//cout << "Alocare   " << j->nrProces << endl << endl;
					esuate++;
					outputFirst << reusite + esuate << " " << reusite / (reusite + esuate) * 100 << endl;
				}
			}
			if (i == j->sfarsitTimp) {
				//SetConsoleTextAttribute(hConsole, 15);
				if (Dezalocare(j->nrProces)) {
					//cout << "Dealocare " << j->nrProces << endl;
					//SituatieMemorie();
				}
			}
		}
	}
	dateOutputFirst << reusite<<endl;
	dateOutputFirst << esuate;
}

void SimulareBestFit() {
	//cout << endl;
	float reusite = 0;
	float esuate = 0;
	for (int i = 0; i <= DS; i++) {
		for (list<procese>::iterator j = listProces.begin(); j != listProces.end(); j++) {
			if (i == j->inceputTimp) {
				if (bestFit(j->nrProces, j->memoria)) {
					//SetConsoleTextAttribute(hConsole, 10);
					//cout <<"Alocare   "<< j->nrProces << endl;
					//SituatieMemorie();
					reusite++;
					outputBest << reusite + esuate << " " << reusite / (reusite + esuate) * 100 << endl;
				}
				else {
					//SetConsoleTextAttribute(hConsole, 14);
					//cout << "Alocare   " << j->nrProces << endl << endl;
					esuate++;
					outputBest << reusite + esuate << " " << reusite / (reusite + esuate) * 100 << endl;
				}
			}
			if (i == j->sfarsitTimp) {
				//SetConsoleTextAttribute(hConsole, 15);
				if (Dezalocare(j->nrProces)) {
					//cout << "Dealocare " << j->nrProces << endl;
					//SituatieMemorie();
				}
			}
		}
	}
	dateOutputBest << reusite << endl;
	dateOutputBest << esuate;
}

void read(string fileName) {
	ifstream file(fileName);

	string s;

	getline(file, s);
	DS = stoi(s);				//Durata Simularii
	getline(file, s);
	TotalMemory = stoi(s);		//Memoria Maxima
	getline(file, s);
	DeltaMin = stoi(s);			//Durata Minima dintre procese
	getline(file, s);
	DeltaMax = stoi(s);			//Durata Maxima dintre procese
	getline(file, s);
	Mmin = stoi(s);				//Memoria Minima
	getline(file, s);
	Mmax = stoi(s);				//Memoria Maxima
	getline(file, s);
	Dmin = stoi(s);				//Durata Minima a proceselor
	getline(file, s);
	Dmax = stoi(s);				//Durata Maxima a proceselor
	file.close();
}

int randomNumber(int Min, int Max) {
		return Min + std::rand() % (Max - Min);
}

int main(){
	outputBest.open("outputBest.txt");
	outputFirst.open("outputFirst.txt");
	dateOutputBest.open("dateAditionaleBest.txt");
	dateOutputFirst.open("dateAditionaleFirst.txt");
	read("input.in");
	srand((unsigned int)time(nullptr));
	generator();
	//writeListProcese();

	partitie pr;
	pr.memorie = TotalMemory;
	Memorie.push_back(pr);

	SimulareFirstFit();
	SimulareBestFit();


	outputBest.close();
	outputFirst.close();
	dateOutputBest.close();
	dateOutputFirst.close();

	return 0;
}