#include "stdafx.h"
#include "NeuralNetwork.h"

// Matrix functions

Matrix Matrix::operator+(const Matrix& o) const
{
	assert(this->rows == o.rows && this->columns == o.columns);
	Matrix result(this->rows, this->columns);
	for (size_t i = 0; i < this->data.size(); i++) {
		result.data[i] = this->data[i] + o.data[i];
	}
	return result;
}

Matrix Matrix::operator-(const Matrix& o) const
{
	assert(this->rows == o.rows && this->columns == o.columns);
	Matrix result(this->rows, this->columns);
	for (size_t i = 0; i < this->data.size(); i++) {
		result.data[i] = this->data[i] - o.data[i];
	}
	return result;
}

Matrix Matrix::operator*(float s) const
{
	Matrix result(this->rows, this->columns);
	for (size_t i = 0; i < this->data.size(); i++) {
		result.data[i] = this->data[i] * s;
	}
	return result;
}

Matrix Matrix::elementMul(const Matrix& o) const
{
	assert(this->rows == o.rows && this->columns == o.columns);
	Matrix result(this->rows, this->columns);
	for (size_t i = 0; i < this->data.size(); i++) {
		result.data[i] = this->data[i] * o.data[i];
	}
	return result;
}

Matrix Matrix::dotProduct(const Matrix& o) const
{
	assert(this->columns == o.rows);
	Matrix result(this->rows, o.columns, 0.0f);
	for (int i = 0; i < this->rows; i++) {
		for (int j = 0; j < this->columns; j++) {
			float element = this->at(i, j);
			for (int k = 0; k < o.columns; k++) {
				result.at(i, k) += element * o.at(j, k);
			}
		}
	}
	return result;
}

Matrix Matrix::Transpose() const
{
	Matrix result(this->columns, this->rows);
	for (int i = 0; i < this->rows; i++) {
		for (int j = 0; j < this->columns; j++) {
			result.at(j, i) = this->at(i, j);
		}
	}
	return result;
}

Matrix Matrix::columnSum() const
{
	Matrix result(1, this->rows,0.0f);
	for (int i = 0; i < this->rows; i++) {
		for (int j = 0; j < this->columns; j++) {
			result.at(0, j) += this->at(i, j);
		}
	}
	return result;
}

Matrix Matrix::createFromVector(const std::vector<float>& v)
{
	Matrix result(1, v.size());
	result.data = v;
	return result;
}

// Layer Constructor

Layer::Layer(int in, int out, std::mt19937& rng)
{
	std::normal_distribution<float> nd(0.f, std::sqrt(2.f / in));
	W = Matrix(in, out, 0.f);
	b = Matrix(1, out, 0.f);
	mW = Matrix(in, out, 0.f);
	vW = Matrix(in, out, 0.f);
	mb = Matrix(1, out, 0.f);
	vb = Matrix(1, out, 0.f);
}

// Neural Network

// Neural Network constructors

NeuralNetwork::NeuralNetwork(const std::vector<int>& layerSizes, unsigned seed)
	: layerSizes_(layerSizes), rng_(seed)
{
	assert(layerSizes.size() >= 2);
	for (size_t i = 0; i < layerSizes.size()-1; i++) {
		this->layers_.emplace_back(layerSizes[i], layerSizes[i + 1], rng_);
	}
	this->layers_fwd_ = this->layers_;
}

// Static functions for applying ReLU

Matrix NeuralNetwork::applyReLU(const Matrix& matrix)
{
	Matrix result = matrix;
	for (float& x : result.data) {
		x = NeuralNetwork::ReLU(x);
	}
	return result;
}

Matrix NeuralNetwork::applyReLU_Derivative(const Matrix& matrix)
{
	Matrix result = matrix;
	for (float& x : result.data) {
		x = NeuralNetwork::ReLU_derivative(x);
	}
	return result;
}

// Forward Pass (fill layer cahces)

Matrix NeuralNetwork::forwardPass(const Matrix& input)
{
	Matrix a = input;
	for (size_t i = 0; i < this->layers_.size(); i++) {
		Layer& L = this->layers_[i];
		L.x_cache = a;

		//z = a dot W + b
		Matrix z = a.dotProduct(L.W);
		for (int i = 0; i < z.rows; i++) {
			for (int j = 0; j < z.columns; j++) {
				z.at(i, j) += L.b.at(0, j);
			}
		}
		L.z_cache = z;

		//Apply ReLU to hidden layers, not the last
		bool isLastLayer = (i == this->layers_.size()-1);
		a = isLastLayer ? z : NeuralNetwork::applyReLU(z);
		L.a_cache = a;
	}
	return a;
}

// Evaluate (uses temporary copy of the layers)

std::vector<float> NeuralNetwork::evaluate(const std::vector<float>& input) const
{
	this->layers_fwd_ = this->layers_;

	Matrix a = Matrix::createFromVector(input);
	for (size_t i = 0; i < this->layers_fwd_.size(); i++) {
		const Layer& L = this->layers_fwd_[i];
		Matrix z = a.dotProduct(L.W);
		for (int i = 0; i < z.columns; i++) {
			z.at(0, i) += L.b.at(0, i);
		}
		bool isLast = (i == this->layers_fwd_.size() - 1);
		a = isLast ? z : NeuralNetwork::applyReLU(z);
	}
	return a.matrixToVector();
}


// Backward pass (Gradient loss + weight update)

