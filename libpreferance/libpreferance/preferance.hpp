#pragma once

#include <array>
#include <bitset>
#include <numeric>
#include <functional>
#include <algorithm>

enum class PlayerBid {
	MISERE = 0,
	SIX = 6,
	SEVEN = 7,
	EIGHT = 8,
	NINE = 9,
	TEN = 10,
	PASS = 16,
};

enum class DefenderBid {
	PASS = 16,
	WHIST = 17,
};

struct RoundResult {
	PlayerBid player;
	std::array<DefenderBid, 2> defenders;
	std::array<int, 3> tricks;
};

struct PlayerScore {
	int mount;
	int pool;
	std::array<int, 4> whists;
};

using Score = std::array<PlayerScore, 4>;

struct PlayerScoreUpdate {
	int mount;
	int pool;
	int whists;
};

using ScoreUpdate = std::array<PlayerScoreUpdate, 3>;


ScoreUpdate updateScore(const RoundResult& roundResult) {
	ScoreUpdate update{};

	if (roundResult.player == PlayerBid::PASS) {
		for (int i = 0; i < 3; ++i) {
			update[i].mount = 2 * roundResult.tricks[i];
		}
		return update;
	}

	if (roundResult.player == PlayerBid::MISERE) {
		int weight = 10;
		int bad = roundResult.tricks[0];
		if (bad == 0) {
			update[0].pool = weight;
		}
		else {
			update[0].mount = weight * bad;
		}

		return update;
	}

	const int playerBid = static_cast<int>(roundResult.player);
	const int weight = 2 * playerBid - 10;
	const int whistTricks = (10 - roundResult.tricks[0]);
	if (roundResult.tricks[0] >= playerBid) {
		static const int requiredTricksConfig[5] = { 4, 2, 1, 1, 1 };
		const int requiredTricks = requiredTricksConfig[playerBid - 6];
		update[0].pool = weight;
		if (roundResult.defenders[0] == DefenderBid::WHIST && roundResult.defenders[1] == DefenderBid::WHIST) {
			update[1].whists = weight * roundResult.tricks[1];
			update[2].whists = weight * roundResult.tricks[2];
			if (whistTricks < requiredTricks) {
				if (roundResult.tricks[1] < requiredTricks / 2) {
					update[1].mount = weight * (requiredTricks / 2 - roundResult.tricks[1]);
				}
				if (roundResult.tricks[2] < (requiredTricks+1) / 2) {
					update[2].mount = weight * ((requiredTricks+1) / 2 - roundResult.tricks[2]);
				}
			}
		}
		else if (roundResult.defenders[0] == DefenderBid::WHIST) {
			update[1].whists = weight * whistTricks;
			if (whistTricks < requiredTricks) {
				update[1].mount = (requiredTricks - whistTricks) * weight;
			}
		}
		else if (roundResult.defenders[1] == DefenderBid::WHIST) {
			update[2].whists = weight * (10 - roundResult.tricks[0]);
			if (whistTricks < requiredTricks) {
				update[2].mount = (requiredTricks - whistTricks) * weight;
			}
		}
	}
	else {
		int bad = playerBid - roundResult.tricks[0];
		update[0].mount = weight * bad;
		if (roundResult.defenders[0] == DefenderBid::WHIST && roundResult.defenders[1] == DefenderBid::WHIST) {
			update[1].whists = weight * (roundResult.tricks[1] + bad);
			update[2].whists = weight * (roundResult.tricks[2] + bad);
		}
		else if (roundResult.defenders[0] == DefenderBid::WHIST) {
			update[1].whists = weight * (whistTricks + bad);
			update[2].whists = weight * bad;
		}
		else if (roundResult.defenders[1] == DefenderBid::WHIST) {
			update[1].whists = weight * bad;
			update[2].whists = weight * (whistTricks + bad);
		}
	}
	return update;
}

using RoundScore = std::array<int, 3>;
RoundScore roundScore(const ScoreUpdate& update, int nPlayers) {
	return {
		((update[0].pool - update[0].mount) * 10 - update[1].whists - update[2].whists) * (nPlayers - 1) + (update[1].mount - update[1].pool + update[2].mount - update[2].pool) * 10,
		((update[1].pool - update[1].mount) * 10 + update[1].whists) * (nPlayers - 1) + (update[0].mount - update[0].pool + update[2].mount - update[2].pool) * 10,
		((update[2].pool - update[2].mount) * 10 + update[2].whists) * (nPlayers - 1) + (update[0].mount - update[0].pool + update[1].mount - update[1].pool) * 10,
	};
}

using Card = std::size_t;

using Round = std::array<Card, 3>;

using CardSet = std::bitset<32>;

CardSet operator "" _spades(const char* s, size_t l) {
	CardSet result{};
	for (std::size_t i = 0; i < l; ++i) {
		switch (s[i]) {
		case 'A':
			result.set(7);
			break;
		case 'K':
			result.set(6);
			break;
		case 'Q':
			result.set(5);
			break;
		case 'J':
			result.set(4);
			break;
		case '0':
			result.set(3);
			break;
		default:
			result.set(s[i] - '7');
		}
	}
	return result;
}
CardSet operator "" _clubs(const char* s, size_t l) {
	return operator "" _spades(s, l) << 8;
}
CardSet operator "" _diamonds(const char* s, size_t l) {
	return operator "" _spades(s, l) << 16;
}
CardSet operator "" _hearts(const char* s, size_t l) {
	return operator "" _spades(s, l) << 24;
}

