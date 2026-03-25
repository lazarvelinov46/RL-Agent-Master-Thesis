#pragma once
#include "stdafx.h"
#include "NeuralNetwork.h"
#include "ReplayBuffer.h"
#include "StateRL.h"
#include "ActionRL.h"


/// <summary>
/// Class for Deep Q Network Agent
/// 
/// Creates feature vector for StateRL
///		- converts state (int) to feature state vector (std::vector<float>)
/// Selects actions
/// Fills replay buffer with transitions
/// Samples batches from transition buffer and trains Q network
/// Syncs the target network
/// </summary>
class DQNAgent {
public:
	//Hyperparameters
	float gamma = 0.95f;
	float alphaStart = 1e-3f;	//initial adam learning rate
	float alphaEnd = 1e-4f;		//final adam learning rate
	int alphaDecay = 20000;		//steps over which alpha decays linearly
	int batchSize = 64;			//batch size of transitions from replay buffer
	int minBufferSize = 1000;	//minimum bumber of samples requiring for training to start
	int targetSyncFreq = 200;	//number of steps after which target network is synced
	float epsilonStart = 0.9f;
	float epsilonEnd = 0.05f;
	float epsilonDecay = 5000;

	//constructor

	/// <summary>
	/// 
	/// </summary>
	/// <param name="stateSize">length of feature vector for input</param>
	/// <param name="numActions">total action count</param>
	/// <param name="hiddenSize">neurons in two hidden layers</param>
	/// <param name="bufferCap">replay buffer capacity</param>
	/// <param name="seed">random seed</param>
	DQNAgent(
		int stateSize,
		int numActions,
		int hiddenSize = 128,
		int bufferCap = 20000,
		unsigned seed = 42u
	);

	/// <summary>
	/// Converting state representation from QTable (bitmask)
	/// into feature vector (vector<float>)
	/// Perhaps later upgrade of state feature vector into tracking
	/// balls position and velocity
	/// </summary>
	/// <param name="state">state info</param>
	/// <param name="numGears">total num of gears</param>
	/// <param name="numWheels">total num of wheels (platforms)</param>
	/// <returns></returns>
	static std::vector<float> buildStateVector(const StateRL* state,
		int numGears, int numWheels);

	/// <summary>
	/// Epsilon-greedy action selection
	/// </summary>
	/// <param name="stateVector">state feature vector which represents
	/// state from which action is selected</param>
	/// <param name="epsilon">exploration rate</param>
	/// <param name="forbiddenActions">forbidden actions for given state
	/// they have -inf Q value</param>
	/// <returns>id of selected action</returns>
	int selectAction(const std::vector<float>& stateVector,
		double epsilon, const std::unordered_set<int>& forbiddenActions);

	//store one experience transition tuple
	void storeTransition(const std::vector<float>& state,
		int action,
		float reward,
		const std::vector<float>& nextState,
		bool done);

	/// <summary>
	/// Function for doing one training step
	/// Samples mini batch, updates online network
	/// Called once per environment step (after transition event)
	/// </summary>
	/// <returns>Mean squared error loss of the step, or -1 if buffer not ready</returns>
	float trainStep();

	/// <summary>
	/// Function called after end of every episode (reaching terminal state)
	/// Increments episode counter, updates alpha and epsilon
	/// </summary>
	void endEpisode() { this->episodeCount_++; }

	double currentEpsilon()const {
		return this->linearDecay(
			this->epsilonStart,this->epsilonEnd,this->episodeCount_,this->epsilonDecay
		);
	}
	double currentAlpha()const;

	//serialization

	/// <summary>
	/// saves network_online.bin and network_target.bin
	/// </summary>
	void save(const std::string& network)const;
	bool load(const std::string& network);

private:
	int stateSize_;
	int numActions_;

	NeuralNetwork online_;		//trained every step
	NeuralNetwork target_;		//synced every targetSyncFreq steps
	ReplayBuffer buffer_;

	int episodeCount_ = 0;
	int trainStepCount_ = 0;
	float lastLoss_ = 0.f;

	std::mt19937 rng_;

	static double linearDecay(double start, double end,
		int current, int total);
};