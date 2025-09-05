#pragma once

typedef struct BeltActionInfo {
	bool isElementGear;
	int idElement;
}BeltActionInfo;


/// <summary>
/// Enumerates descrete actions which agent could take
/// </summary>
enum AgentAction { PLACE_GEAR, PLACE_BELT, number, NO_ACTION};
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
	const static int GRID_WIDTH = 19;
	const static int GRID_HEIGHT = 14;
	const static int GRID_WIDTH_BOX = 9;
	const static int GRID_HEIGHT_BOX = 7;
	int numberOfWheels;
	int numberOfGears;
public:
	ActionRL(int numberOfWheels, int numberOfGears);
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


	/// <summary>
	/// Gets action type based on action id from QTable
	/// </summary>
	/// <param name="actionId">Id of an action gathered from QTable</param>
	/// <returns>AgentAction enum value which represents the type of action</returns>
	AgentAction getActionType(int actionId);
	/// <summary>
	/// Gets gear coordinates based on action id that represents gear placemenet
	/// </summary>
	/// <param name="gearActionId">Id from QTable that refers to gear placement action</param>
	/// <returns>pair of integers that represent x and y coordinates</returns>
	std::pair<int,int> getGearCoordinates(int gearActionId);
	/// <summary>
	/// Gets belt placement info based on action id that represents belt placement
	/// </summary>
	/// <param name="beltActionId">Id from QTable that refers to belt placement action</param>
	/// <returns>
	/// pair of BeltActionInfo structures which contains both end's element type and id
	/// </returns>
	std::pair<BeltActionInfo, BeltActionInfo> getBeltPlacement(int beltActionId);
	/// <summary>
	/// helper function that calculates mathematical combination C(n,r)
	/// </summary>
	/// <param name="n">Total elements in set</param>
	/// <param name="r">Number of elements to choose from (2 in this case)</param>
	/// <returns>number of possible combinations</returns>
	static long long combination(int n, int r);
	static int getGridWidth() { return ActionRL::GRID_WIDTH; }
	static int getGridHeight() { return ActionRL::GRID_HEIGHT; }
	static int getGridWidthBox() { return ActionRL::GRID_WIDTH_BOX; }
	static int getGridHeightBox() { return ActionRL::GRID_HEIGHT_BOX; }
};

