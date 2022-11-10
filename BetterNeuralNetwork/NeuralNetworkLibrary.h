#pragma once
#include <math.h>
#include "MatrixClass.h"
#include <vector>

namespace NNLib {

	std::vector<double> softMax(std::vector<float> NNoutputValues, int inSize, bool singleProbability = false)
	{
		double* exp_values = new double[inSize];
		double add_exp_values = 0;
		for (auto i = 0; i < inSize; ++i) {
			add_exp_values += exp_values[i] = exp(double(NNoutputValues[i])); // secte vsechny hodnoty do jedne a rozdeli je do dynamickeho pole

		}

		std::vector<double> normalize_exp_values;
		std::vector<double> add_normalize_exp_values(1); // or std::vector<double> add_normalize_exp_values = std::vector<double>(1);
		for (auto i = 0; i < inSize; ++i) {
			add_normalize_exp_values[0] += exp_values[i] / add_exp_values;
			normalize_exp_values.push_back(exp_values[i] / add_exp_values);

		}

		if (singleProbability == false)
			return add_normalize_exp_values;
		else {
			return normalize_exp_values;
		}
	}

	template<typename Base, typename T>
	inline bool instanceof(const T*) {
		return std::is_base_of<Base, T>::value;
	}


		double sigmoidFunction(double x)
		{
			return 1 / (1 + exp(-x));
		}

		double dSigmoidFunction(double y)
		{
			return y * (1 - y);
		}

		double hyperbolicTanhgens(double x)
		{
			return tanh(x);
		}

		double dHyperbolicTanhgens(double y)
		{
			return 1 - (y * y);
		}
	

	template<typename NNType>
	class NeuralNetwork
	{
	private:
		std::vector<int> layers_count;

		mxLib::Matrix<NNType>* matrix_of_layers;
		mxLib::Matrix<NNType>* matrix_of_weights;
		mxLib::Matrix<NNType>* matrix_of_bias;

		mxLib::Matrix<NNType>* matrix_of_inputs;
		mxLib::Matrix<NNType>* matrix_of_targets;

		mxLib::Matrix<NNType>* matrix_of_gradients;
		mxLib::Matrix<NNType>* matrix_of_errors;

		mxLib::Matrix<NNType>* matrix_of_layersForTranspose;
		mxLib::Matrix<NNType>* matrix_of_weightsForTranspose;
		mxLib::Matrix<NNType>* matrix_of_deltaWeights;

		//using ActivationFunction = NNType(ActivationFunctions<NNType>::*)(NNType);
		using ActivationFunction = NNType(*)(NNType);
		ActivationFunction func, dFunc;

		float learningRate = 0.01;

	public:

		NeuralNetwork(std::vector<int> layers)
		{
			layers_count = layers;

			if (layers_count.size() > 1) {
				matrix_of_weights = new mxLib::Matrix<NNType>[layers_count.size()-1];
				matrix_of_layers = new mxLib::Matrix<NNType>[layers_count.size()];
				matrix_of_bias = new mxLib::Matrix<NNType>[layers_count.size()-1];
				for (int i = 0; i < layers_count.size()-1; ++i) {

					matrix_of_layers[i] = mxLib::Matrix<NNType>(layers_count[i + 1], layers_count[i]);
					matrix_of_weights[i] = mxLib::Matrix<NNType>(layers_count[i + 1], layers_count[i]);
					matrix_of_bias[i] = mxLib::Matrix<NNType>(layers_count[i + 1], layers_count[i]);

				}
				for (int i = 0; i < layers_count.size()-1; ++i) {
					matrix_of_bias[i].setRandomValue(-1, 1);
					matrix_of_weights[i].setRandomValue(-1, 1);
				}

			}
			else {
				matrix_of_weights = new mxLib::Matrix<NNType>[layers_count.size()];
				matrix_of_layers = new mxLib::Matrix<NNType>[layers_count.size()];
				matrix_of_weights = new mxLib::Matrix<NNType>(layers_count[0], 1);
				matrix_of_layers = new mxLib::Matrix<NNType>(layers_count[0], 1);
				
				matrix_of_bias[layers_count.size()-1].setRandomValue(-1, 1);
				matrix_of_weights[layers_count.size()-1].setRandomValue(-1, 1);
			}

			matrix_of_inputs = new mxLib::Matrix<NNType>();
			matrix_of_targets = new mxLib::Matrix<NNType>();
		}

		~NeuralNetwork()
		{
			delete[] matrix_of_layers;
			delete[] matrix_of_weights;
			delete[] matrix_of_bias;
			delete matrix_of_inputs;
			delete matrix_of_targets;
		}

