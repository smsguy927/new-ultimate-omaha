#pragma once
#include <iostream>
#include <string>
#include <array>
#include <random>
#include <algorithm>
#include <functional>
#include <chrono>
#include "Card.h"
//using namespace std;

const int DECK_SIZE = 52;
const int MIN_CUT = 10;
const int MAX_CUT = 42;
class DeckOfCards
{
public:
	DeckOfCards(); // Makes deck
	~DeckOfCards();
	void setCurrentCard(int);
	Card getCurrentCard();
	Card getCard(int);
	void shuffleCards();
	void cutCards();
	Card dealCard();
	bool moreCards();
	void sortDeck();
	void reverseSortDeck();
	void sortBySuit();
	void reverseSortBySuit();
	void reverseDeck();

private:
	int currentCard;
	std::array<Card, DECK_SIZE> deck;
};

