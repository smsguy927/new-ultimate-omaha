#pragma once
#include <array>
#include "Hand.h"
#include "SideBet.h"
class UltTexTripsBet :
	public SideBet
{
public:
	const static int HAND_TYPES = 11;
	const static int ULT_PAYTABLE_WIDTH = 3;
	const static std::array<double, HAND_TYPES> ULT_TRIPS_PAYOUTS;
	UltTexTripsBet();
	~UltTexTripsBet();

	void printPayTable() override;
	void calcPayout(int) override; // from Bet class
	void printResult() override;

private:
	int payline;
};

