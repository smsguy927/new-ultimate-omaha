#pragma once
#include <array>
#include "MainBet.h"
#include "Hand.h"
class UltTexBlindBet :
	public MainBet
{
public:
	const static int HAND_TYPES = 11;
	const static std::array<double, HAND_TYPES> ULT_BLIND_PAYOUTS;
	UltTexBlindBet();
	~UltTexBlindBet();

	void printPayTable();
	void calcPayout(int) override; // from Bet class
	void printResult() override;

private:
	int payline;
};

