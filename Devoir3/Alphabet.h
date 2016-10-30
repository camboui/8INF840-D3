#ifndef ALPHABET_H
#define ALPHABET_H

#include <vector>

using namespace std;

template <typename T>
class Alphabet
{
private:
	vector<T> m_letters;
public:
	Alphabet();

	void addLetter(T letter);
	vector<T> getLetters();
	bool wordIsValide(vector<T> word);
};



template<typename T>
Alphabet<T>::Alphabet()
{
	m_letters = vector<T>();
}


template<typename T>
void Alphabet<T>::addLetter(T letter)
{
	if (find(m_letters.begin(), m_letters.end(), letter) != m_letters.end()) throw logic_error("this letter already exists");
	m_letters.push_back(letter);
}

template<typename T>
vector<T> Alphabet<T>::getLetters()
{
	return m_letters;
}

template<typename T>
bool Alphabet<T>::wordIsValide(vector<T> word)
{
	for (int i = 0; i < word.size(); i++) {
		if (find(m_letters.begin(), m_letters.end(), word[i]) == m_letters.end()) return false;
	}
	return true;
}

#endif