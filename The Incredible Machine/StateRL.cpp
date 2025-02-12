#include "stdafx.h"
#include "StateRL.h"

bool StateRL::getWheelStarted(int wheelId) const
{
    return this->wheelsStarted[wheelId];
}

bool StateRL::getGearStarted(int gearId) const
{
    return this->gearsStarted[gearId];
}

bool StateRL::getBallMoving(int ballId) const
{
    return this->ballsMoving[ballId];
}

void StateRL::setWheelStarted(int wheelId,bool started)
{
    this->wheelsStarted[wheelId] = started;
}

void StateRL::setGearStarted(int gearId,bool started)
{
    this->gearsStarted[gearId] = started;
}

void StateRL::setBallMoving(int ballId,bool moving)
{
    this->ballsMoving[ballId] = moving;
}
