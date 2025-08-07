#include "stdafx.h"
#include "ActionRL.h"

std::pair<int, int> ActionRL::getGearPlaced(int gearId) const
{
    return this->gearPlaced[gearId];
}

std::pair<int, int> ActionRL::getBeltPlaced(int beltId) const
{
    return this->beltPlaced[beltId];
}

void ActionRL::setGearPlaced(int gearId, int x, int y)
{
    this->gearPlaced[gearId] = { x,y };
}

void ActionRL::setBeltPlaced(int beltId, int startId, int endId)
{
    this->beltPlaced[beltId] = { startId,endId };
}

int ActionRL::getActionIdGear(int gearId, int x, int y)
{
    return (y/50)*ActionRL::GRID_WIDTH+x/50;
}

int ActionRL::getActionIdBelt(int beltId, int startId, int endId)
{
    //TODO:
    return ActionRL::GRID_WIDTH*ActionRL::GRID_HEIGHT+(endId*5-startId*5+endId-startId);
}

AgentAction ActionRL::getActionType(int actionId)
{
    if (actionId == -1) {
        return AgentAction::NO_ACTION;
    }
    if (actionId < ActionRL::GRID_WIDTH * ActionRL::GRID_HEIGHT) {
        return AgentAction::PLACE_GEAR;
    }
    else {
        return AgentAction::PLACE_BELT;
    }
}

std::pair<int, int> ActionRL::getGearCoordinates(int gearActionId)
{
    int x = gearActionId % ActionRL::GRID_WIDTH;
    int y = gearActionId / ActionRL::GRID_HEIGHT;
    return std::pair<int, int>(x, y);
}

std::pair<BeltActionInfo, BeltActionInfo> ActionRL::getBeltPlacement(int beltActionId)
{
    beltActionId -= ActionRL::GRID_WIDTH * ActionRL::GRID_HEIGHT;
    BeltActionInfo beggining;
    BeltActionInfo end;
    int crossCombinations = ActionRL::NO_OF_BELTS * ActionRL::NO_OF_GEARS;
    int gearCombinations = this->combination(ActionRL::NO_OF_GEARS, 2);
    int wheelCombinations = this->combination(ActionRL::NO_OF_WHEELS, 2);
    if (beltActionId < crossCombinations) {
        beggining.isElementGear = true;
        beggining.idElement = crossCombinations / ActionRL::NO_OF_GEARS;
        end.isElementGear = false;
        end.idElement = crossCombinations % ActionRL::NO_OF_BELTS;
    }
    else if(beltActionId<gearCombinations+crossCombinations){
        //TODO: check this
        int idBeggining = 1;
        int startGears = crossCombinations;
        int gearNumber = beltActionId - crossCombinations;
        while ((gearNumber - (ActionRL::NO_OF_GEARS - idBeggining)) >= 0) {
            gearNumber -= (ActionRL::NO_OF_GEARS - idBeggining);
            idBeggining++;
        }
        int idEnd = idBeggining + gearNumber;
        beggining.isElementGear = true;
        beggining.idElement = idBeggining;
        end.isElementGear = true;
        end.idElement = idEnd;
    }
    else {
        //TODO: check this
        int idBeggining = 1;
        int startWheels = crossCombinations+gearCombinations;
        int wheelNumber = beltActionId - startWheels;
        while ((wheelNumber - (ActionRL::NO_OF_WHEELS - idBeggining)) >= 0) {
            wheelNumber -= (ActionRL::NO_OF_WHEELS - idBeggining);
            idBeggining++;
        }
        int idEnd = idBeggining + wheelNumber;
        beggining.isElementGear = false;
        beggining.idElement = idBeggining;
        end.isElementGear = false;
        end.idElement = idEnd;
    }
    return std::pair<BeltActionInfo, BeltActionInfo>(beggining,end);
}

long long ActionRL::combination(int n, int r)
{
    if (r > n)return 0;
    long long retVal = 1;
    for (int i = 1; i <= r; i++) {
        retVal = retVal * (n - r + i) / i;
    }
    return retVal;
}
