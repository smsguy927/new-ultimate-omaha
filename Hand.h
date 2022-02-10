#pragma once
#include <map>
#include <iomanip>
#include <algorithm>
#include <functional>
#include "Card.h"
enum class EHandType
{
	royalFlush,
	straightFlush,
	fourOfaKind,
	fullHouse,
	flush,
	straight,
	threeOfaKind,
	twoPair,
	jacksOrBetter,
	otherPair,
	noPair
};

enum class EGameType
{
	jacksBetter,
	ultHoldEm,
	ultOmaha
};
const int BOARD_SIZE = 5;
const int DRAW_HAND_SIZE = 5;
const int HAND_SIZE = 5;
const int OMAHA_HAND_SIZE = 4;
const int FLOP_SIZE = 3;
const int TURN_INDEX = 3;
const int RIVER_INDEX = 4;
const int STRIKES_OFFSET = 4;
const int FLUSH_COUNT = 5;
const int STRAIGHT_COUNT = 5;
const int FOUR_OF_KIND_COUNT = 4;
const int THREE_OF_KIND_COUNT = 3;
const int PAIR_COUNT = 2;
const int NUM_PAIRS_TP = 2;

const int HAND_TYPES = 11;
const int MIN_HAND_NAME_LENGTH = 8;
const int ULT_PAYTABLE_WIDTH = 3;
const int ULT_BBB_PAYTABLE_WIDTH = 5;
const int ULT_PAYOUT_WIDTH = 10;

const std::array<std::string, HAND_TYPES> HAND_NAMES
{ 
	"Royal Flush", "Straight Flush", "Four Of A Kind", "Full House", "Flush", 
	"Straight", "Three of a Kind", "Two Pair", "Jacks Or Better", "Pair",
	"No Pair"                                /*^Draw Poker Only^*/
};
class Hand
{
public:
	Hand();
	Hand(int);
	~Hand();

	void setCards(std::vector<Card>&);
	void setHandSize(int);
	void setHandType(int);

	std::vector<Card> getCards();
	int getHandType();
	std::string getHandName(int);
	void printHand();
	void printHoleCards(int);
	void printBoard(int);

	virtual void evaluateHand();
	void setRanksAndCounts();
	virtual void setFlushSuit();

	virtual std::vector<Card> findRoyalFlush();
	virtual std::vector<Card> findStraightFlush();
	virtual std::vector<Card> findFourOfaKind();
	virtual std::vector<Card> findFullHouse();
	virtual std::vector<Card> findFlush();
	virtual std::vector<Card> findStraight();
	virtual std::vector<Card> find3OfaKind();
	virtual std::vector<Card> findTwoPair();
	std::vector<Card> findJacksBetter();
	virtual std::vector<Card> findPair();
	virtual std::vector<Card> findNoPair();

	void setBest5Cards(std::vector<Card>);

	void printBest5Cards();
	void printFlop();
	void printTurn();
	void printRiver();

	bool operator<(Hand) const;
	bool operator<=(Hand) const;
	bool operator>(Hand) const;
	bool operator>=(Hand) const;
	bool operator==(Hand) const;

protected:
	std::vector<Card> cards; // visible to the player
	std::vector<Card> sortedHand; // used internally
	std::array<Card, HAND_SIZE> best5Cards; // used in holdem, stud, omaha
	std::vector<int> cardRanks;
	std::vector<int> rankCounts;
	int handType;
	int flushSuit;
};

