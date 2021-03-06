#ifndef EDGE_H
#define EDGE_H

template <typename T>
class Vertex;

template <typename T>
class Edge
{
private:
	Vertex<T>* m_initialVertex;
	Vertex<T>* m_destinationVertex;
	int		m_cost;
	T		m_letter;
public:
	Edge(Vertex<T>* initialVertex, Vertex<T>* destinationVertex, int cost, T letter);

	Vertex<T>* getInitial();
	Vertex<T>* getDestination();
	int		getCost();
	T		getLetter();
};

template <typename T>
Edge<T>::Edge(Vertex<T>* initialVertex, Vertex<T>* destinationVertex, int cost, T letter) :
	m_initialVertex(initialVertex),
	m_destinationVertex(destinationVertex),
	m_cost(cost),
	m_letter(letter)
{
}


template <typename T>
Vertex<T> * Edge<T>::getInitial()
{
	return m_initialVertex;
}

template <typename T>
Vertex<T> * Edge<T>::getDestination()
{
	return m_destinationVertex;
}

template <typename T>
int Edge<T>::getCost()
{
	return m_cost;
}

template<typename T>
T Edge<T>::getLetter()
{
	return m_letter;
}




#endif