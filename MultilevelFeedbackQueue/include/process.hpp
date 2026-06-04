#ifndef MLFQ_PROCESS_HPP
#define MLFQ_PROCESS_HPP

#include <map>
#include <memory>
#include <cstdint>
#include <random>

enum struct PriorityLevel : int{
	LEVEL0,
	LEVEL1,
	LEVEL2,
	NONE = -1
};

enum struct ProcessState : int {
	READY = 0,
	RUNNING = 1,
	BLOCKED = 2,
	TERMINATED = -1
};

enum struct ProcessType : int{
	CPU_BOUND = 0,
	IO_BOUND = 1
};

class Process {
private:
	uint32_t processID_{};
	PriorityLevel priorityLevel_{ PriorityLevel::LEVEL0 };
	ProcessState processState_{ ProcessState::READY };
	ProcessType processType_{};
	uint32_t remainingRuntime_{};
	uint32_t remainingBurstTime_{};
	uint32_t totalBlockCount_{};

	std::random_device randomDevice_;

public:
	Process(const uint32_t processID) 
		: processID_(processID)
	{

	}

	// Getter Functions
	uint32_t GetProcessId()const;
	uint32_t GetRemainingRuntime()const;
	uint32_t GetRemainingBurstTime()const;
	PriorityLevel GetPriorityLevel()const;
	ProcessState GetProcessState()const;
	ProcessType GetProcessType()const;
	uint32_t GetTotalBlockCount()const;

	// Setter Functions
	void SetRemainingBurstTime(const uint32_t);
	void SetRemainingRuntime(const uint32_t);
	void SetRandomRuntime();
	void SetRandomBurstTime();
	void SetPriorityLevel(const PriorityLevel);
	void SetProcessState(const ProcessState);
	void SetProcessType(const ProcessType updateProcessType);
	void SetTotalBlockCount(const uint32_t);
};

std::unique_ptr<Process> CreateProcess(const uint32_t);

#endif