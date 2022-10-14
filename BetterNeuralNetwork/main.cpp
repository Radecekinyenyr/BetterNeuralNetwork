#include <iostream>
#include "NeuralNetworkLibrary.h"
#include <unordered_map>


int main()
{
	

	NNLib::NeuralNetwork<double> nn(std::vector<int>{ 2, 2, 1 });
	std::vector<double> inputs{ 1,0 };
	nn.feedForward(inputs);
}