#include "stdafx.h"
#include "GameAI.h"


void GameAI::initFont()
{
	if (!font.loadFromFile("assets/Fonts/ARIAL.TTF")) {
		std::cout << "ERROR: Could not load font" << std::endl;
	}
}

void GameAI::initText()
{
	this->playButtonText.setFont(font);
	this->playButtonText.setString("PLAY");
	this->playButtonText.setCharacterSize(24);
	this->playButtonText.setPosition(1050, 50);

}

void GameAI::initPanel()
{
	this->panel.setSize(sf::Vector2f(200, 800));
	this->panel.setPosition(sf::Vector2f(1000, 0));
	this->panel.setFillColor(sf::Color::White);

	this->playButton.setSize(sf::Vector2f(150, 100));
	this->playButton.setPosition(sf::Vector2f(1025, 50));
	//this->playButton.setFillColor(sf::Color::Green);
}

void GameAI::initGameScreen()
{
	this->gameScreen.setSize(sf::Vector2f(1000, 800));
	this->gameScreen.setPosition(sf::Vector2f(0, 0));
	//this->gameScreen.setFillColor(sf::Color(128, 128, 128));
	
}

void GameAI::initTextures()
{
	sf::Texture& textureBackground = TextureManager::getTexture("assets/Textures/backgroundAI_ready.png");
	this->gameScreen.setTexture(&textureBackground);
	sf::Texture& texturePanel = TextureManager::getTexture("assets/Textures/panelAI.jpg");
	this->panel.setTexture(&texturePanel);
	sf::Texture& texturePlay = TextureManager::getTexture("assets/Textures/play.png");
	this->playButton.setTexture(&texturePlay);
}

void GameAI::initQTable()
{
	int statesNum = std::pow(2,Level::getNumberOfGearsStatic() + Level::getNumberOfWheels() + 1 + 1);
	int actionsNum = ActionRL::getGridWidth() * ActionRL::getGridHeight();//gear placements
	actionsNum += Level::getNumberOfGearsStatic() * Level::getNumberOfWheels();//belt placements between gear and wheel
	actionsNum += ActionRL::combination(Level::getNumberOfWheels(), 2);//belt placements between two wheels
	this->table = QTable(statesNum, actionsNum, "qtable.txt");
}

void GameAI::updateActionState()
{
	if (this->stateId == -1) {
		this->isPlaying = true;
		level->setIsPlaying(this->isPlaying);
		this->stateId = 1;
		//this->actionId = 248;
		double eps = GameAI::linearDecay(this->E_START, this->E_END, this->iterations, this->E_DECAY);
		//FORBIDDEN ACTION
		this->forbiddenActions = this->level->getBallZonesPassed();
		this->actionId = this->table.getAction(this->stateId, eps,this->forbiddenActions);
		this->lastExecutedAction = this->actionId;
		return;
	}
	if (this->selectAction) {
		/*
		if (this->nextStateId ==3) {
			this->actionId = 266;
		}
		else{
			this->actionId = 1;
		}
		*/
		//FORBIDDEN ACTION
		this->forbiddenActions = this->level->getBallZonesPassed();
		this->actionId = this->table.getAction(this->nextStateId, 0.1,this->forbiddenActions);
		this->stateId = this->nextStateId;
		this->lastExecutedAction = this->actionId;
	}
	else {
		this->actionId = -1;
	}
}

bool GameAI::updateState()
{
	//std::cout << this->stateId << " " << this->nextStateId << std::endl;
	//get state changed
	if (this->level->getStateChanged()) {
		//TODO: add reward getter from level
		this->nextStateId = this->level->getStatusChange().getStateId();
		std::cout << this->stateId << " s " << this->nextStateId << " r " << this->level->getReward() << std::endl;
		if (this->nextStateId != this->stateId) {
			this->table.updateValidActions(
				Level::getNumberOfGearsStatic()-this->level->getNumberOfGears(),
				Level::getNumberOfBeltsStatic()-this->level->getNumberOfBelts());
			bool terminalState = this->nextStateId % 2 == 0 || this->nextStateId > 127;
			this->table.updateQValue(this->stateId, this->lastExecutedAction, this->level->getReward(), this->nextStateId,terminalState);
			this->episode.push_back(new Transition({ this->stateId,this->lastExecutedAction,0.0,-1 }));
		}
		if (nextStateId%2 == 0) {
			this->gameOver = true;
			this->iterations++;
			this->resetResources();
			this->stateId = -1;
			this->actionId = -1;
			this->nextStateId = 1;
			this->table.updateValidActions(0,0);
			this->forbiddenActions.clear();
			if (this->iterations % 10 == 0) {
				this->table.printTable("qtable.txt",this->iterations);
				std::string csvFilename = "qtable" + std::to_string(this->iterations) + ".csv";
				std::cout << csvFilename << std::endl;
				this->table.saveQTableCSV(csvFilename);
			}
		}
		return true;
	}
	return false;
}

void GameAI::resetResources()
{
	delete this->level;
	this->level = new Level();
	this->level->initLevel();
}
GameAI::GameAI()
{
	this->stateId = -1;
	this->actionId = -1;
	this->nextStateId = 1;
	this->initFont();
	//this->initText();
	this->initGameScreen();
	this->initPanel();
	this->initTextures();
	this->initQTable();
	this->level = new Level();
	this->level->initLevel();
	this->nextState = nullptr;
	this->agent=new AgentRL();
	
}

GameAI::~GameAI()
{
	delete this->nextState;
	delete this->agent;
}

