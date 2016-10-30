#ifndef GRAPH_PARSER_H
#define GRAPH_PARSER_H

#include <iostream>
#include <fstream>
#include <sstream>

#include "Alphabet.h"
#include "Graph.h"

using namespace std;

template <typename T>
class GraphParser
{
private:
	Alphabet<T> m_alphabet;
	Graph<T> m_graph;
	ifstream m_file;
public:
	GraphParser(string filename);
	~GraphParser();

	Alphabet<T> getAlphabet() { return m_alphabet; };
	Graph<T> getGraph() { return m_graph; };
};

#endif 

template<typename T>
GraphParser<T>::GraphParser(string filename)
{
}

template<typename T>
GraphParser<T>::~GraphParser()
{
	m_alphabet.~Alphabet();
	m_graph.~Graph();
}
