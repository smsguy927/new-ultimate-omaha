#include "SideBet.h"



SideBet::SideBet()
{
}


SideBet::~SideBet()
{
}

void SideBet::setName(std::string name)
{
	this->name = name;
}

void SideBet::setDescription(std::string description)
{
	this->description = description;
}

void SideBet::setBetChoice(char choice)
{
	choice = tolower(choice);
	if (choice = 'y')
	{
		this->betChoice = choice;
	}
	else
	{
		this->betChoice = 'n';
	}
}

std::string SideBet::getName()
{
	return SideBet::name;
}

std::string SideBet::getDescription()
{
	return SideBet::description;
}

char SideBet::getBetChoice()
{
	return betChoice;
}
