#include <cstdint>
#include <memory>
#include <random>

#include "process.hpp"


std::unique_ptr<Process> CreateProcess(const uint32_t pId) {
	return std::make_unique<Process>(pId);
}

// Getter Functions
uint32_t Process::GetProcessId() const {
	return processID_;
}

uint32_t Process::GetRemainingRuntime() const {
	return remainingRuntime_;
}

uint32_t Process::GetRemainingBurstTime()const {
	return remainingBurstTime_;
}

PriorityLevel Process::GetPriorityLevel() const {
	return priorityLevel_;
}

ProcessState Process::GetProcessState() const {
	return processState_;
}

ProcessType Process::GetProcessType() const{
	return processType_;
};

uint32_t Process::GetTotalBlockCount() const {
	return totalBlockCount_;
};

// Setter Functions
void Process::SetRemainingBurstTime(const uint32_t updateRemainingBurstTime) {
	remainingBurstTime_ = updateRemainingBurstTime;
}

void Process::SetRemainingRuntime(const uint32_t updateRemainingRuntime) {
	remainingRuntime_ = updateRemainingRuntime;
}

void Process::SetRandomRuntime() {
	std::mt19937_64 engine(randomDevice_());

	if (processType_ == ProcessType::CPU_BOUND) {
		std::uniform_int_distribution<uint32_t> cpuBoundDistribution(500, 2000);
		processType_ = ProcessType::CPU_BOUND;
		remainingRuntime_ = cpuBoundDistribution(engine);
	}
	else {
		std::uniform_int_distribution<uint32_t> ioBoundDistribution(50, 200);
		processType_ = ProcessType::IO_BOUND;
		remainingRuntime_ = ioBoundDistribution(engine);
	}
}

void Process::SetRandomBurstTime() {
	std::mt19937_64 engine(randomDevice_());

	if (processType_ == ProcessType::CPU_BOUND) {
		std::uniform_int_distribution<uint32_t> cpuBoundDistribution((remainingRuntime_ > 50) ? 50 : remainingRuntime_, (remainingRuntime_ > 500) ? 500 : remainingRuntime_);
		remainingBurstTime_ = cpuBoundDistribution(engine);
	}
	else {
		std::uniform_int_distribution<uint32_t> ioBoundDistribution(1, (remainingRuntime_ > 10) ? 10 : remainingRuntime_);
		remainingBurstTime_ = ioBoundDistribution(engine);
	}
};

void Process::SetPriorityLevel(const PriorityLevel updatedPriorityLevel) {
	priorityLevel_ = updatedPriorityLevel;
};

void Process::SetProcessState(const ProcessState updateProcessState) {
	processState_ = updateProcessState;
}

void Process::SetProcessType(const ProcessType updateProcessType) {
	processType_ = updateProcessType;
}

void Process::SetTotalBlockCount(const uint32_t updateTotalBlockCount) {
	totalBlockCount_ += updateTotalBlockCount;
};