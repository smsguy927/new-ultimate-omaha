#include "Bet.h"



Bet::Bet()
{
	this->betAmount = 0.0;
	this->payout = 0.0;
	this->betResult = int(EBetResult::null);
}


Bet::~Bet()
{
}


void Bet::setBetAmount(double amount)
{
	this->betAmount = amount;
}

void Bet::setPayout(double payout)
{
	this->payout = payout;
}

void Bet::setBetResult(int result)
{
	switch (result)
	{
	case int(EBetResult::null) :
		this->betResult = int(EBetResult::null);
		break;

	case int(EBetResult::loss) :
		this->betResult = int(EBetResult::loss);
		break;

	case int(EBetResult::push) :
		this->betResult = int(EBetResult::push);
		break;

	case int(EBetResult::win) :
		this->betResult = int(EBetResult::win);
		break;
	
	default:
		std::cerr << "Invalid bet result." << std::endl;
		break;
	}
}

double Bet::getBetAmount()
{
	return betAmount;
}

double Bet::getPayout()
{
	return payout;
}

int Bet::getBetResult()
{
	return betResult;
}
