#include "UltTexTripsBet.h"

const std::array<double, UltTexTripsBet::HAND_TYPES> UltTexTripsBet::ULT_TRIPS_PAYOUTS{ 50, 40, 30, 8, 6, 5, 3, -1, -1, -1, -1 };

UltTexTripsBet::UltTexTripsBet()
{
	setName("trips bet");
	setDescription("You will be paid if your hand type\n"
		"is Three of a Kind or better.\n");
}


UltTexTripsBet::~UltTexTripsBet()
{
}

void UltTexTripsBet::printPayTable()
{
	std::cout << std::endl;
	for (unsigned int i = 0; i < int(EHandType::twoPair); i++)
	{
		if (HAND_NAMES[i].length() < MIN_HAND_NAME_LENGTH)
		{
			std::cout << HAND_NAMES[i] + "\t"; // print payout values on same line
		}
		else
		{
			std::cout << HAND_NAMES[i]; // print payout values on same line
		}

		std::cout << "\t" << std::setw(ULT_PAYTABLE_WIDTH) << ULT_TRIPS_PAYOUTS[i] << " to 1";
		std::cout << std::endl;
	}
	std::cout << std::endl;
	std::cout << "If your hand type is Two Pair or less, you lose the trips bet.\n" << std::endl;
	std::cout << "You can win the trips bet even if you fold or the dealer wins.\n" << std::endl;
}

void UltTexTripsBet::calcPayout(int handType)
{
	setPayout(getBetAmount() * ULT_TRIPS_PAYOUTS[handType] * 1.0);
	this->payline = handType;
}

void UltTexTripsBet::printResult()
{
	std::cout << "Trips Bet\t";
	// Trips Bet Does Not Push
	if (this->getBetResult() == int(EBetResult::loss))
	{
		std::cout << "    loss";
	} 
	else
	{
		std::cout << std::setw(ULT_PAYTABLE_WIDTH) << ULT_TRIPS_PAYOUTS[payline] << " to 1";
	}

	std::cout << std::setw(ULT_PAYOUT_WIDTH) << std::showpos << std::showpoint <<
		std::fixed << std::setprecision(2) << getPayout();

	std::cout << std::noshowpos;
	return;
}
