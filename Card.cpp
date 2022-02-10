#include "Card.h"



Card::Card()
{
}

Card::Card(int ID)
{
	setId(ID);
}

Card::Card(int rankID, int suitID)
{
	
}


Card::~Card()
{
}

void Card::setId(int id)
{
	this->id = id;
	setRank(id % NUM_RANKS);
	setRankId(id % NUM_RANKS);
	//setSuit(id % NUM_SUITS);
	//setSuitId(id % NUM_SUITS);
	setSuit(id / NUM_RANKS);
	setSuitId(id / NUM_RANKS);
}

void Card::setRankId(int num)
{
	this->rankId = num;
}

void Card::setSuitId(int num)
{
	this->suitId = num;
}

void Card::setRank(int rank)
{
	
	// Use Switch
	switch (rank)
	{
	case int(ERankNames::two) :
		rank = '2';
		
		setRankName(RANK_NAMES[int(ERankNames::two)]);
		break;

	case int(ERankNames::three) :
		rank = '3';
		
		setRankName(RANK_NAMES[int(ERankNames::three)]);
		break;

	case int(ERankNames::four) :
		rank = '4';
		
		setRankName(RANK_NAMES[int(ERankNames::four)]);
		break;

	case int(ERankNames::five) :
		rank = '5';
		
		setRankName(RANK_NAMES[int(ERankNames::five)]);
		break;

	case int(ERankNames::six) :
		rank = '6';
		
		setRankName(RANK_NAMES[int(ERankNames::six)]);
		break;

	case int(ERankNames::seven) :
		rank = '7';
		
		setRankName(RANK_NAMES[int(ERankNames::seven)]);
		break;

	case int(ERankNames::eight) :
		rank = '8';
		
		setRankName(RANK_NAMES[int(ERankNames::eight)]);
		break;

	case int(ERankNames::nine) :
		rank = '9';
		
		setRankName(RANK_NAMES[int(ERankNames::nine)]);
		break;

	case int(ERankNames::ten) :
		rank = 'T';
		
		setRankName(RANK_NAMES[int(ERankNames::ten)]);
		break;

	case int(ERankNames::jack) :
		rank = 'J';
		
		setRankName(RANK_NAMES[int(ERankNames::jack)]);
		break;

	case int(ERankNames::queen) :
		rank = 'Q';
		
		setRankName(RANK_NAMES[int(ERankNames::queen)]);
		break;

	case int(ERankNames::king) :
		rank = 'K';
		
		setRankName(RANK_NAMES[int(ERankNames::king)]);
		break;

	case int(ERankNames::ace) :
		rank = 'A';
		
		setRankName(RANK_NAMES[int(ERankNames::ace)]);
		break;

	default:
		std::cout << "Invalid rank." << std::endl;
		rank = '0';
		throw std::invalid_argument("Not a rank");

	}
	this->rank = rank;
}

void Card::setSuit(int suit)
{
	// Use Switch
	switch (suit)
	{
	case(int(ESuitNames::clubs)):
		suit = 'c';
		setSuitName(SUIT_NAMES[int(ESuitNames::clubs)]);
		break;

	case(int(ESuitNames::diamonds)):
		suit = 'd';
		setSuitName(SUIT_NAMES[int(ESuitNames::diamonds)]);
		break;

	case(int(ESuitNames::hearts)):
		suit = 'h';
		setSuitName(SUIT_NAMES[int(ESuitNames::hearts)]);
		break;

	case(int(ESuitNames::spades)):
		suit = 's';
		setSuitName(SUIT_NAMES[int(ESuitNames::spades)]);
		break;

	default:
		std::cerr << "Invalid suit." << std::endl;
		suitId = NUM_SUITS;
		suit = 'e';
		throw std::invalid_argument("Not a suit symbol");
		break;

	}
	this->suit = suit;
}

void Card::setRankName(std::string rankName)
{
	this->rankName = rankName;
}

void Card::setSuitName(std::string suitName)
{
	this->suitName = suitName;
}

int Card::getId()
{
	return this->id;
}

char Card::getRank()
{
	return this->rank;
}

char Card::getSuit()
{
	return this->suit;
}

int Card::getRankId() const
{
	return this->rankId;
}

int Card::getSuitId() const
{
	return this->suitId;
}

std::string Card::getRankName()
{
	return this->rankName;
}

std::string Card::getSuitName()
{
	return this->suitName;
}

void Card::print()
{
	std::cout << this->rankName << " of " << this->suitName << "  ";
}

void Card::printSymbol()
{
	std::cout << this->rank << this->suit << " ";
}

bool Card::operator<(Card card2) const
{
	if (this->rankId < card2.rankId)
	{
		return true;
	}
	else if (this->rankId > card2.rankId)
	{
		return false;
	}
	else if (this->suitId < card2.suitId)
	{
		return true;
	}
	else if (this->suitId > card2.suitId)
	{
		return false;
	}
	else
	{
		return false;
	}
	
}

bool Card::operator>(Card card2) const
{
	if (this->rankId > card2.rankId)
	{
		return true;
	}
	else if (this->rankId < card2.rankId)
	{
		return false;
	}
	else if (this->suitId > card2.suitId)
	{
		return true;
	}
	else if (this->suitId < card2.suitId)
	{
		return false;
	}
	else
	{
		return false;
	}
}

// Used when checking to see if a card already exists in a container
bool Card::operator==(Card card2) const
{
	return this->rankId == card2.rankId && this->suitId == card2.suitId;
}

bool Card::isLowerSuit(const Card &card1 , const Card &card2) 
{
	if (card1.suitId < card2.suitId)
	{
		return true;
	}
	else if (card1.suitId > card2.suitId)
	{
		return false;
	}
	else if (card1.rankId < card2.rankId)
	{
		return true;
	}
	else if (card1.rankId > card2.rankId)
	{
		return false;
	}
	else
	{
		return false;
	}
}

bool Card::isHigherSuit(Card card1, Card card2)
{
	if (card1.suitId > card2.suitId)
	{
		return true;
	}
	else if (card1.suitId < card2.suitId)
	{
		return false;
	}
	else if (card1.rankId > card2.rankId)
	{
		return true;
	}
	else if (card1.rankId < card2.rankId)
	{
		return false;
	}
	else
	{
		return false;
	}
	
}
