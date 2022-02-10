#include "FourCards.h"



FourCards::FourCards()
{
	displayHand.resize(FC_SIZE);
	sortedHand.resize(FC_SIZE);
	best4Cards.resize(FC_SIZE);
}


FourCards::~FourCards()
{
}

void FourCards::setCards(std::vector<Card>& hand)
{
	for (int i = 0; i < hand.size(); i++)
	{
		this->displayHand[i] = hand[i];
	}
	
	for (int i = 0; i < hand.size(); i++)
	{
		this->sortedHand[i] = hand[i];
	}
	sort(sortedHand.begin(), sortedHand.end(), std::greater<Card>());
}

void FourCards::setHandType(int num)
{
	this->handType = num;
}

void FourCards::setRanksAndCounts()
{
	int crIndex = -1;
	for (auto it = sortedHand.begin(); it != sortedHand.end(); it++)
	{
		if (std::find(cardRanks.begin(), cardRanks.end(), it->getRankId()) == cardRanks.end())
		{
			cardRanks.push_back(it->getRankId());
			rankCounts.push_back(1);
			crIndex++;
		}
		else
		{
			rankCounts[crIndex]++;
		}
	}
}

void FourCards::setFlushSuit()
{
	std::array<int, NUM_SUITS> suitCounts{ 0,0,0,0 };
	flushSuit = -1;
	// check to see if all the suits are the same
	for (unsigned i = 0; i < sortedHand.size(); i++)
	{
		suitCounts[sortedHand[i].getSuitId()]++;
	}

	for (int i = 0; i < NUM_SUITS; i++)
	{
		if (suitCounts[i] >= FC_FLUSH_COUNT)
		{
			flushSuit = i;
			// break because a flush cannot have more than one suit
			break;
		}
	}
}

int FourCards::getHandType()
{
	return handType;
}

std::string FourCards::getHandName()
{
	switch (handType)
	{
		case int(EFourCardType::royalFlush) :
		case int(EFourCardType::straightFlush) :
		case int(EFourCardType::flush) :
		case int(EFourCardType::straight) :
		case int(EFourCardType::pair) :
			return "a " + FC_HAND_NAMES[handType];

		default:
			return FC_HAND_NAMES[handType];
	}
}

std::vector<Card> FourCards::getCards()
{
	return displayHand;
}

std::vector<Card> FourCards::getBest4Cards()
{
	return best4Cards;
}

void FourCards::printHand()
{
	for (unsigned int i = 0; i < displayHand.size(); i++)
	{
		displayHand[i].printSymbol();
	}
}

void FourCards::printBest4Cards()
{
	for (unsigned int i = 0; i < best4Cards.size(); i++)
	{
		best4Cards[i].printSymbol();
	}
}

void FourCards::evaluateHand()
{
	setFlushSuit(); // flushSuit is set to -1 if a flush is not possible
	setRanksAndCounts();

	std::vector<Card> bestHand;
	bestHand = findRoyalFlush();
	bool bestHandFound = false;
	if (bestHand.size())
	{
		this->handType = int(EFourCardType::royalFlush);
		setBest4Cards(bestHand);
		bestHandFound = true;
	}
	else
	{
		bestHand = findFourOfaKind();
		if (bestHand.size())
		{
			this->handType = int(EFourCardType::fourOfaKind);
			setBest4Cards(bestHand);
			bestHandFound = true;
		}
	}
	if (bestHandFound == false)
	{
		bestHand = findStraightFlush();
		if (bestHand.size())
		{
			this->handType = int(EFourCardType::straightFlush);
			setBest4Cards(bestHand);
			bestHandFound = true;
		}
		
	}
	if (bestHandFound == false)
	{
		bestHand = find3OfaKind();
		if (bestHand.size())
		{
			this->handType = int(EFourCardType::threeOfaKind);
			setBest4Cards(bestHand);
			bestHandFound = true;
		}
		
	}
	if (bestHandFound == false)
	{
		bestHand = findStraight();
		if (bestHand.size())
		{
			this->handType = int(EFourCardType::straight);
			setBest4Cards(bestHand);
			bestHandFound = true;
		}
	}
	if (bestHandFound == false)
	{
		bestHand = findTwoPair();
		if (bestHand.size())
		{
			this->handType = int(EFourCardType::twoPair);
			setBest4Cards(bestHand);
			bestHandFound = true;
		}
	}
	if (bestHandFound == false)
	{
		bestHand = findFlush();
		if (bestHand.size())
		{
			this->handType = int(EFourCardType::flush);
			setBest4Cards(bestHand);
			bestHandFound = true;
		}
	}
	if (bestHandFound == false)
	{
		bestHand = findPair();
		if (bestHand.size())
		{
			this->handType = int(EFourCardType::pair);
			setBest4Cards(bestHand);
			bestHandFound = true;
		}
	}
	if (bestHandFound == false)
	{
		bestHand = findNoPair();
		if (bestHand.size())
		{
			this->handType = int(EFourCardType::noPair);
			setBest4Cards(bestHand);
			bestHandFound = true;
		}
	}

	return;
}

std::vector<Card> FourCards::findRoyalFlush()
{
	std::vector<Card> bestHand;
	if (flushSuit >= 0 && sortedHand[0].getRankId() == int(ERankNames::ace))
	{
		bestHand.resize(FC_SIZE);
		std::copy(sortedHand.begin(), sortedHand.end(), bestHand.begin());
	}
	return bestHand;
}

