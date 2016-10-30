#ifndef GRAPH_PARSER_H
#define GRAPH_PARSER_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include "Alphabet.h"
#include "Graph.h"

using namespace std;

template <typename T>
class GraphParser
{
private:
	Alphabet<T>* m_alphabet;
	Graph<T>* m_graph;
	ifstream m_file;
public:
	GraphParser(string filename);
	~GraphParser();

	Alphabet<T> getAlphabet() { return *m_alphabet; };
	Graph<T> getGraph() { return *m_graph; };
	Graph<T> parseFile();
};

#endif 

template<typename T>
GraphParser<T>::GraphParser(string filename)
{
	m_graph = nullptr;
	m_alphabet = nullptr;
	//open the file filename
	m_file = ifstream(filename);

	if (!m_file.is_open())
	{
		std::string message = filename + " opening failed !";
		throw exception(message.c_str());
	}
}

template<typename T>
GraphParser<T>::~GraphParser()
{

}

template<typename T>
inline Graph<T> GraphParser<T>::parseFile()
{
	Graph<T> graph;
	string line;
	string finalVertex;
	int alphabetSize = 0;
	int nbStates = 0;
	int initialVertex = 0;
	int nbFinalVertex = 0;
	vector<int> finalVertex;
	
	//check if the file is still open
	if (m_file.is_open())
	{
		m_file >> alphabetSize;
		m_file >> nbStates;
		m_file >> initialVertex;
		m_file >> nbFinalVertex;
		getline(m_file, finalVertex);
	

		//read all transitions
		while (!m_file.eof()) {
			T letter;
			m_file >> letter;
			m_alphabet.addLetter(letter);

			int srcId, targetId, weight;
			m_file >> srcId;
			m_file >> targetId;
			m_file >> weight;

			if (getVertex(src) != nullptr) {
				addVertex(new Vertex<T>* (srcId, srcId));
			}
			
			//line
		}
		//Final Vertex

		//close the file
		m_file.close();
	}
	return legos;
}
