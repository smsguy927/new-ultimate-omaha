#pragma once
#include <array>
#include "Bet.h"
class SideBet :
	public Bet
{
public:
	const static int HAND_TYPES = 11;
	const static std::array<double, HAND_TYPES> ULT_BLIND_PAYOUTS;
	SideBet();
	virtual ~SideBet();

	void setName(std::string);
	void setDescription(std::string);
	void setBetChoice(char);

	std::string getName();
	std::string getDescription();
	char getBetChoice();

	virtual void printPayTable() = 0;
	virtual void calcPayout(int) = 0;

private:
	std::string name;
	std::string description;
	char betChoice;
};

