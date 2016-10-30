#ifndef VERTEX_H
#define VERTEX_H

#include <vector>

#include "Edge.h"

using namespace std;

template <typename T>
class Vertex
{
private:
	int m_ID;
	bool m_isFinal;	
	vector<Edge<T>*> m_edges; //outgoing edges
public:
	Vertex(int id, bool isFinal);

	int ID();
	bool isFinal();
	vector<Edge<T>*> getEdges();
	Edge<T>* getEdge(int index);
	void addEdge(Vertex<T>* initialVertex, Vertex<T>* destinationVertex, int cost, T letter);
	Vertex<T>* nextVertex(T letter);
	Vertex<T>* nextVertex(T letter, int *w);
};



template<typename T>
Vertex<T>::Vertex(int id, bool isFinal):
	m_ID(id),
	m_isFinal(isFinal)
{
	m_edges = vector<Edge<T>*>();
}


template<typename T>
int Vertex<T>::ID()
{
	return m_ID;
}

template<typename T>
bool Vertex<T>::isFinal()
{
	return m_isFinal;
}

template<typename T>
vector<Edge<T>*> Vertex<T>::getEdges()
{
	return m_edges;
}

template<typename T>
Edge<T>* Vertex<T>::getEdge(int index)
{
	return m_edges[index];
}


template<typename T>
void Vertex<T>::addEdge(Vertex<T>* initialVertex, Vertex<T>* destinationVertex, int cost, T letter)
{
	Edge<T>* e = new Edge<T>(initialVertex, destinationVertex, cost, letter);
	if (find(m_edges.begin(), m_edges.end(), e) == m_edges.end()) {
		for (int i = 0; i < m_edges.size(); i++) {
			if (m_edges[i]->getLetter() == letter && m_edges[i]->getCost() == cost) {
				throw logic_error("this edge already exists");
			}
		}
		m_edges.push_back(e);
	}
	else {
		throw logic_error("this edge already exists");
	}
}

template<typename T>
Vertex<T>* Vertex<T>::nextVertex(T letter)
{
	for (int i = 0; i < m_edges.size(); i++) {
		if (m_edges[i]->getLetter() == letter) {
			return m_edges[i]->getDestination();
		}
	}
	throw logic_error("there is no transition by this letter");
}

template<typename T>
Vertex<T>* Vertex<T>::nextVertex(T letter, int * w)
{
	for (int i = 0; i < m_edges.size(); i++) {
		if (m_edges[i]->getLetter() == letter) {
			*w += m_edges[i]->getCost();
			return m_edges[i]->getDestination();
		}
	}
	throw logic_error("there is no transition by this letter");
}

#endif 