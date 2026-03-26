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

GameDQN::~GameDQN()
{
	delete this->nextState_;
	delete this->agent_;
	delete this->actionFunctions_;
	delete this->level_;
}

void GameDQN::handleInput(sf::RenderWindow& window)
{
	sf::Event ev;
	while (window.pollEvent(ev)) {
		if (ev.type == sf::Event::Closed) {
			window.close();
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
			this->nextState_ = new Menu();
		}
	}
}

void GameDQN::update(float deltaTime)
{
	this->updateActionState();
	if ((level_->getNumberOfBelts() + level_->getNumberOfGears() + this->level_->getNumberOfBoxes()) == 0) {
		this->actionId_ = -1;
	}
	AgentAction actionType = this->actionFunctions_->getActionType(this->actionId_);
	if (actionType == AgentAction::PLACE_GEAR) {
		//gear placement
		std::pair<int, int> coordinates = this->actionFunctions_->getGearCoordinates(this->actionId_);
		if (!level_->tryGearPlacement(sf::Vector2f(coordinates.first * 50, coordinates.second * 50))) {
			this->currentReward_ += QTable::GetWrongGearPlacementReward();
		}
	}
	else if (actionType == AgentAction::PLACE_BELT) {
		//belt placement
		try {
			std::pair<BeltActionInfo, BeltActionInfo> beltActionInfo = this->actionFunctions_->getBeltPlacement(actionId_);
			sf::Vector2f start, end;
			if (beltActionInfo.first.isElementGear) {
				start = this->level_->getGearLocation(beltActionInfo.first.idElement);
			}
			else {
				start = this->level_->getWheelLocation(beltActionInfo.first.idElement);
			}
			if (beltActionInfo.second.isElementGear) {
				end = this->level_->getGearLocation(beltActionInfo.second.idElement);
			}
			else {
				end = this->level_->getWheelLocation(beltActionInfo.second.idElement);
			}
			if (start.x == -2 || end.x == -2) {
				this->currentReward_ += QTable::GetWrongBeltPlacementReward();
			}
			std::cout << sqrt(powf(abs(start.x - end.x), 2) + powf(abs(start.y - end.y), 2)) << std::endl;
			if (this->selectedLevel_ != LevelDifficulty::HARD &&
				sqrt(powf(abs(start.x - end.x), 2) + powf(abs(start.y - end.y), 2)) > level_->getMaxBeltDistance()) {
				start.x = -1;
				end.x = -1;
			}
			else if (this->selectedLevel_ == LevelDifficulty::HARD &&
				abs(start.y - end.y) > 250.f || abs(start.x - end.x) > 350.f) {
				start.x = -1;
				end.x = -1;
			}
			if (start.x == -1 || end.x == -1) {
				this->currentReward_ += QTable::GetWrongBeltPlacementReward();
			}
			else {
				if (!this->level_->placeBelt(start, end, beltActionInfo.first.isElementGear)) {
					this->currentReward_ += QTable::GetWrongBeltPlacementReward();
				}
			}
		}
		catch (std::exception e) {
			std::cout << e.what() << std::endl;
		}

	}
	else if (actionType == AgentAction::PLACE_BOX) {
		//box placement
		std::pair<int, int> coordinates = this->actionFunctions_->getBoxCoordinates(this->actionId_);
		if (!level_->tryBoxPlacement(sf::Vector2f(coordinates.first * 100, coordinates.second * 100))) {
			this->currentReward_ += QTable::GetWrongGearPlacementReward();
		}
	}
	this->level_->update(deltaTime);
	this->selectAction_ = this->updateState();
}

void GameDQN::render(sf::RenderTarget& target)
{
	target.clear(sf::Color::White);
	target.draw(this->panel_);
	target.draw(this->gameScreen_);
	this->level_->render(target);
	target.draw(this->playButton_);
	target.draw(this->playButtonText_);
}

State* GameDQN::getNextState()
{
	return this->nextState_;
}

void GameDQN::initUI()
{
	this->panel_.setSize(sf::Vector2f(200, 800));
	this->panel_.setPosition(sf::Vector2f(1000, 0));

	this->playButton_.setSize(sf::Vector2f(150, 100));
	this->playButton_.setPosition(sf::Vector2f(1025, 50));

	this->gameScreen_.setSize(sf::Vector2f(1000, 800));
	this->gameScreen_.setPosition(sf::Vector2f(0, 0));

	this->playButtonText_.setFont(this->font_);
	this->playButtonText_.setString("PLAY");
	this->playButtonText_.setCharacterSize(24);
	this->playButtonText_.setPosition(1050, 50);
}

