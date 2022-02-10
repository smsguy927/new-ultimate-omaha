////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9
// Sean Shaughnessy
#include <iostream>
#include <string>
#include <limits>
#include <iomanip>
#include <array>
#include <deque>
#include <vector>
#include <algorithm> 
#include <numeric>
#include <functional>
#include <random>
#include <iterator>
#include <fstream>
#include <regex>
#include "Card.h"
#include "DeckOfCards.h"
#include "Hand.h"
#include "OmahaHand.h"
#include "FourCards.h"
#include "JacksOrBetterBet.h"
#include "UltAnteBet.h"
#include "UltTexBlindBet.h"
#include "UltPlayBet.h"
#include "UltTexTripsBet.h"
#include "UltTexBadBeatBet.h"
#include "OmahaFourCardBet.h"
using namespace std;

enum class EGameMenu
{
	draw = 1,
	ultHoldem,
	ultOmaha,
	display,
	viewBalace,
	quit = 0
};

enum class EDeckMenu
{
	sort = 1,
	reverseSort,
	sortBySuit,
	revSortBySuit,
	reverse,
	shuffleCut,
	dispNames,
	dispSymbols,
	goBack = 0
};

// Constants
const int CENTS_TO_DOLLARS = 100;
const int STUD_HAND_SIZE = 7;
const int HOLDEM_ALL_CARDS = 7;
const int HOLDEM_HOLE_CARDS = 2;
const int OMAHA_ALL_CARDS = 9;
const int OMAHA_HOLE_CARDS = 4;
const int COMMUNITY_CARDS = 5;
const int PREFLOP_PLAY_MIN = 3;
const int PREFLOP_PLAY_MAX = 4;
const int FLOP_PLAY_BET = 2;
const int ULT_NUM_BETS = 3;
const int MAX_CREDITS = 5;
const int HOLDEM_SIDE_BETS = 2;
const int OMAHA_SIDE_BETS = 3;
const double ROUND_UP_MIN = 0.99;
const double ROUND_UP_ADD = 0.01;

const string INPUT_FILE = "Money.txt";
const array<char, NUM_SUITS> SUIT_SYMBOLS
{
	int(ESuitSymbols::hearts),
	int(ESuitSymbols::diamonds),
	int(ESuitSymbols::clubs),
	int(ESuitSymbols::spades)
};

// Error Message
void displayMoneyNotFound();

// Games
double playJacksOrBetter(double);
double playUltimateHoldem(double);
double playUltimateOmaha(double);

// Print / Display Functions
void askForAnteBlind();
void printTurnAndRiver(Hand& boardHand);
template <class T> void printBetResults(vector<T*>);
void printTotalWin(double totalWin);
void displayBestHands(Hand&, Hand&, int);

// Game Helper Functions
void dealCards(std::vector<Card>&, std::vector<Card>&, std::vector<Card>&, int);
double calcMaxPreFlopBet(double);
void drawCards(Hand&, JacksOrBetterBet&, vector<Card>&, DeckOfCards&);

// Deck Menu Functions
void deckMenu();
void displayCardNames(DeckOfCards&);
void displayCardSymbols(DeckOfCards&);

// Game Specific Validation
void validateSideBets(std::vector<SideBet*>&, double, double&);
char validatePreflopChoice();
double validatePreflopBet(UltTexBlindBet& ultTexBlindBet, double moneyIn);
int validatePlayBet(double, double, double);
char validateFlopChoice();
char validateRiverChoice();
char validatePlayAgain(double, double);

// General Validation
int validate(int = 0, int = 0);
double validateDub(double = 0, double = 0);

int main()
{
	ifstream moneyInFile{ INPUT_FILE, ios::in };
	double moneyIn;
	string moneyStr;
	cout << "Please insert money." << endl;
	//double moneyIn = validateDub();
	if (!moneyInFile)
	{
		displayMoneyNotFound();
		exit(1);
	}
	getline(moneyInFile, moneyStr);
	try
	{
		moneyIn = stod(moneyStr);
	}
	catch (invalid_argument)
	{
		cerr << INPUT_FILE << " does not have a valid money amount. Please edit the file so" << endl;
		cerr << "it contains only a positive number without a $ symbol such as 20 or 123.45" << endl;
		cerr << "The program has ended. Press any key to exit." << endl;
		cin.get();
		exit(1);
	}
	if (moneyIn < 0.01)
	{
		cerr << "You do not have enough money to play any games." << endl;
		cerr << "The program has ended. Press any key to exit." << endl;
		cin.get();
		exit(1);
	}
	else
	{
		// Add .0001 to a moneyIn value that has 99 *after* two decimal places.
		// Example: 33.2999 -> 33.3001
		moneyIn *= CENTS_TO_DOLLARS;
		double decimalPart = moneyIn - static_cast<int>(moneyIn);
		if (decimalPart > ROUND_UP_MIN)
		{
			moneyIn += ROUND_UP_ADD;
		}
		moneyIn /= CENTS_TO_DOLLARS;
		cout << "Money accepted." << endl;
		cout << "Welcome to the Video Poker Demonstration" << endl;
	}

	int gameChoice;
	do
	{
		cout << "Game Menu" << endl;
		for (unsigned i = 0; i < DECK_SIZE; i++)
		{
			cout << SUIT_SYMBOLS[i % NUM_SUITS];
		}
		cout << endl;
		cout << "What game would you like to play?" << endl;
		cout << "1. Jacks or Better Single Draw" << endl;
		cout << "2. Ultimate Texas Hold'em" << endl;
		cout << "3. Ultimate Omaha" << endl;
		cout << "4. Sort and Display Deck Menu" << endl;
		cout << "5. Display Your Balance" << endl;
		cout << "0. Quit" << endl;
		gameChoice = validate();

		switch (gameChoice)
		{
			case int(EGameMenu::draw) :
				moneyIn = playJacksOrBetter(moneyIn);
				break;

				case int(EGameMenu::ultHoldem) :
					moneyIn = playUltimateHoldem(moneyIn);
					break;

					case int(EGameMenu::ultOmaha) :
						moneyIn = playUltimateOmaha(moneyIn);
						break;

						case int(EGameMenu::display) :
							deckMenu();
							break;

							case int(EGameMenu::viewBalace) :
								cout << "You have $" << showpoint << setprecision(2) << fixed << moneyIn << endl;
								break;

								case int(EGameMenu::quit) :
									break;

								default:
									cout << "I don't know that game. Please try again." << endl;
		}

	} while (gameChoice != int(EGameMenu::quit) && moneyIn >= 0.01);

	// Program End

	moneyInFile.close();
	// Rewrite money.txt with the new amount
	ofstream moneyOutFile{ INPUT_FILE, ios::out };
	moneyOutFile << moneyIn;
	cout << "Thanks for playing!" << endl;
	cout << "You get $" << showpoint << setprecision(2) << fixed << moneyIn << endl;
	moneyOutFile.close();
	cin.get();
	cin.get();
	cout << endl << endl;
	cin.clear();
	//cout << "The program has ended. Press any key to exit.";
	//cin.ignore(numeric_limits<streamsize>::max(), '\n');
	return 0;
}


