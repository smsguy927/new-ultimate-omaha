#include "JacksOrBetterBet.h"


const std::array<int, JacksOrBetterBet::HAND_TYPES> JacksOrBetterBet::JB_PAYOUTS{ 250, 50, 25, 9, 6, 4, 3, 2, 1, 0, 0 };

JacksOrBetterBet::JacksOrBetterBet()
{
}


JacksOrBetterBet::~JacksOrBetterBet()
{
}

void JacksOrBetterBet::printPayTable()
{
	for (unsigned int i = 0; i < HAND_TYPES; i++)
	{
		if (HAND_NAMES[i].length() < MIN_HAND_NAME_LENGTH)
		{
			std::cout << HAND_NAMES[i] + "\t"; // print payout values on same line
		}
		else
		{
			std::cout << HAND_NAMES[i]; // print payout values on same line
		}

		for (unsigned int j = 0; j < PAYOUT_COLUMNS; j++)
		{
			if (i == 0 && j == PAYOUT_COLUMNS - 1)
			{
				std::cout << "\t" << std::showpoint << std::setprecision(2)
					<< std::fixed << (JB_PAYOUTS[i] + RF_MAX_BONUS) * (j + 1) * getBetAmount();
			}
			else
			{
				std::cout << "\t" << std::showpoint << std::setprecision(2)
					<< std::fixed << JB_PAYOUTS[i] * (j + 1) * getBetAmount();
			}

		}
		std::cout << std::endl;
	}
}

void JacksOrBetterBet::calcPayout(int handType)
{
	if (handType == int(EHandType::royalFlush))
	{
		setPayout(getBetAmount() * (JB_PAYOUTS[handType] + RF_MAX_BONUS) * credits * 1.0);
	}
	else
	{
		setPayout(getBetAmount() * JB_PAYOUTS[handType] * credits * 1.0);
	}
	this->payline = handType;
}

void JacksOrBetterBet::printResult()
{
	std::cout << HAND_NAMES[payline] << "\t" << std::showpos << std::showpoint << 
		std::setprecision(2) << getPayout();

	std::cout << std::noshowpos;
	return;
}

void JacksOrBetterBet::setCredits(int credits)
{
	if (credits > MAX_CREDITS)
	{
		this->credits = MAX_CREDITS;
	}
	else
	{
		this->credits = credits;
	}
}

int JacksOrBetterBet::getCredits()
{
	return credits;
}

