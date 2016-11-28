#include <cstdlib>
#include <iostream>
#include <map>
#include <algorithm>
#include <ctime>

#include "GraphParser.h"
#include "LayeredGraph.h"
#include "ConstraintParser.h"


using namespace std;
template<typename T>
void showAcceptedWords(vector<pair<vector<T>, int>> results, int nbPrint);

template<typename T>
bool value_comparer(pair<vector<T>, int> & a, pair<vector<T>, int> const & b);

template<typename T>
void printWordWithWeight(vector<T> word, int weight);

template<typename T>
void printWord(vector<T> word);

template<typename T>
vector<T> enterWord(int nbLetters);

void showTime(clock_t startTime);

template<typename T>
void EdgeAccepted(Graph<T> g);


int main(void) {

	//parser file to get graph
	string inTrans = "test23.afdC", inLimits = "test23_limite.afdC";
	int nbResult = 1;
	int choice;
	bool quit = false;


	cout << "\t\t\t\t--- Enter your choice ---" << endl;
	cout << "\t1 . Use default entry files : '" << inTrans << "' and '" << inLimits << "'" << endl;
	cout << "\t2 . Give your own entry files" << endl;
	cin >> choice;
	if (choice == 2) {
		cout << " -> Enter filename containing transitions : (ex : test23.afdC)" << endl;
		cin >> inTrans;
		
		cout << " -> Enter filename containing limits: (ex : test23_limite.afdC)" << endl;
		cin >> inLimits;
	}
	else
		cout << "Using DEFAULT files" << endl;

	try
	{
		
		cout << endl;
		GraphParser<int> graphParser(inTrans.c_str());
		cout << "Parsing graph from '" << inTrans << "' ..." << endl;
		Graph<int> baseGraph = graphParser.parseFile();

		ConstraintParser<int> constrParser(inLimits.c_str(), baseGraph);
		cout << "Parsing constraints from '" << inLimits << "' ..." << endl;
		map<int, Constraint> constrs = constrParser.parseFile();

		cout << "Converting graph to layered graph ..." << endl;
		LayeredGraph<int> lg(baseGraph, constrParser.getWordLength());


		
		

		while (!quit) {
			cout << endl << endl;
			cout << "\t\t\t\t--- Enter your choice ---" << endl;
			cout << "\t1 . Enter a word to know weather it's accepted or not WITHOUT constraints by layered graph (size : " << constrParser.getWordLength() << ")" << endl;
			cout << "\t2 . Enter a word to know weather it's accepted or not WITH constraints by layered graph (size : " << constrParser.getWordLength() << ")" << endl;
			cout << "\t3 . Look for EVERY accepted word according to constraints (MIGHT TAKE TIME)" << endl;
			cout << "\t4 . Look for LOWEST accepted word according to constraints" << endl;
			cout << "\t5 . Check if an edge can be accepted" << endl;
			cout << "\t6 . Exit" << endl;

			cin >> choice;
			std::clock_t start;

			pair<vector<int>, int> acceptedLowestWeight;
			vector<pair<vector<int>, int>> acceptedWords;
			vector<int> wordToRead = vector<int>();
			cout << endl;
			switch (choice)
			{
			case 1:
				wordToRead = enterWord<int>(constrParser.getWordLength());
				start = std::clock();
				lg.traceAccept(wordToRead);
				showTime(start);
				break;
			case 2:
				wordToRead = enterWord<int>(constrParser.getWordLength());
				start = std::clock();
				lg.traceAccept(wordToRead, constrs);
				showTime(start);
				break;
			case 3:
				cout << "Finding ALL words (MIGHT TAKE TIME) corresponding to limits in layered graph ..." << endl;
				start = std::clock();
				acceptedWords = lg.findAll(constrs);
				showTime(start);
				cout << " -> ENTER NUMBER OF RESULTS TO DISPLAY ..." << endl;
				cin >> nbResult;
				showAcceptedWords(acceptedWords, nbResult);
				break;
			case 4:
				cout << "Finding lowest weight accepted ..." << endl;
				start = std::clock();
				acceptedLowestWeight = lg.findOnlyFirst(constrs);
				//acceptedLowestWeight = lg.findOnlyFirstDijkstra(constrs);
				showTime(start);
				cout << "Lowest weight accepted word is :" << endl;
				printWordWithWeight(get<0>(acceptedLowestWeight), get<1>(acceptedLowestWeight));
				break;
			case 5:
				EdgeAccepted(lg);
				break;
			case 6:
				quit = true;
				break;
			default:
				cout << "INVALID ENTRY" << endl;
				break;
			}
		}

	}
	catch (const std::exception& e)
	{
		cout << "ERROR : " << e.what() << endl;
		system("pause");
		return 0;
	}
	return EXIT_SUCCESS;
}

