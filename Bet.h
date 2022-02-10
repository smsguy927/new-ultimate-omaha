#pragma once
#include <iostream>
enum class EBetResult 
{
	null = -2,
	loss = -1,
	push,
	win
};
class Bet
{
public:
	Bet();
	virtual ~Bet();
	void setBetAmount(double);
	void setPayout(double);
	void setBetResult(int);
	double getBetAmount();
	double getPayout();
	int getBetResult();

	virtual void calcPayout(int) = 0;
	virtual void printResult() = 0;
	

private:
	double betAmount;
	double payout;
	int betResult;
};

