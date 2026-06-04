#ifndef MLFQ_MLFQ_HPP
#define MLFQ_MLFQ_HPP

#include <cstdint>
#include <deque>
#include <vector>
#include <map>

#include "process.hpp"

struct Queue {
	std::deque<std::unique_ptr<Process>> processes;
	uint32_t timeSlice{};
};

class MLFQ {
private:
	std::vector<Queue> queues_{ 3 };
	std::map<uint32_t, std::unique_ptr<Process>> blockedProcesses_{};
	std::deque<uint32_t> blockedProcessIDs_{};

public:
	MLFQ() 
	{  
		queues_[0].timeSlice = 5;
		queues_[1].timeSlice = 10;
		queues_[2].timeSlice = 15;
	};

	void PriorityBoost(); 
	uint32_t GetTimeSlice(std::unique_ptr<Process>&)const;
	size_t GetBlockedProcessesCount()const;
	std::unique_ptr<Process> GetNextProcess();
	void InsertProcess(std::unique_ptr<Process>&&);
	void BlockProcess(std::unique_ptr<Process>&&);
	void ResumeProcess();
	void KillProcess(std::unique_ptr<Process>&&);
};

#endif 

