//...and can even make play the optional Bad Beat Bet if they can afford it.
// 2017 © — Lake Elsinore Casino, Lake Elsinore, CA 
#pragma once
#include <array>
#include "Hand.h"
#include "SideBet.h"
class UltTexBadBeatBet :
	public SideBet
{
public:
	const static int HAND_TYPES = 11;
	const static int ULT_BBB_PAYTABLE_WIDTH = 5;
	const static std::array<int, HAND_TYPES> BAD_BEAT_PAYOUTS;// { 0, 10'000, 500, 40, 25, 20, 9, -1, -1, -1, -1 };
	UltTexBadBeatBet();
	~UltTexBadBeatBet();

	void printPayTable() override;
	void calcPayout(int) override; // from Bet class
	void printResult() override;

private:
	int payline;
};

