#include "UltTexBadBeatBet.h"


const std::array<int, UltTexBadBeatBet::HAND_TYPES> UltTexBadBeatBet::BAD_BEAT_PAYOUTS{ 0, 10'000, 500, 40, 25, 20, 9, -1, -1, -1, -1 };


UltTexBadBeatBet::UltTexBadBeatBet()
{
	setName("bad beat bet");
	setDescription("You will be paid if the losing hand type\n" 
		"is Three of a Kind or better by either the player or dealer.\n");
	this->payline = 0;
}


UltTexBadBeatBet::~UltTexBadBeatBet()
{
}

void UltTexBadBeatBet::printPayTable()
{
	std::cout << std::endl;
	// i starts at 1 because there is no such thing as a losing royal flush!
	for (unsigned int i = 1; i < int(EHandType::twoPair); i++)
	{
		if (HAND_NAMES[i].length() < MIN_HAND_NAME_LENGTH)
		{
			std::cout << HAND_NAMES[i] + "\t"; // print payout values on same line
		}
		else
		{
			std::cout << HAND_NAMES[i]; // print payout values on same line
		}

		std::cout << "\t" << std::setw(ULT_BBB_PAYTABLE_WIDTH) << BAD_BEAT_PAYOUTS[i] << " to 1";
		std::cout << std::endl;
	}
	std::cout << std::endl;
	std::cout << "If the losing hand type is Two Pair or less, you lose the bad beat bet.";
	std::cout << std::endl << std::endl;
}

void UltTexBadBeatBet::calcPayout(int handType)
{
	setPayout(getBetAmount() * BAD_BEAT_PAYOUTS[handType] * 1.0);
	this->payline = handType;
}

void UltTexBadBeatBet::printResult()
{
	std::cout << "Bad Beat Bet";
	if (this->getBetResult() == int(EBetResult::push))
	{
		std::cout << "\t    push";
	}
	else if (this->getBetResult() == int(EBetResult::loss))
	{
		std::cout << "\t    loss";
	}
	else
	{
		std::cout << "    " << std::setw(ULT_PAYTABLE_WIDTH) << BAD_BEAT_PAYOUTS[payline] << " to 1";
	}

	std::cout << std::setw(ULT_PAYOUT_WIDTH) << std::showpos << std::showpoint <<
		std::fixed << std::setprecision(2) << getPayout();
	
	std::cout << std::noshowpos;
	return;
}
