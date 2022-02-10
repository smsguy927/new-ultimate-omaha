#pragma once
#include "Hand.h"

enum class ERankLocations
{
	hand,
	board,
	both
};
const int MAX_HOLE_CARDS = 2;
const int NUM_LOCATIONS = 2;
const int HOLE_CARDS_HAND = 2;
const int MAX_BOARD_CARDS = 3;
const int BOARD_CARDS_HAND = 3;
const int STRAIGHT_LAST_INDEX = 4;
class OmahaHand :
	public Hand
{
public:
	OmahaHand();
	~OmahaHand();

	void setHoleCards(std::vector<Card>);
	void setBoardCards(std::vector<Card> &);
	void setIsBoardCard();
	void setRanks();
	void setRankLocations();
	void setFlushSuit() override;

	//bool isValidOmahaHand(std::vector<Card>);

	void evaluateHand() override;
	

	void setStraightStartRanks();

	std::vector<Card> findRoyalFlush();
	std::vector<Card> findStraightFlush();
	std::vector<Card> findFourOfaKind() override;
	std::vector<Card> findFullHouse() override;
	std::vector<Card> findFlush() override;
	std::vector<Card> findStraight() override;
	std::vector<Card> find3OfaKind() override;
	std::vector<Card> findTwoPair() override;
	std::vector<Card> findPair() override;
	std::vector<Card> findNoPair() override;

	void assignKicker(std::vector<int> &, std::array<int, NUM_LOCATIONS> &, std::vector<int> &);

	void setBest5Cards(std::vector<Card>);

	std::vector<Card> getHoleCards();
	std::vector<Card> getBoardCards();

private:
	std::vector<Card> holeCards;
	std::vector<Card> boardCards;
	// These are used to find Four of a Kind, Full House, 
	// Three of a Kind, and Two/One/No Pair
	std::vector<int> holeCardRanks;
	std::vector<int> holeCardRankCounts;
	std::vector<int> boardCardRanks;
	std::vector<int> boardCardRankCounts;
	std::map<Card, bool, std::greater<Card>> cardLocations;
	// These are used to find Straights
	std::vector<int> ranks;
	std::vector<int> rankLocations;
	std::vector<int> straightStartRanks;
};

