#include <cstdlib>
#include<iostream>

#include "GraphParser.h"
#include "LayeredGraph.h"

using namespace std;

template<typename T>
void printAndAccept(vector<T> word, Graph<T> g);

int main (void) {
	
	//TODO PARSER

	GraphParser<char> parserChar("test.afdC");
	Graph<char> gChar = parserChar.parseFile();

	printAndAccept({ 'a','b','c','a','b','c','b','a' }, gChar);
	printAndAccept({ 'a','b','c','a','b','c' }, gChar);
	
	cout << endl;

	GraphParser<int> parserInt("test8.afdC");
	Graph<int> gInt = parserInt.parseFile();

	printAndAccept({ 1,2,3,4,5 }, gInt);
	printAndAccept({ 1,2,3,4,5 }, gInt);

	
	//for word with a length of 5
	//LayeredGraph<int> lg(gInt, 5);

	//printAndAccept({ 1,2,3,4,5 }, lg);
	//printAndAccept({ 1,2,3,4,5 }, lg);


	//TODO min weight and others...
	
	
	system("pause");
	return EXIT_SUCCESS;
}

template<typename T>
void printAndAccept(vector<T> word,Graph<T> g) {

	//print word
	for (int i = 0; i < word.size(); i++)
		cout << word[i] << " ";
	cout << endl;
	//show if accepted or not
	if (g.accepte(word))
		cout << "... is accepted by g" << endl;
	else
		cout << "... is rejected by g" << endl;
	cout << endl;
}