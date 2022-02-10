#pragma once
#include <iostream>
#include <string>
#include <array>
#include <random>
#include <algorithm>
//using namespace std;

enum class ERankNames
{
	two,
	three,
	four,
	five,
	six,
	seven,
	eight,
	nine,
	ten,
	jack,
	queen,
	king,
	ace
};

enum class ESuitNames
{
	clubs,
	diamonds,
	hearts, 
	spades
};

// ASCII symbol codes
enum class ESuitSymbols
{
	hearts = 3,
	diamonds,
	clubs,
	spades
};

const static int NUM_RANKS = 13;
const static int NUM_SUITS = 4;

const std::array<std::string, NUM_RANKS> RANK_NAMES{ "Two", "Three", "Four", "Five", "Six",
"Seven", "Eight", "Nine", "Ten", "Jack", "Queen", "King", "Ace" };

const std::array<std::string, NUM_SUITS> SUIT_NAMES{ "Clubs", "Diamonds", "Hearts", "Spades" };

class Card
{
public:
	Card();
	Card(int);
	Card(int, int);
	~Card();
	
	
	void setId(int id);
	void setRankId(int num);
	void setSuitId(int num);
	void setRank(int rank);
	void setSuit(int suit);
	void setRankName(std::string rankName);
	void setSuitName(std::string suitName);
	int getId();
	char getRank();
	char getSuit();
	int getRankId() const;
	int getSuitId() const;
	std::string getRankName();
	std::string getSuitName();
	void print();
	void printSymbol();
	bool operator<(Card) const;
	bool operator>(Card) const;
	bool operator==(Card) const;
	static bool isLowerSuit(const Card &, const Card &);
	static bool isHigherSuit(Card, Card);

private:
	int id;
	int rankId;
	int suitId;
	char rank;
	char suit;
	std::string rankName;
	std::string suitName;
	static std::array<char, NUM_RANKS> ranks;
	static std::array<char, NUM_SUITS> suits;
	static std::array<std::string, NUM_RANKS> rankNames;
	static std::array<std::string, NUM_SUITS> suitNames;
};

