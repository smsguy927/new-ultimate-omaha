#pragma once
#include "MainBet.h"
#include "Hand.h"
class UltPlayBet :
	public MainBet
{
public:
	UltPlayBet();
	virtual ~UltPlayBet();

	void calcPayout(int) override; // from Bet class
	void printResult() override;
};
