#include <iostream>
#include <cstdint>
#include <memory>
#include <utility>
#include <string>
#include <fstream>

#include "simulation.hpp"


std::ofstream outputFile("MLFQOutput.txt");

void Simulation::StartSimulation() {
	// Create Processes
	uint32_t processID = 1;

	for (uint32_t i = 1; i <= cpuBoundProcessCount_; ++i) {
		auto processPtr = CreateProcess(processID);
		processPtr->SetProcessType(ProcessType::CPU_BOUND);
		processPtr->SetRandomRuntime();
		processPtr->SetRandomBurstTime();
		
		mlfq_.InsertProcess(std::move(processPtr));
		++processID;
	}

	for (uint32_t i = 1; i <= ioBoundProcessCount_; ++i) {
		auto processPtr = CreateProcess(processID);
		processPtr->SetProcessType(ProcessType::IO_BOUND);
		processPtr->SetRandomRuntime();
		processPtr->SetRandomBurstTime();

		mlfq_.InsertProcess(std::move(processPtr));
		++processID;
	}
	
	// Schedule Processes
	while(totalProcessCount_){
		auto processPtr = mlfq_.GetNextProcess();
		const uint32_t remainingRuntime = processPtr->GetRemainingRuntime();

		std::cout << "---Before Running---\n\n";
		outputFile << "---Before Running---\n\n";
		PrintProcess(processPtr);

		processPtr = core_.RunProcess(std::move(processPtr));

		std::cout << "---After Running---\n\n";
		outputFile << "---After Running---\n\n";
		PrintProcess(processPtr);

		switch (processPtr->GetProcessState()) {
		case ProcessState::RUNNING:
			timeElapsedThisCycle_ = mlfq_.GetTimeSlice(processPtr);
			mlfq_.InsertProcess(std::move(processPtr));
			break;
		case ProcessState::BLOCKED:
			timeElapsedThisCycle_ = (remainingRuntime - processPtr->GetRemainingRuntime());
			processPtr->SetTotalBlockCount(1);
			processPtr->SetRandomBurstTime();
			mlfq_.BlockProcess(std::move(processPtr));
			break;
		default:
			timeElapsedThisCycle_ = remainingRuntime;
			mlfq_.KillProcess(std::move(processPtr));
			--totalProcessCount_;
			break;
		}
		totalTimeElapsed_ += timeElapsedThisCycle_;
		priorityElapsedTime_ += timeElapsedThisCycle_;
		

		mlfq_.ResumeProcess();

		if (priorityElapsedTime_ >= 100) {
			priorityElapsedTime_ = 0;
			mlfq_.PriorityBoost();
			std::cout << "\033[34mPriority Boost\033[0m\n\n";
			outputFile << "Priority Boost\n\n";
		}
	}
	std::cout << "Total Scheduler Time Elapsed: " << totalTimeElapsed_ << " ticks" << "\n";
	outputFile << "Total Scheduler Time Elapsed: " << totalTimeElapsed_ << " ticks" << "\n";
}

