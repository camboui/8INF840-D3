#ifndef LAYERED_GRAPH_H
#define LAYERED_GRAPH_H

#include <vector>
#include <map>
#include <climits>

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
	void findOnlyFirst_rec(pair<vector<T>, int> & results, int * currentMin, Vertex<T>* v, vector<T> word, int weight, map<T, Constraint> constraints, map<T, int> letterCount);
public:
	LayeredGraph(Graph<T> g, int n);

	//virtual Vertex<T>* getVertexByID(int id);
	vector<vector<Vertex<T>*>> getLayers();
	vector<Vertex<T>*> getLayer(int index);
	vector<pair<vector<T>, int>> findAll(map<T, Constraint> constraints);
	pair<vector<T>, int> findOnlyFirst(map<T, Constraint> constraints);
	virtual bool accept(vector<T> word, map<T, Constraint> constraints = nullptr);
	virtual int weight(vector<T> word, map<T, Constraint> constraints);
};


template<typename T>
void LayeredGraph<T>::findAll_rec(vector<pair<vector<T>, int>>* results, Vertex<T>* v, vector<T> word, int weight, map<T, Constraint> constraints, map<T, int> letterCount)
{
	//For current vertex, try each transition
	for (int j = 0; j < v->getEdges().size(); j++) {
		T letter = v->getEdge(j)->getLetter();
		letterCount[letter]++;

		//Check if it respects max constraint when reading letter
		if (letterCount[letter] <= constraints[letter].getMax()) {
			//put values in temp variable to prevent other transitions to be modified because of for loop
			vector<T> temp = vector<T>();
			temp.insert(temp.end(), word.begin(), word.end());
			temp.push_back(letter);
			findAll_rec(results, v->getEdge(j)->getDestination(), temp, weight+v->getEdge(j)->getCost(), constraints, letterCount);
			letterCount[letter]--;
		}
		if (v->getEdge(0)->getDestination()->isFinal())
		{
			//Check if it respects min constraint when reaching accept state
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
void LayeredGraph<T>::findOnlyFirst_rec(pair<vector<T>, int>& results, int *currentMinWeight, Vertex<T>* v, vector<T> word, int weight, map<T, Constraint> constraints, map<T, int> letterCount)
{
	//For current vertex, try each transition
	for (int j = 0; j < v->getEdges().size(); j++) {
		T letter = v->getEdge(j)->getLetter();
		letterCount[letter]++;

		//Check if it respects max constraint when reading letter
		if (letterCount[letter] <= constraints[letter].getMax()) {
			//put values in temp variable to prevent other transitions to be modified because of for loop
			vector<T> temp = vector<T>();
			temp.insert(temp.end(), word.begin(), word.end());
			temp.push_back(letter);
			int newWeight = weight + v->getEdge(j)->getCost();
			//Ignore if new Weight higher than found currentMinWeight
			if (newWeight < *currentMinWeight) {
				findOnlyFirst_rec(results, currentMinWeight, v->getEdge(j)->getDestination(), temp, newWeight, constraints, letterCount);
			}
			letterCount[letter]--;
		}
		if (v->getEdge(0)->getDestination()->isFinal())
		{
			//Check if it respects min constraint when reaching accept state
			bool minOk = true;
			for (int i = 0; i < getAlphabet()->getLetters().size(); i++)
			{
				if (letterCount[letter] < constraints[letter].getMin()) {
					minOk = false;
					break;
				}
			}
			if (minOk) {
				//Update currentMinWeight if found word has smaller weight
				if (weight < *currentMinWeight) {
					*currentMinWeight = weight;
					results.first = word;
					results.second = weight;
				}
			}
		}
	}
}


template<typename T>
LayeredGraph<T>::LayeredGraph(Graph<T> g, int n) :
	Graph(new Vertex<T>(-1, false)),
	m_nbLayers(n)
{
	//set alphabet
	for (int i = 0; i < g.getAlphabet()->getLetters().size(); i++)
	{
		getAlphabet()->addLetter(g.getAlphabet()->getLetter(i));
	}

	//create each layer and add it to the graph
	m_layers = vector<vector<Vertex<T>*>>();
	map<int, Vertex<T>*> gVertices = g.getVertices();
	for (int i = 0; i < n + 1; i++) {
		vector<Vertex<T>*> layer = vector<Vertex<T>*>();
		for (map<int, Vertex<T>*>::iterator it = gVertices.begin(); it != gVertices.end(); ++it)
		{
			Vertex<T> *v = new Vertex<T>(i * 1000 + it->first, false);
			layer.push_back(v);
			addVertex(v);
		}
		m_layers.push_back(layer);
	}

	int p = 0;

	//for each vertex
	for (map<int, Vertex<T>*>::iterator it = gVertices.begin(); it != gVertices.end(); ++it){
		//add transition
		for (int k = 0; k < it->second->getEdges().size(); k++) {
			//for each layer
			for (int i = 0; i < n; i++) {
				//create all transitions
				try
				{
					Vertex<T>* initial = getVertexByID(i * 1000 + it->first);
					Vertex<T>* destination = getVertexByID((i + 1) * 1000 + it->second->getEdge(k)->getDestination()->ID());
					addEdge(initial, destination, it->second->getEdge(k)->getCost(), it->second->getEdge(k)->getLetter());
				}
				catch (logic_error e)
				{
					cout << "Error : " << e.what() << endl;
				}
			}
		}
	}

	//link every final vertex of simpleGraph to LayeredGraph final vertex
	Vertex<T>* finalVertex = new Vertex<T>(-2, true); //special ID
	addVertex(finalVertex);
	for (map<int, Vertex<T>*>::iterator it = gVertices.begin(); it != gVertices.end(); ++it) {

		//start transition
		if (g.getInitialVertex()->ID() == it->first) {
			addEdge(getInitialVertex(), getVertexByID(it->first), 0, numeric_limits<T>::min()); //special values
		}
		//end transitions
		if (it->second->isFinal()) {
			Vertex<T>* initial = getVertexByID((n)* 1000 + it->first);
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
pair<vector<T>, int> LayeredGraph<T>::findOnlyFirst(map<T, Constraint> constraints)
{ 
	pair<vector<T>, int> results = pair<vector<T>, int>();
	int minFound = numeric_limits<int>::max();

	findOnlyFirst_rec(results, &minFound, getInitialVertex()->getEdge(0)->getDestination(), vector<T>(), 0, constraints, map<T, int>());

	return results;
}


template<typename T>
bool LayeredGraph<T>::accept(vector<T> word, map<T, Constraint> constraints)
{
	//Without constraint, just try to read the word with transitions
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
		//Else, check constraints at each transition
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
	Vertex<T>* current = getInitialVertex()->getEdge(0)->getDestination();
	int* w = new int(0);
	for (int i = 0; i < word.size(); i++) {
		letterCount[word[i]]++;
		if (letterCount[word[i]] > constraints[word[i]].getMax()) {
			cout << "Can't accept : max constraint letter " << word[i] << " -> " << letterCount[word[i]] << " > " << constraints[word[i]].getMax() << endl;
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

	if (current->getEdges().size() > 0)
		current = current->getEdge(0)->getDestination();

	for (int i = 0; i < getAlphabet()->getLetters().size(); i++) {
		if (letterCount[i] < constraints[i].getMin()) {
			cout << "Can't accept : min constraint letter " << getAlphabet()->getLetter(i) << " -> " << letterCount[i] << " < " << constraints[i].getMin() << endl;
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