void showTime(clock_t startTime)
{
	double duration = (std::clock() - startTime) / (double)CLOCKS_PER_SEC;
	std::cout << "It took " << duration << " seconds" << endl;
}

template<typename T>
vector<T> enterWord(int nbLetters)
{
	int cpt = 1;
	vector<T>wordToRead = vector<T>();
	T letter;
	cout << endl << "Enter the word you want of size " << nbLetters << endl;
	cin >> letter;
	wordToRead.push_back(letter);
	do
	{
		cpt++;
		cout << " " << nbLetters - cpt +1 << " letters left -- ";
		cout << "Current word is : ";
		printWord(wordToRead);
		cin >> letter;
		wordToRead.push_back(letter);
	} while (cpt < nbLetters);
	cout << endl;
	return wordToRead;
}

template<typename T>
void printID(pair<int, Vertex<T>*> p)
{
	p.second->ID();
}

template<typename T>
bool value_comparer(pair<vector<T>, int> & a, pair<vector<T>, int> const & b)
{
	return a.second < b.second;
}

template<typename T>
void showAcceptedWords(vector<pair<vector<T>, int>> results, int nbPrint)
{
	sort(results.begin(), results.end(), value_comparer<T>);

	cout << "List of " << nbPrint << " smallest weight accepted words : " << endl;
	int cpt = 0;
	for (auto it = results.begin(); it != results.end(); ++it)
	{
		printWordWithWeight(it->first, it->second);

		cpt++;
		if (cpt >= nbPrint)
			break;
	}
}

template<typename T>
void printWordWithWeight(vector<T> word, int weight)
{
	if (word.size() == 0)
	{
		cout << "No word find" << endl;
	}
	else {
		printWord(word);
		cout << " with WEIGHT of " << weight << endl;
	}
}

template<typename T>
void printWord(vector<T> word)
{
		for (int i = 0; i < word.size(); i++)
			cout << word[i] << ",";
}

template<typename T>
void EdgeAccepted(Graph<T> g) {
	int initialVertexID, edgeNumber;
	for (auto const & i : g.getVertices())
	{
		if (i.second != nullptr) {
			cout << i.second->ID() << " / ";
		}
	}
	cout << endl << "Choose an initial vertex : " << endl;
	cin >> initialVertexID;
	Vertex<T>* v = g.getVertexByID(initialVertexID);
	cout << "Choose an edge : " << endl;
	for (int i = 0; i < v->getEdges().size(); i++) {
		cout << i + 1 << ". FROM " << v->getEdge(i)->getInitial()->ID()
			<< " TO " << v->getEdge(i)->getDestination()->ID()
			<< " WITH " << v->getEdge(i)->getLetter()
			<< " FOR " << v->getEdge(i)->getCost() << endl;
	}
	cin >> edgeNumber;
	Edge<T>* e = v->getEdge(edgeNumber - 1);
	cout <<"\tThe edge FROM " << e->getInitial()->ID()
		<< " TO " << e->getDestination()->ID()
		<< " WITH " << e->getLetter()
		<< " FOR " << e->getCost();
	if (g.acceptEdge(e) == true) {
		cout << " CAN be accepted" << endl;
	}
	else {
		cout << " CAN'T be accepted" << endl;
	}
}