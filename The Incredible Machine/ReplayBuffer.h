#pragma once
#include "stdafx.h"


/// <summary>
/// One transition tuple which consists of:
///		- state
///		- action
///		- reward
///		- next state
///		- done (boolean)
/// </summary>
struct DQNTransition {
	std::vector<float> state;
	int action;
	float reward;
	std::vector<float> nextState;
	bool done;
};

/// <summary>
/// Circular buffer which is used to memorise transitions
/// </summary>
class ReplayBuffer {
public:
	explicit ReplayBuffer(int capacity, unsigned seed = 1234u)
		: capacity_(capacity), rng_(seed) {
		this->buffer_.reserve(capacity);
	}

	/// <summary>
	/// Adds a new transition to the buffer
	/// If buffer is full, oldest transition is removed
	/// </summary>
	/// <param name="t">Transition to be added</param>
	void push(const DQNTransition& t) {
		if (this->buffer_.size() < this->capacity_) {
			this->buffer_.push_back(t);
		}
		else {
			this->buffer_[this->head_] = t;
		}
		this->head_ = (this->head_ + 1) % this->capacity_;
	}

	/// <summary>
	/// Sample transitions uniformly
	/// </summary>
	/// <param name="batchSize">batch size of returned transitions</param>
	/// <returns>vector of DQNTransitions from buffer</returns>
	std::vector<DQNTransition> sample(int batchSize) {
		if (this->buffer_.size() < batchSize) {
			throw std::runtime_error("ReplayBuffer error: not enough samples");
		}
		std::uniform_int_distribution<int> dist(0, this->buffer_.size()-1);
		std::vector<DQNTransition> batch;
		batch.reserve(batchSize);
		for (int i = 0; i < batchSize; i++) {
			batch.push_back(this->buffer_[dist(this->rng_)]);
		}
		return batch;
	}

	int size() const { return this->buffer_.size(); }
	int capacity() const { return this->capacity_; }
	bool ready(int minSamples) const { return this->size() >= minSamples; }
private:
	int capacity_;
	std::vector<DQNTransition> buffer_;
	int head_ = 0;
	std::mt19937 rng_;
};