#pragma once

#include <array>


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
	bool operator==(const PlayerScoreUpdate&) const = default;
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
