#include "OmahaFourCardBet.h"

const std::array<int, OmahaFourCardBet::HAND_TYPES> OmahaFourCardBet::FOUR_CARD_PAYOUTS{ 1000, 200, 50, 25, 10, 10, 8, 1, -1};


OmahaFourCardBet::OmahaFourCardBet()
{
	setName("four card bet");
	setDescription("You will be paid if your four card hand type\n" 
		"is a pair or better.\n");
}


OmahaFourCardBet::~OmahaFourCardBet()
{
}

void OmahaFourCardBet::printPayTable()
{
	std::cout << std::endl;
	for (unsigned int i = 0; i < int(EFourCardType::noPair); i++)
	{
		if (FC_HAND_NAMES[i].length() < FC_MIN_HAND_NAME_LENGTH)
		{
			std::cout << FC_HAND_NAMES[i] + "\t"; // print payout values on same line
		}
		else
		{
			std::cout << FC_HAND_NAMES[i]; // print payout values on same line
		}

		std::cout << "\t" << std::setw(OMAHA_4CB_PAYTABLE_WIDTH) << FOUR_CARD_PAYOUTS[i] << " to 1";
		std::cout << std::endl;
	}
	std::cout << std::endl;
	std::cout << "If your four card hand type is no pair, you lose the four card bet.";
	std::cout << std::endl << std::endl;
}

void OmahaFourCardBet::calcPayout(int handType)
{
	setPayout(getBetAmount() * FOUR_CARD_PAYOUTS[handType] * 1.0);
	this->payline = handType;
}

void OmahaFourCardBet::printResult()
{
	std::cout << "Four Card Bet ";
	// Four Card Bet Does Not Push
	if (this->getBetResult() == int(EBetResult::loss))
	{
		std::cout << "      loss";
	}
	else
	{
		std::cout << std::setw(OMAHA_4CB_PAYTABLE_WIDTH) << FOUR_CARD_PAYOUTS[payline] << " to 1";
	}

	std::cout << std::setw(FC_PAYOUT_WIDTH) << std::showpos << std::showpoint <<
		std::fixed << std::setprecision(2) << getPayout();

	std::cout << std::noshowpos;
	return;
}
