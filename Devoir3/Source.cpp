#include <cstdlib>
#include<iostream>

#include "GraphParser.h"
#include "LayeredGraph.h"

using namespace std;

int main (void) {
	
	//parser file to get graph
	GraphParser<char> parserChar("test.afdC");
	Graph<char> gChar = parserChar.parseFile();

	
	gChar.traceAccept({ 'a','b','c','b','c','b','a' });
	gChar.traceAccept({ 'a','b','c','a','b','c' });
	
	cout << "------------------------------" << endl << endl;

	//parser file to get graph
	GraphParser<int> parserInt("test8.afdC");
	Graph<int> gInt = parserInt.parseFile();

	gInt.traceAccept({ 1,2,3,4,5 });
	gInt.traceAccept({ 1,2,3,4,5 });

	cout << "------------------------------" << endl << endl;
	
	//for word with a length of 5
	LayeredGraph<char> lg(gChar, 5);

	lg.traceAccept({ 'a','b','c','b','a' });
	lg.traceAccept({ 'a','b','c','a','b' });


	//TODO min weight and others...
	
	
	system("pause");
	return EXIT_SUCCESS;
}
