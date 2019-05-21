#pragma once
#include <iostream>
#include <random>

double generateRandomNumber(double first, double last)
{
	std::uniform_real_distribution<double> distribution(first, last);
	std::random_device rd;
	std::default_random_engine generator(rd());
	return distribution(generator);
}