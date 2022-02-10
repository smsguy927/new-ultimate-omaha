#include "UltTexBlindBet.h"

const std::array<double, UltTexBlindBet::HAND_TYPES> UltTexBlindBet::ULT_BLIND_PAYOUTS{ 500, 50, 10, 3, 1.5, 1, 0, 0, 0, 0, 0 };

UltTexBlindBet::UltTexBlindBet()
{
}


UltTexBlindBet::~UltTexBlindBet()
{
}

void UltTexBlindBet::printPayTable()
{
	for (unsigned int i = 0; i < int(EHandType::threeOfaKind); i++)
	{
		if (HAND_NAMES[i].length() < MIN_HAND_NAME_LENGTH)
		{
			std::cout << HAND_NAMES[i] + "\t"; // print payout values on same line
		}
		else
		{
			std::cout << HAND_NAMES[i]; // print payout values on same line
		}

		if (i != int(EHandType::flush))
		{
			std::cout << "\t" << std::setw(ULT_PAYTABLE_WIDTH) <<
				static_cast<int>(ULT_BLIND_PAYOUTS[i]) << " to 1";
			std::cout << std::endl;
		}
		else
		{
			std::cout << "\t" << std::setw(ULT_PAYTABLE_WIDTH) << static_cast<int>
				(ULT_BLIND_PAYOUTS[i] * 2) << " to 2";
			std::cout << std::endl;
		}

	}
	std::cout << std::endl;
	std::cout << "If your hand type is Three of a Kind or less, the bet is a push." << std::endl;
	std::cout << "You don't win anything, but you don't lose anything either.\n" << std::endl;
}

void UltTexBlindBet::calcPayout(int handType)
{
	setPayout(getBetAmount() * ULT_BLIND_PAYOUTS[handType] * 1.0);
	this->payline = handType;
}

void UltTexBlindBet::printResult()
{
	std::cout << "Blind Bet";
	if (this->getBetResult() == int(EBetResult::push))
	{
		std::cout << "\t    push";
	}
	else if (this->getBetResult() == int(EBetResult::loss))
	{
		std::cout << "\t    loss";
	}
	else if (payline != int(EHandType::flush))
	{
		std::cout << "\t" << std::setw(ULT_PAYTABLE_WIDTH) <<
			static_cast<int>(ULT_BLIND_PAYOUTS[payline]) << " to 1";
		
	}
	else
	{
		std::cout << "\t" << std::setw(ULT_PAYTABLE_WIDTH) << static_cast<int>
			(ULT_BLIND_PAYOUTS[payline] * 2) << " to 2";
		
	} 
	std::cout << std::setw(ULT_PAYOUT_WIDTH) << std::showpos << std::showpoint 
		<< std::fixed << std::setprecision(2) << getPayout();
	
	std::cout << std::noshowpoint << std::setprecision(0) << std::noshowpos;
	return;
}
