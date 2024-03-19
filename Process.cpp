#include "Process.h"
#include <string>

Process::Process()
{
}

Process::Process(std::string id, int arrivalTime, int burstTime, int priority)
{
	this->id = id;
	this->arrivalTime = arrivalTime;
	this->burstTime = burstTime;
	this->priority = priority;
	remainingTime = burstTime;
	waitForIO = false;
}

void Process::setId(std::string id)
{
	this->id = id;
}

void Process::setArrivalTime(int arrivalTime)
{
	this->arrivalTime = arrivalTime;
}

void Process::setBurstTime(int burstTime)
{
	this->burstTime = burstTime;
}

void Process::setRemainingTime(int remainingTime)
{
	this->remainingTime = remainingTime;
}

void Process::setPriority(int priority)
{
	this->priority = priority;
}

void Process::setWaitForIO()
{
	if (waitForIO)
	{
		waitForIO = false;
	}
	else
	{
		waitForIO = true;
	}
}

bool operator==(Process& p1, Process& p2)
{
	return
		(p1.getId() == p2.getId())
		&&
		(p1.getArrivalTime() == p2.getArrivalTime())
		&&
		(p1.getBurstTime() == p2.getBurstTime())
		&&
		(p1.getRemainingTime() == p2.getRemainingTime())
		&&
		(p1.getPriority() == p2.getPriority())
		&&
		(p1.getWaitForIO() == p2.getWaitForIO());
}

bool operator!=(Process& p1, Process& p2)
{
	return
		(p1.getId() != p2.getId())
		||
		(p1.getArrivalTime() != p2.getArrivalTime())
		||
		(p1.getBurstTime() != p2.getBurstTime())
		||
		(p1.getRemainingTime() != p2.getRemainingTime())
		||
		(p1.getPriority() != p2.getPriority())
		||
		(p1.getWaitForIO() != p2.getWaitForIO());
}

bool operator<(Process& p1, Process& p2)
{
	return p1.getArrivalTime() < p2.getArrivalTime();
}

bool shortest(Process& p1, Process& p2)
{
	return p1.getBurstTime() < p2.getBurstTime();
}

bool highestPriority(Process& p1, Process& p2)
{
	return p1.getPriority() > p2.getPriority();
}