void NeuralNetwork::backward(const Matrix& lossGradient) {
	this->adamT_++;
	float bc1 = 1.f - std::pow(this->beta1_, this->adamT_);
	float bc2 = 1.f - std::pow(this->beta2_, this->adamT_);

	Matrix dA = lossGradient;

	for (int i = this->layers_.size() - 1;i >= 0;i--) {
		Layer& L = this->layers_[i];
		bool isLast = (i == this->layers_.size() - 1);

		//dZ = dA * relu_derivative(z)
		Matrix dZ = isLast ? dA : dA.elementMul(applyReLU_Derivative(L.z_cache));

		//dW = x^T dot dZ / batch(number of rows)
		int batch = L.x_cache.rows;
		Matrix dW = L.x_cache.Transpose().dotProduct(dZ) * (1.f / batch);

		//db = mean over batch of dZ rows
		Matrix db = dZ.columnSum() * (1.f / batch);

		//propagate gradient to previoud layer
		dA = dZ.dotProduct(L.W.Transpose());

		//Adam update for W
		for (size_t i = 0;i < L.W.data.size();i++) {
			float g = dW.data[i];
			L.mW.data[i] = this->beta1_ * L.mW.data[i] + (1.f - this->beta1_) * g;
			L.vW.data[i] = this->beta2_ * L.vW.data[i] + (1.f - this->beta2_) * g * g;
			float mHat = L.mW.data[i] / bc1;
			float vHat = L.vW.data[i] / bc2;
			L.W.data[i] = L.W.data[i] - this->alpha_ * mHat / (std::sqrt(vHat) + this->eps_);

		}
		//Adam update for bias
		for (size_t i = 0;i < L.b.columns;i++) {
			float g = db.data[i];
			L.mb.data[i] = this->beta1_ * L.mb.data[i] + (1.f - this->beta1_) * g;
			L.vb.data[i] = this->beta2_ * L.vb.data[i] + (1.f - this->beta2_) * g * g;
			float mHat = L.mb.data[i] / bc1;
			float vHat = L.vb.data[i] / bc2;
			L.b.data[i] = L.b.data[i] - this->alpha_ * mHat / (std::sqrt(vHat) + this->eps_);

		}
	}
}

// Training the network (user interface)

void NeuralNetwork::trainStep(const std::vector<std::vector<float>>& inputs, const std::vector<std::vector<float>>& targets)
{
	assert(!inputs.empty());
	int batch = inputs.size();
	int inSize = inputs[0].size();
	int outSize = targets[0].size();

	//packing inputs into matrix; batch*inSize
	Matrix X(batch, inSize);
	for (int i = 0;i < batch;i++) {
		for (int j = 0;j < inSize;j++) {
			X.at(i, j) = inputs[i][j];
		}
	}

	//Forward pass
	Matrix pred = this->forwardPass(X);

	//Pack targets
	Matrix T(batch, outSize);
	for (int i = 0;i < batch;i++) {
		for (int j = 0;j < outSize;j++) {
			T.at(i, j) = targets[i][j];
		}
	}

	//MSE gradient loss: dL/dpred = 2*(pred-T)/(batch*outSize)
	//2/N constant is implemented into learning rate so I just use pred-T
	Matrix grad = pred - T;

	this->backward(grad);
}

// Target Neural Network update (weight sync)

void NeuralNetwork::copyWeightsFrom(const NeuralNetwork& src)
{
	assert(this->layerSizes_ == src.layerSizes_);
	for (size_t i = 0; i < this->layers_.size();i++) {
		this->layers_[i].W = src.layers_[i].W;
		this->layers_[i].b = src.layers_[i].b;
	}
	this->layers_fwd_ = this->layers_;
}

// Serialization

void NeuralNetwork::save(const std::string& filename) const
{
	std::ofstream f(filename, std::ios::binary);
	if (!f)return;

	//first layer sizes
	int n = this->layerSizes_.size();
	f.write(reinterpret_cast<const char*>(&n), sizeof(n));
	for (int s : this->layerSizes_) {
		f.write(reinterpret_cast<const char*>(&s), sizeof(s));
	}

	//then weights and biases
	for (const Layer& L : this->layers_) {
		int weightsSize = L.W.data.size();
		int biasesSize = L.b.data.size();
		f.write(reinterpret_cast<const char*>(&weightsSize), sizeof(weightsSize));
		f.write(reinterpret_cast<const char*>(L.W.data.data()), weightsSize * sizeof(float));
		f.write(reinterpret_cast<const char*>(&biasesSize), sizeof(biasesSize));
		f.write(reinterpret_cast<const char*>(L.b.data.data()), biasesSize * sizeof(float));
	}
	f.write(reinterpret_cast<const char*>(&this->adamT_), sizeof(this->adamT_));
}

bool NeuralNetwork::load(const std::string& filename)
{
	std::ifstream f(filename, std::ios::binary);
	if (!f)return false;

	//first layer sizes
	int n = this->layerSizes_.size();
	f.read(reinterpret_cast<char*>(&n), sizeof(n));
	std::vector<int> sizes(n);
	for (int& s : sizes) {
		f.read(reinterpret_cast<char*>(&s), sizeof(s));
	}
	if (sizes != this->layerSizes_)return false;

	//then weights and biases
	for (Layer& L : this->layers_) {
		int weightsSize;
		int biasesSize;
		f.read(reinterpret_cast<char*>(&weightsSize), sizeof(weightsSize));
		L.W.data.resize(weightsSize);
		f.read(reinterpret_cast<char*>(L.W.data.data()), weightsSize * sizeof(float));
		f.read(reinterpret_cast<char*>(&biasesSize), sizeof(biasesSize));
		L.b.data.resize(biasesSize);
		f.read(reinterpret_cast<char*>(L.b.data.data()), biasesSize * sizeof(float));
	}
	f.read(reinterpret_cast<char*>(&this->adamT_), sizeof(this->adamT_));
	this->layers_fwd_ = this->layers_;
	return true;
}