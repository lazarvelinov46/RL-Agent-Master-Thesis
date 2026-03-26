#include "stdafx.h"
#include "GameDQN.h"

GameDQN::GameDQN(LevelDifficulty difficulty) : selectedLevel_(difficulty)
{
	switch (selectedLevel_)
	{
	case EASY:
		this->level_ = new EasyLevel(true);
		this->level_->setPenaltyPerPlacedResource(0.6);
		break;
	case MEDIUM:
		this->level_ = new MediumLevel(true);
		this->level_->setPenaltyPerPlacedResource(0.6);
		break;
	case HARD:
		this->level_ = new HardLevel(true);
		this->level_->setPenaltyPerPlacedResource(0.8);
		break;
	default:
		this->level_ = new MediumLevel(true);
		this->level_->setPenaltyPerPlacedResource(0.6);
		break;
	}
	// UI ---------------------------------------------------------------------
	if (!font_.loadFromFile("assets/Fonts/ARIAL.TTF")) {
		std::cout << "ERROR: font not loaded\n";
	}

	this->initUI();
	this->initTextures();

	this->level_->initLevel();

	this->actionFunctions_ = new ActionRL(this->level_->getNumberOfWheels(), 
		this->level_->getStartingNumberOfGears());
	this->initDQNAgent();
}