void displayMoneyNotFound()
{
	cerr << INPUT_FILE << " not found. Please make sure " << INPUT_FILE << " and" << endl;
	cerr << "this program's .cpp file are in the same folder." << endl;
	cerr << "If " << INPUT_FILE << "does not exist, create a new file" << endl;
	cerr << "called " << INPUT_FILE << ", enter the amount of money to deposit" << endl;
	cerr << "without $ symbol and place it in the same folder as" << endl;
	cerr << "this program's .cpp file" << endl;
	cerr << "The program has ended. Press any key to exit." << endl;
	cin.get();
	return;
}

void deckMenu()
{
	int gameChoice;
	DeckOfCards deck = DeckOfCards();
	do
	{

		cout << "Sort and Display Deck Menu" << endl;
		for (unsigned i = 0; i < DECK_SIZE; i++)
		{
			cout << SUIT_SYMBOLS[i % NUM_SUITS];
		}
		cout << endl;
		cout << "What would you like to do?" << endl;
		cout << "1. Sort Deck By Rank" << endl;
		cout << "2. Reverse Sort Deck By Rank" << endl;
		cout << "3. Sort Deck By Suit " << endl;
		cout << "4. Reverse Sort Deck By Suit" << endl;
		cout << "5. Reverse the Deck" << endl;
		cout << "6. Shuffle and Cut the Deck" << endl;
		cout << "7. Display Card Names" << endl;
		cout << "8. Display Card Symbols" << endl;
		cout << "0. Go Back to Game Menu" << endl;
		gameChoice = validate();

		switch (gameChoice)
		{
			case int(EDeckMenu::sort) :
				deck.sortDeck();
				break;

				case int(EDeckMenu::reverseSort) :
					deck.reverseSortDeck();
					break;

					case int(EDeckMenu::sortBySuit) :
						deck.sortBySuit();
						break;

						case int(EDeckMenu::revSortBySuit) :
							deck.reverseSortBySuit();
							break;

							case int(EDeckMenu::reverse) :
								deck.reverseDeck();
								break;

								case int(EDeckMenu::shuffleCut) :
									deck.shuffleCards();
									deck.shuffleCards();
									deck.shuffleCards();
									deck.shuffleCards();
									deck.cutCards();
									break;

									case int(EDeckMenu::dispNames) :
										displayCardNames(deck);
										break;

										case int(EDeckMenu::dispSymbols) :
											displayCardSymbols(deck);
											break;

											case int(EDeckMenu::goBack) :
												break;

											default:
												cout << "That's not a menu choice. Please try again." << endl;
		}

	} while (gameChoice != int(EDeckMenu::goBack));
}

double playJacksOrBetter(double moneyIn)
{
	cout << "Please select stakes." << endl;
	double stakes = validateDub(.01);
	char playAgain = 'y';
	do
	{
		JacksOrBetterBet jacksBetterBet = JacksOrBetterBet();
		cout << "How many credits do you want to play?" << endl;
		cout << "Enter 0 to play max credits." << endl;
		int credits = validate(0, MAX_CREDITS);
		credits ? credits : credits = MAX_CREDITS;
		if (moneyIn < stakes * credits)
		{
			cout << "You don't have enough money to play." << endl;
			break;
		}
		moneyIn -= stakes * credits;
		jacksBetterBet.setBetAmount(stakes);
		jacksBetterBet.setCredits(credits);

		vector<Card> myHand(DRAW_HAND_SIZE);


		DeckOfCards myDeck = DeckOfCards();
		Card myCard;
		Hand hand = Hand(DRAW_HAND_SIZE);
		myDeck.shuffleCards();
		myDeck.shuffleCards();
		myDeck.shuffleCards();
		myDeck.shuffleCards();
		myDeck.cutCards();

		cout << endl;
		myDeck.setCurrentCard(0);
		for (int i = 0; i < DRAW_HAND_SIZE; i++)
		{
			myCard = myDeck.dealCard();

			myHand[i] = myCard;
		}
		hand.setCards(myHand);
		hand.printHand();
		cout << endl << endl;

		drawCards(hand, jacksBetterBet, myHand, myDeck);
		hand.setCards(myHand);

		hand.evaluateHand();
		cout << "You have " << hand.getHandName(int(EGameType::jacksBetter)) << endl;
		jacksBetterBet.calcPayout(hand.getHandType());
		double totalWin = jacksBetterBet.getPayout();
		moneyIn += totalWin;
		if (totalWin)
		{
			cout << "You win $" << showpoint << setprecision(2) << fixed << totalWin << endl;
			hand.printBest5Cards();
			cout << endl << endl;
		}
		else
		{
			cout << "You lose." << endl;
		}
		hand.printHand();
		cout << endl << endl;
		double totalInitialBets = 0;
		playAgain = validatePlayAgain(moneyIn, totalInitialBets);
	} while (tolower(playAgain) != 'n');
	return moneyIn;
}

