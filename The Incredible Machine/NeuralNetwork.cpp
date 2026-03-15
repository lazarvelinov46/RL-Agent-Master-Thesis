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
