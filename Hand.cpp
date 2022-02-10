#include "Hand.h"



//using namespace std;
Hand::Hand()
{
	cards.resize(HAND_SIZE);
	sortedHand.resize(HAND_SIZE);
}

Hand::Hand(int handSize)
{
	cards.resize(handSize);
	sortedHand.resize(handSize);
}


Hand::~Hand()
{
}

void Hand::setCards(std::vector<Card>& myHand)
{
	for (int i = 0; i < myHand.size(); i++)
	{
		this->cards[i] = myHand[i];
	}
	
	for (int i = 0; i < myHand.size(); i++)
	{
		this->sortedHand[i] = myHand[i];
	}
	sort(sortedHand.begin(), sortedHand.end(), std::greater<Card>());
}

void Hand::setHandSize(int num)
{
	cards.resize(num);
	sortedHand.resize(num);
}

void Hand::setHandType(int num)
{
	this->handType = num;
}

std::vector<Card> Hand::getCards()
{
	return cards;
}

int Hand::getHandType()
{

	return handType;
}

std::string Hand::getHandName(int gameType)
{
	if (gameType != int(EGameType::jacksBetter) && handType ==
		int(EHandType::jacksOrBetter))
	{
		return "a " + HAND_NAMES[int(EHandType::otherPair)];
	}
	else
	{
		switch (handType)
		{
		case int(EHandType::royalFlush) :
		case int(EHandType::straightFlush) :
		case int(EHandType::fullHouse) :
		case int(EHandType::flush) :
		case int(EHandType::straight) :
		case int(EHandType::otherPair) :
			return "a " + HAND_NAMES[handType];

		default:
			return HAND_NAMES[handType];

		}
	}
}


void Hand::printHand()
{
	for (unsigned int i = 0; i < cards.size(); i++)
	{
		cards[i].printSymbol();
	}
}

void Hand::printHoleCards(int numCards)
{
	for (unsigned i = 0; i < numCards; i++)
	{
		cards[i].printSymbol();
	}
}

void Hand::printBoard(int boardIndex)
{
	for (unsigned i = boardIndex; i < cards.size(); i++)
	{
		cards[i].printSymbol();
	}
}


