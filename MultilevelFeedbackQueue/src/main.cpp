#include <iostream>
#include <chrono>

#include "simulation.hpp"


int main() {
	const size_t cpuBoundProcessCount = 1;
	const size_t ioBoundProcessCount = 1;

	Simulation simulation(cpuBoundProcessCount, ioBoundProcessCount);

	const auto start = std::chrono::steady_clock::now(); 

	simulation.StartSimulation();

	const auto end = std::chrono::high_resolution_clock::now();
	const  std::chrono::duration<double> duration = end - start;

	std::cout << "Elapsed Simulation Time: " << duration << "\n";
	
	return 0;
}