void GameDQN::initTextures()
{
	sf::Texture& textureBackground = TextureManager::getTexture("assets/Textures/backgroundAI_ready.png");
	this->gameScreen_.setTexture(&textureBackground);
	sf::Texture& texturePanel = TextureManager::getTexture("assets/Textures/panelAI.jpg");
	this->panel_.setTexture(&texturePanel);
	sf::Texture& texturePlay = TextureManager::getTexture("assets/Textures/play.png");
	this->playButton_.setTexture(&texturePlay);
}

void GameDQN::initDQNAgent()
{
	int ng = this->level_->getStartingNumberOfGears();
	int nw = this->level_->getNumberOfWheels();

	//state feature vector size
	int stateSize = 1 + ng + nw + 1;

	this->actionsNum_ = ActionRL::getGridWidth() * ActionRL::getGridHeight();
	this->actionsNum_ += ng * nw;
	this->actionsNum_ += ActionRL::combination(nw, 2);
	if (this->selectedLevel_ == HARD) {
		this->actionsNum_ += ActionRL::getGridWidthBox() * ActionRL::getGridHeightBox();
	}

	this->statesNum_ = 1 << stateSize;
	std::cout << "[DQN] stateSize=" << stateSize
		<< "  actionsNum=" << actionsNum_ << "\n";

	int hiddenSize = 128;
	int bufferCap = 20000;

	switch (this->selectedLevel_)
	{
	case EASY:
		this->agent_->epsilonStart = 0.6;
		this->agent_->epsilonEnd = 0.05;
		this->agent_->epsilonDecay = 2000;
		this->agent_->alphaStart = 1e-3f;
		this->agent_->alphaEnd = 2e-4f;
		this->agent_->alphaDecay = 30000;
		this->agent_->gamma = 0.95f;
		this->agent_->targetSyncFreq = 150;
		break;
	case MEDIUM:
		this->agent_->epsilonStart = 0.7;
		this->agent_->epsilonEnd = 0.05;
		this->agent_->epsilonDecay = 5000;
		this->agent_->alphaStart = 1e-3f;
		this->agent_->alphaEnd = 1e-4f;
		this->agent_->alphaDecay = 80000;
		this->agent_->gamma = 0.95f;
		this->agent_->targetSyncFreq = 200;
		break;
	case HARD:
		this->agent_->epsilonStart = 0.8;
		this->agent_->epsilonEnd = 0.05;
		this->agent_->epsilonDecay = 15000;
		this->agent_->alphaStart = 5e-4f;
		this->agent_->alphaEnd = 5e-5f;
		this->agent_->alphaDecay = 200000;
		this->agent_->gamma = 0.96f;
		this->agent_->targetSyncFreq = 300;
		break;
	default:
		break;
	}
	std::string prefix = (this->selectedLevel_ == EASY ? "easy" :
		this->selectedLevel_ == MEDIUM ? "medium" : "hard");
	prefix += "_dqn";
	if (!this->agent_->load(prefix))
		std::cout << "[DQN] Starting fresh (no saved network found)\n";
}

void GameDQN::resetResources()
{
	delete this->level_;
	switch (this->selectedLevel_)
	{
	case LevelDifficulty::EASY:
		this->level_ = new EasyLevel(true);
		break;
	case LevelDifficulty::MEDIUM:
		this->level_ = new MediumLevel(true);
		break;
	case LevelDifficulty::HARD:
		this->level_ = new HardLevel(true);
		break;
	default:
		this->level_ = new MediumLevel(true);
		break;
	}
	this->level_->initLevel();
}