std::vector<Card> FourCards::findFourOfaKind()
{
	std::vector<Card> bestHand;
	for (auto it = rankCounts.begin(); it != rankCounts.end(); it++)
	{
		if (*it == FC_SIZE)
		{
			bestHand.resize(FC_SIZE);
			std::copy(sortedHand.begin(), sortedHand.end(), bestHand.begin());
		}
	}
	return bestHand;
}

std::vector<Card> FourCards::findStraightFlush()
{
	std::vector<Card> bestHand;
	if (flushSuit >= 0 && sortedHand[0].getRankId() - sortedHand[FC_LAST_INDEX].getRankId() == FC_STRAIGHT_OFFSET)
	{
		bestHand.resize(FC_SIZE);
		std::copy(sortedHand.begin(), sortedHand.end(), bestHand.begin());
	}
	else if (flushSuit >= 0 && sortedHand[0].getRankId() == int(ERankNames::ace)
		&& sortedHand[1].getRankId() == int(ERankNames::four)
		&& sortedHand[FC_3RD_INDEX].getRankId() == int(ERankNames::three)
		&& sortedHand[FC_LAST_INDEX].getRankId() == int(ERankNames::two))
	{
		bestHand.resize(FC_SIZE);
		std::rotate_copy(sortedHand.begin(), sortedHand.begin() + 1, sortedHand.end(),
		bestHand.begin());
	}
	return bestHand;
}

std::vector<Card> FourCards::find3OfaKind()
{
	std::vector<Card> bestHand;
	for (auto it = rankCounts.begin(); it != rankCounts.end(); it++)
	{
		if (*it == FC_3OK_COUNT)
		{
			bestHand.resize(FC_SIZE);
			if (sortedHand[0].getRankId() < sortedHand[1].getRankId())
			{
				std::rotate_copy(sortedHand.begin(), sortedHand.begin() + 1, sortedHand.end(),
				bestHand.begin());
			}
			else
			{
				std::copy(sortedHand.begin(), sortedHand.end(), bestHand.begin());
			}
			
		}
	}
	return bestHand;
}

std::vector<Card> FourCards::findStraight()
{
	bool isStraight = true;
	int currentRank = sortedHand[0].getRankId();
	std::vector<Card> bestHand;
	for (unsigned i = 0; i < sortedHand.size(); i++)
	{
		if (sortedHand[i].getRankId() == currentRank)
		{
			currentRank--;
		}
		else
		{
			isStraight = false;
		}
	}
	if(isStraight)
	{
		bestHand.resize(FC_SIZE);
		std::copy(sortedHand.begin(), sortedHand.end(), bestHand.begin());
	}
	else if (sortedHand[0].getRankId() == int(ERankNames::ace)
		&& sortedHand[1].getRankId() == int(ERankNames::four)
		&& sortedHand[FC_3RD_INDEX].getRankId() == int(ERankNames::three)
		&& sortedHand[FC_LAST_INDEX].getRankId() == int(ERankNames::two))
	{
		bestHand.resize(FC_SIZE);
		std::rotate_copy(sortedHand.begin(), sortedHand.begin() + 1, sortedHand.end(),
			bestHand.begin());
	}
	return bestHand;
}

std::vector<Card> FourCards::findTwoPair()
{
	int pairsFound = 0;
	std::vector<Card> bestHand;
	for (auto it = rankCounts.begin(); it != rankCounts.end(); it++)
	{
		if (*it == FC_PAIR_COUNT)
		{
			pairsFound++;
		}
		
	}
	if (pairsFound == FC_PAIR_COUNT)
	{
		bestHand.resize(FC_SIZE);
		std::copy(sortedHand.begin(), sortedHand.end(), bestHand.begin());
	}
	return bestHand;
}

std::vector<Card> FourCards::findFlush()
{
	std::vector<Card> bestHand;
	if (flushSuit >= 0)
	{
		bestHand.resize(FC_SIZE);
		std::copy(sortedHand.begin(), sortedHand.end(), bestHand.begin());
	}
	return bestHand;
}

std::vector<Card> FourCards::findPair()
{
	std::vector<Card> bestHand;
	int pairIndex = -1;
	for (unsigned i = 0; i < cardRanks.size(); i++)
	{
		if (rankCounts[i] == FC_PAIR_COUNT)
		{
			pairIndex = i;
			break;
		}
	}
	if (pairIndex > 0)
	{
		bestHand.resize(FC_SIZE);
		std::rotate_copy(sortedHand.begin(), sortedHand.begin() + pairIndex, sortedHand.end(),
			bestHand.begin());
	}
	else if (pairIndex == 0)
	{
		bestHand.resize(FC_SIZE);
		std::copy(sortedHand.begin(), sortedHand.end(), bestHand.begin());
	}
	return bestHand;
}

std::vector<Card> FourCards::findNoPair()
{
	std::vector<Card> bestHand;
	bestHand.resize(FC_SIZE);
	std::copy(sortedHand.begin(), sortedHand.end(), bestHand.begin());
	return bestHand;
}

void FourCards::setBest4Cards(std::vector<Card> bestHand)
{
	for (unsigned i = 0; i < bestHand.size(); i++)
	{
		best4Cards[i] = bestHand[i];
	}
}