// Use Hand::getHandName to return the hand name
void Hand::evaluateHand()
{
	setRanksAndCounts();
	setFlushSuit(); // flushSuit is set to -1 if a flush is not possible
	std::vector<Card>bestHand;
	bestHand = findRoyalFlush();
	bool bestHandFound = false;
	if (bestHand.size())
	{
		this->handType = int(EHandType::royalFlush);
		setBest5Cards(bestHand);
		bestHandFound = true;
	}
	else
	{
		bestHand = findStraightFlush();
		if (bestHand.size())
		{
			this->handType = int(EHandType::straightFlush);
			setBest5Cards(bestHand);
			bestHandFound = true;
		}
	}
	if (bestHandFound == false)
	{
		bestHand = findFourOfaKind();
		if (bestHand.size())
		{
			this->handType = int(EHandType::fourOfaKind);
			setBest5Cards(bestHand);
			bestHandFound = true;
		}
	}
	if (bestHandFound == false)
	{
		bestHand = findFullHouse();
		if (bestHand.size())
		{
			this->handType = int(EHandType::fullHouse);
			setBest5Cards(bestHand);
			bestHandFound = true;
		}
	}
	if (bestHandFound == false)
	{
		bestHand = findFlush();
		if (bestHand.size())
		{
			this->handType = int(EHandType::flush);
			setBest5Cards(bestHand);
			bestHandFound = true;
		}
	}
	if (bestHandFound == false)
	{
		bestHand = findStraight();
		if (bestHand.size())
		{
			this->handType = int(EHandType::straight);
			setBest5Cards(bestHand);
			bestHandFound = true;
		}
	}
	if (bestHandFound == false)
	{
		bestHand = find3OfaKind();
		if (bestHand.size())
		{
			this->handType = int(EHandType::threeOfaKind);
			setBest5Cards(bestHand);
			bestHandFound = true;
		}
	}
	if (bestHandFound == false)
	{
		bestHand = findTwoPair();
		if (bestHand.size())
		{
			this->handType = int(EHandType::twoPair);
			setBest5Cards(bestHand);
			bestHandFound = true;
		}
	}
	if (bestHandFound == false && cards.size() == DRAW_HAND_SIZE)
	{
		bestHand = findJacksBetter();
		if (bestHand.size())
		{
			this->handType = int(EHandType::jacksOrBetter);
			setBest5Cards(bestHand);
			bestHandFound = true;
		}
	}
	if (bestHandFound == false)
	{
		bestHand = findPair();
		if (bestHand.size())
		{
			this->handType = int(EHandType::otherPair);
			setBest5Cards(bestHand);
			bestHandFound = true;
		}
	}
	if (bestHandFound == false)
	{
		bestHand = findNoPair();
		if (bestHand.size())
		{
			this->handType = int(EHandType::noPair);
			setBest5Cards(bestHand);
			bestHandFound = true;
		}
	}

	return;
}
void Hand::setRanksAndCounts()
{
	int rankIndex = 0;
	bool result = false;
	for (unsigned i = 0; i < sortedHand.size(); i++)
	{
		if (i == 0)
		{
			cardRanks.push_back(sortedHand[i].getRankId());
			rankCounts.push_back(1);
		}
		else if (cardRanks[rankIndex] == sortedHand[i].getRankId())
		{
			rankCounts[rankIndex]++;
		}
		else
		{
			cardRanks.push_back(sortedHand[i].getRankId());
			rankCounts.push_back(1);
			rankIndex++;
		}
	}
}
void Hand::setFlushSuit()
{
	std::array<int, NUM_SUITS> suitCounts{ 0,0,0,0};
	flushSuit = -1;
	// check to see if all the suits are the same
	for (unsigned i = 0; i < sortedHand.size(); i++)
	{
		suitCounts[sortedHand[i].getSuitId()]++;
	}

	for (int i = 0; i < NUM_SUITS; i++)
	{
		if (suitCounts[i] >= FLUSH_COUNT)
		{
			flushSuit = i;
			// break because a flush cannot have more than one suit
			break;
		}
	}
}
std::vector<Card> Hand::findRoyalFlush()
{
	bool result = false;
	std::vector<Card> bestRfCards;
	if (sortedHand[0].getRankId() == int(ERankNames::ace) && flushSuit >= 0)
	{

		// this assumes the hand is sorted
		int maxStrikes = sortedHand.size() - STRIKES_OFFSET;
		int strikes = 0;
		int rfCards = 0;
		int rank = int(ERankNames::ace);
		int suit = flushSuit;
		for (int i = 0; i < sortedHand.size(); i++)
		{
			int nextSuit = sortedHand[i].getSuitId();
			int nextRank = sortedHand[i].getRankId();
			if (nextRank == rank && nextSuit == suit && rfCards == FLUSH_COUNT - 1)
			{
				bestRfCards.push_back(sortedHand[i]);
				result = true;
				return bestRfCards;
			}
			else if (nextRank == rank && nextSuit == suit)
			{
				bestRfCards.push_back(sortedHand[i]);
				rank--;
				rfCards++;
			}
			else
			{
				strikes++;
			}
		}
	}
	else
	{
		result = false;
	}
	bestRfCards.clear();
	return bestRfCards;
}

