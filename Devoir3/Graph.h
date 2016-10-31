#ifndef GRAPH_H
#define GRAPH_H

#include <vector>

#include "Vertex.h"

using namespace std;


template <typename T>
class Graph
{
private:
	vector<Vertex<T>*> m_vertices;
	Vertex<T>* m_initialVertex;
	Alphabet<T>* m_alphabet;
public:
	Graph(Vertex<T>* initialVertex);

	Alphabet<T>* getAlphabet() {return m_alphabet;};
	Vertex<T>* getInitialVertex();
	vector<Vertex<T>*> getVertices();
	Vertex<T>* getVertex(int index);
	virtual Vertex<T>* getVertexByID(int id);
	void addVertex(Vertex<T>* v);
	void addVertex(int id);
	void addEdge(Vertex<T>* initialVertex, Vertex<T>* destinationVertex, int cost, T letter);
	virtual bool accepte(vector<T> word);
	virtual int weight(vector<T> word);
};



template<typename T>
Graph<T>::Graph(Vertex<T>* initialVertex):
	m_initialVertex(initialVertex)
{
	m_alphabet = new Alphabet<T>();
	m_vertices = vector<Vertex<T>*>();
	m_vertices.push_back(initialVertex);
}


template<typename T>
Vertex<T>* Graph<T>::getInitialVertex()
{
	return m_initialVertex;
}

template<typename T>
vector<Vertex<T>*> Graph<T>::getVertices()
{
	return m_vertices;
}

template<typename T>
Vertex<T>* Graph<T>::getVertex(int index)
{
	return m_vertices[index];
}

template<typename T>
Vertex<T>* Graph<T>::getVertexByID(int id)
{
	for (int i = 0; i < getVertices().size(); i++) {
		if (getVertex(i)->ID() == id) return getVertex(i);
	}
	throw logic_error("there is no vertex with the ID " + id);
}

template<typename T>
void Graph<T>::addVertex(Vertex<T>* v)
{
	if (find(m_vertices.begin(), m_vertices.end(), v) != m_vertices.end()) throw logic_error("this vertex already exists");
	m_vertices.push_back(v);
}

template<typename T>
void Graph<T>::addVertex(int id)
{
	Vertex<T>* v = new Vertex<T>(id);
	if (find(m_vertices.begin(), m_vertices.end(), v) != m_vertices.end()) throw logic_error("this vertex already exists");
	m_vertices.push_back(v);
}

template<typename T>
void Graph<T>::addEdge(Vertex<T>* initialVertex, Vertex<T>* destinationVertex, int cost, T letter)
{
	try
	{
		initialVertex->addEdge(initialVertex, destinationVertex, cost, letter);
	}
	catch (logic_error e)
	{
		cout << "Error : " << e.what() << endl;
	}
}


template<typename T>
bool Graph<T>::accepte(vector<T> word)
{
	Vertex<T>* current = m_initialVertex;

	for (int i = 0; i < word.size(); i++) {
		try
		{
			current = current->nextVertex(word[i]);
		}
		catch (logic_error e)
		{
			cout << "Staying in the same vertex : " << e.what() << endl;
		}
	}

	return current->isFinal();
}

template<typename T> 
int Graph<T>::weight(vector<T> word)
{
	Vertex<T>* current = m_initialVertex;
	int* w = new int(0);
	for (int i = 0; i < word.size(); i++) {
		try
		{
			current = current->nextVertex(word[i], w);
		}
		catch (logic_error e)
		{
			cout << "Error : " << e.what() << endl;
		}
	}
	if (current->isFinal()) {
		return *w;
	}
	else {
		cout << "Error : word rejected" << endl;
		return -1;
	}
}



#endif 