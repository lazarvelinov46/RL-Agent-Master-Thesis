#pragma once

#include "stdafx.h"

class StateRL
{
private:
	/*
	* OLD MODEL
	std::vector<std::pair<int, int>> gears;
	std::vector<std::pair<std::pair<int, int>, std::pair<int, int>>> belts;
	std::vector<bool> wheelStarted;
	std::vector<bool> ballMoving;
	*/
	std::vector<bool> wheelsStarted;
	std::vector<bool> gearsStarted;
	std::vector<bool> ballsMoving;
	bool targetHit;
public:
	/*
	* OLD MODEL
	bool operator==(const StateRL& state)const {
		return this->gears == state.gears && this->belts == state.belts && this->wheelStarted == state.wheelStarted && this->ballMoving == state.ballMoving;
	}
	*/
	StateRL();

	bool getWheelStarted(int wheelId)const;
	bool getGearStarted(int gearId)const;
	bool getBallMoving(int ballId)const;
	int getStateId() const;

	void setWheelStarted(int wheelId,bool started);
	void setGearStarted(int gearId,bool started);
	void setBallMoving(int ballId,bool moving);

};