double playUltimateHoldem(double moneyIn)
{
	char playAgain = 'y';
	do
	{
		if (moneyIn < 0.01 * ULT_NUM_BETS)
		{
			cout << "You do not have enough money to play." << endl;
			break;
		}

		Hand myHand = Hand(HOLDEM_HOLE_CARDS);
		Hand dealerHand = Hand(HOLDEM_HOLE_CARDS);
		Hand boardHand = Hand(COMMUNITY_CARDS);

		vector<Card> myHoleCards(HOLDEM_HOLE_CARDS);
		vector<Card> dealerHoleCards(HOLDEM_HOLE_CARDS);
		vector<Card> board(COMMUNITY_CARDS);

		dealCards(myHoleCards, dealerHoleCards, board, int(EGameType::ultHoldEm));
		myHand.setCards(myHoleCards);
		dealerHand.setCards(dealerHoleCards);
		boardHand.setCards(board);

		// Betting
		UltAnteBet ultAnteBet = UltAnteBet();
		UltTexBlindBet ultTexBlindBet = UltTexBlindBet();
		//The player must make an equal bet on both the Ante and Blind... 
		double ante;
		double blindBet;
		double totalBet = 0; // Used for validation against moneyIn
		// They cannot bet what they don't have. No credit.
		ante = validatePreflopBet(ultTexBlindBet, moneyIn);
		totalBet += ante * ULT_NUM_BETS;
		blindBet = ante;
		ultAnteBet.setBetAmount(ante);
		ultTexBlindBet.setBetAmount(blindBet);

		UltTexTripsBet ultTexTripsBet = UltTexTripsBet();
		UltTexBadBeatBet ultBadBeatBet = UltTexBadBeatBet();
		vector<SideBet*> sideBets{ &ultTexTripsBet, &ultBadBeatBet };
		validateSideBets(sideBets, moneyIn, totalBet);


		// Reveal Player Hand
		cout << "Your Hole Cards: ";
		myHand.printHand();
		cout << endl << endl;

		// The player can check or make a Play bet equal to three or four times the Ante
		// if they can afford it, of course.
		char preflopCheckOrBet;
		int playBet = 0;
		UltPlayBet ultPlayBet = UltPlayBet();
		if ((PREFLOP_PLAY_MIN - 1) * blindBet + totalBet <= moneyIn)
		{
			preflopCheckOrBet = validatePreflopChoice();

			if (preflopCheckOrBet == 'b')
			{
				playBet = validatePlayBet(blindBet, totalBet, moneyIn);
				totalBet += (playBet - 1) * blindBet;
				ultPlayBet.setBetAmount(playBet * ante);
			}
		}
		else // Player has no money left
		{
			preflopCheckOrBet = 'c';
		}

		if (preflopCheckOrBet == 'b')
		{
			// Display Player Hand again
			cout << "Your Hole Cards: ";
			myHand.printHand();
			cout << endl << endl;
		}


		// The dealer turns over three community cards.

		// Reveal board cards
		cout << "Flop: ";
		boardHand.printFlop();
		cout << endl;


		char flopCheckOrBet;
		// If the player already made a Play bet, then he may not bet further.
		// If the player cannot afford the 2x bet, he cannot make it either.
		if (playBet || totalBet >= moneyIn)
		{
			flopCheckOrBet = 'c';
		}
		else
		{
			flopCheckOrBet = validateFlopChoice();

			if (flopCheckOrBet == 'b')
			{
				totalBet += (FLOP_PLAY_BET - 1) * blindBet;
				playBet = FLOP_PLAY_BET;
				ultPlayBet.setBetAmount(playBet * ante);
			}
		}

		printTurnAndRiver(boardHand);

		// If the player already raised he may not bet further.
		char riverChoice;
		if (playBet)
		{
			riverChoice = 'c';
		}
		else
		{
			riverChoice = validateRiverChoice();
			if (riverChoice == 'b')
			{
				playBet = 1;
				ultPlayBet.setBetAmount(playBet * ante);
			}
			else
			{
				// Subtract the 1x bet from their total bet if they fold.
				totalBet -= blindBet;
			}
		}
		cout << endl << endl;


		vector<MainBet*> mainBets{ &ultAnteBet, &ultPlayBet, &ultTexBlindBet, };

		// The player and dealer will both make the best possible hand using any combination 
		// of their own two cards and the five community cards.
		if (riverChoice != 'f')
		{

			// Reveal Dealer Hole Cards
			cout << "Dealer Hole Cards: ";
			dealerHand.printHand();
			cout << endl;

			for (unsigned i = 0; i < COMMUNITY_CARDS; i++)
			{
				myHoleCards.push_back(board[i]);
				dealerHoleCards.push_back(board[i]);
			}
			myHand.setHandSize(HOLDEM_ALL_CARDS);
			dealerHand.setHandSize(HOLDEM_ALL_CARDS);
			myHand.setCards(myHoleCards);
			dealerHand.setCards(dealerHoleCards);

			cout << endl;


			cout << endl;

			myHand.evaluateHand();
			dealerHand.evaluateHand();
			cout << "You have " << myHand.getHandName(int(EGameType::ultHoldEm)) << endl;
			cout << "The dealer has " << dealerHand.getHandName(int(EGameType::ultHoldEm)) << endl;
			bool dealerPlays;
			bool areSameHands = myHand == dealerHand;
			bool playerWins = myHand > dealerHand;
			bool playerLoses = myHand < dealerHand;
			double payout = blindBet * playBet * 1.0;

			ultTexBlindBet.calcPayout(myHand.getHandType());
			ultTexTripsBet.calcPayout(myHand.getHandType());

			if (myHand.getHandType() > int(EHandType::straight))
			{
				ultTexBlindBet.setBetResult(int(EBetResult::push));
			}
			else
			{
				// Set Blind Bet result to win if handType is straight
				// or better. It will be set to push or loss later 
				// if dealer's hand is better or areSameHands.
				ultTexBlindBet.setBetResult(int(EBetResult::win));
			}

			double blindPayout = ultTexBlindBet.getPayout();
			double tripsPayout;
			double badBeatPayout;
			double totalWin = 0;

			if (sideBets[0]->getBetChoice() == 'y' && myHand.getHandType() > int(EHandType::threeOfaKind))
			{
				tripsPayout = ultTexTripsBet.getPayout();
				ultTexTripsBet.setBetResult(int(EBetResult::loss));
			}
			else if (sideBets[0]->getBetChoice() == 'y')
			{
				tripsPayout = ultTexTripsBet.getPayout();
				ultTexTripsBet.setBetResult(int(EBetResult::win));
			}
			else
			{
				tripsPayout = 0;
				ultTexTripsBet.setBetResult(int(EBetResult::null));
			}


			if (!areSameHands && myHand.getHandType() <= int(EHandType::threeOfaKind)
				&& dealerHand.getHandType() <= int(EHandType::threeOfaKind)
				&& sideBets[1]->getBetChoice() == 'y')
			{
				if (playerWins)
				{
					ultBadBeatBet.calcPayout(dealerHand.getHandType());
					badBeatPayout = ultBadBeatBet.getPayout() * 1.0;
					ultBadBeatBet.setBetResult(int(EBetResult::win));
				}
				else
				{
					ultBadBeatBet.calcPayout(myHand.getHandType());
					badBeatPayout = ultBadBeatBet.getPayout() * 1.0;
					ultBadBeatBet.setBetResult(int(EBetResult::win));
				}
			}
			else if (sideBets[1]->getBetChoice() == 'y')
			{
				ultBadBeatBet.calcPayout(int(EHandType::noPair));
				badBeatPayout = ultBadBeatBet.getPayout() * 1.0;
				ultBadBeatBet.setBetResult(int(EBetResult::loss));
			}
			else
			{
				badBeatPayout = 0;
				ultBadBeatBet.setBetResult(int(EBetResult::null));
			}

			// The dealer will need at least a pair to play for the ante.
			if (dealerHand.getHandType() == int(EHandType::noPair))
			{
				dealerPlays = false;
			}
			else
			{
				dealerPlays = true;
			}
			// The Trips bet will pay according to the poker value of the player's hand 
			//regardless of the value of the dealer's hand, according to the trips bet 
			// pay table on the Hand.h file.
			for (auto it = sideBets.begin(); it != sideBets.end(); it++)
			{
				if ((*it)->getPayout() > 0)
				{
					cout << "You won the " << (*it)->getName() << "." << endl;
				}
				else if ((*it)->getPayout() < 0)
				{
					cout << "You lost the " << (*it)->getName() << "." << endl;

				}
			}
			totalWin += tripsPayout;
			totalWin += badBeatPayout;
			if (areSameHands)
			{
				// Blind Pushes, Ante Pushes, Play Pushes
				ultTexBlindBet.setBetResult(int(EBetResult::push));
				ultAnteBet.setBetResult(int(EBetResult::push));
				ultPlayBet.setBetResult(int(EBetResult::push));
				cout << "The hands are the same." << endl;
				myHand.printBest5Cards();
				cout << endl << endl;

			}
			else if (playerWins && dealerPlays)
			{
				// Blind Wins/Pushes, Ante Wins, Play Wins
				ultAnteBet.setBetResult(int(EBetResult::win));
				ultPlayBet.setBetResult(int(EBetResult::win));
				totalWin += payout + ante + blindPayout;
				cout << "You beat the dealer." << endl;
			}
			else if (!playerLoses)
			{
				// Blind Wins/Pushes, Ante Pushes, Play Wins
				ultAnteBet.setBetResult(int(EBetResult::push));
				ultPlayBet.setBetResult(int(EBetResult::win));
				totalWin += payout + blindPayout;
				cout << "You win, but the dealer's hand doesn't qualify for the ante." << endl;
			}
			else if (!dealerPlays)
			{
				// Blind Loses, Ante Pushes, Play Loses
				ultTexBlindBet.setBetResult(int(EBetResult::loss));
				ultAnteBet.setBetResult(int(EBetResult::push));
				ultPlayBet.setBetResult(int(EBetResult::loss));
				ultTexBlindBet.setPayout(ultTexBlindBet.getBetAmount() * -1);
				totalWin -= payout + blindBet;
				cout << "The dealer's hand is better, but doesn't qualify for the ante." << endl;
			}
			else
			{
				// Blind Loses, Ante Loses, Play Loses
				ultTexBlindBet.setBetResult(int(EBetResult::loss));
				ultAnteBet.setBetResult(int(EBetResult::loss));
				ultPlayBet.setBetResult(int(EBetResult::loss));
				ultTexBlindBet.setPayout(ultTexBlindBet.getBetAmount() * -1);
				totalWin -= payout + blindBet + ante;
				cout << "The dealer's hand is better than yours. Too bad." << endl;
			}
			ultAnteBet.calcPayout(ultAnteBet.getBetResult());
			ultPlayBet.calcPayout(ultPlayBet.getBetResult());
			cout << endl << endl;

			printBetResults(mainBets);
			printBetResults(sideBets);
			printTotalWin(totalWin);
			cout << endl;

			moneyIn += totalWin;
			displayBestHands(myHand, dealerHand, int(EGameType::ultHoldEm));
		}
		else
		{
			for (unsigned i = 0; i < COMMUNITY_CARDS; i++)
			{
				myHoleCards.push_back(board[i]);
			}
			myHand.setHandSize(HOLDEM_ALL_CARDS);
			myHand.setCards(myHoleCards);
			myHand.evaluateHand();

			if (ultBadBeatBet.getBetAmount() >= 0.01)
			{
				ultBadBeatBet.setBetResult(int(EBetResult::loss));
			}
			else
			{
				ultBadBeatBet.setBetResult(int(EBetResult::null));
			}

			ultTexBlindBet.setBetResult(int(EBetResult::loss));
			ultAnteBet.setBetResult(int(EBetResult::loss));
			ultPlayBet.setBetResult(int(EBetResult::null));
			ultTexBlindBet.setPayout(ultTexBlindBet.getBetAmount() * -1);
			ultAnteBet.calcPayout(ultAnteBet.getBetResult());
			ultPlayBet.calcPayout(ultPlayBet.getBetResult());
			ultTexTripsBet.calcPayout(myHand.getHandType());

			double tripsPayout = ultTexTripsBet.getPayout();
			if (tripsPayout >= 0.01)
			{
				ultTexTripsBet.setBetResult(int(EBetResult::win));
			}
			else if (tripsPayout < 0.00)
			{
				ultTexTripsBet.setBetResult(int(EBetResult::loss));
			}
			else
			{
				ultTexTripsBet.setBetResult(int(EBetResult::null));
			}

			double moneyLost = ante + blindBet;
			moneyIn -= moneyLost;

			cout << "You folded." << endl;
			ultAnteBet.printResult();
			cout << endl;
			ultTexBlindBet.printResult();
			cout << endl;

			printBetResults(sideBets);

			cout << "-----------------------------------" << endl;

			if (tripsPayout > 0)
			{
				cout << "You have " << myHand.getHandName(int(EGameType::ultHoldEm));
				double netWin = tripsPayout - moneyLost;
				moneyIn += tripsPayout;
				cout << "You win\t\t       $" << std::setw(ULT_PAYOUT_WIDTH) << showpoint
					<< setprecision(2) << fixed << netWin << endl;
				myHand.printBest5Cards();
				cout << endl;
				myHand.printHand();
				cout << endl << endl;
			}
			else
			{
				moneyIn += tripsPayout;
				moneyLost += abs(tripsPayout);

				cout << "You lose\t       $" << std::setw(ULT_PAYOUT_WIDTH) << showpoint
					<< setprecision(2) << fixed << moneyLost << endl;
			}
		}

		double totalInitialBets = 0;
		playAgain = validatePlayAgain(moneyIn, totalInitialBets);

	} while (tolower(playAgain) != 'n');
	return moneyIn;
}

