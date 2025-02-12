#pragma once
class ActionRL
{
private:
	/*
	bool gear;
	std::pair<int, int> gearPosition;
	std::pair<std::pair<int, int>, std::pair<int, int>> beltPositions;
	*/
	std::vector<std::pair<int, int>> gearPlaced;
	std::vector<std::pair<int, int>> beltPlaced;
	const static int GRID_WIDTH=18;
	const static int GRID_HEIGHT=13;
public:
	/*
	bool operator==(const ActionRL& action)const {
		return this->gear == action.gear && this->gearPosition == action.gearPosition && this->beltPositions == action.beltPositions;
	}
	*/
	std::pair<int, int> getGearPlaced(int gearId)const;
	std::pair<int, int> getBeltPlaced(int beltId)const;

	void setGearPlaced(int gearId, int x, int y);
	void setBeltPlaced(int beltId, int startId, int endId);

	int getActionIdGear(int gearId, int x, int y);
	int getActionIdBelt(int beltId, int startId, int endId);
};

