#include <limits>

#include "Constraint.h"

Constraint::Constraint() :
	m_min(0), m_max(numeric_limits<int>::max())
{
}


int Constraint::getMin()
{
	return m_min;
}

int Constraint::getMax()
{
	return m_max;
}

void Constraint::setMin(int min)
{
	m_min = min;
}

void Constraint::setMax(int max)
{
	m_max = max;
}