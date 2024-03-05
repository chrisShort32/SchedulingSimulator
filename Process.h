#pragma once
#include <string>
class Process
{
private:
	std::string id;
	int arrivalTime;
	int burstTime;
	int remainingTime;
	int priority;
	bool waitForIO;
public:
	Process();
	Process(std::string id, int arrivalTime, int burstTime, int priority);
	
	// Setter Functions
	void setId(std::string id);
	void setArrivalTime(int arrivalTime);
	void setBurstTime(int burstTime);
	void setRemainingTime(int remainingTime);
	void setPriority(int priority);
	void setWaitForIO();

	// Getter Functions
	std::string getId() { return id; }
	int getArrivalTime() { return arrivalTime; }
	int getBurstTime() { return burstTime; }
	int getRemainingTime() { return remainingTime; }
	int getPriority() { return priority; }
	bool getWaitForIO() { return waitForIO; }
};

bool operator == (Process& p1, Process& p2);
bool operator != (Process& p1, Process& p2);

