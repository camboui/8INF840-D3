#ifndef CONSTRAINT_PARSER_H
#define CONSTRAINT_PARSER_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>

#include "Graph.h"
#include "Alphabet.h"
#include "Constraint.h"

using namespace std;

template <typename T>
class ConstraintParser
{
private:
	Graph<T> m_graph;
	ifstream m_file;
	int m_wordLength;
public:
	ConstraintParser(string filename, Graph<T> g);

	map<T,Constraint> parseFile();
	int getWordLength();
};



template<typename T>
ConstraintParser<T>::ConstraintParser(string filename, Graph<T> g):
	m_graph(g), m_wordLength(0)
{
	//open the file filename
	m_file = ifstream(filename);

	if (!m_file.is_open())
	{
		string message = filename + " opening failed !";
		throw exception(message.c_str());
	}
}

template<typename T>
map<T, Constraint> ConstraintParser<T>::parseFile()
{
	int alphabetLength;
	m_file >> alphabetLength;
	m_file >> m_wordLength;
	map<T, Constraint> constraints = map<T, Constraint>();
	//contraints creation
	Alphabet<T>* alphabet = m_graph.getAlphabet();
	for (int i = 0; i < alphabetLength; i++) {
		constraints[alphabet->getLetter(i)] = Constraint();
	}
	//min
	int min;
	for (int i = 0; i < alphabetLength; i++) {
		m_file >> min;
		constraints[alphabet->getLetter(i)].setMin(min);
	}
	//max
	int max;
	for (int i = 0; i < alphabetLength; i++) {
		m_file >> max;
		constraints[alphabet->getLetter(i)].setMax(max);
	}
	return constraints;
}

template<typename T>
int ConstraintParser<T>::getWordLength()
{
	return m_wordLength;
}

#endif