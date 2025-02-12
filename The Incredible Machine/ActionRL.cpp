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
    //TO DO
    return ActionRL::GRID_WIDTH*ActionRL::GRID_HEIGHT+(endId*5-startId*5+endId-startId);
}