double playUltimateOmaha(double moneyIn)
{
	char playAgain = 'y';
	do
	{
		if (moneyIn < 0.01 * ULT_NUM_BETS)
		{
			cout << "You do not have enough money to play." << endl;
			break;
		}

		vector<Card> myHoleCards(OMAHA_HOLE_CARDS);
		vector<Card> dealerHoleCards(OMAHA_HOLE_CARDS);
		vector<Card> board(COMMUNITY_CARDS);
		OmahaHand myHand = OmahaHand();
		OmahaHand dealerHand = OmahaHand();
		Hand boardHand = Hand(COMMUNITY_CARDS);
		FourCards myFourCards = FourCards();
		dealCards(myHoleCards, dealerHoleCards, board, int(EGameType::ultOmaha));

		myHand.setCards(myHoleCards);
		dealerHand.setCards(dealerHoleCards);
		boardHand.setCards(board);

		myFourCards.setCards(myHoleCards);

		// Betting
		UltTexBlindBet ultTexBlindBet = UltTexBlindBet();
		UltAnteBet ultAnteBet = UltAnteBet();
		//The player must make an equal bet on both the Ante and Blind... 
		double ante;
		double blindBet;
		double totalBet = 0;
		// ^ Used for validation against moneyIn
		// ^ They cannot bet what they don't have. No credit.

		ante = validatePreflopBet(ultTexBlindBet, moneyIn);
		totalBet += ante * ULT_NUM_BETS;
		blindBet = ante;
		ultAnteBet.setBetAmount(ante);
		ultTexBlindBet.setBetAmount(blindBet);


		UltTexTripsBet ultTexTripsBet = UltTexTripsBet();
		OmahaFourCardBet omahaFourCardBet = OmahaFourCardBet();
		UltTexBadBeatBet ultBadBeatBet = UltTexBadBeatBet();
		vector<SideBet*> sideBets{ &ultTexTripsBet, &omahaFourCardBet, &ultBadBeatBet };
		validateSideBets(sideBets, moneyIn, totalBet);

		// Reveal Player Hand
		cout << "Your Hole Cards: ";
		myHand.printHand();
		cout << endl << endl;

		// The player can check or make a Play bet equal to three or four times the Ante
		// if they can afford it, of course.
		char preflopCheckOrBet;
		int playBet = 0;
		UltPlayBet ultPlayBet = UltPlayBet();
		if ((PREFLOP_PLAY_MIN - 1) * blindBet + totalBet <= moneyIn)
		{
			preflopCheckOrBet = validatePreflopChoice();

			if (preflopCheckOrBet == 'b')
			{
				playBet = validatePlayBet(blindBet, totalBet, moneyIn);
				totalBet += (playBet - 1) * blindBet;
				ultPlayBet.setBetAmount(playBet * ante);
			}
		}
		else // Player has no money left
		{
			preflopCheckOrBet = 'c';
		}

		if (preflopCheckOrBet == 'b')
		{
			// Display Player Hand again
			cout << "Your Hole Cards: ";
			myHand.printHand();
			cout << endl << endl;
		}


		// The dealer turns over three community cards.

		// Reveal board cards
		cout << "Flop: ";
		boardHand.printFlop();
		cout << endl;


		char flopCheckOrBet;
		// If the player already made a Play bet, then he may not bet further.
		// If the player cannot afford the 2x bet, he cannot make it either.
		if (playBet || totalBet >= moneyIn)
		{
			flopCheckOrBet = 'c';
		}
		else
		{
			flopCheckOrBet = validateFlopChoice();
			if (flopCheckOrBet == 'b')
			{
				totalBet += (FLOP_PLAY_BET - 1) * blindBet;
				playBet = FLOP_PLAY_BET;
				ultPlayBet.setBetAmount(playBet * ante);
			}
		}

		printTurnAndRiver(boardHand);

		// If the player already raised he may not bet further.
		char riverChoice;
		if (playBet)
		{
			riverChoice = 'c';
		}
		else
		{
			riverChoice = validateRiverChoice();
			if (riverChoice == 'b')
			{
				playBet = 1;
				ultPlayBet.setBetAmount(playBet * ante);
			}
			else
			{
				// Subtract the 1x bet from their total bet if they fold.
				totalBet -= blindBet;
			}
		}
		cout << endl << endl;

		vector<MainBet*> mainBets{ &ultAnteBet, &ultTexBlindBet, &ultPlayBet };

		// The player and dealer will both make the best possible hand using any combination 
		// of their own two cards and the five community cards.
		if (riverChoice != 'f')
		{

			// Reveal Dealer Hole Cards
			cout << "Dealer Hole Cards: ";
			dealerHand.printHand();
			cout << endl;

			myHand.setHoleCards(myHoleCards);
			dealerHand.setHoleCards(dealerHoleCards);
			myHand.setBoardCards(board);
			dealerHand.setBoardCards(board);
			myHand.setIsBoardCard();
			dealerHand.setIsBoardCard();

			for (unsigned i = 0; i < COMMUNITY_CARDS; i++)
			{
				myHoleCards.push_back(board[i]);
				dealerHoleCards.push_back(board[i]);
			}
			myHand.setHandSize(OMAHA_ALL_CARDS);
			dealerHand.setHandSize(OMAHA_ALL_CARDS);
			myHand.setCards(myHoleCards);
			dealerHand.setCards(dealerHoleCards);
			//hand.printHand();
			cout << endl;
			//printHand(myHand);

			cout << endl;

			myHand.evaluateHand();
			dealerHand.evaluateHand();
			cout << "You have " << myHand.getHandName(int(EGameType::ultHoldEm)) << endl;
			cout << "The dealer has " << dealerHand.getHandName(int(EGameType::ultHoldEm)) << endl;

			ultTexBlindBet.calcPayout(myHand.getHandType());
			if (myHand.getHandType() > int(EHandType::straight))
			{
				ultTexBlindBet.setBetResult(int(EBetResult::push));
			}
			else
			{
				// Set Blind Bet result to win if handType is straight
				// or better. It will be set to push or loss later 
				// if dealer's hand is better or areSameHands.
				ultTexBlindBet.setBetResult(int(EBetResult::win));
			}
			bool dealerPlays;
			bool areSameHands = myHand == dealerHand;
			bool playerWins = myHand > dealerHand;
			bool playerLoses = myHand < dealerHand;

			double payout = blindBet * playBet * 1.0;
			array<double, OMAHA_SIDE_BETS> sideBetPayouts{ 0.0, 0.0, 0.0 };
			double totalWin = 0.0;

			double blindPayout = ultTexBlindBet.getPayout();

			for (unsigned i = 0; i < sideBets.size(); i++)
			{
				if (i == sideBets.size() - 1)
				{
					if (!areSameHands && myHand.getHandType() <= int(EHandType::threeOfaKind)
						&& dealerHand.getHandType() <= int(EHandType::threeOfaKind) &&
						sideBets[i]->getBetChoice() == 'y')
					{
						if (playerWins)
						{
							sideBets[i]->calcPayout(dealerHand.getHandType());
							sideBets[i]->setBetResult(int(EBetResult::win));
						}
						else
						{
							sideBets[i]->calcPayout(myHand.getHandType());
							sideBets[i]->setBetResult(int(EBetResult::win));
						}
						sideBetPayouts[i] = sideBets[i]->getPayout();
					}
					else if (sideBets[i]->getBetChoice() == 'y')
					{
						sideBets[i]->calcPayout(int(EHandType::noPair));
						sideBetPayouts[i] = sideBets[i]->getPayout() * 1.0;
						sideBets[i]->setBetResult(int(EBetResult::loss));
					}
				}
				else if (i == 1)
				{
					if (sideBets[i]->getBetChoice() == 'y')
					{
						myFourCards.evaluateHand();
						cout << "Your four card hand is " << myFourCards.getHandName() << ".";
						cout << endl;
						sideBets[i]->calcPayout(myFourCards.getHandType());
						sideBetPayouts[i] = sideBets[i]->getPayout();
					}
				}
				else
				{
					sideBets[i]->calcPayout(myHand.getHandType());
				}

				if (sideBets[i]->getBetChoice() == 'y' && i != sideBets.size() - 1)
				{
					sideBetPayouts[i] = sideBets[i]->getPayout();
				}

				if (sideBetPayouts[i] > 0)
				{
					cout << "You won the " << sideBets[i]->getName() << "." << endl;
					sideBets[i]->setBetResult(int(EBetResult::win));
				}
				else if (sideBetPayouts[i] < 0)
				{
					cout << "You lost the " << sideBets[i]->getName() << "." << endl;
					sideBets[i]->setBetResult(int(EBetResult::loss));
				}
				else
				{
					sideBets[i]->setBetResult(int(EBetResult::null));
				}
				totalWin += sideBetPayouts[i];
			}


			// In Ultimate Omaha, the dealer will need at least two pair 
			// to play for the ante because hand strengths are stronger.
			if (dealerHand.getHandType() > int(EHandType::twoPair))
			{
				dealerPlays = false;
			}
			else
			{
				dealerPlays = true;
			}
			// The Trips bet will pay according to the poker value of the player's hand 
			//regardless of the value of the dealer's hand, according to the trips bet 
			// pay table on the Hand.h file.

			if (areSameHands)
			{
				ultTexBlindBet.setBetResult(int(EBetResult::push));
				ultAnteBet.setBetResult(int(EBetResult::push));
				ultPlayBet.setBetResult(int(EBetResult::push));
				cout << "The hands are the same." << endl;
				myHand.printBest5Cards();
				cout << endl << endl;

			}
			else if (!playerLoses && dealerPlays)
			{
				ultAnteBet.setBetResult(int(EBetResult::win));
				ultPlayBet.setBetResult(int(EBetResult::win));
				totalWin += payout + ante + blindPayout;
				cout << "You beat the dealer." << endl;
			}
			else if (!playerLoses)
			{
				ultAnteBet.setBetResult(int(EBetResult::push));
				ultPlayBet.setBetResult(int(EBetResult::win));
				totalWin += payout + blindPayout;
				cout << "You win, but the dealer's hand doesn't qualify for the ante." << endl;
			}
			else if (!dealerPlays)
			{
				ultTexBlindBet.setBetResult(int(EBetResult::loss));
				ultAnteBet.setBetResult(int(EBetResult::push));
				ultPlayBet.setBetResult(int(EBetResult::loss));
				totalWin -= payout + blindBet;
				ultTexBlindBet.setPayout(ultTexBlindBet.getBetAmount() * -1);
				cout << "The dealer's hand is better, but doesn't qualify for the ante." << endl;
			}
			else
			{
				ultTexBlindBet.setBetResult(int(EBetResult::loss));
				ultAnteBet.setBetResult(int(EBetResult::loss));
				ultPlayBet.setBetResult(int(EBetResult::loss));
				totalWin -= payout + blindBet + ante;
				ultTexBlindBet.setPayout(ultTexBlindBet.getBetAmount() * -1);
				cout << "The dealer's hand is better than yours. Too bad." << endl;
			}

			ultAnteBet.calcPayout(ultAnteBet.getBetResult());
			ultPlayBet.calcPayout(ultPlayBet.getBetResult());

			cout << endl;

			printBetResults(mainBets);
			printBetResults(sideBets);
			printTotalWin(totalWin);

			moneyIn += totalWin;
			displayBestHands(myHand, dealerHand, int(EGameType::ultOmaha));
		}
		else
		{
			cout << "You folded" << endl;
			myHand.setHoleCards(myHoleCards);
			myHand.setBoardCards(board);
			myHand.setIsBoardCard();

			for (unsigned i = 0; i < COMMUNITY_CARDS; i++)
			{
				myHoleCards.push_back(board[i]);
			}

			// The Trips Bet and Four Card Bet will pay according to the poker value 
			// of the player's hand regardless of the value of the dealer's hand or 
			// if the player folds. The bet is paid according to the trips bet 
			// pay tables on the TripsBet.cpp and FourCardBet.cpp files.
			myHand.setHandSize(OMAHA_ALL_CARDS);
			myHand.setCards(myHoleCards);
			cout << endl;
			myHand.evaluateHand();
			cout << "You have " << myHand.getHandName(int(EGameType::ultHoldEm)) << endl;
			ultTexBlindBet.setBetResult(int(EBetResult::loss));
			ultTexBlindBet.calcPayout(myHand.getHandType());
			ultTexBlindBet.setPayout(ultTexBlindBet.getBetAmount() * -1.0);

			double payout = 0;
			array<double, OMAHA_SIDE_BETS> sideBetPayouts{ 0.0, 0.0, 0.0 };
			double totalWin = 0.0;

			double blindPayout = ultTexBlindBet.getPayout();

			for (unsigned i = 0; i < sideBets.size(); i++)
			{
				if (i == sideBets.size() - 1)
				{
					if (sideBets[i]->getBetChoice() == 'y')
					{
						sideBets[i]->calcPayout(int(EHandType::noPair));
						sideBetPayouts[i] = sideBets[i]->getPayout() * 1.0;
					}
				}
				else if (i == 1)
				{
					if (sideBets[i]->getBetChoice() == 'y')
					{
						myFourCards.evaluateHand();
						cout << "Your four card hand is " << myFourCards.getHandName() << ".";
						cout << endl;
						sideBets[i]->calcPayout(myFourCards.getHandType());
						sideBetPayouts[i] = sideBets[i]->getPayout() * 1.0;
					}

				}
				else
				{
					if (sideBets[i]->getBetChoice() == 'y')
					{
						sideBets[i]->calcPayout(myHand.getHandType());
						sideBetPayouts[i] = sideBets[i]->getPayout() * 1.0;
					}
				}

				if (sideBets[i]->getBetChoice() == 'y' && i != sideBets.size() - 1)
				{
					sideBetPayouts[i] = sideBets[i]->getPayout();
				}

				if (sideBetPayouts[i] > 0)
				{
					cout << "You won the " << sideBets[i]->getName() << "." << endl;
					sideBets[i]->setBetResult(int(EBetResult::win));
				}
				else if (sideBetPayouts[i] < 0)
				{
					cout << "You lost the " << sideBets[i]->getName() << "." << endl;
					sideBets[i]->setBetResult(int(EBetResult::loss));
				}
				else
				{
					sideBets[i]->setBetResult(int(EBetResult::null));
				}
				totalWin += sideBetPayouts[i];
			}


			ultTexBlindBet.setBetResult(int(EBetResult::loss));
			ultAnteBet.setBetResult(int(EBetResult::loss));
			ultPlayBet.setBetResult(int(EBetResult::null));
			totalWin -= payout + blindBet + ante;
			ultTexBlindBet.setPayout(ultTexBlindBet.getBetAmount() * -1);
			cout << "You folded." << endl;

			ultAnteBet.calcPayout(ultAnteBet.getBetResult());
			ultPlayBet.calcPayout(ultPlayBet.getBetResult());

			cout << endl;

			printBetResults(mainBets);
			printBetResults(sideBets);
			moneyIn += totalWin;
			printTotalWin(totalWin);
			cout << endl;
		}
		double totalInitialBets = 0;
		playAgain = validatePlayAgain(moneyIn, totalInitialBets);
	} while (tolower(playAgain) != 'n');

	return moneyIn;
}

