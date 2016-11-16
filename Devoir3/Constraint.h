#ifndef CONSTRAINT_H
#define CONSTRAINT_H


using namespace std;

class Constraint
{
	int m_min;
	int m_max;
public:
	Constraint();

	int getMin();
	int getMax();

	void setMin(int min);
	void setMax(int max);
};



#endif