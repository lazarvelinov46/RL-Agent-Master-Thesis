#pragma once

#include "stdafx.h"

class QTable
{
private:
	std::unordered_map<int, std::unordered_map<int, double>> values;
	double alpha;
	double gamma;
public:
	QTable() {
		this->alpha = 0.1;
		this->gamma = 0.9;
	}
	QTable(int numberOfStates, int numberOfActions) {
		for (int i = 0;i < numberOfStates;i++) {
			for (int j = 0;j < numberOfActions;j++) {
				this->values[i][j] = 0.0;
			}
		}
		this->alpha = 0.1;
		this->gamma = 0.9;
	}

	double getQValue(int stateId, int actionId)const {
		if (this->values.count(stateId) && this->values.at(stateId).count(actionId)) {
			return this->values.at(stateId).at(actionId);
		}
		return 0.0;
	}
};