void Simulation::PrintProcess(std::unique_ptr<Process>& processPtrReference) {
	std::string processStateString;
	switch (static_cast<int>(processPtrReference->GetProcessState())) {
	case 0:
		std::cout << "\033[32mProcess ID: \033[0m" << processPtrReference->GetProcessId() << "\n";
		std::cout << "\033[32mProcess State: \033[0m" << "Ready" << "\n";
		std::cout << "\033[32mProcess Priority Level: \033[0m" << static_cast<int>(processPtrReference->GetPriorityLevel()) << "\n";
		std::cout << "\033[32mProcess Type: \033[0m" << ((static_cast<int>(processPtrReference->GetProcessType()) == 0) ? "CPU_BOUND" : "IO_BOUND") << "\n";
		std::cout << "\033[32mRemaining Runtime: \033[0m" << processPtrReference->GetRemainingRuntime() << "\n";
		std::cout << "\033[32mRemaining Burst Time: \033[0m" << processPtrReference->GetRemainingBurstTime() << "\n";
		std::cout << "\033[32mTotal Block Count: \033[0m" << processPtrReference->GetTotalBlockCount() << "\n\n";

		outputFile << "Process ID: " << processPtrReference->GetProcessId() << "\n";
		outputFile << "Process State: " << "Ready" << "\n";
		outputFile << "Process Priority Level: " << static_cast<int>(processPtrReference->GetPriorityLevel()) << "\n";
		outputFile << "Process Type: " << ((static_cast<int>(processPtrReference->GetProcessType()) == 0) ? "CPU_BOUND" : "IO_BOUND") << "\n";
		outputFile << "Remaining Runtime: " << processPtrReference->GetRemainingRuntime() << "\n";
		outputFile << "Remaining Burst Time: " << processPtrReference->GetRemainingBurstTime() << "\n";
		outputFile << "Total Block Count: " << processPtrReference->GetTotalBlockCount() << "\n\n";
		break;
	case 1:
		std::cout << "\033[32mProcess ID: \033[0m" << processPtrReference->GetProcessId() << "\n";
		std::cout << "\033[32mProcess State: \033[0m" << "Running" << "\n";
		std::cout << "\033[32mProcess Priority Level: \033[0m" << static_cast<int>(processPtrReference->GetPriorityLevel()) << "\n";
		std::cout << "\033[32mProcess Type: \033[0m" << ((static_cast<int>(processPtrReference->GetProcessType()) == 0) ? "CPU_BOUND" : "IO_BOUND") << "\n";
		std::cout << "\033[32mRemaining Runtime: \033[0m" << processPtrReference->GetRemainingRuntime() << "\n";
		std::cout << "\033[32mRemaining Burst Time: \033[0m" << processPtrReference->GetRemainingBurstTime() << "\n";
		std::cout << "\033[32mTotal Block Count: \033[0m" << processPtrReference->GetTotalBlockCount() << "\n\n";

		outputFile << "Process ID: " << processPtrReference->GetProcessId() << "\n";
		outputFile << "Process State: " << "Running" << "\n";
		outputFile << "Process Priority Level: " << static_cast<int>(processPtrReference->GetPriorityLevel()) << "\n";
		outputFile << "Process Type: " << ((static_cast<int>(processPtrReference->GetProcessType()) == 0) ? "CPU_BOUND" : "IO_BOUND") << "\n";
		outputFile << "Remaining Runtime: " << processPtrReference->GetRemainingRuntime() << "\n";
		outputFile << "Remaining Burst Time: " << processPtrReference->GetRemainingBurstTime() << "\n";
		outputFile << "Total Block Count: " << processPtrReference->GetTotalBlockCount() << "\n\n";
		break;
	case 2:
		std::cout << "\033[33mProcess ID: \033[0m" << processPtrReference->GetProcessId() << "\n";
		std::cout << "\033[33mProcess State: \033[0m" << "Blocked" << "\n";
		std::cout << "\033[33mProcess Priority Level: \033[0m" << static_cast<int>(processPtrReference->GetPriorityLevel()) << "\n";
		std::cout << "\033[33mProcess Type: \033[0m" << ((static_cast<int>(processPtrReference->GetProcessType()) == 0) ? "CPU_BOUND" : "IO_BOUND") << "\n";
		std::cout << "\033[33mRemaining Runtime: \033[0m" << processPtrReference->GetRemainingRuntime() << "\n";
		std::cout << "\033[33mRemaining Burst Time: \033[0m" << processPtrReference->GetRemainingBurstTime() << "\n";
		std::cout << "\033[33mTotal Block Count: \033[0m" << processPtrReference->GetTotalBlockCount() << "\n\n";

		outputFile << "Process ID: " << processPtrReference->GetProcessId() << "\n";
		outputFile << "Process State: " << "Blocked" << "\n";
		outputFile << "Process Priority Level: " << static_cast<int>(processPtrReference->GetPriorityLevel()) << "\n";
		outputFile << "Process Type: " << ((static_cast<int>(processPtrReference->GetProcessType()) == 0) ? "CPU_BOUND" : "IO_BOUND") << "\n";
		outputFile << "Remaining Runtime: " << processPtrReference->GetRemainingRuntime() << "\n";
		outputFile << "Remaining Burst Time: " << processPtrReference->GetRemainingBurstTime() << "\n";
		outputFile << "Total Block Count: " << processPtrReference->GetTotalBlockCount() << "\n\n";
		break;
	default:
		std::cout << "\033[31mProcess ID: \033[0m" << processPtrReference->GetProcessId() << "\n";
		std::cout << "\033[31mProcess State: \033[0m" << "Terminated" << "\n";
		std::cout << "\033[31mProcess Priority Level: \033[0m" << static_cast<int>(processPtrReference->GetPriorityLevel()) << "\n";
		std::cout << "\033[31mProcess Type: \033[0m" << ((static_cast<int>(processPtrReference->GetProcessType()) == 0) ? "CPU_BOUND" : "IO_BOUND") << "\n";
		std::cout << "\033[31mRemaining Runtime: \033[0m" << processPtrReference->GetRemainingRuntime() << "\n";
		std::cout << "\033[31mRemaining Burst Time: \033[0m" << processPtrReference->GetRemainingBurstTime() << "\n";
		std::cout << "\033[31mTotal Block Count: \033[0m" << processPtrReference->GetTotalBlockCount() << "\n\n";

		outputFile << "Process ID: " << processPtrReference->GetProcessId() << "\n";
		outputFile << "Process State: " << "Terminated" << "\n";
		outputFile << "Process Priority Level: " << static_cast<int>(processPtrReference->GetPriorityLevel()) << "\n";
		outputFile << "Process Type: " << ((static_cast<int>(processPtrReference->GetProcessType()) == 0) ? "CPU_BOUND" : "IO_BOUND") << "\n";
		outputFile << "Remaining Runtime: " << processPtrReference->GetRemainingRuntime() << "\n";
		outputFile << "Remaining Burst Time: " << processPtrReference->GetRemainingBurstTime() << "\n";
		outputFile << "Total Block Count: " << processPtrReference->GetTotalBlockCount() << "\n\n";
		break;
	}

	
}