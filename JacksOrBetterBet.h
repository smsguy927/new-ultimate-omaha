#pragma once
#include <array>
#include "MainBet.h"
#include "Hand.h"
class JacksOrBetterBet : public MainBet
{
public:
	const static int MAX_CREDITS = 5;
	const int PAYOUT_COLUMNS = 5;
	const static int HAND_TYPES = 11;
	const static int RF_MAX_BONUS = 550;
	const static std::array<int, HAND_TYPES> JB_PAYOUTS;
	JacksOrBetterBet();
	virtual ~JacksOrBetterBet();

	void printPayTable();
	void calcPayout(int) override; // from Bet class
	void printResult() override;
	void setCredits(int);
	int getCredits();

private:
	int credits;
	int payline;
};

