#include "stdafx.h"
#include "DQNAgent.h"

DQNAgent::DQNAgent(int stateSize, int numActions, int hiddenSize, int bufferCap, unsigned seed)
	:stateSize_(stateSize),
	numActions_(numActions),
	online_({ stateSize,hiddenSize,hiddenSize,numActions }, seed),
	target_({ stateSize,hiddenSize,hiddenSize,numActions }, seed),
	buffer_(bufferCap, seed + 2),
	rng_(seed + 3)
{
	this->target_.copyWeightsFrom(this->online_);
	this->online_.setLearningRate(this->alphaStart);
}

std::vector<float> DQNAgent::buildStateVector(const StateRL* state, int numGears, int numWheels)
{
	std::vector<float> v;
	//balls moving + gears started + platforms started + target hit
	v.reserve(1 + numGears + numWheels + 1);

	v.push_back(state->getAllBallsMoving() ? 1.f : 0.f);

	for (int i = 0; i < numGears; i++) {
		v.push_back(state->getGearStarted(i) ? 1.f : 0.f);
	}

	for (int i = 0; i < numWheels; i++) {
		v.push_back(state->getWheelStarted(i) ? 1.f : 0.f);
	}

	v.push_back(state->getTargetHit() ? 1.f : 0.f);

	return v;
}

int DQNAgent::selectAction(const std::vector<float>& stateVector, double epsilon, const std::unordered_set<int>& forbiddenActions)
{
	std::vector<int> allowed;
	allowed.reserve(this->numActions_);
	for (int i = 0; i < this->numActions_; i++) {
		if (forbiddenActions.find(i) == forbiddenActions.end()) {
			allowed.push_back(i);
		}
	}

	if (allowed.empty())return -1;

	std::uniform_real_distribution<double> uDist(0.0, 1.0);
	if (uDist(rng_) < epsilon)
	{
		std::uniform_int_distribution<int> aDist(0, static_cast<int>(allowed.size()) - 1);
		return allowed[aDist(rng_)];
	}

	//forward pass
	std::vector<float> qValues = this->online_.evaluate(stateVector);

	//mask forbidden actions
	for (int i = 0; i < this->numActions_; i++) {
		if (forbiddenActions.count(i)) {
			qValues[i] = -std::numeric_limits<float>::infinity();
		}
	}

	//argmax
	float bestQ = -std::numeric_limits<float>::infinity();
	for (int a : allowed)
		if (qValues[a] > bestQ) bestQ = qValues[a];

	std::vector<int> tied;
	for (int a : allowed)
		if (qValues[a] >= bestQ - 1e-6f) tied.push_back(a);

	std::uniform_int_distribution<int> pick(0, (int)tied.size() - 1);
	return tied[pick(this->rng_)];
}

void DQNAgent::storeTransition(const std::vector<float>& state, int action, float reward, const std::vector<float>& nextState, bool done)
{
	this->buffer_.push({ state,action,reward,nextState,done });

}

float DQNAgent::trainStep()
{
	if (!this->buffer_.ready(minBufferSize))return -1.f;

	std::vector<DQNTransition> batch = this->buffer_.sample(batchSize);

	std::vector<std::vector<float>> inputs(batchSize);
	std::vector<std::vector<float>> targetQValues(batchSize);

	for (int i = 0; i < batchSize; i++) {
		const DQNTransition& t = batch[i];

		//Current predictions for state s from online network
		std::vector<float> currentQValues = this->online_.evaluate(t.state);

		targetQValues[i] = currentQValues;
		if (t.done) {
			targetQValues[i][t.action] = t.reward;
		}
		else {
			std::vector<float> nextQValues = this->target_.evaluate(t.nextState);
			float maxQ = *std::max_element(nextQValues.begin(), nextQValues.end());
			targetQValues[i][t.action] = t.reward + this->gamma * maxQ;
		}
		inputs[i] = t.state;
	}
	this->online_.trainStep(inputs, targetQValues);

	float loss = 0.0f;
	for (int i = 0; i < batchSize; i++) {
		std::vector<float> predictions = this->online_.evaluate(inputs[i]);
		float diff = predictions[batch[i].action] - targetQValues[i][batch[i].action];
		loss += diff * diff;
	}
	this->lastLoss_ = loss / batchSize;
	this->trainStepCount_++;
	if (this->trainStepCount_ % this->targetSyncFreq==0) {
		this->target_.copyWeightsFrom(this->online_);
		std::cout << "Target DQN synced at step: " << this->trainStepCount_ << std::endl;
	}

	float newAlpha = this->linearDecay(this->alphaStart, this->alphaEnd,
		this->trainStepCount_, this->alphaDecay);
	this->online_.setLearningRate(newAlpha);

	return this->lastLoss_;
}

void DQNAgent::save(const std::string& network) const
{
	this->online_.save(network + "_online.bin");
	this->target_.save(network + "_target.bin");
	std::cout << "[DQN] Saved networks to " << network << "_*.bin" << std::endl;
}

bool DQNAgent::load(const std::string& network)
{
	bool ok = online_.load(network + "_online.bin")
		&& target_.load(network + "_target.bin");
	if (ok)
		std::cout << "[DQN] Loaded networks from " << network << "_*.bin" << std::endl;
	return ok;
}

double DQNAgent::linearDecay(double start, double end, int current, int total)
{
	if(current>=total)return end;
	double t = static_cast<double>(current) / static_cast<double>(total);
	return start + (end - start) * t;
}
