#ifndef MLFQ_SIMULATION_HPP
#define MLFQ_SIMULATION_HPP

#include <cstdint>
#include <memory>

#include "mlfq.hpp"
#include "process.hpp"
#include "core.hpp"

class Simulation {
private:
	MLFQ mlfq_;
	Core core_{ mlfq_ };

	uint32_t timeElapsedThisCycle_{};
	uint32_t blockedElapsedTime_{};
	uint32_t priorityElapsedTime_{};
	uint32_t totalTimeElapsed_{};

	const size_t cpuBoundProcessCount_{};
	const size_t ioBoundProcessCount_{};
	size_t totalProcessCount_{};

public:
	Simulation(size_t cpuBoundProcessCount, size_t ioBoundProcessCount) 
		: cpuBoundProcessCount_(cpuBoundProcessCount), 
		ioBoundProcessCount_(ioBoundProcessCount)
	{
		totalProcessCount_ = cpuBoundProcessCount_ + ioBoundProcessCount_;
	}

	// Simulation Functions
	void StartSimulation();

	// Print Functions
	void PrintProcess(std::unique_ptr<Process>&);
};

#endif