void GameDQN::updateActionState()
{
	if (this->stateId_ == -1) {
		this->isPlaying_ = true;
		this->level_->setIsPlaying(this->isPlaying_);
		this->stateId_ = 1;
		//FORBIDDEN ACTION
		this->forbiddenActions_ = this->level_->getBallZonesPassed();
		this->currentStateVector_ = DQNAgent::buildStateVector(
			this->level_->getStatusChange(),
			this->level_->getStartingNumberOfGears(),
			this->level_->getNumberOfWheels()
		);

		double eps = this->agent_->currentEpsilon();
		this->actionId_ = this->agent_->selectAction(this->currentStateVector_, eps, forbiddenActions_);
		this->lastExecutedAction_ = this->actionId_;
		this->episodeStart_ = std::chrono::system_clock::now();
		this->currentReward_ = 0.0f;
		this->cumulativeReward_ = 0.0f;
		return;
	}
	if (this->selectAction_) {

		//FORBIDDEN ACTION
		this->forbiddenActions_ = this->level_->getBallZonesPassed();
		this->currentStateVector_ = DQNAgent::buildStateVector(
			this->level_->getStatusChange(),
			this->level_->getStartingNumberOfGears(),
			this->level_->getNumberOfWheels()
		);
		double eps = this->agent_->currentEpsilon();
		this->actionId_ = agent_->selectAction(currentStateVector_, eps, forbiddenActions_);
		this->stateId_ = this->nextStateId_;
		this->lastExecutedAction_ = this->actionId_;
	}
	else {
		this->actionId_ = -1;
	}
}

bool GameDQN::updateState()
{
	if (this->level_->getStateChanged()) {
		//TODO: add reward getter from level
		this->nextStateId_ = this->level_->getStatusChange()->getStateId();
		this->currentReward_ += this->level_->getReward();
		std::cout << "[DQN] s=" << stateId_
			<< "  s'=" << nextStateId_
			<< "  r=" << currentReward_ << "\n";
		if (this->nextStateId_ != this->stateId_) {
			std::vector<float> nextStateVector = DQNAgent::buildStateVector(
				level_->getStatusChange(),
				level_->getStartingNumberOfGears(),
				level_->getNumberOfWheels()
			);
			bool terminalState = this->nextStateId_ % 2 == 0 || this->nextStateId_ >= (this->statesNum_ / 2);
			this->agent_->storeTransition(
				currentStateVector_,
				lastExecutedAction_,
				currentReward_,
				nextStateVector,
				terminalState
			);
			float loss = agent_->trainStep();
			if (loss >= 0.f) {
				std::cout << "[DQN] train loss=" << loss << "\n";
			}
			this->cumulativeReward_ += this->currentReward_;
			this->currentReward_ = 0;
		}
		if (nextStateId_ % 2 == 0 || this->nextStateId_ >= (this->statesNum_ / 2)) {
			//terminal state

			int win = this->nextStateId_ >= (this->statesNum_ / 2) ? 1 : 0;

			auto now = std::chrono::system_clock::now();
			double duration = std::chrono::duration<double>(now - this->episodeStart_).count();
			this->appendLog(episodeCount_,
				agent_->currentEpsilon(),
				cumulativeReward_,
				win,
				0,
				duration);
			if (win) std::cout << "[DQN] WON\n";
			else     std::cout << "[DQN] LOST\n";

			agent_->endEpisode();
			if (episodeCount_ % 100 == 0)
			{
				std::string prefix = (selectedLevel_ == EASY ? "easy" :
					selectedLevel_ == MEDIUM ? "medium" : "hard");
				agent_->save(prefix + "_dqn");
			}
			if (episodeCount_ == agent_->epsilonDecay) {
				nextState_ = new Menu();
			}
			this->resetResources();
			this->stateId_ = -1;
			this->actionId_ = -1;
			this->nextStateId_ = 1;
			this->forbiddenActions_.clear();
		}
		return true;
	}
	return false;
}

void GameDQN::appendLog(int ep, double eps, float totalReward, int win, int steps, double duration)
{
	std::string name = (selectedLevel_ == EASY ? "easy" :
		selectedLevel_ == MEDIUM ? "medium" : "hard");
	name += "_dqn_training_log.csv";
	bool header = !std::ifstream(name).good();
	std::ofstream f(name, std::ios::app);
	if (!f) return;
	if (header)
		f << "episode,timestamp,epsilon,total_reward,win,duration_s\n";
	f << ep << "," << isoNow() << ","
		<< std::fixed << std::setprecision(6) << eps << ","
		<< totalReward << "," << win << ","
		<< std::setprecision(3) << duration << "\n";
}

std::string GameDQN::isoNow()
{
	auto now = std::chrono::system_clock::now();
	auto t = std::chrono::system_clock::to_time_t(now);
	std::tm tm; localtime_s(&tm, &t);
	auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
		now.time_since_epoch()) % 1000;
	std::ostringstream ss;
	ss << std::put_time(&tm, "%Y-%m-%dT%H:%M:%S")
		<< '.' << std::setw(3) << std::setfill('0') << ms.count();
	return ss.str();
}
