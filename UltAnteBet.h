#pragma once
#include "MainBet.h"
#include "Hand.h"
class UltAnteBet :
	public MainBet
{
public:
	UltAnteBet();
	virtual ~UltAnteBet();

	void calcPayout(int) override; // from Bet class
	void printResult() override;
};

