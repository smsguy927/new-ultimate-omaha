#include "UltAnteBet.h"

UltAnteBet::UltAnteBet()
{
}

UltAnteBet::~UltAnteBet()
{
}

void UltAnteBet::calcPayout(int result)
{
	this->setPayout(this->getBetAmount() * result);
}

void UltAnteBet::printResult()
{
	// Extra spaces in display statement are Intentional.
	std::cout << "Ante Bet";
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
		std::cout << "\t" << std::setw(ULT_PAYTABLE_WIDTH) << "  1 to 1";

	}
	std::cout << std::setw(ULT_PAYOUT_WIDTH) << std::showpos << std::showpoint
		<< std::fixed << std::setprecision(2) << getPayout();

	std::cout << std::noshowpoint << std::setprecision(0) << std::noshowpos;
	return;
}