CardSet cardSuit(const Card& card) {
	return CardSet(255) << (card & 24);
}

Card firstCard(const CardSet& cards) {
	return std::countr_zero(cards.to_ulong());
}

Card nextCard(const Card& card, const CardSet& cards) {
	return firstCard(cards >> (card + 1)) + card + 1;
}

CardSet legalPlays(const CardSet& suit, const CardSet& trumps, const CardSet& hand) {
	CardSet result = hand & suit;
	if (result.any()) {
		return result;
	}
	result = hand & trumps;
	if (result.any()) {
		return result;
	}
	return hand;
}

RoundScore findPlayFirst(std::array<CardSet, 3> hands, const RoundResult& bids, const CardSet& trumps, int player, std::function<RoundScore(RoundResult)> scoreFunc, std::array<Round, 10>& play, std::array<int, 3>& tricks, int round);
RoundScore findPlaySecond(std::array<CardSet, 3> hands, const RoundResult& bids, const CardSet& trumps, int player, std::function<RoundScore(RoundResult)> scoreFunc, std::array<Round, 10>& play, std::array<int, 3>& tricks, int round, Card initCard);
RoundScore findPlayThird(std::array<CardSet, 3> hands, const RoundResult& bids, const CardSet& trumps, int player, std::function<RoundScore(RoundResult)> scoreFunc, std::array<Round, 10>& play, std::array<int, 3>& tricks, int round, Card initCard);

std::array<Round, 10> findPlay(std::array<CardSet, 3> hands, const RoundResult& bids, const CardSet& trumps, int player, std::function<RoundScore(RoundResult)> scoreFunc) {
	std::array<Round, 10> play;
	std::array<int, 3> tricks = {};
	auto score = findPlayFirst(hands, bids, trumps, player, scoreFunc, play, tricks, 0);
	return play;
}

int cmpCards(const Card a, const Card b, const CardSet& trumps) {
	if ((a & 24) == (b & 24)) {
		return a - b;
	}
	if (cardSuit(b) == trumps) {
		return -1;
	}
	return 1;
}

int findWinner(const std::array<Card, 3>& play, const CardSet& trumps, int firstPlayer) {
	return std::max_element(play.begin(), play.end(), [&trumps](Card a, Card b) { return cmpCards(a, b, trumps) < 1; }) - play.begin();
}

RoundScore findPlayFirst(std::array<CardSet, 3> hands, const RoundResult& bids, const CardSet& trumps, int player, std::function<RoundScore(RoundResult)> scoreFunc, std::array<Round, 10>& play, std::array<int, 3>& tricks, int round) {
	if (round >= 10) {
		return scoreFunc({ bids.player, bids.defenders, tricks });
	}
	
	CardSet legal = hands[player];
	Card bestCard = 0;
	RoundScore bestScore = { -10000, -10000, -10000 };
	for (Card card = firstCard(legal); card < 32; card = nextCard(card, legal)) {
		play[round][player] = card;
		hands[player].reset(card);
		auto score = findPlaySecond(hands, bids, trumps, (player + 1) % 3, scoreFunc, play, tricks, round, card);
		hands[player].set(card);
		if (score[player] > bestScore[player]) {
			bestCard = card;
			bestScore = score;
		}
	}
	play[round][player] = bestCard;
	return bestScore;
}

RoundScore findPlaySecond(std::array<CardSet, 3> hands, const RoundResult& bids, const CardSet& trumps, int player, std::function<RoundScore(RoundResult)> scoreFunc, std::array<Round, 10>& play, std::array<int, 3>& tricks, int round, Card initCard) {
	CardSet legal = legalPlays(cardSuit(initCard), trumps, hands[player]);
	Card bestCard = 0;
	RoundScore bestScore = { -10000, -10000, -10000 };
	for (Card card = firstCard(legal); card < 32; card = nextCard(card, legal)) {
		play[round][player] = card;
		hands[player].reset(card);
		auto score = findPlayThird(hands, bids, trumps, (player + 1) % 3, scoreFunc, play, tricks, round, initCard);
		hands[player].set(card);
		if (score[player] > bestScore[player]) {
			bestCard = card;
			bestScore = score;
		}
	}
	play[round][player] = bestCard;
	return bestScore;
}

RoundScore findPlayThird(std::array<CardSet, 3> hands, const RoundResult& bids, const CardSet& trumps, int player, std::function<RoundScore(RoundResult)> scoreFunc, std::array<Round, 10>& play, std::array<int, 3>& tricks, int round, Card initCard) {
	CardSet legal = legalPlays(cardSuit(initCard), trumps, hands[player]);
	Card bestCard = 0;
	RoundScore bestScore = { -10000, -10000, -10000 };
	for (Card card = firstCard(legal); card < 32; card = nextCard(card, legal)) {
		play[round][player] = card;
		int nextPlayer = findWinner(play[round], trumps, (player + 1) % 3);
		tricks[nextPlayer] += 1;
		hands[player].reset(card);
		auto score = findPlayFirst(hands, bids, trumps, nextPlayer, scoreFunc, play, tricks, round+1);
		hands[player].set(card);
		tricks[nextPlayer] -= 1;
		if (score[player] > bestScore[player]) {
			bestCard = card;
			bestScore = score;
		}
	}
	play[round][player] = bestCard;
	return bestScore;
}
