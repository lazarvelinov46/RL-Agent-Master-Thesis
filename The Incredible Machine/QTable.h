#pragma once

#include "stdafx.h"
#include "State.h"

class QTable
{
private:
	static float WRONG_GEAR_PLACEMENT;
	static float WRONG_BELT_PLACEMENT;
	static float LOST_GAME_BASE;
	static float GEAR_ACTIVATED;
	static float WHEEL_ACTIVATED;
	static float WON_GAME;
	std::vector<std::vector<double>> values;
	std::vector<std::vector<int>> visits;
	std::vector<int> validActions;
	double alpha;
	double gamma;
	int numStates;
	int numActions;
	std::string filename;

	std::mt19937 rng;
public:
	QTable();
	QTable(int numberOfStates, int numberOfActions, std::string filename,LevelDifficulty dfficulty);
	

	double getQValue(int stateId, int actionId)const;

	void updateQValue(int stateId, int actionId, double reward, int nextStateId, bool nextIsTerminal=false);

	int getAction(int stateId,double epsilon,std::unordered_set<int> forbiddenActions);

	void printTable(const std::string& filename,int iteration);

	void saveQTableCSV(const std::string& filename,int iteration,double alpha,double epsilon);

	void setAlpha(double alpha) { this->alpha = alpha; }
	void setGamma(double gamma) { this->gamma = gamma; }
	void seedRng(unsigned int seed) { this->rng.seed(seed); }
	void updateValidActions(int gearsPlaced, int beltsPlaced);

	std::pair<double, double> getQStats()const;

	int getUniqueVisitsCount() const;

	bool loadQTableCSV(std::istream& in);

	static float GetWrongGearPlacementReward();
	static float GetWrongBeltPlacementReward();
	static float GetLostGameBaseReward();
	static float GetGearActivatedReward();
	static float GetWheelActivatedReward();
	static float GetWonGameReward();
};

