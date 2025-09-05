#include "stdafx.h"
#include "GameAI.h"


bool GameAI::loadQTableFromFile(const std::string& filename)
{
	std::ifstream in(filename);
	if (!in.is_open())return false;

	auto read_after_coma = [&](std::string& line)->std::string {
		std::getline(in, line);
		if (line.empty())return {};
		size_t c = line.find(',');
		return (c == std::string::npos) ? std::string{} : line.substr(c + 1);
	};

	std::string line, s;
	s = read_after_coma(line);
	if (s.empty()) {
		return false;
	}
	this->iterations = std::stoi(s);
	s = read_after_coma(line);
	if (s.empty()) {
		return false;
	}
	s = read_after_coma(line);
	if (s.empty()) {
		return false;
	}
	bool ok = this->table.loadQTableCSV(in);
	return ok;
}

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
	int statesNum = std::pow(2,level->getStartingNumberOfGears() + level->getNumberOfWheels() + 1 + 1);
	int actionsNum = ActionRL::getGridWidth() * ActionRL::getGridHeight();//gear placements
	actionsNum += level->getStartingNumberOfGears() * level->getNumberOfWheels();//belt placements between gear and wheel
	actionsNum += ActionRL::combination(level->getNumberOfWheels(), 2);//belt placements between two wheels
	if (this->selectedLevel == LevelDifficulty::HARD) {
		actionsNum += ActionRL::getGridWidthBox() * ActionRL::getGridHeightBox();
	}
	std::cout << "States: " << statesNum << " Actions: " << actionsNum<<std::endl;
	this->statesNum = statesNum;
	this->actionsNum = actionsNum;
	this->table = QTable(statesNum, actionsNum, "qtable.txt");
	this->iterations = 0;
	if (this->loadQTableFromFile("qtable490000.csv")) {
		std::cout << "Loaded QTable from file qtable4900.csv" << std::endl;
		double alphaCap = linearDecay(ALPHA_START, ALPHA_END, iterations, ALPHA_DECAY);
		this->table.setAlpha(alphaCap);
	}
	else {
		std::cout << "Started with default QTable" << std::endl;
		this->table.setAlpha(GameAI::ALPHA_START);
	}
	this->table.setGamma(GameAI::GAMMA);

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
		this->episodeStartTime = std::chrono::system_clock::now();
		this->currentReward = 0.0f;
		this->episode.clear();
		return;
	}
	if (this->selectAction) {
		
		//FORBIDDEN ACTION
		
		this->forbiddenActions = this->level->getBallZonesPassed();
		/*
		for (int a : this->forbiddenActions) {
			std::cout << a << " ";
		}
		
		std::cout<<std::endl;
		switch (this->nextStateId) {
		case 3:
			this->actionId = 266;
			break;
		case 19:
			this->actionId = 255;
			break;
		case 23:
			this->actionId = 270;
			//this->actionId = 252;
			break;
		case 55:
			this->actionId = 265;
			break;
		default:
			this->actionId = this->table.getAction(this->nextStateId, 0.1, this->forbiddenActions);
		}
		*/
		double eps = GameAI::linearDecay(this->E_START, this->E_END, this->iterations, this->E_DECAY);
		this->actionId = this->table.getAction(this->nextStateId, eps,this->forbiddenActions);
		std::cout << "ACTION " << this->actionId << std::endl;
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
		this->nextStateId = this->level->getStatusChange()->getStateId();
		this->currentReward += this->level->getReward();
		std::cout << this->stateId << " s " << this->nextStateId << " r " << this->currentReward << std::endl;
		if (this->nextStateId != this->stateId) {
			this->table.updateValidActions(
				level->getStartingNumberOfGears() - this->level->getNumberOfGears(),
				level->getStartingNumberOfBelts() - this->level->getNumberOfBelts());
			bool terminalState = this->nextStateId % 2 == 0 || this->nextStateId >= (this->statesNum/2);
			this->table.updateQValue(this->stateId, this->lastExecutedAction, this->currentReward, this->nextStateId,terminalState);
			this->episode.push_back(new Transition({ this->stateId,this->lastExecutedAction,0.0,-1 }));
			this->currentReward = 0;
		}
		if (nextStateId % 2 == 0 || this->nextStateId >= (this->statesNum/2)) {
			auto now = std::chrono::system_clock::now();
			double duration = 0.0;
			if (this->episodeStartTime.time_since_epoch().count() != 0) {
				duration = std::chrono::duration<double>(now - this->episodeStartTime).count();
			}
			std::pair<double,double> qstats = this->table.getQStats();
			int uniqueVisited = this->table.getUniqueVisitsCount();
			double eps = GameAI::linearDecay(this->E_START, this->E_END, this->iterations, this->E_DECAY);
			double alphaCap = linearDecay(ALPHA_START, ALPHA_END, iterations, ALPHA_DECAY);
			int win = this->nextStateId >= (this->statesNum / 2) ? 1 : 0;
			int steps = static_cast<int>(this->episode.size());
			this->appendEpisodeLog(this->iterations, eps, alphaCap, this->currentReward, win, steps, duration,
				uniqueVisited, qstats.first, qstats.second);
			this->gameOver = true;
			if (win)std::cout << "WON GAME" << std::endl;
			else std::cout << "LOST GAME" << std::endl;
			this->iterations++;
			this->resetResources();
			this->stateId = -1;
			this->actionId = -1;
			this->nextStateId = 1;
			this->table.updateValidActions(0,0);
			this->forbiddenActions.clear();
			if (this->iterations % 100 == 0) {
				std::string csvFilename = this->selectedLevel+"qtable" + std::to_string(this->iterations) + ".csv";
				std::cout << csvFilename << std::endl;
				this->table.saveQTableCSV(csvFilename,iterations,alphaCap,0.0);
			}
			alphaCap = linearDecay(ALPHA_START, ALPHA_END, iterations, ALPHA_DECAY);
			this->table.setAlpha(alphaCap);
			if (this->iterations == 5000) {
				this->nextState = new Menu();
			}
		}
		return true;
	}
	return false;
}

