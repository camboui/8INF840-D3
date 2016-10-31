#include <cstdlib>
#include<iostream>

#include "GraphParser.h"
#include "LayeredGraph.h"

using namespace std;

template<typename T>
void printAndAccept(vector<T> word, Graph<T> g);

int main (void) {
	
	//parser file to get graph
	GraphParser<char> parserChar("test.afdC");
	Graph<char> gChar = parserChar.parseFile();

	printAndAccept({ 'a','b','c','b','c','b','a' }, gChar);
	printAndAccept({ 'a','b','c','a','b','c' }, gChar);
	
	cout << endl;

	//parser file to get graph
	GraphParser<int> parserInt("test8.afdC");
	Graph<int> gInt = parserInt.parseFile();

	printAndAccept({ 1,2,3,4,5 }, gInt);
	printAndAccept({ 1,2,3,4,5 }, gInt);

	cout << endl;
	
	//for word with a length of 5
	LayeredGraph<char> lg(gChar, 5);

	printAndAccept({ 'a','b','c','b','c','b','a' }, lg);
	printAndAccept({ 'a','b','c','a','b','c' }, lg);


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
		cout << "... is ACCEPTED by g" << endl;
	else
		cout << "... is REJECTED by g" << endl;
	cout << endl;
}