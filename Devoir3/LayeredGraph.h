#ifndef LAYERED_GRAPH_H
#define LAYERED_GRAPH_H

#include <vector>

#include "Graph.h"

using namespace std;

template <typename T>
class LayeredGraph :
	public Graph<T>
{
private:
	vector<vector<Vertex<T>*>> m_layers;
	int m_nbLayers;
public:
	LayeredGraph(Graph<T> g, int n);

	//virtual Vertex<T>* getVertexByID(int id);
	vector<vector<Vertex<T>*>> getLayers();
	vector<Vertex<T>*> getLayer(int index);
	virtual bool accept(vector<T> word);
	virtual int weight(vector<T> word);
};


template<typename T>
LayeredGraph<T>::LayeredGraph(Graph<T> g, int n) :
	Graph(new Vertex<T>(-1, false)),
	m_nbLayers(n)
{

	//create each layer and add it to the graph
	m_layers = vector<vector<Vertex<T>*>>();
	for (int i = 0; i < n + 1; i++) {
		vector<Vertex<T>*> layer = vector<Vertex<T>*>();
		for (int j = 0; j < g.getVertices().size(); j++) {
			Vertex<T> *v = new Vertex<T>(i * 1000 + g.getVertex(j)->ID(), false);
			layer.push_back(v);
			addVertex(v);
		}
		m_layers.push_back(layer);
	}

	//for each layer
	for (int i = 0; i < n; i++) {
		//for each vertex
		for (int j = 0; j < g.getVertices().size(); j++) {
			//add transition
			for (int k = 0; k < g.getVertex(j)->getEdges().size(); k++) {
				try
				{
					Vertex<T>* initial = getVertexByID(i * 1000 + g.getVertex(j)->ID());
					Vertex<T>* destination = getVertexByID((i + 1) * 1000 + g.getVertex(j)->getEdge(k)->getDestination()->ID());
					addEdge(initial, destination, g.getVertex(j)->getEdge(k)->getCost(), g.getVertex(j)->getEdge(k)->getLetter());
				}
				catch (logic_error e)
				{
					cout << "Error : " << e.what() << endl;
				}
			}
		}
	}

	Vertex<T>* temp = getVertexByID(1);

	//link every final vertex of simpleGraph to LayeredGraph final vertex
	Vertex<T>* finalVertex = new Vertex<T>(-2, true); //special ID
	addVertex(finalVertex);
	for (int i = 0; i < g.getVertices().size(); i++) {
		//start transition
		if (g.getInitialVertex()->ID() == i) {
			addEdge(getInitialVertex(), getVertexByID(i), 0, numeric_limits<T>::min()); //special values
		}
		//end transitions
		if (g.getVertex(i)->isFinal()) {
			Vertex<T>* initial = getVertexByID((n)* 1000 + g.getVertex(i)->ID());
			addEdge(initial, finalVertex, 0, numeric_limits<T>::min()); //special values
		}
	}

}


//template<typename T>
//Vertex<T>* LayeredGraph<T>::getVertexByID(int id)
//{
//	int layer = id / 1000;
//	for (int i = 0; i < m_layers[layer].size(); i++) {
//		if (m_layers[layer][i]->ID() == id) return m_layers[layer][i];
//	}
//	throw logic_error("there is no vertex with the ID " + id);
//}

template<typename T>
vector<vector<Vertex<T>*>> LayeredGraph<T>::getLayers()
{
	return m_layers;
}

template<typename T>
vector<Vertex<T>*> LayeredGraph<T>::getLayer(int index)
{
	return m_layers[index];
}


template<typename T>
bool LayeredGraph<T>::accept(vector<T> word)
{
	Vertex<T>* test = getInitialVertex();
	Vertex<T>* current = getInitialVertex()->getEdge(0)->getDestination();
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
	current = current->getEdge(0)->getDestination();
	return current->isFinal();
}




template<typename T>
int LayeredGraph<T>::weight(vector<T> word)
{
	Vertex<T>* current =  getInitialVertex()->getEdge(0)->getDestination();
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
	current = current->getEdge(0)->getDestination();
	if (current->isFinal()) {
		return *w;
	}
	else {
		cout << "Error : word rejected" << endl;
		return -1;
	}
}


#endif 