#pragma once
#include "stdafx.h"


/// <summary>
/// One transition tuple which consists of:
///		- state
///		- action
///		- reward
///		- next state
///		- done (boolean)
/// </summary>
struct DQNTransition {
	std::vector<float> state;
	int action;
	float reward;
	std::vector<float> nextState;
	bool done;
};