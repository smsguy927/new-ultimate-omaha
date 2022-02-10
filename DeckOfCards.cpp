

#include "DeckOfCards.h"



DeckOfCards::DeckOfCards()
{
	for (int i = 0; i < DECK_SIZE; i++)
	{
		Card card(i);
		deck[i] = card;
	}
	this->currentCard = 0;
	 
}


DeckOfCards::~DeckOfCards()
{
}

void DeckOfCards::setCurrentCard(int num)
{
	this->currentCard = num;
}

Card DeckOfCards::getCurrentCard()
{
	return this->currentCard;
}

Card DeckOfCards::getCard(int index)
{
	return deck[index];
}

void DeckOfCards::shuffleCards()
{
	unsigned seed = std::chrono::system_clock::now()
		.time_since_epoch().count();
	shuffle(deck.begin(), deck.end(), std::default_random_engine(seed));
}

void DeckOfCards::cutCards()
{
	unsigned seed = std::chrono::system_clock::now()
		.time_since_epoch().count();
	std::default_random_engine generator(seed);
	std::uniform_int_distribution<int> intDistribution(MIN_CUT, MAX_CUT);
	std::rotate(deck.begin(), deck.begin() + 
		intDistribution(generator), deck.end());
}

Card DeckOfCards::dealCard()
{
	Card topCard = deck[currentCard];
	currentCard++;
	return topCard;
}

bool DeckOfCards::moreCards()
{
	if (currentCard < DECK_SIZE)
	{
		return true;
	}
	else
	{
		return false;
	}
	
}

void DeckOfCards::sortDeck()
{
	sort(deck.begin(), deck.end());
}

void DeckOfCards::reverseSortDeck()
{
	sort(deck.begin(), deck.end(), std::greater<Card>());
}

void DeckOfCards::sortBySuit()
{
	sort(deck.begin(), deck.end(), Card::isLowerSuit);
}

void DeckOfCards::reverseSortBySuit()
{
	sort(deck.begin(), deck.end(), Card::isHigherSuit);
}

void DeckOfCards::reverseDeck()
{
	std::reverse(deck.begin(), deck.end());
}