std::string GameAI::isoTimestampNow()
{
	auto now = std::chrono::system_clock::now();
	std::time_t time = std::chrono::system_clock::to_time_t(now);
	std::tm tm;
	localtime_s(&tm, &time);
	auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;
	std::ostringstream oss;
	oss << std::put_time(&tm, "%Y-%m-%dT%H:%M:%S") << '.' << std::setw(3) << std::setfill('0') << ms.count();
	return oss.str();
}

void GameAI::appendEpisodeLog(int episode, double eps, double alphaCap, double totalReward, int win, int steps, double duration_s, int unique_sa_visited, double q_max, double q_mean)
{
	const std::string fname = this->selectedLevel+"training_log.csv";
	bool needHeader = false;
	std::ifstream fin(fname);
	if (!fin.good())needHeader = true;
	fin.close();
	std::ofstream f(fname, std::ios::app);
	if (!f.is_open())return;
	if (needHeader) {
		f << "episode,timestamp,eps,alphaCap,total_reward,win,steps,episode_duration_s,unique_sa_visited,q_max,q_mean\n";
	}
	f << episode << ",";
	f << isoTimestampNow() << ",";
	f << std::fixed << std::setprecision(6) << eps << ",";
	f << std::fixed << std::setprecision(6) << alphaCap << ",";
	f << std::fixed << std::setprecision(6) << totalReward << ",";
	f << win << ",";
	f << steps << ",";
	f << std::fixed << std::setprecision(3) << duration_s << ",";
	f << unique_sa_visited << ",";
	f << std::fixed << std::setprecision(6) << q_max << "," << q_mean << "\n";
	f.close();
}


void GameAI::resetResources()
{
	delete this->level;
	switch (this->selectedLevel)
	{
	case LevelDifficulty::EASY:
		this->level = new EasyLevel(true);
		break;
	case LevelDifficulty::MEDIUM:
		this->level = new MediumLevel(true);
		break;
	case LevelDifficulty::HARD:
		this->level = new HardLevel(true);
		break;
	default:
		this->level = new MediumLevel(true);
		break;
	}
	this->level->initLevel();
}
GameAI::GameAI(LevelDifficulty difficulty)
{
	this->selectedLevel = difficulty;
	this->stateId = -1;
	this->actionId = -1;
	this->nextStateId = 1;
	this->initFont();
	//this->initText();
	this->initGameScreen();
	this->initPanel();
	this->initTextures();
	switch (this->selectedLevel)
	{
	case LevelDifficulty::EASY:
		this->level = new EasyLevel(true);
		break;
	case LevelDifficulty::MEDIUM:
		this->level = new MediumLevel(true);
		break;
	case LevelDifficulty::HARD:
		this->level = new HardLevel(true);
		break;
	default:
		this->level = new MediumLevel(true);
		break;
	}
	this->initQTable();
	this->level->initLevel();
	this->actionFunctions=new ActionRL(this->level->getNumberOfWheels(),this->level->getStartingNumberOfGears());
	this->nextState = nullptr;
	this->agent=new AgentRL();
	
}