void GameAI::handleInput(sf::RenderWindow& window)
{
	
		//level.handleInput(window);
	sf::Event ev;
	while (window.pollEvent(ev)) {
		if (ev.type == sf::Event::Closed) {
			window.close();
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
			this->nextState = new Menu();
		}
	}
	//handle input on level side
}

void GameAI::update(float deltaTime)
{
	/*
	if (!this->isPlaying) {
		this->agent->generateAction();
		switch (this->agent->getAction())
		{
		case AgentAction::PLACE_GEAR:
			do {
				this->agent->generateGearPosition();
			} while (level.getNumberOfGears() > 0 && level.tryGearPlacement(this->agent->getGearPosition()));
			break;
		case AgentAction::PLACE_BELT:
			this->agent->generateBeltStart(level.getStaticObjects(), level.getStaticWheels());
			this->agent->generateBeltEnd(level.getStaticWheels());
			if (level.getNumberOfBelts() > 0) {
				level.placeBelt(this->agent->getBeltStartPosition(), this->agent->getBeltEndPosition());
			}
			break;
		case AgentAction::START:
			this->isPlaying = true;
			level.setIsPlaying(this->isPlaying);
			break;
		default:
			break;
		}
	}
	*/
	this->updateActionState();
	if ((level->getNumberOfBelts() + level->getNumberOfGears())==0) {
		std::cout << "Nema vise " << std::endl;
		this->actionId = -1;
	}
	AgentAction actionType=this->actionFunctions.getActionType(this->actionId);
	if (actionType == AgentAction::PLACE_GEAR) {
		//gear placement
		std::pair<int,int> coordinates=this->actionFunctions.getGearCoordinates(this->actionId);
		if (!level->tryGearPlacement(sf::Vector2f(coordinates.first*50, coordinates.second*50))) {
		//if (this->stateId==0&&!level->tryGearPlacement(sf::Vector2f(0*50, 13*50))) {
			//TODO: apply penalty
			this->table.updateQValue(this->stateId, this->actionId, WRONG_GEAR_PLACEMENT, this->stateId);
		}
		/*
		if (this->stateId != 0 && !level->tryGearPlacement(sf::Vector2f(coordinates.first * 50, coordinates.second * 50))) {

			this->table.updateQValue(this->stateId, this->actionId, WRONG_GEAR_PLACEMENT, this->stateId);
		}
		*/
	}
	else if(actionType==AgentAction::PLACE_BELT){
		//belt placement
		/*
		TODO: give some restriction to belt placement
		for example max 200px between start and end
		*/
		try {
			std::cout << "Belt " <<actionId<< std::endl;
			std::pair<BeltActionInfo, BeltActionInfo> beltActionInfo = this->actionFunctions.getBeltPlacement(actionId);
			std::cout << "Is start gear " << beltActionInfo.first.isElementGear << " id " << beltActionInfo.first.idElement << std::endl;
			std::cout << "Is end gear " << beltActionInfo.second.isElementGear << " id " << beltActionInfo.second.idElement << std::endl;
			sf::Vector2f start, end;
			if (beltActionInfo.first.isElementGear) {
				start = this->level->getGearLocation(beltActionInfo.first.idElement);
			}
			else {
				start = this->level->getWheelLocation(beltActionInfo.first.idElement);
			}
			if (beltActionInfo.second.isElementGear) {
				end = this->level->getGearLocation(beltActionInfo.second.idElement);
			}
			else {
				end = this->level->getWheelLocation(beltActionInfo.second.idElement);
			}
			std::cout << sqrt(powf(abs(start.x - end.x), 2) + powf(abs(start.y - end.y), 2)) << std::endl;
			if (sqrt(powf(abs(start.x - end.x), 2) + powf(abs(start.y - end.y), 2)) > 350.0) {
				start.x = -1;
				end.x = -1;
			}
			if (start.x == -1 || end.x == -1) {
				this->table.updateQValue(this->stateId, this->actionId, WRONG_BELT_PLACEMENT, this->stateId);
			}
			else {
				if (!this->level->placeBelt(start, end, beltActionInfo.first.isElementGear)) {
					this->table.updateQValue(this->stateId, this->actionId, WRONG_BELT_PLACEMENT, this->stateId);
				}
			}
		}
		catch (std::exception e) {
			std::cout << e.what() << std::endl;
		}
		
	}
	this->level->update(deltaTime);
	this->selectAction=this->updateState();
}

void GameAI::render(sf::RenderTarget& target)
{
	target.clear(sf::Color::White);
	target.draw(this->panel);
	target.draw(this->gameScreen);
	this->level->render(target);
	target.draw(this->playButton);
	target.draw(this->playButtonText);
}

State* GameAI::getNextState()
{
	return this->nextState;
}

double GameAI::linearDecay(double start, double end, int episode, int decayEpisodes)
{
	if (episode >= decayEpisodes)return end;
	double t = (double)episode / (double)decayEpisodes;
	return start + (end - start) * t;
}
/*
void GameAI::updateForbiddenActions(int gearsPlaced, int beltsPlaced)
{
	if (gearsPlaced == 3) {
		for (int i = 0;i < ActionRL::getGridHeight() * ActionRL::getGridWidth();i++) {

		}
	}
	if (gearsPlaced < 3) {
		//3 is max num of belts
		if (beltsPlaced < 3) {
			for (int i = 0;i < this->numActions;i++) {
				this->validActions.push_back(i);
			}
		}
		else {
			//12 is number of belt actions
			for (int i = 0;i < this->numActions - 12;i++) {
				this->validActions.push_back(i);
			}
		}
	}
	else {
		//3 is max num of belts
		if (beltsPlaced < 3) {
			for (int i = 19 * 14;i < this->numActions;i++) {
				this->validActions.push_back(i);
			}
		}
	}
}
*/