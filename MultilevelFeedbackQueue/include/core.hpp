#ifndef MLFQ_CORE_HPP
#define MLFQ_CORE_HPP

#include <cstdint>
#include <memory>

#include "process.hpp"
#include "mlfq.hpp"

class Core {
private:
	MLFQ& mlfq_;

public:
	Core(MLFQ& mlfq)
		: mlfq_(mlfq)
	{

	}

	std::unique_ptr<Process> RunProcess(std::unique_ptr<Process>&&);
};


#endif