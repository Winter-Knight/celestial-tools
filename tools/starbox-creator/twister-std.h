#ifndef TWISTER_H
#define TWISTER_H

#include <stdio.h>
#include <stdlib.h>
#include <random>
#include <climits>

class Twister {
public:
	bool Init();
	bool Init(uint32_t seed);
	void Free();
	uint32_t NextUInt32();
	double NextDouble();
	
	// For custom distributions
	std::mt19937 & GetGenerator() { return generator; }

private:
	void InitDistributions();

	std::mt19937 generator;
	std::uniform_int_distribution<uint32_t> distributionI;
	std::uniform_real_distribution<double>  distributionD;
};

inline void Twister::InitDistributions()
{
	std::uniform_int_distribution<uint32_t> l_distributionI(0, UINT_MAX);
	distributionI.param(l_distributionI.param());
	
	std::uniform_real_distribution<double> l_distributionD(0.0, 1.0);
	distributionD.param(l_distributionD.param());
}

inline bool Twister::Init()
{
	static const unsigned int N = 624;
	unsigned int len;

	std::vector<uint32_t> seedBuffer(N);
	
	FILE * file = fopen("/dev/urandom", "rb");
	if (!file)
		return false;
	len = fread(&seedBuffer[0], sizeof(uint32_t), N, file);
	fclose(file);
	if (len < N)
		return false;
	
	std::seed_seq seed(seedBuffer.begin(), seedBuffer.end());
	generator.seed(seed);
	InitDistributions();
	return true;
}

inline bool Twister::Init(uint32_t seed)
{
	generator.seed(seed);
	generator.discard(10000);
	InitDistributions();
	return true;
}

inline void Twister::Free()
{
	// Nothing needed here
}

inline uint32_t Twister::NextUInt32()
{
	return distributionI(generator);
}

inline double Twister::NextDouble()
{
	return distributionD(generator);
}

#endif // TWISTER_H