std::vector<Card> Hand::findStraightFlush()
{
	
	std::vector<Card> bestSfCards;
	if (flushSuit >= 0)
	{
		int numLoops = 2;
		// loop twice if the first card is an ace to check for ace low
		for (int loops = 0; loops < numLoops; loops++)
		{
			bestSfCards.clear();
			int rank = sortedHand[0].getRankId();
			int straightFlushCards = 0;

			// check to see if the ranks are consecutive
			for (unsigned int i = 0; i < sortedHand.size(); i++)
			{

				if (straightFlushCards == STRAIGHT_COUNT - 1)
				{
					int nextCard = sortedHand[i].getRankId();
					int nextSuit = sortedHand[i].getSuitId();
					if (nextSuit == flushSuit && nextCard == rank && loops == 1)
					{
						bestSfCards.push_back(sortedHand[i]);
						std::rotate(bestSfCards.begin(), bestSfCards.begin() + 1, bestSfCards.end());
						return bestSfCards;
					}
					else if (nextSuit == flushSuit && nextCard == rank)
					{
						bestSfCards.push_back(sortedHand[i]);
						return bestSfCards;
					}
					
				}
				else
				{
					int nextCard = sortedHand[i].getRankId();
					int nextSuit = sortedHand[i].getSuitId();
					if (nextCard == rank && nextSuit == flushSuit)
					{
						bestSfCards.push_back(sortedHand[i]);
						straightFlushCards++;
						if (straightFlushCards == 1)
						{
							if (rank == int(ERankNames::ace) && loops == 1)
							{
								rank = int(ERankNames::five);
							}
							else if (rank == int(ERankNames::ace))
							{
								rank--;
							}
							else
							{
								numLoops--;
								rank--;
							}
						}
						else
						{
							rank--;
						}
					}
					else if (nextCard < rank && nextSuit == flushSuit)
					{
						bestSfCards.clear();
						straightFlushCards = 1;
						bestSfCards.push_back(sortedHand[i]);
						rank = nextCard - 1;
					}
					
				}
			}

		}
	}
	else
	{
		// flushSuit is < 0 and a flush is impossible
		bestSfCards.clear();
		return bestSfCards;
	}

	if (bestSfCards.size() < HAND_SIZE)
	{
		bestSfCards.clear();
	}
	
	return bestSfCards;
}

std::vector<Card> Hand::findFourOfaKind()
{
	std::vector<Card> bestCards;
	int fourOfKindRank = -1;
	int kickerRank = -1;

	for (unsigned i = 0; i < rankCounts.size(); i++)
	{
		if (rankCounts[i] >= FOUR_OF_KIND_COUNT)
		{
			fourOfKindRank = cardRanks[i];
			break;
		}
	}

	if (fourOfKindRank < 0)
	{
		bestCards.clear();
		return bestCards;
	}

	for (unsigned i = 0; i < rankCounts.size(); i++)
	{
		if (rankCounts[i] != fourOfKindRank)
		{
			kickerRank = cardRanks[i];
			break;
		}
	}

	int fourOfKindCards = 0;
	for (auto it = sortedHand.begin(); it != sortedHand.end(); it++)
	{
		if (it->getRankId() == fourOfKindRank)
		{
			bestCards.push_back(*it);
			fourOfKindCards++;
			if (fourOfKindCards == FOUR_OF_KIND_COUNT)
			{
				break;
			}
		}
	}
	for (auto it = sortedHand.begin(); it != sortedHand.end(); it++)
	{
		if (it->getRankId() != fourOfKindRank)
		{
			bestCards.push_back(*it);
			break;
		}
	}

	return bestCards;
}