GameAI::~GameAI()
{
	delete this->nextState;
	delete this->agent;
	delete this->actionFunctions;
	delete this->level;
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
	if ((level->getNumberOfBelts() + level->getNumberOfGears()+this->level->getNumberOfBoxes())==0) {
		this->actionId = -1;
	}
	AgentAction actionType=this->actionFunctions->getActionType(this->actionId);
	if (actionType == AgentAction::PLACE_GEAR) {
		//gear placement
		std::pair<int,int> coordinates=this->actionFunctions->getGearCoordinates(this->actionId);
		if (!level->tryGearPlacement(sf::Vector2f(coordinates.first*50, coordinates.second*50))) {
		//if (this->stateId==0&&!level->tryGearPlacement(sf::Vector2f(0*50, 13*50))) {
			//TODO: apply penalty
			//this->table.updateQValue(this->stateId, this->actionId, WRONG_GEAR_PLACEMENT, this->stateId);
			this->currentReward += WRONG_GEAR_PLACEMENT;
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
			std::pair<BeltActionInfo, BeltActionInfo> beltActionInfo = this->actionFunctions->getBeltPlacement(actionId);
			std::cout << "Is start gear " << beltActionInfo.first.isElementGear << " id " << beltActionInfo.first.idElement << std::endl;
			std::cout << "Is end gear " << beltActionInfo.second.isElementGear << " id " << beltActionInfo.second.idElement << std::endl;
			sf::Vector2f start, end;
			if (this->stateId == 23) {
				std::cout << "prob" << std::endl;
			}
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
			if (start.x == -2 || end.x == -2) {
				//this->table.updateQValue(this->stateId, this->actionId, WRONG_BELT_PLACEMENT, this->stateId);
				this->currentReward += WRONG_BELT_PLACEMENT;
				std::cout << "NMZ" << std::endl;
			}
			std::cout << sqrt(powf(abs(start.x - end.x), 2) + powf(abs(start.y - end.y), 2)) << std::endl;
			if (sqrt(powf(abs(start.x - end.x), 2) + powf(abs(start.y - end.y), 2)) > level->getMaxBeltDistance()) {
				start.x = -1;
				end.x = -1;
			}
			if (start.x == -1 || end.x == -1) {
				this->currentReward += WRONG_BELT_PLACEMENT;
				//this->table.updateQValue(this->stateId, this->actionId, WRONG_BELT_PLACEMENT, this->stateId);
			}
			else {
				if (!this->level->placeBelt(start, end, beltActionInfo.first.isElementGear)) {
					//this->table.updateQValue(this->stateId, this->actionId, WRONG_BELT_PLACEMENT, this->stateId);
					this->currentReward += WRONG_BELT_PLACEMENT;
				}
			}
		}
		catch (std::exception e) {
			std::cout << e.what() << std::endl;
		}
		
	}
	else {
		//box placement
		std::pair<int, int> coordinates = this->actionFunctions->getGearCoordinates(this->actionId);
		if (!level->tryGearPlacement(sf::Vector2f(coordinates.first * 100, coordinates.second * 100))) {
			//if (this->stateId==0&&!level->tryGearPlacement(sf::Vector2f(0*50, 13*50))) {
				//TODO: apply penalty
				//this->table.updateQValue(this->stateId, this->actionId, WRONG_GEAR_PLACEMENT, this->stateId);
			this->currentReward += WRONG_GEAR_PLACEMENT;
		}
		/*
		if (this->stateId != 0 && !level->tryGearPlacement(sf::Vector2f(coordinates.first * 50, coordinates.second * 50))) {

			this->table.updateQValue(this->stateId, this->actionId, WRONG_GEAR_PLACEMENT, this->stateId);
		}
		*/
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