int validatePlayBet(double blindBet, double totalBet, double moneyIn)
{
	int playBet = 0;
	do
	{
		cout << "How many times your blind do you want to bet?" << endl;
		cout << "You may bet 3 or 4 times your blind, but you cannot bet" << endl;
		cout << "any more afterwards." << endl;
		playBet = validate();

		if (playBet < PREFLOP_PLAY_MIN || playBet > PREFLOP_PLAY_MAX)
		{
			cout << "You cannot bet that amount. Please try again." << endl;
		}
		else if ((playBet - 1) * blindBet + totalBet > moneyIn)
		{
			cout << "You cannot afford that bet. Setting to ";
			cout << PREFLOP_PLAY_MIN << "x instead." << endl;
			playBet = PREFLOP_PLAY_MIN;
		}
	} while (playBet < PREFLOP_PLAY_MIN || playBet > PREFLOP_PLAY_MAX);
	return playBet;
}

char validatePreflopChoice()
{
	char preflopCheckOrBet;
	do
	{
		cout << "Would you like to check or bet 3-4x your blind?" << endl;
		cout << "Enter c to check or b to bet." << endl;
		cin >> preflopCheckOrBet;
		preflopCheckOrBet = tolower(preflopCheckOrBet);
		cin.ignore(numeric_limits<streamsize>::max(), '\n');

		if (preflopCheckOrBet != 'c' && preflopCheckOrBet != 'b')
		{
			cout << "I didn't understand. Please try again." << endl;
		}
	} while (preflopCheckOrBet != 'c' && preflopCheckOrBet != 'b');
	return preflopCheckOrBet;
}

