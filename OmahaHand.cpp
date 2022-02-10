#include "OmahaHand.h"


OmahaHand::OmahaHand():Hand(OMAHA_HAND_SIZE)
{
	holeCards.resize(OMAHA_HAND_SIZE);
	boardCards.resize(BOARD_SIZE);
}


OmahaHand::~OmahaHand()
{
}

void OmahaHand::setHoleCards(std::vector<Card> cards)
{
	for (unsigned i = 0; i < cards.size(); i++)
	{
		this->holeCards[i] = cards[i];
	}
}

void OmahaHand::setBoardCards(std::vector<Card>& board)
{
	for (unsigned i = 0; i < board.size(); i++)
	{
		this->boardCards[i] = board[i];
	}
}

void OmahaHand::setIsBoardCard()
{
	for (unsigned i = 0; i < holeCards.size(); i++)
	{
		cardLocations.insert(std::make_pair(holeCards[i], false));
	}
	for (unsigned i = 0; i < boardCards.size(); i++)
	{
		cardLocations.insert(std::make_pair(boardCards[i], true));
	}
}


void OmahaHand::setRanks()
{
	for (int i = 0; i < NUM_RANKS; i++)
	{
		holeCardRankCounts.push_back(0);
		boardCardRankCounts.push_back(0);
	}
	auto it = cardLocations.begin();
	int currentRank = int(ERankNames::ace);
	for (unsigned i = 0; i < NUM_RANKS; i++)
	{
		bool currentRankFound;
		do
		{
			if (it != cardLocations.end() && it->first.getRankId() == currentRank)
			{
				currentRankFound = true;
				if (it->second && boardCardRankCounts[i] < MAX_BOARD_CARDS)
				{
					boardCardRankCounts[i]++;
				}
				else if(!it->second && holeCardRankCounts[i] < MAX_HOLE_CARDS)
				{
					holeCardRankCounts[i]++;
				}
				it++;
			}
			else
			{
				currentRankFound = false;
			}
		} 
		while (currentRankFound);
		currentRank--;
	}
	int lastRank = -1;
	for (auto it = holeCards.begin(); it != holeCards.end(); it++)
	{
		int currentRank = it->getRankId();
		if (lastRank != currentRank)
		{
			holeCardRanks.push_back(currentRank);
			lastRank = currentRank;
		}
	}

	lastRank = -1;
	for (auto it = boardCards.begin(); it != boardCards.end(); it++)
	{
		int currentRank = it->getRankId();
		if (lastRank != currentRank)
		{
			boardCardRanks.push_back(currentRank);
			lastRank = currentRank;
		}
	}

	setRankLocations();

}

void OmahaHand::setRankLocations()
{
	for (int i = int(ERankNames::ace); i > -1; i--)
	{
		if (find(holeCardRanks.begin(), holeCardRanks.end(), i) != holeCardRanks.end() &&
			find(boardCardRanks.begin(), boardCardRanks.end(), i) != boardCardRanks.end())
		{
			ranks.push_back(i);
			rankLocations.push_back(int(ERankLocations::both));
		}
		else if (find(holeCardRanks.begin(), holeCardRanks.end(), i) != holeCardRanks.end())
		{
			ranks.push_back(i);
			rankLocations.push_back(int(ERankLocations::hand));
		}
		else if (find(boardCardRanks.begin(), boardCardRanks.end(), i) != boardCardRanks.end())
		{
			ranks.push_back(i);
			rankLocations.push_back(int(ERankLocations::board));
		}
	}
}

void OmahaHand::setFlushSuit()
{
	std::array<int, NUM_SUITS> suitCounts{ 0,0,0,0 };
	flushSuit = -1;
	for (auto it = boardCards.begin(); it != boardCards.end(); it++)
	{
		suitCounts[it->getSuitId()]++;
	}
	for (unsigned i = 0; i < suitCounts.size(); i++)
	{
		if (suitCounts[i] >= BOARD_CARDS_HAND)
		{
			flushSuit = i;
			break; // there can be only one flushSuit
		}
	}

	if (flushSuit < 0)
	{
		return;
	}
	else
	{
		int flushSuitCount = 0;
		for (auto it = holeCards.begin(); it != holeCards.end(); it++)
		{
			if (it->getSuitId() == flushSuit)
			{
				flushSuitCount++;
			}
		}
		
		if (flushSuitCount < HOLE_CARDS_HAND)
		{
			flushSuit = -1;
		}
	}

	return;
}

