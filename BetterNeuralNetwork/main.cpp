#include <iostream>
#include "NeuralNetworkLibrary.h"
#include <unordered_map>


int main()
{
	NNLib::NeuralNetwork<double> nn(std::vector<int>{ 2, 2, 1 });
	std::vector<std::vector<double>> inputs{ {1,0}, {0,1}, {0,0}, {1,1} };
	std::vector<double> targets{ 1,1,0,0 };
	nn.setActivationFunction(&NNLib::sigmoidFunction, &NNLib::dSigmoidFunction);
	std::vector<std::vector<double>> outputs;

	for (int i = 0; i < 4; ++i)
		nn.feedForward(inputs[i]);

	//std::cout << "1 a 0: " << outputs[0][0] << std::endl;
	
	for (int i = 0; i < 1000; ++i)
	{
		//for (int j = 0;j < 4;j++)
		nn.backPropagation(targets);

	}
	
	for (int i = 0; i < 4; ++i)
		outputs.push_back(nn.feedForward(inputs[i]));
	

	std::cout << "1 a 0: "<< outputs[0][0] << std::endl;
	std::cout << "0 a 1: "<< outputs[1][0] << std::endl;
	std::cout << "0 a 0: "<< outputs[2][0] << std::endl;
	std::cout << "1 a 1: "<< outputs[3][0] << std::endl;
	
}