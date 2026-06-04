#include <cstdint>
#include <memory>

#include "core.hpp"


std::unique_ptr<Process> Core::RunProcess(std::unique_ptr<Process>&& processPtr) {
	const uint32_t timeSlice = mlfq_.GetTimeSlice(processPtr);

	if (processPtr->GetRemainingRuntime() > timeSlice) {
		if (processPtr->GetRemainingBurstTime() > 0) {
			if (processPtr->GetRemainingBurstTime() > timeSlice) {
				processPtr->SetProcessState(ProcessState::RUNNING);
				processPtr->SetRemainingRuntime(processPtr->GetRemainingRuntime() - timeSlice);
				processPtr->SetRemainingBurstTime(processPtr->GetRemainingBurstTime() - timeSlice);

				if (processPtr->GetPriorityLevel() == PriorityLevel::LEVEL0) {
					processPtr->SetPriorityLevel(PriorityLevel::LEVEL1);
				}
				else if (processPtr->GetPriorityLevel() == PriorityLevel::LEVEL1) {
					processPtr->SetPriorityLevel(PriorityLevel::LEVEL2);
				}
				// Processs will be ran normally without blocking or terminating
			}
			else {
				processPtr->SetProcessState(ProcessState::BLOCKED);
				processPtr->SetRemainingRuntime(processPtr->GetRemainingRuntime() - processPtr->GetRemainingBurstTime());
				processPtr->SetRemainingBurstTime(0);
				processPtr->SetPriorityLevel(PriorityLevel::LEVEL0);
				// Process will be blocked 
			}
		}
		else {
			processPtr->SetProcessState(ProcessState::RUNNING);
			processPtr->SetRemainingRuntime(processPtr->GetRemainingRuntime() - timeSlice);
			processPtr->SetRemainingBurstTime(0);

			if (processPtr->GetPriorityLevel() == PriorityLevel::LEVEL0) {
				processPtr->SetPriorityLevel(PriorityLevel::LEVEL1);
			}
			else if (processPtr->GetPriorityLevel() == PriorityLevel::LEVEL1) {
				processPtr->SetPriorityLevel(PriorityLevel::LEVEL2);
			}
			// Processs will be ran normally without blocking or terminating
		}
	}
	else if(processPtr->GetRemainingRuntime() > processPtr->GetRemainingBurstTime()){
		processPtr->SetProcessState(ProcessState::BLOCKED);
		processPtr->SetRemainingRuntime(processPtr->GetRemainingRuntime() - processPtr->GetRemainingBurstTime());
		processPtr->SetRemainingBurstTime(0);
		processPtr->SetPriorityLevel(PriorityLevel::LEVEL0);
		// Process will be blocked 
	}
	else {
		processPtr->SetProcessState(ProcessState::TERMINATED);
		processPtr->SetRemainingRuntime(0);
		processPtr->SetRemainingBurstTime(0);
		processPtr->SetPriorityLevel(PriorityLevel::NONE);
		// Kill process
	}
	return processPtr;
}