std::vector<Card> Hand::findFullHouse()
{
	std::vector<Card> bestCards;
	int rankIndex = 0;
	int threeOfKindRank = -1;
	int pairRank = -1;

	for (unsigned i = 0; i < rankCounts.size(); i++)
	{
		if (rankCounts[i] >= THREE_OF_KIND_COUNT)
		{
			threeOfKindRank = cardRanks[i];
			break;
		}
	}
	for (int i = 0; i < rankCounts.size(); i++)
	{
		if (rankCounts[i] >= PAIR_COUNT && cardRanks[i] != threeOfKindRank)
		{
			pairRank = cardRanks[i];
			break;
		}
	}

	if (threeOfKindRank < 0 || pairRank < 0)
	{
		bestCards.clear();
		return bestCards;
	}

	int threeOfKindCards = 0;
	int pairCards = 0;
	for (unsigned i = 0; i < sortedHand.size(); i++)
	{
		if (sortedHand[i].getRankId() == threeOfKindRank)
		{
			bestCards.push_back(sortedHand[i]);
			threeOfKindCards++;
			if (threeOfKindCards == THREE_OF_KIND_COUNT)
			{
				break;
			}
		}
	}
	for (int i = 0; i < sortedHand.size(); i++)
	{
		if (sortedHand[i].getRankId() == pairRank)
		{
			bestCards.push_back(sortedHand[i]);
			pairCards++;
			if (pairCards == PAIR_COUNT)
			{
				break;
			}
		}
	}
	return bestCards;
}

std::vector<Card> Hand::findFlush()
{
	std::vector<Card> bestCards;
	int flushCards = 0;
	if (flushSuit < 0)
	{
		bestCards.clear();
		return bestCards;
	}
	for (int i = 0; i < sortedHand.size(); i++)
	{
		if (sortedHand[i].getSuitId() == flushSuit)
		{
			bestCards.push_back(sortedHand[i]);
			flushCards++;
			if (flushCards == FLUSH_COUNT)
			{
				break;
			}
		}
	}
	return bestCards;
}

std::vector<Card> Hand::findStraight()
{
	
	int straightCards = 0;
	int numLoops = 2;
	std::vector<Card>bestStraightCards;
	// loop twice if the first card is an ace to check for ace low
	for (int loops = 0; loops < numLoops; loops++)
	{
		int straightCards = 0;
		bestStraightCards.clear();
		int rank = sortedHand[0].getRankId();
		

		// check to see if the ranks are consecutive
		for (unsigned int i = 0; i < sortedHand.size(); i++)
		{
			if (straightCards == STRAIGHT_COUNT - 1)
			{
				int nextCard = sortedHand[i].getRankId();
				if (nextCard == rank && loops == 1)
				{
					bestStraightCards.push_back(sortedHand[i]);
					std::rotate(bestStraightCards.begin(), bestStraightCards.begin() + 1, bestStraightCards.end());
					return bestStraightCards;
				}
				else if (nextCard == rank)
				{
					bestStraightCards.push_back(sortedHand[i]);
					return bestStraightCards;
				}
				
			}
			else
			{
				int nextCard = sortedHand[i].getRankId();
				if (nextCard == rank)
				{
					bestStraightCards.push_back(sortedHand[i]);
					straightCards++;
					if (straightCards == 1)
					{
						if (rank == int(ERankNames::ace) && loops == 1)
						{
							rank = int(ERankNames::five);
						}
						else if (rank == int(ERankNames::ace))
						{
							rank--;
						}
						else
						{
							numLoops--;
							rank--;
						}
					}
					else
					{
						rank--;
					}
				}
				else if (nextCard < rank)
				{
					bestStraightCards.clear();
					straightCards = 1;
					bestStraightCards.push_back(sortedHand[i]);
					rank = nextCard - 1;
				}
				

				
			}
		}
		
	}
	if (straightCards < STRAIGHT_COUNT)
	{
		bestStraightCards.clear();
	}
	return bestStraightCards;
}