char validateFlopChoice()
{
	//If the player previously checked, then he may make a Play bet equal 
	//to two times his Ante
	char flopCheckOrBet;
	do
	{
		cout << "Would you like to check or bet 2x your blind?" << endl;
		cout << "Enter c to check or b to bet." << endl;
		cin >> flopCheckOrBet;
		flopCheckOrBet = tolower(flopCheckOrBet);
		cin.ignore(numeric_limits<streamsize>::max(), '\n');

		if (flopCheckOrBet != 'c' && flopCheckOrBet != 'b')
		{
			cout << "I didn't understand. Please try again." << endl;
		}
	} while (flopCheckOrBet != 'c' && flopCheckOrBet != 'b');
	return flopCheckOrBet;
}

void dealCards(std::vector<Card>& myHoleCards, std::vector<Card>& dealerHoleCards, std::vector<Card>& board, int gameType)
{

	DeckOfCards deck = DeckOfCards();
	Card myCard;
	Card dealerCard;
	Card boardCard;


	deck.shuffleCards();
	deck.shuffleCards();
	deck.shuffleCards();
	deck.shuffleCards();
	deck.cutCards();
	cout << endl;
	deck.setCurrentCard(0);
	int cardsToDeal;
	switch (gameType)
	{
		case int(EGameType::ultHoldEm) :
			cardsToDeal = HOLDEM_HOLE_CARDS;
			break;

			case int(EGameType::ultOmaha) :
				cardsToDeal = OMAHA_HOLE_CARDS;
				break;

			default:
				throw invalid_argument("I don't know how many cards to deal.");
	}
	for (int i = 0; i < cardsToDeal; i++)
	{
		myCard = deck.dealCard();
		myHoleCards[i] = myCard;
		dealerCard = deck.dealCard();
		dealerHoleCards[i] = dealerCard;
	}
	for (int i = 0; i < COMMUNITY_CARDS; i++)
	{
		boardCard = deck.dealCard();
		board[i] = boardCard;
	}
}

