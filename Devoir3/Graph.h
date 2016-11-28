#ifndef GRAPH_H
#define GRAPH_H

#include <map>
#include <vector>

#include "Vertex.h"
#include "Alphabet.h"
#include "Constraint.h"

using namespace std;


template <typename T>
class Graph
{
private:
	map<int, Vertex<T>*> m_vertices;
	Vertex<T>* m_initialVertex;
	Alphabet<T>* m_alphabet;
	bool leadToFinalVertex(Vertex<T>* v);
	bool accessibleFromInitialVertex(Vertex<T>* v);
public:
	Graph(Vertex<T>* initialVertex);

	Alphabet<T>* getAlphabet();
	Vertex<T>* getInitialVertex();
	map<int, Vertex<T>*> getVertices();
	Vertex<T>* getVertex(int index);
	virtual Vertex<T>* getVertexByID(int id);
	void addVertex(Vertex<T>* v);
	void addVertex(int id);
	void addEdge(Vertex<T>* initialVertex, Vertex<T>* destinationVertex, int cost, T letter);
	virtual bool accept(vector<T> word);
	virtual void traceAccept(vector<T> word, map<T, Constraint> constraints = map<T, Constraint>());
	virtual int weight(vector<T> word, map<T, Constraint> constraints = map<T, Constraint>());
	bool acceptEdge(Edge<T>* e);
};



template<typename T>
Graph<T>::Graph(Vertex<T>* initialVertex):
	m_initialVertex(initialVertex)
{
	m_alphabet = new Alphabet<T>();
	m_vertices = map<int,Vertex<T>*>();
	m_vertices.insert(pair<int, Vertex<T>*>(initialVertex->ID(), initialVertex));

}


template<typename T>
Alphabet<T>* Graph<T>::getAlphabet()
{
	return m_alphabet;
}

template<typename T>
Vertex<T>* Graph<T>::getInitialVertex()
{
	return m_initialVertex;
}

template<typename T>
map<int, Vertex<T>*> Graph<T>::getVertices()
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
	map<int, Vertex<T>*>::iterator it = m_vertices.find(id);
	
	if (it != m_vertices.end()) { return it->second; }
	else { throw logic_error("there is no vertex with the ID " + id); }
}

template<typename T>
void Graph<T>::addVertex(Vertex<T>* v)
{
	map<int, Vertex<T>*>::iterator it = m_vertices.find(v->ID());

	if (it == m_vertices.end()) { m_vertices.insert(pair<int, Vertex<T>*>(v->ID(), v)); }
	else { throw logic_error("this vertex already exists"); }
}

template<typename T>
void Graph<T>::addVertex(int id)
{
	Vertex<T>* v = new Vertex<T>(id);
	map<int, Vertex<T>*>::iterator it = m_map_vertices.find(id);
	if (it == m_vertices.end()) { m_vertices.insert(pair<int, Vertex<T>*>(v->ID(), v)); }
	else { throw logic_error("this vertex already exists"); }
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
bool Graph<T>::accept(vector<T> word)
{
	Vertex<T>* current = m_initialVertex;

	for (int i = 0; i < word.size(); i++) {
		try
		{
			current = current->nextVertex(word[i]);
		}
		catch (logic_error e)
		{
			cout << "Can't accept : " << e.what() << endl;
			return false;
		}
	}

	return current->isFinal();
}

template<typename T>
inline void Graph<T>::traceAccept(vector<T> word, map<T, Constraint> constraints)
{
	//print word
	for (int i = 0; i < word.size(); i++)
		cout << word[i] << " ";
	cout << endl;

	//show if accepted or not
	int w = weight(word, constraints);
	if (w!=-1)
		cout << "... is ACCEPTED by g with a weight of : "<< w << endl;
	else
		cout << "... is REJECTED by g" << endl;
	cout << endl;
}

template<typename T> 
int Graph<T>::weight(vector<T> word, map<T, Constraint> constraints)
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
			cout << "Can't accept : " << e.what() << endl;
			return -1;
		}
	}
	if (current->isFinal()) {
		return *w;
	}
	else {
		cout << "Can't accept : Not in a final State " << endl;
		return -1;
	}
}


template<typename T>
bool Graph<T>::accessibleFromInitialVertex(Vertex<T>* v)
{
	if (v->ID() != m_initialVertex->ID()){
		for (int i = 0; i < v->getIncomingEdges().size(); i++){
			Vertex<T>* current = v->getIncomingEdge(i)->getInitial();
			if (current->ID() == m_initialVertex->ID()){
				return true;
			}
			else if (accessibleFromInitialVertex(current) == true){
				return true;
			}
		}
		return false;
	}
	else return true;
}

template<typename T>
bool Graph<T>::leadToFinalVertex(Vertex<T>* v)
{
	if (!v->isFinal()){
		for (int i = 0; i < v->getEdges().size(); i++){
			Vertex<T>* current = v->getEdge(i)->getDestination();
			if (current->isFinal()){
				return true;
			}
			else if (leadToFinalVertex(current)){
				return true;
			}
		}
		return false;
	}
	return true;
}

template<typename T>
bool Graph<T>::acceptEdge(Edge<T>* e)
{
	Vertex<T>* from = e->getInitial();
	Vertex<T>* to = e->getDestination();
	return accessibleFromInitialVertex(from) && leadToFinalVertex(to);
}




#endif 