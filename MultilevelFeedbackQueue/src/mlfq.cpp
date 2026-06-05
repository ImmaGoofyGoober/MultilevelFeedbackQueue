#include <cstdint>
#include <memory>
#include <utility>

#include "mlfq.hpp"


void MLFQ::PriorityBoost(){
	for (size_t i = 1; i < queues_.size(); ++i) {
		for (auto& processPtr : queues_[i].processes) {
			processPtr->SetPriorityLevel(PriorityLevel::LEVEL0);
			InsertProcess(std::move(processPtr));
		}
		queues_[i].processes.clear();
	}
}

uint32_t MLFQ::GetTimeSlice(std::unique_ptr<Process>& processPtr) const{
	return queues_[static_cast<int>(processPtr->GetPriorityLevel())].timeSlice;
}

size_t MLFQ::GetBlockedProcessesCount() const{
	return blockedProcesses_.size();
}

std::unique_ptr<Process> MLFQ::GetNextProcess() {
	for (size_t i = 0; i < queues_.size(); ++i) {
		if (!queues_[i].processes.empty()) {
			std::unique_ptr<Process> processPtr = std::move(queues_[i].processes.front());
			queues_[i].processes.pop_front();
			return processPtr;
		}
	}
	return nullptr;
}

void MLFQ::InsertProcess(std::unique_ptr<Process>&& processPtr) {
	if (processPtr->GetProcessState() == ProcessState::READY || processPtr->GetProcessState() == ProcessState::RUNNING) {
		processPtr->SetProcessState(ProcessState::READY);
		int i = static_cast<int>(processPtr->GetPriorityLevel());
		queues_[i].processes.emplace_back(std::move(processPtr));
	}
}

void MLFQ::BlockProcess(std::unique_ptr<Process>&& processPtr) {
	blockedProcessIDs_.emplace_back(processPtr->GetProcessId());
	blockedProcesses_.try_emplace(processPtr->GetProcessId(), std::move(processPtr));
}

void MLFQ::ResumeProcess() {
	if (!blockedProcesses_.empty()) {
		auto it = blockedProcesses_.find(blockedProcessIDs_.front());
		if (it != blockedProcesses_.end()) {
			std::unique_ptr<Process> processPtr = std::move(it->second);
			processPtr->SetProcessState(ProcessState::READY);
			blockedProcesses_.erase(blockedProcessIDs_.front());
			blockedProcessIDs_.pop_front();
			InsertProcess(std::move(processPtr));
		}
	}
}

void MLFQ::KillProcess(std::unique_ptr<Process>&& processPtr) {
	blockedProcesses_.erase(processPtr->GetProcessId());
}