double validatePreflopBet(UltTexBlindBet& ultTexBlindBet, double moneyIn)
{
	double ante = 0.00;
	do
	{
		askForAnteBlind();
		ante = validateDub();
		if (ante == 0.0)
		{
			ultTexBlindBet.printPayTable();
		}
		else if (ante * ULT_NUM_BETS > moneyIn)
		{
			double maxPreFlopBet = calcMaxPreFlopBet(moneyIn);
			cout << "You do not have enough money. Please bet $" <<
				showpoint << setprecision(2) << fixed << maxPreFlopBet << " or less." << endl;
		}
	} while (ante * ULT_NUM_BETS > moneyIn || ante == 0.0);
	return ante;
}





void printTurnAndRiver(Hand& boardHand)
{

	//Two final community cards are turned over.

	// Reveal board card
	cout << "Turn: ";
	boardHand.printTurn();
	cout << endl;

	// Reveal board card
	cout << "River: ";
	boardHand.printRiver();
	cout << endl;
	return;
}

char validateRiverChoice()
{
	char riverChoice;
	// If the player previously checked twice, then he must either make a Play bet 
	// equal to exactly his Ante, or fold, losing both his Ante and Blind bets.
	// The 1x bet is already included in the total bet for validation puropses.
	do
	{
		cout << "Would you like to bet 1x your blind or fold?" << endl;
		cout << "Enter b to bet or f to fold." << endl;
		cout << "If you fold, you lose all bets and your hand will not" << endl;
		cout << "be evaluated and the dealer's cards will not be revealed." << endl;
		cin >> riverChoice;
		riverChoice = tolower(riverChoice);
		cin.ignore(numeric_limits<streamsize>::max(), '\n');

		if (riverChoice != 'b' && riverChoice != 'f')
		{
			cout << "I didn't understand. Please try again." << endl;
		}
	} while (riverChoice != 'b' && riverChoice != 'f');
	return riverChoice;
}

void printTotalWin(double totalWin)
{
	cout << "-----------------------------------" << endl;
	if (totalWin > 0)
	{
		cout << "You win\t\t       $" << std::setw(ULT_PAYOUT_WIDTH) << showpoint
			<< setprecision(2) << fixed << totalWin << endl;
	}
	else if (totalWin < 0)
	{
		cout << "You lose\t       $" << std::setw(ULT_PAYOUT_WIDTH) << showpoint
			<< setprecision(2) << fixed << totalWin * -1 << endl;
	}
	else
	{
		cout << "You break even.\t $0.00" << endl;
	}
}



void displayBestHands(Hand& myHand, Hand& dealerHand, int gameType)
{
	cout << "Your best hand    : ";
	myHand.printBest5Cards();
	cout << endl;
	cout << "Dealer's best hand: ";
	dealerHand.printBest5Cards();
	cout << endl;
	cout << "Your hole cards    : ";
	if (gameType == int(EGameType::ultHoldEm))
	{
		myHand.printHoleCards(HOLDEM_HOLE_CARDS);
	}
	else if (gameType == int(EGameType::ultOmaha))
	{
		myHand.printHoleCards(OMAHA_HOLE_CARDS);
	}

	cout << endl;
	cout << "Dealer's hole cards: ";
	if (gameType == int(EGameType::ultHoldEm))
	{
		dealerHand.printHoleCards(HOLDEM_HOLE_CARDS);
	}
	else if (gameType == int(EGameType::ultOmaha))
	{
		dealerHand.printHoleCards(OMAHA_HOLE_CARDS);
	}
	cout << endl << endl;
	cout << "Board Cards: ";
	if (gameType == int(EGameType::ultHoldEm))
	{
		dealerHand.printBoard(HOLDEM_HOLE_CARDS);
	}
	else if (gameType == int(EGameType::ultOmaha))
	{
		dealerHand.printBoard(OMAHA_HOLE_CARDS);
	}
	cout << endl;
	cout << endl;
	return;
}

void validateSideBets(std::vector<SideBet*>& sideBets, double moneyIn, double& totalBet)
{
	for (auto it = sideBets.begin(); it != sideBets.end(); it++)
	{
		char sideBetChoice;
		double sideBetAmount = 0.0;
		if (moneyIn - totalBet >= .01)
		{

			do
			{
				cout << "Do you want to make a " << (*it)->getName() << "? Enter y for yes." << endl;
				cout << "Enter p to print the pay table: ";
				cin >> sideBetChoice;
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				;
				sideBetChoice = tolower(sideBetChoice);
				if (sideBetChoice == 'p')
				{
					(*it)->printPayTable();
				}
				if (sideBetChoice == 'y')
				{
					(*it)->setBetChoice(sideBetChoice);
					do
					{
						cout << "Enter your " << (*it)->getName() << "." << endl;
						cout << (*it)->getDescription();
						sideBetAmount = validateDub(.01);

						if (sideBetAmount + totalBet > moneyIn)
						{
							cout << "You cannot afford to make that bet. Please bet $"
								<< moneyIn - totalBet << " or less." << endl;
						}

					} while (sideBetAmount + totalBet > moneyIn);
					totalBet += sideBetAmount;
					(*it)->setBetAmount(sideBetAmount);
				}
				else
				{
					(*it)->setBetChoice('n');
					(*it)->setBetAmount(0);
				}
			} while (sideBetChoice == 'p');
		}
		else
		{
			(*it)->setBetChoice('n');
		}
	}
	return;
}

void askForAnteBlind()
{
	cout << "What is your ante and blind bet? The same value will be assigned" << endl;
	cout << "to both bets. Example: entering 5 will result in a bet of 5.00" << endl;
	cout << "on the ante and 5.00 on the blind for a total bet of 10.00." << endl;
	cout << "Enter 0 to print the blind pay table." << endl;
	return;
}

double calcMaxPreFlopBet(double moneyIn)
{
	return floor(moneyIn * CENTS_TO_DOLLARS / ULT_NUM_BETS) / CENTS_TO_DOLLARS;
}

