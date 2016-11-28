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
	vector<Edge<T>*> m_incomingEdges; 
public:
	Vertex(int id, bool isFinal);

	int ID();
	bool isFinal();
	vector<Edge<T>*> getEdges();
	vector<Edge<T>*> getIncomingEdges();
	Edge<T>* getEdge(int index);
	Edge<T>* getIncomingEdge(int index);
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
vector<Edge<T>*> Vertex<T>::getIncomingEdges()
{
	return m_incomingEdges;
}

template<typename T>
Edge<T>* Vertex<T>::getEdge(int index)
{
	return m_edges[index];
}


template<typename T>
Edge<T>* Vertex<T>::getIncomingEdge(int index)
{
	return m_incomingEdges[index];
}

template<typename T>
void Vertex<T>::addEdge(Vertex<T>* initialVertex, Vertex<T>* destinationVertex, int cost, T letter)
{
	Edge<T>* e = new Edge<T>(initialVertex, destinationVertex, cost, letter);
	if (find(m_edges.begin(), m_edges.end(), e) == m_edges.end()) {
		for (int i = 0; i < m_edges.size(); i++) {
			if (m_edges[i]->getLetter() == letter) {
				throw logic_error("this edge already exists");
			}
		}
		m_edges.push_back(e);
		destinationVertex->m_incomingEdges.push_back(e);
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
			cout << m_edges[i]->getInitial()->ID() << "-> '" << letter << "' ->" << m_edges[i]->getDestination()->ID() << endl;
			return m_edges[i]->getDestination();
		}
	}
	string msg = "there is no transition for letter '"s + to_string(letter) + "' from vertex "s + to_string(m_ID);
	throw logic_error(msg.c_str());
}

template<typename T>
Vertex<T>* Vertex<T>::nextVertex(T letter, int * w)
{
	for (int i = 0; i < m_edges.size(); i++) {
		if (m_edges[i]->getLetter() == letter) {
			*w += m_edges[i]->getCost();
			cout << m_edges[i]->getInitial()->ID() << "-> '" << letter << "' ->" << m_edges[i]->getDestination()->ID() << endl;
			return m_edges[i]->getDestination();
		}
	}
	string msg = "there is no transition for letter '"s + to_string(letter) +"' from vertex "s + to_string(m_ID);
	throw logic_error(msg.c_str());
}

#endif 