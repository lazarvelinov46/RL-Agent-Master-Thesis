#pragma once
#include "stdafx.h"

struct Matrix {
	int rows;
	int columns;
	std::vector<float> data;

	Matrix() = default;
	Matrix(int r, int c, float val = 0.f) :
		rows(r),columns(c),data(static_cast<size_t>(r*c),val)
	{
	}

	float& at(int r, int c) { return data[r * columns + c]; }
	float at(int r, int c) const { return data[r * columns + c]; }

	Matrix operator+(const Matrix& o) const;
	Matrix operator-(const Matrix& o) const;
	Matrix operator*(float s) const;
	Matrix elementMul(const Matrix& o) const;
	Matrix dotProduct(const Matrix& o) const;
	Matrix Transpose() const;

	Matrix columnSum() const;

	static Matrix createFromVector(const std::vector<float>& v);
	std::vector<float> matrixToVector()const { return data; }
};

struct Layer {
	Matrix W, b; //weights and biases; W - (l*(l-1)) and b - (1*l)
	Matrix mW, vW, mb, vb;

	Matrix x_cache; //layer input 
	Matrix z_cache; //activation function input
	Matrix a_cache; //activation function output

	Layer() = default;
	Layer(int in, int out, std::mt19937& rng);
};

class NeuralNetwork {
public:
	/// <summary>
	/// Creates new neural network with layers, nodes, and initial random values
	/// </summary>
	/// <param name="layerSizes">
	///		vector of integers that represent layer sizes
	///		for example easy level {6,128,128,271}
	///		first element - state features
	///		last element - actions
	/// </param>
	/// <param name="seed">
	///		seed for random number generator
	/// </param>
	explicit NeuralNetwork(const std::vector<int>& layerSizes, unsigned seed = 42u);

	NeuralNetwork(const NeuralNetwork& network) = default;
	NeuralNetwork& operator=(const NeuralNetwork& network) = default;

	//Inference part

	/// <summary>
	///		Function that predicts Q values for each action in given state
	/// </summary>
	/// <param name="input">
	///		state feature vector, length of layerSizes[0]
	/// </param>
	/// <returns>
	///		estimated Q-value for each action, length of layerSizes.back()
	/// </returns>
	std::vector<float> evaluate(const std::vector<float>& input)const;

	//Training part

	/// <summary>
	/// Adam gradient step on a miniBatch
	/// </summary>
	/// <param name="inputs">
	///		(batchSize) state vectors in the batch
	/// </param>
	/// <param name="targets">
	///		(batchSize) full Q value vectors
	///		target[i][action_taken] = update formula
	///		only for action taken, others intact so gradient is zero for them
	/// </param>
	void trainStep(const std::vector<std::vector<float>>& inputs,
		const std::vector<std::vector<float>>& targets);

	//Networsk sync

	/// <summary>
	/// Copyng network weights from src to current network
	/// </summary>
	/// <param name="src">
	///		Network from which weights are copied
	/// </param>
	void copyWeightsFrom(const NeuralNetwork& src);

	//Serialization

	void save(const std::string& filename)const;
	void load(const std::string& filename);

	//Getters

	int getInputSize() const { return this->layerSizes_.front(); }
	int getOutputSize() const { return this->layerSizes_.back(); }

	//Setters

	void setLearningRate(float learningRate) { this->alpha_ = learningRate; }

private:
	std::vector<int> layerSizes_;
	std::vector<Layer> layers_;
	std::mt19937 rng_;

	float alpha_ = 1e-3f;//learning rate
	float beta1_ = 0.900f;//momentum
	float beta2_ = 0.999f;//RMSProp decay
	float eps_ = 1e-8f;
	int adamT_ = 0;

	mutable std::vector<Layer> layers_fwd_;//working copy for const evaluate

	Matrix forwardPass(const Matrix& input);//fills caches
	void backward(const Matrix& lossGrad);

	static float ReLU(float x) { return x > 0.f ? x : 0.f; }
	static float ReLU_derivative(float x) { return x > 0.f ? 1.f : 0.f; }
	static Matrix applyReLU(const Matrix& matrix);
	static Matrix applyReLU_Derivative(const Matrix& matrix);
};