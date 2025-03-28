#include "stdafx.h"
#include "StateRL.h"

StateRL::StateRL()
{
    this->ballsMoving.push_back(true);
    for (int i = 0;i < 3;i++) {
        this->ballsMoving.push_back(false);
    }
    for (int i = 0;i < 3;i++) {
        this->gearsStarted.push_back(false);
    }
    for (int i = 0;i < 3;i++) {
        this->wheelsStarted.push_back(false);
    }
    this->targetHit = false;
}

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

bool StateRL::getTargetHit() const
{
    return this->targetHit;
}

int StateRL::getStateId() const
{
    int id = 0;
    int scnt = 0;
    for (int i = 0;i < this->ballsMoving.size();i++) {
        id |= this->ballsMoving[i] ? (1 << scnt++) : 0;
    }
    for (int i = 0;i < this->gearsStarted.size();i++) {
        id |= this->gearsStarted[i] ? (1 << scnt++) : 0;
    }
    for (int i = 0;i < this->wheelsStarted.size();i++) {
        id |= this->wheelsStarted[i] ? (1<<scnt++) : 0;
    }
    id |= this->targetHit ? (1 << scnt++) : 0;
    return id;
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

void StateRL::setTargetHit(bool hit)
{
    this->targetHit = hit;
}

void StateRL::setInitialState()
{
    this->ballsMoving.at(0) = true;
    for (int i = 1;i < 4;i++) {
        this->ballsMoving.at(i)=false;
    }
    for (int i = 0;i < 3;i++) {
        this->gearsStarted.at(i) = false;
    }
    for (int i = 0;i < 3;i++) {
        this->wheelsStarted.at(i) = false;
    }
    this->targetHit = false;
}