std::vector<Card> Hand::find3OfaKind()
{
	std::vector<Card> bestCards;
	int rankIndex = 0;
	int threeOfKindRank = -1;
	
	for (int i = 0; i < rankCounts.size(); i++)
	{
		if (rankCounts[i] >= THREE_OF_KIND_COUNT)
		{
			threeOfKindRank = cardRanks[i];
			break;
		}
	}

	if (threeOfKindRank < 0)
	{
		bestCards.clear();
		return bestCards;
	}

	int threeOfKindCards = 0;
	int kickers = 0;
	for (int i = 0; i < sortedHand.size(); i++)
	{
		if (sortedHand[i].getRankId() == threeOfKindRank)
		{
			bestCards.push_back(sortedHand[i]);
			threeOfKindCards++;
			if (threeOfKindCards == THREE_OF_KIND_COUNT)
			{
				break;
			}
		}
	}
	for (int i = 0; i < sortedHand.size(); i++)
	{
		if (sortedHand[i].getRankId() != threeOfKindRank)
		{
			bestCards.push_back(sortedHand[i]);
			kickers++;
			if (kickers == PAIR_COUNT)
			{
				break;
			}
		}
	}
	
	return bestCards;
}

std::vector<Card> Hand::findTwoPair()
{
	std::vector<Card> bestCards;
	
	int rankIndex = 0;
	std::vector<int> pairRanks;
	
	for (int i = 0; i < NUM_PAIRS_TP; i++)
	{
		for (unsigned j = 0; j < rankCounts.size(); j++)
		{
			auto findIt = std::find(pairRanks.begin(), pairRanks.end(), cardRanks[j]);
			if (rankCounts[j] >= PAIR_COUNT && findIt == pairRanks.end())
			{
				pairRanks.push_back(cardRanks[j]);
				break;
			}
		}
	}
	
	if (pairRanks.size() < NUM_PAIRS_TP)
	{
		bestCards.clear();
		return bestCards;
	}

	std::array<int, NUM_PAIRS_TP> pairCards{ 0, 0 };
	int pairsIndex = 0;

	for (int i = 0; i < NUM_PAIRS_TP; i++)
	{
		for (int j = 0; j < sortedHand.size(); j++)
		{
			if (sortedHand[j].getRankId() == pairRanks[i])
			{
				bestCards.push_back(sortedHand[j]);
				pairCards[i]++;
				if (pairCards[i] == PAIR_COUNT)
				{
					break;
				}
			}
		}
	}
	
	
	for (int i = 0; i < sortedHand.size(); i++)
	{
		auto findIt = std::find(pairRanks.begin(), pairRanks.end(), sortedHand[i].getRankId());
		if (findIt == pairRanks.end())
		{
			bestCards.push_back(sortedHand[i]);
			break;
		}
	}

	return bestCards;
	
}

std::vector<Card> Hand::findJacksBetter()
{
	std::vector<Card> bestCards;
	
	int jBPairRank = -1;
	for (int i = 0; i < rankCounts.size(); i++)
	{
		if (rankCounts[i] >= PAIR_COUNT && cardRanks[i] >= int(ERankNames::jack))
		{
			jBPairRank = cardRanks[i];
			break;
		}
	}

	if (jBPairRank < 0)
	{
		bestCards.clear();
		return bestCards;
	}

	int pairCards = 0;
	int kickers = 0;
	for (int i = 0; i < sortedHand.size(); i++)
	{
		if (sortedHand[i].getRankId() == jBPairRank)
		{
			bestCards.push_back(sortedHand[i]);
			pairCards++;
			if (pairCards == PAIR_COUNT)
			{
				break;
			}
		}
	}
	for (int i = 0; i < sortedHand.size(); i++)
	{
		if (sortedHand[i].getRankId() != jBPairRank)
		{
			bestCards.push_back(sortedHand[i]);
			kickers++;
			if (kickers == THREE_OF_KIND_COUNT)
			{
				break;
			}
		}
	}

	return bestCards;
}

