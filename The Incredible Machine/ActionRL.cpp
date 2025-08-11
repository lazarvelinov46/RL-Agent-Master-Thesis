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
    int crossCombinations = Level::getNumberOfGearsStatic() * Level::getNumberOfBeltsStatic();
    int wheelCombinations = ActionRL::combination(Level::getNumberOfWheels(), 2);
    if (beltActionId < crossCombinations) {
        beggining.isElementGear = true;
        beggining.idElement = beltActionId / Level::getNumberOfGearsStatic();
        end.isElementGear = false;
        end.idElement = beltActionId % Level::getNumberOfBeltsStatic();
    }
    /*
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
    */
    else {
        //TODO: check this
        int idBeggining = -1;
        int idEnd = -1;
        //int startWheels = crossCombinations+gearCombinations;
        int startWheels = crossCombinations;
        int wheelNumber = beltActionId - startWheels;
        int count = 0;
        for (int i = 0; i < Level::getNumberOfWheels(); i++) {
            for (int j = i + 1; j < Level::getNumberOfWheels(); j++) {
                if (count == wheelNumber) {
                    idBeggining = i;
                    idEnd = j;
                    count++;
                    break;
                }
                count++;
            }
        }
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
