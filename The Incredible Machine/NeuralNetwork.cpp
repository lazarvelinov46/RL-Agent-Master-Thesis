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