#include <cstdlib>
#include <iostream>
#include <map>
#include <algorithm>

#include "GraphParser.h"
#include "LayeredGraph.h"
#include "ConstraintParser.h"


using namespace std;
template<typename T>
void showAcceptedWords(vector<pair<vector<T>, int>> results);

template<typename T>
bool value_comparer(pair<vector<T>, int> & a, pair<vector<T>, int> const & b);

int main (void) {
	
	//parser file to get graph
	string inTrans="test8.afdC",inLimits="test8_limite.afdC";

	//cout << "Entrez le nom du fichier d'entrees contenant les transitions : (ex : test8.afdC)" << endl;
	//getline(cin, inTrans);
	//
	//cout << "Entrez le nom du fichier d'entrees contenant les limites : (ex : test8_limite.afdC)" << endl;
	//getline(cin, inLimits);


	try
	{
		cout << endl;
		GraphParser<int> graphParser(inTrans.c_str());
		cout << "Parsing graph from '" << inTrans << "' ..." << endl;
		Graph<int> baseGraph = graphParser.parseFile();

		ConstraintParser<int> constrParser(inLimits.c_str(), baseGraph);
		cout << "Parsing limits from '" << inLimits << "' ..." << endl;
		map<int, Constraint> constrs = constrParser.parseFile();

		cout << "Converting graph to layared graph ..." << endl;
		LayeredGraph<int> lg(baseGraph, constrParser.getWordLength());
		cout << "Finding all words corresponding to limits ..." << endl;
		vector<pair<vector<int>, int>> acceptedWords = lg.findAll(constrs);
		showAcceptedWords(acceptedWords);
	}
	catch (const std::exception& e)
	{
		cout << "ERROR : " << e.what()  << endl;
		system("pause");
		return 0;
	}






	/*gint.traceAccept({ 'a','b','c','b','c' }, constrs);
	lg.traceAccept({ 'a','b','c','b','c' }, constrs);
	system("pause");


	gint.traceAccept({ 'a','b','c','b','a' }, constrs);
	lg.traceAccept({ 'a','b','c','b','a' }, constrs);
	system("pause");


	gint.traceAccept({ 'a','b','c','b','b' }, constrs);
	lg.traceAccept({ 'a','b','c','b','b' }, constrs);
	system("pause");*/


	/*
	gint.traceAccept({ 'a','b','c','b','c','b','a' });
	gint.traceAccept({ 'a','b','c','a','b','c' });
	
	cout << "------------------------------" << endl << endl;

	//parser file to get graph
	GraphParser<int> parserInt("test8.afdC");
	Graph<int> gInt = parserInt.parseFile();

	gInt.traceAccept({ 1,2,3,4,5 });
	gInt.traceAccept({ 1,2,3,4,5 });

	cout << "------------------------------" << endl << endl;
	
	//for word with a length of 5
	LayeredGraph<int> lg(gint, 5);
	lg.traceAccept({ 'a','b','c','b','c' });
	lg.traceAccept({ 'a','b','c','b','a' });
	lg.traceAccept({ 'a','b','c','a','b' });


	//TODO min weight and others...
	*/
	
	system("pause");
	return EXIT_SUCCESS;
}

template<typename T>
bool value_comparer(pair<vector<T>, int> & a, pair<vector<T>, int> const & b)
{
	return a.second > b.second ;
}

template<typename T>
void showAcceptedWords(vector<pair<vector<T>, int>> results)
{
	sort(results.begin(), results.end(), value_comparer<T>);

	cout << "List of accepted words, sorted : " << endl;
	for (auto it = results.begin(); it != results.end(); ++it)
	{
		vector<T> word = it->first;
		for (int i = 0; i < word.size(); i++)
			cout << word[i];

		cout << " with WEIGHT of " << it->second << endl;
	}
}
