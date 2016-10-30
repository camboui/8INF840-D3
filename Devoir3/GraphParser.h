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
	ifstream m_file;
public:
	GraphParser(string filename);
	~GraphParser();

	Graph<T> parseFile();
};

#endif 

template<typename T>
GraphParser<T>::GraphParser(string filename)
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
GraphParser<T>::~GraphParser()
{

}

bool isFinal(vector<int> finals, int num) {
	return find(finals.begin(), finals.end(), num) != finals.end();
}

template<typename T>
inline Graph<T> GraphParser<T>::parseFile()
{
	Vertex<T>* initialV;
	Vertex<T>* finalV;
	string line;
	int alphabetSize = 0;
	int nbStates = 0;
	int initialVertex = 0;
	int nbFinalVertices = 0;
	int nbVertices = 0;
	int temp;
	vector<int> finalVertices;

	//check if the file is still open
	if (m_file.is_open())
	{
		m_file >> alphabetSize;
		m_file >> nbStates;
		m_file >> initialVertex;
		m_file >> nbFinalVertices;

		//get every final state
		for (int i = 0; i < nbFinalVertices; i++) {
			m_file >> temp;
			finalVertices.push_back(temp);
		}
		m_file >> nbVertices;

		//create graph with initial vertex
		Graph<T> graph(new Vertex<T>(initialVertex, isFinal(finalVertices, initialVertex)));

		//read all transitions
		for (int i = 0; i < nbVertices; i++) {
			T letter;
			m_file >> letter;

			//add Letter to graph alphabet
			try
			{
				graph.getAlphabet()->addLetter(letter);
			}
			catch (const std::logic_error&) {}

			int srcId, targetId, weight;
			m_file >> srcId >> targetId >> weight;

			//if states don't already exist, create them
			try
			{
				initialV = graph.getVertexByID(srcId);
			}
			catch (const std::logic_error&)
			{
				initialV = new Vertex<T>(srcId, isFinal(finalVertices, srcId));
				graph.addVertex(initialV);
			}

			try
			{
				finalV = graph.getVertexByID(targetId);
			}
			catch (const std::logic_error&)
			{
				finalV = new Vertex<T>(targetId, isFinal(finalVertices, targetId));
				graph.addVertex(finalV);
			}
			//then create transition
			initialV->addEdge(initialV, finalV, weight, letter);
		}
		//close the file
		m_file.close();
		return graph;
	}
	return nullptr;
}
