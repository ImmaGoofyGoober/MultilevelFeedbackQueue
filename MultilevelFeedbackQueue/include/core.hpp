#ifndef MLFQ_CORE_HPP
#define MLFQ_CORE_HPP

#include <cstdint>
#include <memory>

#include "process.hpp"
#include "mlfq.hpp"

class Core {
private:
	const MLFQ& mlfq_;

public:
	Core(const MLFQ& mlfq)
		: mlfq_(mlfq)
	{

	}

	std::unique_ptr<Process> RunProcess(std::unique_ptr<Process>&&);
};


#endif