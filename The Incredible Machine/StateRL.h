#pragma once

#include "stdafx.h"

/**
* @class StateRL
* @brief Encapsulates descrete state of the game for reinforcement learning agent
* 
* The state consists of
*  - wheelsStarted: which wheels (platforms) have been activated
*  - gearsStarted: which gears have been started
*  - ballsMoving: indicates weither balls are moving or not (if not - end of the game)
*  - targetHit: indicates if target is hit or not (if yes - end game and win)
*/
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
	//flags for each wheel activation
	std::vector<bool> wheelsStarted;
	//flags for each gear activation
	std::vector<bool> gearsStarted;
	//variant 1, tracking movement of each ball
	std::vector<bool> ballsMoving;
	//variant 2, tracking movement of all balls generally
	bool allBallsMoving;
	//flag which indicates if target has been hit
	bool targetHit;
public:
	/*
	* OLD MODEL
	bool operator==(const StateRL& state)const {
		return this->gears == state.gears && this->belts == state.belts && this->wheelStarted == state.wheelStarted && this->ballMoving == state.ballMoving;
	}
	*/
	
	/// <summary>
	/// Constructs initial state:
	/// no wheel started
	/// no gear started
	/// balls moving
	/// target not hit
	/// </summary>
	StateRL();

	/// <summary>
	/// Info if specific wheel is started
	/// </summary>
	/// <param name="wheelId">Id of wheel in question</param>
	/// <returns>True if started false otherwise</returns>
	bool getWheelStarted(int wheelId)const;
	/// <summary>
	/// Info if specific gear is started
	/// </summary>
	/// <param name="wheelId">Id of gear in question</param>
	/// <returns>True if started false otherwise</returns>
	bool getGearStarted(int gearId)const;
	/// <summary>
	/// VARIANT 1
	/// Info if specific ball is moving
	/// </summary>
	/// <param name="wheelId">Id of ball in question</param>
	/// <returns>True if moving false otherwise</returns>
	bool getBallMoving(int ballId)const;
	/// <summary>
	/// VARIANT 2
	/// Info if any ball is moving
	/// </summary>
	/// <returns>True if any ball is moving false otherwise</returns>
	bool getBallMoving()const;
	/// <summary>
	/// VARIANT 2
	/// Info if any ball is moving
	/// </summary>
	/// <returns>True if any ball is moving false otherwise</returns>
	bool getAllBallsMoving()const;
	/// <summary>
	/// Returns info if the target has been hit
	/// </summary>
	/// <returns>True if hit, false otherwise</returns>
	bool getTargetHit()const;
	/// <summary>
	/// Encodes entire state space in single id, used for navigation in QTable
	/// </summary>
	/// <returns>ID of state</returns>
	int getStateId() const;

	/// <summary>
	/// Marks state of specific wheel
	/// </summary>
	/// <param name="wheelId">id of wheel in question</param>
	/// <param name="started">moving/not moving</param>
	void setWheelStarted(int wheelId,bool started);
	/// <summary>
	/// Marks state of specific gear
	/// </summary>
	/// <param name="wheelId">id of gear in question</param>
	/// <param name="started">moving/not moving</param>
	void setGearStarted(int gearId,bool started);
	/// <summary>
	/// VARIANT 1
	/// Marks state of specific ball
	/// </summary>
	/// <param name="wheelId">id of ball in question</param>
	/// <param name="started">moving/not moving</param>
	void setBallMoving(int ballId,bool moving);
	/// <summary>
	/// VARIANT 2
	/// Marks state of balls if they are moving
	/// </summary>
	/// <param name="moving">moving/not moving</param>
	void setBallMoving(bool moving);
	/// <summary>
	/// Sets indication if target has been hit
	/// </summary>
	/// <param name="hit">hit/not hit</param>
	void setTargetHit(bool hit);
	/// <summary>
	/// Resets to initial state
	/// </summary>
	void setInitialState();
};

