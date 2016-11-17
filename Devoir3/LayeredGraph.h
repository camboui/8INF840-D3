#ifndef LAYERED_GRAPH_H
#define LAYERED_GRAPH_H

#include <vector>
#include <map>

#include "Constraint.h"
#include "Graph.h"


using namespace std;

template <typename T>
class LayeredGraph :
	public Graph<T>
{
private:
	vector<vector<Vertex<T>*>> m_layers;
	int m_nbLayers;
	void findAll_rec(vector<pair<vector<T>, int>>* results,Vertex<T>* v, vector<T> word, int weight, map<T, Constraint> constraints, map<T, int> letterCount);
public:
	LayeredGraph(Graph<T> g, int n);

	//virtual Vertex<T>* getVertexByID(int id);
	vector<vector<Vertex<T>*>> getLayers();
	vector<Vertex<T>*> getLayer(int index);
	vector<pair<vector<T>, int>> findAll(map<T, Constraint> constraints);
	virtual bool accept(vector<T> word, map<T, Constraint> constraints = nullptr);
	virtual int weight(vector<T> word, map<T, Constraint> constraints);
};


template<typename T>
void LayeredGraph<T>::findAll_rec(vector<pair<vector<T>, int>>* results, Vertex<T>* v, vector<T> word, int weight, map<T, Constraint> constraints, map<T, int> letterCount)
{
	cout << "State " << v->ID() << endl;
	for (int j = 0; j < v->getEdges().size(); j++) {
		T letter = v->getEdge(j)->getLetter();
		letterCount[letter]++;
		//cout << v->ID() << "->" << letter << "->" << v->getEdge(j)->getDestination()->ID() << endl;

		if (letterCount[letter] <= constraints[letter].getMax()) {
			vector<T> temp = vector<T>();
			temp.insert(temp.end(), word.begin(), word.end());
			temp.push_back(letter);
			findAll_rec(results, v->getEdge(j)->getDestination(), temp, weight+v->getEdge(j)->getCost(), constraints, letterCount);
			letterCount[letter]--;
		}
		if (v->getEdge(0)->getDestination()->isFinal())
		{
			bool minOk = true;
			for (int i = 0; i < getAlphabet()->getLetters().size(); i++)
			{
				if (letterCount[letter] < constraints[letter].getMin()) {
					minOk = false;
					break;
				}
			}
			if (minOk) {
				results->push_back(pair<vector<T>, int>(word, weight));
			}
		}
	}
}

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
	int p = 0;

	//for each vertex
	for (int j = 0; j < g.getVertices().size(); j++) {
		//add transition
		for (int k = 0; k < g.getVertex(j)->getEdges().size(); k++) {
			//for each layer
			for (int i = 0; i < n; i++) {
				try
				{
					//cout << p++ << endl;
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
vector<pair<vector<T>, int>> LayeredGraph<T>::findAll(map<T, Constraint> constraints)
{
	vector<pair<vector<T>, int>> *results = new vector<pair<vector<T>, int>>();
	findAll_rec(results, getInitialVertex()->getEdge(0)->getDestination(), vector<T>(), 0, constraints, map<T, int>());
	return *results;
}


template<typename T>
bool LayeredGraph<T>::accept(vector<T> word, map<T, Constraint> constraints)
{
	if (constraints.size() == 0) {
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
	else {
		map<T, int> letterCount = map<T, int>();
		Vertex<T>* test = getInitialVertex();
		Vertex<T>* current = getInitialVertex()->getEdge(0)->getDestination();
		for (int i = 0; i < word.size(); i++) {
			letterCount[word[i]]++;
			if (letterCount[word[i]] > constraints[word[i]].getMax()) {
				return false;
			}
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
		for (int i = 0; i < getAlphabet()->getLetters().size(); i++) {
			if (letterCount[i] < constraints[i].getMin()) {
				return false;
			}
		}
		return current->isFinal();
	}
}




template<typename T>
int LayeredGraph<T>::weight(vector<T> word, map<T, Constraint> constraints)
{
	map<T, int> letterCount = map<T, int>();
	Vertex<T>* current =  getInitialVertex()->getEdge(0)->getDestination();
	int* w = new int(0);
	for (int i = 0; i < word.size(); i++) {
		letterCount[word[i]]++;
		if (letterCount[word[i]] > constraints[word[i]].getMax()) {
			return -1;
		}
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
	if(current->getEdges().size()>0)
		current = current->getEdge(0)->getDestination();
	for (int i = 0; i < getAlphabet()->getLetters().size(); i++) {
		if (letterCount[i] < constraints[i].getMin()) {
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


#endif 