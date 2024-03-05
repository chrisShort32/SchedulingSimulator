#pragma once
#include <wx/wx.h>
#include <queue>
#include <vector>
#include <string>
#include <wx/spinctrl.h>
#include <wx/grid.h>
#include "Process.h"

class MainFrame : public wxFrame
{
public:
	// Constructor
	MainFrame(const wxString& title);

	

private:
	
	Process CPUprocess;
	
	/***MEMBER FUNCTIONS***/
	
	// Initializes the user controls - makes the constructor less messy
	void createControls();
	
	// Binds the Event Handlers
	void bindEventHandlers();

	// Creates the results grid
	void createGrid();

	void addProcess();
	void removeProcess();
	void vectorToReadyBox();
	void sendToCPU();
	void sendToWaitBox();
	void sendToReadyBox(Process& process);
	void finishProcess();
	void checkForIO();
	void checkWaitQueue();
	void checkReadyQueue();
	void onePassFCFS();

	// Event Handlers
	void onAddProcess(wxCommandEvent& evt);
	void onRemoveProcess(wxCommandEvent& evt);
	void onStart(wxCommandEvent& evt);
	void onStep(wxCommandEvent& evt);
	void onClear(wxCommandEvent& evt);
	void onIObutton(wxCommandEvent& evt);

	// Maximum of two integers
	int max(int a, int b);

	/***SCHEDULING ALGORITHMS***/
	
	// First Come First Served
	void fcfs(std::queue<Process>& processes);
	// Step Version of First Come First Served
	void fcfsOnePass(std::queue<Process>& processes, int stepNumber);
	
	/***DATA STRUCTURES***/
	
	// Vector to hold created processes
	std::vector<Process> createdProcessesVector;
	std::vector<Process> finishedProcesses;
	
	// Queues for holding and displaying Processes
	std::queue<Process> readyQueue;
	std::queue<Process> waitQueue;
	
	/***USER CONTROLS***/
	
	wxPanel* panel;

	// Labels
	wxStaticText* algorithmLabel;

	wxStaticText* createdProcessesLabel;
	wxStaticText* readyLabel;
	wxStaticText* waitLabel;
	wxStaticText* processorLabel;

	wxStaticText* processIDLabel;
	wxStaticText* processArrivalTimeLabel;
	wxStaticText* processBurstTimeLabel;
	wxStaticText* processPriorityLabel;

	

	// Process Modifaction controls
	wxTextCtrl* processID;
	wxSpinCtrl* processArrivalTime;
	wxSpinCtrl* processBurstTime;
	wxSpinCtrl* processPriority;
	wxCheckBox* processInputOutput;
	
	// Boxes that are physical representations of queues
	wxListBox* processesBox;
	wxListBox* readyBox;
	wxListBox* waitBox;
	wxListBox* processor;

	// Buttons 
	wxButton* startButton;
	wxButton* stepButton;
	wxButton* addProcessButton;
	wxButton* removeProcessButton;
	wxButton* clearAllButton;
	wxButton* IOButton;

	// Algorithm options
	wxChoice* algorithms;

	// Results - Grid
	wxGrid* grid;
	
	// Results - Average Wait Time
	wxStaticText* averageWait;
};

