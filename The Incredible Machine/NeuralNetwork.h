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