std::vector<Card> Hand::findPair()
{
	std::vector<Card> bestCards;

	int pairRank = -1;
	for (int i = 0; i < rankCounts.size(); i++)
	{
		if (rankCounts[i] >= PAIR_COUNT)
		{
			pairRank = cardRanks[i];
			break;
		}
	}

	if (pairRank < 0)
	{
		bestCards.clear();
		return bestCards;
	}

	int pairCards = 0;
	int kickers = 0;
	for (int i = 0; i < sortedHand.size(); i++)
	{
		if (sortedHand[i].getRankId() == pairRank)
		{
			bestCards.push_back(sortedHand[i]);
			pairCards++;
			if (pairCards == PAIR_COUNT)
			{
				break;
			}
		}
	}
	for (int i = 0; i < sortedHand.size(); i++)
	{
		if (sortedHand[i].getRankId() != pairRank)
		{
			bestCards.push_back(sortedHand[i]);
			kickers++;
			if (kickers == THREE_OF_KIND_COUNT)
			{
				break;
			}
		}
	}

	return bestCards;
}

std::vector<Card> Hand::findNoPair()
{
	std::vector<Card> bestCards;
	for (unsigned i = 0; i < sortedHand.size(); i++)
	{
		bestCards.push_back(sortedHand[i]);
		{
			if (bestCards.size() == HAND_SIZE)
			{
				break;
			}
		}
	}
	return bestCards;
}

void Hand::setBest5Cards(std::vector<Card>  bestCards)
{
	for (unsigned i = 0; i < bestCards.size(); i++)
	{
		best5Cards[i] = bestCards[i];
	}
}

void Hand::printBest5Cards()
{
	for (unsigned int i = 0; i < best5Cards.size(); i++)
	{
		best5Cards[i].printSymbol();
	}
}

void Hand::printFlop()
{
	for (unsigned int i = 0; i < FLOP_SIZE; i++)
	{
		cards[i].printSymbol();
	}
}

void Hand::printTurn()
{
	cards[TURN_INDEX].printSymbol();
}

void Hand::printRiver()
{
	cards[RIVER_INDEX].printSymbol();
}

// Begin Hand Comparison Operator Overloads
// LOWER handType > HIGHER handType

bool Hand::operator<(Hand hand2) const
{

	if (handType > hand2.handType)
	{
		return true;
	}
	else if (handType < hand2.handType)
	{
		return false;
	}
	else
	{
		for (int i = 0; i < best5Cards.size(); i++)
		{
			if (best5Cards[i].getRankId() < hand2.best5Cards[i].getRankId())
			{
				return true;
			}
			else if (best5Cards[i].getRankId() > hand2.best5Cards[i].getRankId())
			{
				return false;
			}
		}
	}
	return false;
}

bool Hand::operator<=(Hand hand2) const
{
	if (handType >= hand2.handType)
	{
		return true;
	}
	else
	{
		for (int i = 0; i < best5Cards.size(); i++)
		{
			if (best5Cards[i].getRankId() <= hand2.best5Cards[i].getRankId())
			{
				return true;
			}
		}
	}
	return false;
}

bool Hand::operator>(Hand hand2) const
{
	// LOWER handType > HIGHER handType
	if (handType < hand2.handType)
	{
		return true;
	}
	else if (handType > hand2.handType)
	{
		return false;
	}
	else
	{
		for (int i = 0; i < best5Cards.size(); i++)
		{
			if (best5Cards[i].getRankId() > hand2.best5Cards[i].getRankId())
			{
				return true;
			}
			else if (best5Cards[i].getRankId() < hand2.best5Cards[i].getRankId())
			{
				return false;
			}
		}
	}
	return false;
}

bool Hand::operator>=(Hand hand2) const
{
	if (handType <= hand2.handType)
	{
		return true;
	}
	else
	{
		for (int i = 0; i < best5Cards.size(); i++)
		{
			if (best5Cards[i].getRankId() >= hand2.best5Cards[i].getRankId())
			{
				return true;
			}
		}
	}
	return false;
}

bool Hand::operator==(Hand hand2) const
{
	if (handType != hand2.handType)
	{
		return false;
	}
	else
	{
		for (int i = 0; i < best5Cards.size(); i++)
		{
			if (best5Cards[i].getRankId() != hand2.best5Cards[i].getRankId())
			{
				return false;
			}
		}
	}
	return true;
}
