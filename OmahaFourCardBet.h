#pragma once
#include "SideBet.h"
#include "FourCards.h"
class OmahaFourCardBet :
	public SideBet
{
public:
	const static int HAND_TYPES = 9;
	const static int OMAHA_4CB_PAYTABLE_WIDTH = 5;
	const static int FC_PAYOUT_WIDTH = 10;
	const static std::array<int, HAND_TYPES> FOUR_CARD_PAYOUTS;
	OmahaFourCardBet();
	~OmahaFourCardBet();

	void printPayTable() override;
	void calcPayout(int) override; // from Bet class
	void printResult() override;

private:
	int payline;
};

