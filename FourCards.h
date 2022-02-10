#pragma once
#include <map>
#include <iomanip>
#include <algorithm>
#include <functional>
#include "Card.h"

enum class EFourCardType
{
	royalFlush,
	fourOfaKind,
	straightFlush,
	threeOfaKind,
	straight,
	twoPair,
	flush,
	pair,
	noPair
};

const int FC_SIZE = 4;
const int FC_STRAIGHT_OFFSET = 3;
const int FC_LAST_INDEX = 3;
const int FC_3RD_INDEX = 2;
const int FC_3OK_COUNT = 3;
const int FC_PAIR_COUNT = 2;
const int FC_FLUSH_COUNT = 4;
const int FC_HAND_TYPES = 9;
const int FC_MIN_HAND_NAME_LENGTH = 8;

const std::array<std::string, FC_HAND_TYPES> FC_HAND_NAMES
{
	"Royal Flush", "Four Of A Kind", "Straight Flush", 
	"Three of a Kind", "Straight", "Two Pair", "Flush", 
	"Pair", "No Pair"
};

class FourCards
{
	
public:
	FourCards();
	~FourCards();

	void setCards(std::vector<Card>&);
	void setHandType(int);
	void setRanksAndCounts();
	void setFlushSuit();

	
	int getHandType();
	std::string getHandName();
	std::vector<Card> getCards();
	std::vector<Card> getBest4Cards();
	
	void printHand();
	void printBest4Cards();

	void evaluateHand();
	std::vector<Card> findRoyalFlush();
	std::vector<Card> findFourOfaKind();
	std::vector<Card> findStraightFlush();
	std::vector<Card> find3OfaKind();
	std::vector<Card> findStraight();
	std::vector<Card> findTwoPair();
	std::vector<Card> findFlush();
	std::vector<Card> findPair();
	std::vector<Card> findNoPair();
	void setBest4Cards(std::vector<Card>);

private:
	std::vector<Card> displayHand;
	std::vector<Card> sortedHand;
	std::vector<Card> best4Cards;
	std::vector<int> cardRanks;
	std::vector<int> rankCounts;
	int handType;
	int flushSuit;
};