void drawCards(Hand& hand, JacksOrBetterBet& jbBet, vector<Card>& myHand, DeckOfCards& myDeck)
{
	array<bool, DRAW_HAND_SIZE>draw{ 0,0,0,0,0, };
	string drawChoice;
	regex pattern("(h|d){5}");
	do
	{
		cout << "What cards do you want hold / draw?" << endl;
		cout << "Enter 5 characters, h to hold, and d to draw." << endl;
		cout << "Example: hhddd to hold the first two cards." << endl;
		cout << "Or enter 'p' to print the pay table." << endl;
		cin >> drawChoice;
		if (tolower(drawChoice[0]) == 'p')
		{
			//hand.printPayTable(stakes);
			jbBet.printPayTable();
			cout << endl << endl;
			hand.printHand();
			cout << endl;
			//printHand(myHand);
			cout << endl << endl;
		}
		else if (!regex_match(drawChoice, pattern))
		{
			cout << "Please reenter your draw choice." << endl;
		}
	} while (!regex_match(drawChoice, pattern));

	for (int i = 0; i < DRAW_HAND_SIZE; i++)
	{
		if (tolower(drawChoice[i]) == 'd')
		{
			draw[i] = true;
			myHand[i] = myDeck.dealCard();
		}
	}
	return;
}

template <class T> void printBetResults(vector<T*> bets)
{
	for (auto it = bets.begin(); it != bets.end(); it++)
	{
		if ((*it)->getBetResult() != int(EBetResult::null))
		{
			(*it)->printResult();
			cout << endl;
		}
	}
}

char validatePlayAgain(double moneyIn, double totalInitialBets)
{

	char playAgain;
	const char PLAY_CHOICE = 'y';
	const char SAME_BETS_CHOICE = 'b';
	const char QUIT_CHOICE = 'n';
	bool validChoice = false;
	bool playAgainWithSameBets = true;

	do {

		if (moneyIn >= 0.01 * ULT_NUM_BETS)
		{
			cout << "You have $" << showpoint << setprecision(2) << fixed << moneyIn << endl;
			cout << "Your last total initial bet was $" << showpoint << setprecision(2) << fixed << totalInitialBets << endl;
			cout << "Would you like to play again?" << endl;
			cout << "Enter y for yes, n for no, or b to play another hand with the same ante/blind and side bets" << endl;
			cin >> playAgain;
			playAgain = tolower(playAgain);
			if (playAgain == PLAY_CHOICE) {
				validChoice = true;
			}
			if (playAgain == QUIT_CHOICE) {
				validChoice = true;
			}
			else if (playAgain == SAME_BETS_CHOICE) {
				// not implemented yet
				validChoice = true;
			}
		}
		else
		{
			cout << "You ran out of money! Better luck next time." << endl;
			playAgain = QUIT_CHOICE;
			validChoice = true;
		}
		if (!validChoice && playAgainWithSameBets) {
			cout << "That is not a valid choice. Please try again." << endl;
		}
	} while (!validChoice);
	return playAgain;
}

void displayCardNames(DeckOfCards& displayDeck)
{
	Card displayCard;
	const int CARDS_PER_ROW = 4;
	for (int i = 0; i < DECK_SIZE; i++)
	{
		displayCard = displayDeck.getCard(i);
		displayCard.print();
		if (i % CARDS_PER_ROW == NUM_SUITS - 1)
		{
			cout << endl;
		}
	}
}

void displayCardSymbols(DeckOfCards& displayDeck)
{
	Card displayCard;
	const int CARDS_PER_ROW = 13;
	for (int i = 0; i < DECK_SIZE; i++)
	{
		displayCard = displayDeck.getCard(i);
		displayCard.printSymbol();
		if (i % CARDS_PER_ROW == NUM_RANKS - 1)
		{
			cout << endl;
		}
	}
}



int validate(int min, int max)
{
	/*
	/------------------------------------\
	|This function validates an int input|
	\------------------------------------/
	*/

	// Sentinel Value Constant
	const double NO_MIN = -927;

	// function variables

	// This will be returned at the end
	int value;

	// These are flags for min and max values
	bool minBool = false;
	bool maxBool = false;

	// Part One
	// Determine whether there is a min and/or a max value to accept
	// The default arguments are 0,0 which means minimum of zero and 
	// no maximum.
	if (min != 0 && min == max) {
		// No min or max
		cout << "Enter a number" << endl;
	}
	else if (min != NO_MIN && min < max) {
		// Yes min and Yes max
		minBool = true;
		maxBool = true;
		cout << "Enter a number " << min << " or higher and " << max << " or lower." << endl;
	}
	else if (min < max) {
		// No min Yes Max
		maxBool = true;
		cout << "Enter a number " << max << " or lower." << endl;
	}
	else {
		// Yes min No max
		minBool = true;
		cout << "Enter a number " << min << " or higher." << endl;
	}
	// Part Two
	// This is when the user actually enters the value and when it is 
	// tested to make sure it is valid.

	// This controls the do while loop
	bool notReady = true;

	do {
		// Enter the value
		cin >> value;
		// Test the value to make sure it is valid

		if (cin.fail()) {
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << "Please enter numbers only and try again." << endl;
		}
		else if ((minBool && maxBool) && (value < min || value > max)) {
			cout << "Please try again." << endl;
			cout << "Enter a number " << min << " or higher and " << max << " or lower." << endl;
		}
		else if ((minBool) && value < min) {
			cout << "Please try again." << endl;
			cout << "Enter a number " << min << " or higher." << endl;

		}
		else if ((maxBool) && value > max) {
			cout << "Please try again." << endl;
			cout << "Enter a number " << max << " or lower." << endl;

		}
		else {
			// Ignore any characters after the input is accepted
			// e.g decimal points, letters, etc.
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			notReady = false;
		}
	} while (notReady);
	// Once the value is ready, return it.
	return value;
}


double validateDub(double min, double max)
{
	/*
	/--------------------------------------\
	|This function validates a double input|
	\--------------------------------------/
	*/

	// Sentinel Value Constant
	const double NO_MIN = -927;

	// function variables

	// This will be returned at the end
	double value;

	// These are flags for min and max values
	bool minBool = false;
	bool maxBool = false;

	// Part One
	// Determine whether there is a min and/or a max value to accept
	// The default arguments are 0,0 which means minimum of zero and 
	// no maximum.
	if (min != 0 && min == max) {
		// No min or max
		cout << "Enter a number" << endl;
	}
	else if (min != NO_MIN && min < max) {
		// Yes min and Yes max
		minBool = true;
		maxBool = true;
		cout << "Enter a number " << min << " or higher and " << max << " or lower." << endl;
	}
	else if (min < max) {
		// No min Yes Max
		maxBool = true;
		cout << "Enter a number " << max << " or lower." << endl;
	}
	else {
		// Yes min No max
		minBool = true;
		cout << "Enter a number " << min << " or higher." << endl;
	}
	// Part Two
	// This is when the user actually enters the value and when it is 
	// tested to make sure it is valid.

	// This controls the do while loop
	bool notReady = true;

	do {
		// Enter the value
		cin >> value;
		// Test the value to make sure it is valid

		if (cin.fail()) {
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << "Please enter numbers only and try again." << endl;
		}
		else if ((minBool && maxBool) && (value < min || value > max)) {
			cout << "Please try again." << endl;
			cout << "Enter a number " << min << " or higher and " << max << " or lower." << endl;
		}
		else if ((minBool) && value < min) {
			cout << "Please try again." << endl;
			cout << "Enter a number " << min << " or higher." << endl;

		}
		else if ((maxBool) && value > max) {
			cout << "Please try again." << endl;
			cout << "Enter a number " << max << " or lower." << endl;

		}
		else {
			// Ignore any characters after the input is accepted
			// e.g decimal points, letters, etc.
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			notReady = false;
		}
	} while (notReady);
	// Once the value is ready, return it.
	return value;
}