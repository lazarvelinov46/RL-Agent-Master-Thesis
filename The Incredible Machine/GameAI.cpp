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

	this->playButton.setSize(sf::Vector2f(100, 50));
	this->playButton.setPosition(sf::Vector2f(1050, 50));
	this->playButton.setFillColor(sf::Color::Green);
}

void GameAI::initGameScreen()
{
	this->gameScreen.setSize(sf::Vector2f(1000, 800));
	this->gameScreen.setPosition(sf::Vector2f(0, 0));
	this->gameScreen.setFillColor(sf::Color(128, 128, 128));
}

void GameAI::selectAction()
{
	if (this->stateId == -1) {
		this->isPlaying = true;
		level.setIsPlaying(this->isPlaying);
		this->stateId = 1;
		return;
	}
	if (this->stateId != this->nextStateId) {
		std::cout << this->stateId << " ee " << this->nextStateId << " oo ";
		this->actionId = this->table.getAction(this->nextStateId, 0.1);

		std::cout << "ID " << this->actionId << std::endl;
		this->stateId = this->nextStateId;
		this->episode.push_back(new Transition({ this->stateId,this->actionId,0.0,-1 }));
	}
	else {
		this->actionId = -1;
	}
}

void GameAI::updateState()
{
	//get state changed
	if (this->level.getStateChanged()) {
		//TODO: add reward getter from level
		this->nextStateId = this->level.getStatusChange().getStateId();
		this->table.updateQValue(this->stateId, this->actionId, 1.0, this->nextStateId);
		std::cout <<"ee"<< this->nextStateId << std::endl;
	}
}

GameAI::GameAI()
{
	this->stateId = -1;
	this->actionId = -1;
	this->nextStateId = 0;
	this->initFont();
	this->initText();
	this->initGameScreen();
	this->initPanel();
	this->table = QTable(2048, 252);
	this->level.initLevel();
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
	this->selectAction();
	AgentAction actionType=this->actionFunctions.getActionType(this->actionId);
	if (actionType == AgentAction::PLACE_GEAR) {
		//gear placement
		std::pair<int,int> coordinates=this->actionFunctions.getGearCoordinates(this->actionId);
		std::cout << coordinates.first << " asdfa " << coordinates.second << std::endl;
		if (!level.tryGearPlacement(sf::Vector2f(coordinates.first*50, coordinates.second*50))) {
			//TODO: apply penalty
			std::cout << "NMZ" << std::endl;
			this->table.updateQValue(this->stateId, this->actionId, WRONG_GEAR_PLACEMENT, this->stateId);
		}
	}
	else {
		//belt placement
		/*
		TODO: give some restriction to belt placement
		for example max 200px between start and end
		*/
		std::pair<BeltActionInfo,BeltActionInfo> beltActionInfo=this->actionFunctions.getBeltPlacement(actionId);
		sf::Vector2f start, end;
		if (beltActionInfo.first.isElementGear) {
			start = this->level.getGearLocation(beltActionInfo.first.idElement);
		}
		else {
			start = this->level.getWheelLocation(beltActionInfo.first.idElement);
		}
		if (beltActionInfo.second.isElementGear) {
			end = this->level.getGearLocation(beltActionInfo.second.idElement);
		}
		else {
			end = this->level.getWheelLocation(beltActionInfo.second.idElement);
		}
		if (sqrt(powf(abs(start.x - end.x), 2) + powf(abs(start.y - end.y), 2)) < 300) {
			start.x = -1;
			end.x = -1;
		}
		if (start.x == -1 || end.x == -1) {
			this->table.updateQValue(this->stateId, this->actionId, WRONG_BELT_PLACEMENT, this->stateId);
		}
		else {
			this->level.placeBelt(start, end);
		}
	}
	this->level.update(deltaTime);
	this->updateState();
}

void GameAI::render(sf::RenderTarget& target)
{
	target.clear(sf::Color::White);
	target.draw(this->panel);
	target.draw(this->gameScreen);
	this->level.render(target);
	target.draw(this->playButton);
	target.draw(this->playButtonText);
}

State* GameAI::getNextState()
{
	return this->nextState;
}