void OmahaHand::evaluateHand()
{
	setFlushSuit(); // flushSuit is set to -1 if a flush is not possible
	setRanks();
	setStraightStartRanks();

	std::vector<Card> bestHand;
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



void OmahaHand::setStraightStartRanks()
{
	for (unsigned i = 0; i < ranks.size() - STRAIGHT_LAST_INDEX; i++)
	{
		if (ranks[i] - STRAIGHT_LAST_INDEX == ranks[i + STRAIGHT_LAST_INDEX])
		{
			straightStartRanks.push_back(ranks[i]);
		}
		
	}
	if (ranks[0] == int(ERankNames::ace))
	{
		for (unsigned i = 0; i < ranks.size(); i++)
		{
			if (ranks[i] == int(ERankNames::five))
			{
				if (ranks[i + MAX_BOARD_CARDS] == int(ERankNames::two))
				{
					straightStartRanks.push_back(int(ERankNames::five));
				}
			}
			break;	
		}
	}
}

std::vector<Card> OmahaHand::findRoyalFlush()
{
	
	std::vector<Card>bestRfCards;
	if (!straightStartRanks.size()|| straightStartRanks[0] != int(ERankNames::ace) 
		|| flushSuit < 0)
	{
		bestRfCards.clear();
		return bestRfCards;
	}

	for (auto it = boardCards.begin(); it != boardCards.end(); it++)
	{
		if (it->getRankId() <= int(ERankNames::ace) && it->getRankId() >= int(ERankNames::ten)
			&& it->getSuitId() == flushSuit)
		{
			bestRfCards.push_back(*it);
		}
	}

	if (bestRfCards.size() != MAX_BOARD_CARDS)
	{
		bestRfCards.clear();
		return bestRfCards;
	}
	else
	{
		for (auto it = holeCards.begin(); it != holeCards.end(); it++)
		{
			if (it->getRankId() <= int(ERankNames::ace) && it->getRankId() >= int(ERankNames::ten)
				&& it->getSuitId() == flushSuit)
			{
				bestRfCards.push_back(*it);
			}
		}
	}

	if (bestRfCards.size() != HAND_SIZE)
	{
		bestRfCards.clear();
		return bestRfCards;
	}
	else
	{
		sort(bestRfCards.begin(), bestRfCards.end(), std::greater<Card>());
	}

	return bestRfCards;

	
}

std::vector<Card> OmahaHand::findStraightFlush()
{
	bool aceLow = false;
	std::vector<Card>bestSfCards;
	if (!straightStartRanks.size() || straightStartRanks[0] != int(ERankNames::ace)
		|| flushSuit < 0)
	{
		bestSfCards.clear();
		return bestSfCards;
	}

	int sfStartRank = -1;
	int sfEndRank = -1;
	for (unsigned i = 0; i < straightStartRanks.size(); i++)
	{
		aceLow = false;
		sfStartRank = straightStartRanks[i];
		sfEndRank = sfStartRank - HAND_SIZE - 1;

		if (sfStartRank == int(ERankNames::five))
		{
			sfEndRank = int(ERankNames::two);
			aceLow = true;
		}
		for (auto it = boardCards.begin(); it != boardCards.end(); it++)
		{
			if (it->getRankId() <= sfStartRank && it->getRankId() >= sfEndRank && it->getSuitId() == flushSuit)
			{
				bestSfCards.push_back(*it);
			}
			else if (aceLow && it->getRankId() == int(ERankNames::ace) && it->getSuitId() == flushSuit)
			{
				bestSfCards.push_back(*it);
			}
		}

		if (bestSfCards.size() != MAX_BOARD_CARDS)
		{
			bestSfCards.clear();
		}
		else
		{
			for (auto it = holeCards.begin(); it != holeCards.end(); it++)
			{
				if (it->getRankId() <= sfStartRank && it->getRankId() >= sfEndRank && it->getSuitId() == flushSuit)
				{
					bestSfCards.push_back(*it);
				}
				else if (aceLow && it->getRankId() == int(ERankNames::ace) && it->getSuitId() == flushSuit)
				{
					bestSfCards.push_back(*it);
				}
			}
		}

		if (bestSfCards.size() != HAND_SIZE)
		{
			bestSfCards.clear();
			
		}
		else
		{
			sort(bestSfCards.begin(), bestSfCards.end(), std::greater<Card>());
			if (aceLow)
			{
				std::rotate(bestSfCards.begin(), bestSfCards.begin() + 1, bestSfCards.end());
			}
			return bestSfCards;
		}

	}
	bestSfCards.clear();
	return bestSfCards;
}


std::vector<Card> OmahaHand::findFourOfaKind()
{
	std::vector<Card> fourOfKindCards;
	std::vector<int> ranksUsed;
	std::vector<int> rankLocations;
	std::array<int, NUM_LOCATIONS> numCardsUsed{0,0};
	

	
	// Determine whether a Four of a Kind is possible
	for (unsigned i = 0; i < boardCardRankCounts.size(); i++)
	{
		if (holeCardRankCounts[i] + boardCardRankCounts[i] >= FOUR_OF_KIND_COUNT)
		{
			ranksUsed.push_back(int(ERankNames::ace) - i);
			numCardsUsed[0] += holeCardRankCounts[i];
			numCardsUsed[1] += boardCardRankCounts[i];

			for (int j = 0; j < numCardsUsed[0]; j++)
			{
				rankLocations.push_back(0);
			}
			for (int j = 0; j < numCardsUsed[1]; j++)
			{
				rankLocations.push_back(1);
			}

			assignKicker(ranksUsed, numCardsUsed, rankLocations);
			break;
		}
	}

	if (ranksUsed.size() == 0)
	{
		fourOfKindCards.clear();
		return fourOfKindCards;
	}

	// Find Four of Kind Cards because they exist

	int foundFourOfKindCards = 0;
	int ranksFound = 0;

	for (auto it = cardLocations.begin(); it != cardLocations.end(); it++)
	{
		if (it->first.getRankId() == ranksUsed[ranksFound])
		{
			fourOfKindCards.push_back(it->first);
			foundFourOfKindCards++;
		}
		if (foundFourOfKindCards == FOUR_OF_KIND_COUNT)
		{
			ranksFound++;
			break;
		}
	}

	for (auto it = cardLocations.begin(); it != cardLocations.end(); it++)
	{
		if (it->second == rankLocations[foundFourOfKindCards] && std::find(ranksUsed.begin(), ranksUsed.end(), it->first.getRankId()) == ranksUsed.end())
		{
			fourOfKindCards.push_back(it->first);
			break;
		}
		
	}
	return fourOfKindCards;
}

std::vector<Card> OmahaHand::findFullHouse()
{
	// look for best possible full house using two hole cards 
	// and three board cards
	std::vector<Card> fullHouseCards;
	std::vector<int> fHCardLocations;
	int threeOfKindRank = -1;
	int pairRank = -1;

	int holeCardsUsed = 0;
	int boardCardsUsed = 0;

	// Check to see if board is at least paired.
	// If false, full house is impossible.
	bool isBoardPaired = false;
	for (unsigned i = 0; i < boardCardRankCounts.size(); i++)
	{
		if (boardCardRankCounts[i] >= PAIR_COUNT)
		{
			isBoardPaired = true;
			break;
		}
	}

	if (!isBoardPaired)
	{
		fullHouseCards.clear();
		return fullHouseCards;
	}

	for (unsigned i = 0; i < boardCardRankCounts.size(); i++)
	{
		if (pairRank == -1)
		{
			fHCardLocations.clear();
			threeOfKindRank = -1;
			holeCardsUsed = 0;
			boardCardsUsed = 0;
		}
		else
		{
			break;
		}

		if (holeCardRankCounts[i] + boardCardRankCounts[i] >= THREE_OF_KIND_COUNT)
		{
			threeOfKindRank = int(ERankNames::ace) - i;
			holeCardsUsed += holeCardRankCounts[i];
			boardCardsUsed += boardCardRankCounts[i];

			for (int i = 0; i < holeCardsUsed; i++)
			{
				fHCardLocations.push_back(0);
			}
			for (int i = 0; i < boardCardsUsed; i++)
			{
				fHCardLocations.push_back(1);
			}

			for (unsigned j = 0; j < boardCardRankCounts.size(); j++)
			{
				if (int(ERankNames::ace) - j == threeOfKindRank)
				{
					// Pair Rank Cannot be same as threeOfKind rank
					continue;
				}
				else if(holeCardRankCounts[j] + boardCardRankCounts[j] >= PAIR_COUNT)
				{
					if (boardCardsUsed == MAX_BOARD_CARDS)
					{
						if (holeCardRankCounts[j] >= PAIR_COUNT)
						{
							fHCardLocations.push_back(0);
							fHCardLocations.push_back(0);
							pairRank = int(ERankNames::ace) - j;
							break;
						}
						else
						{
							continue;
						}
					}
					else if (holeCardsUsed == 1 && boardCardsUsed == MAX_BOARD_CARDS - 1)
					{
						if (holeCardRankCounts[j] >= 1 && boardCardRankCounts[j] >= 1)
						{
							fHCardLocations.push_back(0);
							fHCardLocations.push_back(1);
							pairRank = int(ERankNames::ace) - j;
							break;
						}
						else
						{
							continue;
						}
					}
					else if (holeCardsUsed == MAX_HOLE_CARDS && boardCardsUsed == 1)
					{
						if (boardCardRankCounts[j] >= PAIR_COUNT)
						{
							fHCardLocations.push_back(1);
							fHCardLocations.push_back(1);
							pairRank = int(ERankNames::ace) - j;
							break;
						}
						else
						{
							continue;
						}
					}
				}
			}
			if (pairRank >= 0)
			{
				break;
			}
		}
	}

	if (threeOfKindRank == -1 || pairRank == -1)
	{
		fullHouseCards.clear();
		return fullHouseCards;
	}
	
	// Find Full House Cards because they exist

	int foundThreeOfKindCards = 0;
	int foundPairCards = 0;
	int fHCardLocIndex = 0;
	
	while (foundThreeOfKindCards < THREE_OF_KIND_COUNT)
	{
		for (auto it = cardLocations.begin(); it != cardLocations.end(); it++)
		{
			if (it->first.getRankId() == threeOfKindRank &&  it->second == fHCardLocations[fHCardLocIndex])
			{
				auto findIt = std::find(fullHouseCards.begin(), fullHouseCards.end(), it->first);
				if (findIt == fullHouseCards.end())
				{
					fullHouseCards.push_back(it->first);
					foundThreeOfKindCards++;
					fHCardLocIndex++;
					break;
				}
				
			}
		}
		
	}
	
	while (foundPairCards < PAIR_COUNT)
	{
		for (auto it = cardLocations.begin(); it != cardLocations.end(); it++)
		{
			if (it->first.getRankId() == pairRank &&  it->second == fHCardLocations[fHCardLocIndex])
			{
				auto findIt = std::find(fullHouseCards.begin(), fullHouseCards.end(), it->first);
				if (findIt == fullHouseCards.end())
				{
					fullHouseCards.push_back(it->first);
					foundPairCards++;
					fHCardLocIndex++;
					break;
				}
			}
		}
		
	}
	
	return fullHouseCards;

}

std::vector<Card> OmahaHand::findFlush()
{
	
	std::vector<Card>bestFlushCards;
	if (flushSuit < 0)
	{
		bestFlushCards.clear();
		return bestFlushCards;
	}
	else
	{
		int boardCardsFound = 0;
		int holeCardsFound = 0;
		for (auto it = boardCards.begin(); it != boardCards.end(); it++)
		{
			if (it->getSuitId() == flushSuit)
			{
				bestFlushCards.push_back(*it);
				boardCardsFound++;
			}
			if (boardCardsFound == MAX_BOARD_CARDS)
			{
				break;
			}
		}
		for (auto it = holeCards.begin(); it != holeCards.end(); it++)
		{
			if (it->getSuitId() == flushSuit)
			{
				bestFlushCards.push_back(*it);
				holeCardsFound++;
			}
			if (holeCardsFound == MAX_HOLE_CARDS)
			{
				break;
			}
		}

		sort(bestFlushCards.begin(), bestFlushCards.end(), std::greater<Card>());
	}

	return bestFlushCards;
	
}

std::vector<Card> OmahaHand::findStraight()
{
	
	//bool result = true;
	//int numLoops = 2;
	std::vector<Card>bestStraightCards;
	if (!straightStartRanks.size())
	{
		bestStraightCards.clear();
		return bestStraightCards;
	}

	std::vector<int>straightCardLocations;
	int firstStraightRank = -1;
	int holeCardCount = 0;
	int boardCardCount = 0;
	int bothCardCount = 0;
	for (unsigned straightRankI = 0; straightRankI < straightStartRanks.size(); straightRankI++)
	{
		int firstRank = straightStartRanks[straightRankI];
		
		// look for firstRank in ranks
		int start = 0;
		while (ranks[start] > firstRank)
		{
			start++;
		}
		if (firstRank == int(ERankNames::five))
		{
			// Different method for checking ace low straight
			for (unsigned rankLocI = start; rankLocI < start + (HAND_SIZE - 1); rankLocI++)
			{
				if (rankLocations[rankLocI] == int(ERankLocations::hand))
				{
					straightCardLocations.push_back(rankLocations[rankLocI]);
					holeCardCount++;
				}
				else if (rankLocations[rankLocI] == int(ERankLocations::board))
				{
					straightCardLocations.push_back(rankLocations[rankLocI]);
					boardCardCount++;
				}
				else
				{
					straightCardLocations.push_back(rankLocations[rankLocI]);
					bothCardCount++;
				}
			}

			if (rankLocations[0] == int(ERankLocations::hand))
			{
				straightCardLocations.push_back(rankLocations[0]);
				holeCardCount++;
			}
			else if (rankLocations[0] == int(ERankLocations::board))
			{
				straightCardLocations.push_back(rankLocations[0]);
				boardCardCount++;
			}
			else
			{
				straightCardLocations.push_back(rankLocations[0]);
				bothCardCount++;
			}
		}
		else
		{
			// Normal straight
			for (unsigned rankLocI = start; rankLocI < start + HAND_SIZE; rankLocI++)
			{
				if (rankLocations[rankLocI] == int(ERankLocations::hand))
				{
					straightCardLocations.push_back(rankLocations[rankLocI]);
					holeCardCount++;
				}
				else if (rankLocations[rankLocI] == int(ERankLocations::board))
				{
					straightCardLocations.push_back(rankLocations[rankLocI]);
					boardCardCount++;
				}
				else
				{
					straightCardLocations.push_back(rankLocations[rankLocI]);
					bothCardCount++;
				}
			}
		}
		

		if (holeCardCount <= MAX_HOLE_CARDS && boardCardCount <= MAX_BOARD_CARDS)
		{
			firstStraightRank = firstRank;
			break;
		}
		else
		{
			holeCardCount = 0;
			boardCardCount = 0;
			straightCardLocations.clear();
		}
	}

	if (firstStraightRank == -1)
	{
		bestStraightCards.clear();
		return bestStraightCards;
	}
	
	// Determine location of straight cards
	for (unsigned cardLocI = 0; cardLocI < straightCardLocations.size(); cardLocI++)
	{
		if (straightCardLocations[cardLocI] == int(ERankLocations::both))
		{
			if (holeCardCount < MAX_HOLE_CARDS)
			{
				straightCardLocations[cardLocI] = int(ERankLocations::hand);
			}
			else
			{
				straightCardLocations[cardLocI] = int(ERankLocations::board);
			}
		}
	}

	int currentStraightRank = firstStraightRank;
	int foundStraightCards = 0;

	while (foundStraightCards < STRAIGHT_COUNT)
	{
		for (auto it = cardLocations.begin(); it != cardLocations.end(); it++)
		{
			if (it->first.getRankId() == currentStraightRank &&
				it->second == straightCardLocations[foundStraightCards])
			{
				bestStraightCards.push_back(it->first);
				break;
			}
		}

		foundStraightCards++;
		currentStraightRank--;

		if (currentStraightRank < 0)
		{
			currentStraightRank = int(ERankNames::ace);
		}
	}
	
	return bestStraightCards;
}

std::vector<Card> OmahaHand::find3OfaKind()
{
	// look for best possible Three of a Kind using two hole cards 
	// and three board cards
	std::vector<Card> threeOfKindCards;
	std::vector<int> ranksUsed;
	std::vector<int> rankLocations;
	std::array<int, NUM_LOCATIONS> numCardsUsed{ 0,0 };
	



	for (unsigned i = 0; i < boardCardRankCounts.size(); i++)
	{
		if (holeCardRankCounts[i] + boardCardRankCounts[i] >= THREE_OF_KIND_COUNT)
		{
			ranksUsed.push_back(int(ERankNames::ace) - i);
			numCardsUsed[0] += holeCardRankCounts[i];
			numCardsUsed[1] += boardCardRankCounts[i];

			for (int i = 0; i < numCardsUsed[0]; i++)
			{
				rankLocations.push_back(0);
			}
			for (int i = 0; i < numCardsUsed[1]; i++)
			{
				rankLocations.push_back(1);
			}
			while (rankLocations.size() < HAND_SIZE)
			{
				assignKicker(ranksUsed, numCardsUsed, rankLocations);
			}
		}
	}

	if (ranksUsed.empty())
	{
		threeOfKindCards.clear();
		return threeOfKindCards;
	}

	// Find Three of a Kind Cards because they exist

	int foundThreeOfKindCards = 0;
	int ranksFound = 0;
	int tkCardLocIndex = 0;

	while (threeOfKindCards.size() < HAND_SIZE)
	{
		for (auto it = cardLocations.begin(); it != cardLocations.end(); it++)
		{
			if (it->first.getRankId() == ranksUsed[ranksFound] && it->second == rankLocations[tkCardLocIndex])
			{
				auto findIt = std::find(threeOfKindCards.begin(), threeOfKindCards.end(), it->first);
				if (findIt == threeOfKindCards.end())
				{
					threeOfKindCards.push_back(it->first);
					foundThreeOfKindCards++;
					tkCardLocIndex++;
					if (foundThreeOfKindCards >= THREE_OF_KIND_COUNT)
					{
						ranksFound++;
					}
					break;
				}

			}
		}
		
	}
	return threeOfKindCards;
}

std::vector<Card> OmahaHand::findTwoPair()
{
	// look for best possible full house using two hole cards 
	// and three board cards
	std::vector<Card> twoPairCards;
	
	std::vector<int> ranksUsed;
	std::vector<int> rankLocations;
	std::array<int, NUM_LOCATIONS> numCardsUsed{ 0,0 };


	for (unsigned i = 0; i < boardCardRankCounts.size(); i++)
	{
		if (ranksUsed.size() == 1)
		{
			rankLocations.clear();
			ranksUsed.clear();
			std::fill(numCardsUsed.begin(), numCardsUsed.end(), 0);
		}
		else if(ranksUsed.size() > 1)
		{
			break;
		}

		if (holeCardRankCounts[i] + boardCardRankCounts[i] >= PAIR_COUNT)
		{
			ranksUsed.push_back (int(ERankNames::ace) - i);
			numCardsUsed[0] += holeCardRankCounts[i];
			numCardsUsed[1] += boardCardRankCounts[i];

			for (int i = 0; i < numCardsUsed[0]; i++)
			{
				rankLocations.push_back(0);
			}
			for (int i = 0; i < numCardsUsed[1]; i++)
			{
				rankLocations.push_back(1);
			}

			for (unsigned j = 0; j < boardCardRankCounts.size(); j++)
			{
				if (int(ERankNames::ace) - j == ranksUsed[0])
				{
					// Second Pair Rank Cannot be same as First Pair rank
					continue;
				}
				else if (holeCardRankCounts[j] + boardCardRankCounts[j] >= PAIR_COUNT)
				{
					if (numCardsUsed[0] == MAX_HOLE_CARDS)
					{
						if (boardCardRankCounts[j] >= PAIR_COUNT)
						{
							rankLocations.push_back(1);
							rankLocations.push_back(1);
							numCardsUsed[0] += PAIR_COUNT;
							ranksUsed.push_back(int(ERankNames::ace) - j);
							break;
						}
						else
						{
							continue;
						}
					}
					else if (numCardsUsed[0] == 1 && numCardsUsed[1] ==  1)
					{
						if (holeCardRankCounts[j] == 1 && boardCardRankCounts[j] >= 1)
						{
							rankLocations.push_back(0);
							rankLocations.push_back(1);
							numCardsUsed[0]++;
							numCardsUsed[1]++;
							ranksUsed.push_back(int(ERankNames::ace) - j);
							break;
						}
						else if (boardCardRankCounts[j] >= PAIR_COUNT)
						{
							rankLocations.push_back(1);
							rankLocations.push_back(1);
							numCardsUsed[1] += PAIR_COUNT;
							ranksUsed.push_back(int(ERankNames::ace) - j);
							break;
						}
						else
						{
							continue;
						}
					}
					else if (numCardsUsed[1] == MAX_BOARD_CARDS - 1)
					{
						if (holeCardRankCounts[j] == 1 && boardCardRankCounts[j] >= 1)
						{
							rankLocations.push_back(0);
							rankLocations.push_back(1);
							numCardsUsed[0]++;
							numCardsUsed[1]++;
							ranksUsed.push_back(int(ERankNames::ace) - j);
							break;
						}
						else if (holeCardRankCounts[j] >= PAIR_COUNT)
						{
							rankLocations.push_back(0);
							rankLocations.push_back(0);
							numCardsUsed[0] += PAIR_COUNT;
							ranksUsed.push_back(int(ERankNames::ace) - j);
							break;
						}
						else
						{
							continue;
						}
					}
				}
			}

			if (ranksUsed.size() > 1)
			{   
				assignKicker(ranksUsed, numCardsUsed, rankLocations);
			}

			if (ranksUsed.size() > 1)
			{
				break;
			}
		}
	}

	if (ranksUsed.size() <= 1)
	{
		twoPairCards.clear();
		return twoPairCards;
	}

	// Find Two Pair Cards because they exist

	int ranksFound = 0;
	int tpCardLocIndex = 0;

	while (twoPairCards.size() < HAND_SIZE)
	{
		for (auto it = cardLocations.begin(); it != cardLocations.end(); it++)
		{
			if (it->first.getRankId() == ranksUsed[ranksFound] && it->second == rankLocations[tpCardLocIndex])
			{
				auto findIt = std::find(twoPairCards.begin(), twoPairCards.end(), it->first);
				if (findIt == twoPairCards.end())
				{
					twoPairCards.push_back(it->first);
					tpCardLocIndex++;
					if (twoPairCards.size() % PAIR_COUNT == 0)
					{
						ranksFound++;
					}
					break;
				}
			}
		}	
	}
	
	return twoPairCards;
}

std::vector<Card> OmahaHand::findPair()
{
	// look for best possible full house using two hole cards 
	// and three board cards
	std::vector<Card> pairCards;
	std::vector<int> ranksUsed;
	std::vector<int> rankLocations;
	std::array<int, NUM_LOCATIONS> numCardsUsed{ 0,0 };
	
	for (unsigned i = 0; i < boardCardRankCounts.size(); i++)
	{

		if (holeCardRankCounts[i] + boardCardRankCounts[i] >= PAIR_COUNT)
		{
			ranksUsed.push_back(int(ERankNames::ace) - i);
			numCardsUsed[0] += holeCardRankCounts[i];
			numCardsUsed[1] += boardCardRankCounts[i];

			for (int i = 0; i < numCardsUsed[0]; i++)
			{
				rankLocations.push_back(0);
			}
			for (int i = 0; i < numCardsUsed[1]; i++)
			{
				rankLocations.push_back(1);
			}

			while (rankLocations.size() < HAND_SIZE)
			{
				assignKicker(ranksUsed, numCardsUsed, rankLocations);
			}
			break;
		}
	}

	if (ranksUsed.empty())
	{
		pairCards.clear();
		return pairCards;
	}

	// Find Pair Cards because they exist

	int foundPairCards = 0;
	int ranksFound = 0;
	int pairCardLocIndex = 0;

	while (pairCards.size() < HAND_SIZE)
	{
		for (auto it = cardLocations.begin(); it != cardLocations.end(); it++)
		{
			if (it->first.getRankId() == ranksUsed[ranksFound] && it->second == rankLocations[pairCardLocIndex])
			{
				auto findIt = std::find(pairCards.begin(), pairCards.end(), it->first);
				if (findIt == pairCards.end())
				{
					pairCards.push_back(it->first);
					foundPairCards++;
					pairCardLocIndex++;
					if (foundPairCards >= PAIR_COUNT)
					{
						ranksFound++;
					}
					break;
				}
			}
		}
	}
	return pairCards;
}

std::vector<Card> OmahaHand::findNoPair()
{

	std::vector<Card>bestNpCards;
	
	int boardCardsFound = 0;
	int holeCardsFound = 0;
	for (auto it = boardCards.begin(); it != boardCards.end(); it++)
	{
		bestNpCards.push_back(*it);
		boardCardsFound++;
		
		if (boardCardsFound == MAX_BOARD_CARDS)
		{
			break;
		}
	}
	for (auto it = holeCards.begin(); it != holeCards.end(); it++)
	{
		bestNpCards.push_back(*it);
		holeCardsFound++;

		if (holeCardsFound == MAX_HOLE_CARDS)
		{
			break;
		}
	}

	sort(bestNpCards.begin(), bestNpCards.end(), std::greater<Card>());


	return bestNpCards;
}

void OmahaHand::assignKicker(std::vector<int> &ranksUsed, std::array<int, NUM_LOCATIONS> &numCardsUsed, std::vector<int> &rankLocations)
{
	for (unsigned i = 0; i < boardCardRankCounts.size(); i++)
	{
		if (std::find(ranksUsed.begin(), ranksUsed.end(), int(ERankNames::ace) - i) != ranksUsed.end())
		{
			// Kicker Rank Cannot be same as a rank in ranksUsed
			continue;
		}
		else if (numCardsUsed[0] >= MAX_HOLE_CARDS)
		{
			if (boardCardRankCounts[i] >= 1)
			{
				rankLocations.push_back(1);
				numCardsUsed[1]++;
				ranksUsed.push_back(int(ERankNames::ace) - i);
				break;
			}
			else
			{
				//continue;
			}
		}
		else if (numCardsUsed[1] >= MAX_BOARD_CARDS)
		{
			if (holeCardRankCounts[i] >= 1)
			{
				rankLocations.push_back(0);
				numCardsUsed[0]++;
				ranksUsed.push_back(int(ERankNames::ace) - i);
				break;
			}
			else
			{
				//continue;
			}
		}
		else if (holeCardRankCounts[i] >= 1 || boardCardRankCounts[i] >= 1)
		{
			if (numCardsUsed[1] == MAX_BOARD_CARDS)
			{
				if (holeCardRankCounts[i] >= 1)
				{
					rankLocations.push_back(0);
					numCardsUsed[0]++;
					ranksUsed.push_back(int(ERankNames::ace) - i);
					break;
				}
				else
				{
					//continue;
				}
			}
			else
			{
				if (holeCardRankCounts[i] >= 1 && numCardsUsed[0] < MAX_HOLE_CARDS)
				{
					rankLocations.push_back(0);
					numCardsUsed[0]++;
					ranksUsed.push_back(int(ERankNames::ace) - i);
					break;
				}
				else if (boardCardRankCounts[i] >= 1 && numCardsUsed[1] < MAX_BOARD_CARDS)
				{
					rankLocations.push_back(1);
					numCardsUsed[1]++;
					ranksUsed.push_back(int(ERankNames::ace) - i);
					break;
				}
				else
				{
					//continue;
				}
			}
		}
	}
	return;
}

void OmahaHand::setBest5Cards(std::vector<Card> bestHand)
{
	for (unsigned i = 0; i < bestHand.size(); i++)
	{
		best5Cards[i] = bestHand[i];
	}
}


std::vector<Card> OmahaHand::getHoleCards()
{
	return holeCards;
}

std::vector<Card> OmahaHand::getBoardCards()
{
	return boardCards;
}