		std::vector<NNType> feedForward(std::vector<NNType> inputs)
		{
			matrix_of_inputs->fromArray(inputs, inputs.size());
			for (int i = 0; i < layers_count.size()-1; ++i) {
			
				matrix_of_layers[i].multiplyTwoMatrix(matrix_of_weights[i], *matrix_of_inputs);
				matrix_of_layers[i].addTwoMatrix(matrix_of_bias[i]);
				matrix_of_layers[i].map(func);

			}

			return matrix_of_layers[layers_count.size() - 2].toArray();
		}

		void backPropagation(std::vector<NNType> targets)
		{
			matrix_of_targets->fromArray(targets, targets.size());
			matrix_of_layersForTranspose = new mxLib::Matrix<NNType>[layers_count.size() - 1];
			matrix_of_errors = new mxLib::Matrix<NNType>[layers_count.size() - 1];
			matrix_of_weightsForTranspose = new mxLib::Matrix<NNType>[layers_count.size() - 1];
			matrix_of_gradients = new mxLib::Matrix<NNType>[layers_count.size() - 1];
			matrix_of_deltaWeights = new mxLib::Matrix<NNType>[layers_count.size() - 1];

			for (int i = layers_count.size()-2, j = layers_count.size() - 1; i != 0;--i,--j)
			{
				if (i == layers_count.size() - 2)
				{
					matrix_of_errors[i].subtract(*matrix_of_targets, matrix_of_layers[j]);

					matrix_of_gradients[i].map(matrix_of_layers[j], dFunc);
					matrix_of_gradients[i].multiplyTwoMatrixWithOneOther(matrix_of_errors[i]);
					matrix_of_gradients[i].multiplyWithSingleNumber(learningRate);

					matrix_of_bias[j].addTwoMatrix(matrix_of_gradients[i]);
					matrix_of_layersForTranspose[i].transpose(matrix_of_layers[i]);
					matrix_of_deltaWeights[i].multiplyTwoMatrix(matrix_of_gradients[i], *matrix_of_layersForTranspose);
					matrix_of_weights[i].addTwoMatrix(matrix_of_deltaWeights[i]);
					matrix_of_bias[i].addTwoMatrix(matrix_of_gradients[i]);
				}
				else {
					matrix_of_bias[i].addTwoMatrix(matrix_of_gradients[i]);

					matrix_of_gradients[i].map(matrix_of_layers[i], dFunc);
					matrix_of_gradients[i].multiplyTwoMatrixWithOneOther(matrix_of_errors[i]);
					matrix_of_gradients[i].multiplyWithSingleNumber(learningRate);

					matrix_of_layersForTranspose[i].transpose(matrix_of_layers[j]);
					matrix_of_deltaWeights[i].multiplyTwoMatrix(matrix_of_gradients[i], *matrix_of_layersForTranspose);
					matrix_of_weights[i].addTwoMatrix(matrix_of_deltaWeights[i]);
					matrix_of_bias[i].addTwoMatrix(matrix_of_gradients[i]);
					
				}
			}

			delete[] matrix_of_errors;
			delete[] matrix_of_layersForTranspose;
			delete[] matrix_of_weightsForTranspose;
			delete[] matrix_of_gradients;
			delete[] matrix_of_deltaWeights;
		}

		void setLearningRate(float rate) { learningRate = rate; }

		void setActivationFunction(ActivationFunction function, ActivationFunction dFunction)
		{
			/*
			if (function == &ActivationFunctions<NNType>::sigmoidFunction && dFunction == &ActivationFunctions<NNType>::dSigmoidFunction) {
				func = &ActivationFunctions<NNType>::sigmoidFunction;
				dFunc = &ActivationFunctions<NNType>::dSigmoidFunction;
			}
			else if (function == &ActivationFunctions<NNType>::hyperbolicTanhgens && dFunction == &ActivationFunctions<NNType>::dHyperbolicTanhgens) {
				func = &ActivationFunctions<NNType>::hyperbolicTanhgens;
				dFunc = &ActivationFunctions<NNType>::dHyperbolicTanhgens;
			}*/

			if (function == &sigmoidFunction && dFunction == &dSigmoidFunction) {
				func = sigmoidFunction;
				dFunc = dSigmoidFunction;
			}
			else if (function == &hyperbolicTanhgens && dFunction == &dHyperbolicTanhgens) {
				func = hyperbolicTanhgens;
				dFunc = dHyperbolicTanhgens;
			}

		}

	};
}


