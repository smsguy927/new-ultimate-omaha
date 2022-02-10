#pragma once
#include "Bet.h"
class MainBet :
	public Bet
{
public:
	MainBet();
	virtual ~MainBet();

	virtual void calcPayout(int) override = 0;
};

