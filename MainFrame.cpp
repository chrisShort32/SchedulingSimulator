#include "MainFrame.h"
#include <wx/wx.h>
#include <vector>
#include <wx/grid.h>
#include <iomanip>
#include <wx/numformatter.h>

double AVG_WAIT_TIME;
int totalWaitTime = 0;
int stepNumber = 0;
int currentTime = 0;


MainFrame::MainFrame(const wxString& title) : wxFrame(nullptr, wxID_ANY, title)
{
	// Initialize panel
	panel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize);
	
	createControls();
	bindEventHandlers();
	createGrid();
	
}

void MainFrame::createControls()
{
	// Create fonts here if desired
	// Ready queue and label
	readyLabel = new wxStaticText(panel, wxID_ANY, "Ready Queue", wxPoint(50, 210));
	readyBox = new wxListBox(panel, wxID_ANY, wxPoint(50, 230), wxSize(120, 150), 0, NULL, wxLB_SINGLE);

	// Wait Queue and label
	waitLabel = new wxStaticText(panel, wxID_ANY, "Wait Queue", wxPoint(50, 410));
	waitBox = new wxListBox(panel, wxID_ANY, wxPoint(50, 430), wxSize(120, 150), 0, NULL, wxLB_SINGLE);

	// Processor box and label
	processorLabel = new wxStaticText(panel, wxID_ANY, "Processor", wxPoint(250, 300));
	processor = new wxListBox(panel, wxID_ANY, wxPoint(250, 320), wxSize(100, 50));

	// Choices for choice box
	wxArrayString choices;
	choices.Add("First in First Out");
	choices.Add("Round Robin");
	choices.Add("Shortest Job First");
	choices.Add("Priority");
	// Choice box
	algorithmLabel = new wxStaticText(panel, wxID_ANY, "Algorithms:", wxPoint(50, 50), wxSize(115, -1));
	algorithms = new wxChoice(panel, wxID_ANY, wxPoint(50, 75), wxSize(115, -1), choices);
	algorithms->Select(0);

	// Buttons
	addProcessButton = new wxButton(panel, wxID_ANY, "Add Process", wxPoint(400, 50), wxSize(80, 30));
	removeProcessButton = new wxButton(panel, wxID_ANY, "Remove", wxPoint(400, 90), wxSize(80, 30));
	startButton = new wxButton(panel, wxID_ANY, "Start", wxPoint(650, 50), wxSize(100, 50));
	stepButton = new wxButton(panel, wxID_ANY, "Step", wxPoint(650, 100), wxSize(100, 50));
	clearAllButton = new wxButton(panel, wxID_ANY, "Clear All", wxPoint(650, 150), wxSize(100, 50));
	IOButton = new wxButton(panel, wxID_ANY, "I/O", wxPoint(400, 350));
	IOButton->Hide();

	// Add process - attribute labels
	processIDLabel = new wxStaticText(panel, wxID_ANY, "Process ID:", wxPoint(200, 50));
	processArrivalTimeLabel = new wxStaticText(panel, wxID_ANY, "Arrival Time:", wxPoint(200, 75));
	processBurstTimeLabel = new wxStaticText(panel, wxID_ANY, "Burst Time:", wxPoint(200, 100));
	processPriorityLabel = new wxStaticText(panel, wxID_ANY, "Priority:", wxPoint(200, 125));
	quantTimeLabel = new wxStaticText(panel, wxID_ANY, "CPU Time:", wxPoint(200, 150));

	// Add process - attribute modifiers
	processID = new wxTextCtrl(panel, wxID_ANY, "Process1", wxPoint(266, 50), wxSize(115, -1), wxALIGN_LEFT);
	processArrivalTime = new wxSpinCtrl(panel, wxID_ANY, "", wxPoint(269, 75), wxSize(115, -1), wxSP_ARROW_KEYS, 0, 25, 0);
	processBurstTime = new wxSpinCtrl(panel, wxID_ANY, "", wxPoint(269, 100), wxSize(115, -1), wxSP_ARROW_KEYS, 1, 10, 3);
	processPriority = new wxSpinCtrl(panel, wxID_ANY, "", wxPoint(269, 125), wxSize(115, -1), wxSP_ARROW_KEYS, 1, 5, 1);
	processInputOutput = new wxCheckBox(panel, wxID_ANY, "I/O Required:", wxPoint(250, 175), wxSize(115, -1), wxALIGN_RIGHT);
	quantTime = new wxSpinCtrl(panel, wxID_ANY, "", wxPoint(269, 150), wxSize(115, -1), wxSP_ARROW_KEYS, 1, 6, 1);
	
	// Add process - list box that contains added processes
	createdProcessesLabel = new wxStaticText(panel, wxID_ANY, "Created Processes:", wxPoint(500, 25));
	processesBox = new wxListBox(panel, wxID_ANY, wxPoint(500, 50), wxSize(120, 150), 0, NULL, wxLB_SINGLE);
	
	// Current time
	currentTimeLabel = new wxStaticText(panel, wxID_ANY, "Current Time:", wxPoint(250, 480));
	testCurrentTime = new wxStaticText(panel, wxID_ANY, "0", wxPoint(280, 500));
	testCurrentTime->SetForegroundColour(*wxGREEN);
	wxFont font(24, wxFONTFAMILY_MODERN, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);
	testCurrentTime->SetFont(font);
	
}

void MainFrame::bindEventHandlers()
{
	// Bind Add Process button
	addProcessButton->Bind(wxEVT_BUTTON, &MainFrame::onAddProcess, this);
	
	// Bind Remove Process button
	removeProcessButton->Bind(wxEVT_BUTTON, &MainFrame::onRemoveProcess, this);

	// Bind Start button
	startButton->Bind(wxEVT_BUTTON, &MainFrame::onStart, this);

	// Bind Step Button
	stepButton->Bind(wxEVT_BUTTON, &MainFrame::onStep, this);

	// Bind Clear All button
	clearAllButton->Bind(wxEVT_BUTTON, &MainFrame::onClear, this);

	// Bind I/O button
	IOButton->Bind(wxEVT_BUTTON, &MainFrame::onIObutton, this);
}

void MainFrame::createGrid()
{
	// Results - Grid
	grid = new wxGrid(panel, wxID_ANY);
	grid->CreateGrid(7, 6);
	grid->SetCellValue(0, 0, "Process ID");
	grid->SetCellValue(0, 1, "Arrival Time");
	grid->SetCellValue(0, 2, "Burst Time");
	grid->SetCellValue(0, 3, "Finish Time");
	grid->SetCellValue(0, 4, "TAT");
	grid->SetCellValue(0, 5, "Wait Time");

	grid->HideRowLabels();
	grid->HideColLabels();

	grid->SetSize(wxSize(480, 145));
	grid->SetPosition(wxPoint(400, 400));
}

void MainFrame::addProcess()
{
	int last = createdProcessesVector.size() - 1;
	std::string id = createdProcessesVector[last].getId();
	std::string arrival = std::to_string(createdProcessesVector[last].getArrivalTime());
	std::string burst = std::to_string(createdProcessesVector[last].getBurstTime());
	std::string priority = std::to_string(createdProcessesVector[last].getPriority());
	std::string requiredIO = "No";

	if (createdProcessesVector[last].getWaitForIO())
	{
		requiredIO = "Yes";
	}
	std::string process = id + " (" + arrival + ", " + burst + ", " + priority + ", " + requiredIO + ")";
	processesBox->Append(process);
		
}

void MainFrame::removeProcess()
{
	int selectedIndex = processesBox->GetSelection();
	
	// If no process is selected, remove the last process created
	if (selectedIndex == wxNOT_FOUND)
	{
		if (!createdProcessesVector.empty())
		{
			createdProcessesVector.pop_back();
			processesBox->Delete(processesBox->GetCount() - 1);
		}
		else
		{
			return;
		}
	}
	
	// Re create the vector without the selected process - 
	// for some reason createdProcessesVector.erase(selectedIndex) did not work
	else
	{
		processesBox->Delete(selectedIndex);
		std::vector<Process>temp = createdProcessesVector;
		createdProcessesVector.clear();
		for (int i = 0; i < temp.size(); i++)
		{
			if (i != selectedIndex)
			{
				createdProcessesVector.push_back(temp[i]);
			}
		}
	}
}

void MainFrame::vectorToReadyBox(std::vector<Process>& v)
{
	for (int i = tempVector.size() + 1; i < v.size(); i++)
	{
		if (v[i].getArrivalTime() <= currentTime)
		{
			readyBox->Append(v[i].getId());
			readyQueue.push(v[i]);
			tempVector.push_back(v[i]);
			processesBox->Clear();
		}
	}
	

}
void MainFrame::sendToCPU()
{
	if (readyQueue.empty())
	{
		checkWaitQueue();
		stepNumber++;
		return;
	}
	CPUprocess = readyQueue.front();
	processor->Append(readyQueue.front().getId());
	readyBox->Delete(0);
	readyQueue.pop();
}
void MainFrame::sendToWaitBox()
{
	waitQueue.push(CPUprocess);
	waitBox->Append(CPUprocess.getId());
	processor->Delete(0);
}
void MainFrame::sendToReadyBox(Process& process)
{
	readyQueue.push(process);
	readyBox->Append(process.getId());
}
bool MainFrame::checkForIO()
{
	// process needs IO - send to wait
	if (CPUprocess.getWaitForIO())
	{
		sendToWaitBox();
		IOButton->Show();
		return true;
	}
	return false;
}
void MainFrame::checkWaitQueue()
{
	int i = 0;
	std::queue<Process>temp = waitQueue;
	while (!temp.empty())
	{
		if (temp.front().getWaitForIO())
		{
			temp.pop();
			i++;
		}
		else
		{
			sendToReadyBox(temp.front());
			waitBox->Delete(i);

		}
	}
}
void MainFrame::checkReadyQueue()
{
	if (readyQueue.empty())
	{
		IOButton->Hide();
		while (!waitQueue.empty())
		{
			waitQueue.front().setWaitForIO();
			sendToReadyBox(waitQueue.front());
			waitBox->Delete(0);
			waitQueue.pop();
		}
	}
}
void MainFrame::sortByShortest()
{
	if (readyBox->GetCount() > 1)
	{

		readyBox->Clear();
		while (!readyQueue.empty())
		{
			shortestFirst.push_back(readyQueue.front());
			readyQueue.pop();
		}

		std::sort(shortestFirst.begin(), shortestFirst.end(), shortest);
		for (int i = 0; i < shortestFirst.size(); i++)
		{
			readyQueue.push(shortestFirst[i]);
			readyBox->Append(shortestFirst[i].getId());
		}
		shortestFirst.clear();
	}

}
void MainFrame::sortByPriority()
{
	if (readyBox->GetCount() > 1)
	{

		readyBox->Clear();
		while (!readyQueue.empty())
		{
			priorityVector.push_back(readyQueue.front());
			readyQueue.pop();
		}

		std::sort(priorityVector.begin(), priorityVector.end(), highestPriority);
		for (int i = 0; i < priorityVector.size(); i++)
		{
			readyQueue.push(priorityVector[i]);
			readyBox->Append(priorityVector[i].getId());
		}
		priorityVector.clear();
	}
}
void MainFrame::disableButtons()
{
	algorithms->Disable();
	quantTime->Disable();
	processInputOutput->Disable();
	addProcessButton->Disable();
	removeProcessButton->Disable();
	processPriority->Disable();
	processArrivalTime->Disable();
	processBurstTime->Disable();
}
void MainFrame::enableButtons()
{
	algorithms->Enable();
	quantTime->Enable();
	processInputOutput->Enable();
	addProcessButton->Enable();
	removeProcessButton->Enable();
	processPriority->Enable();
	processArrivalTime->Enable();
	processBurstTime->Enable();
}
void MainFrame::finishProcessFCFS()
{
	int finishTime, waitTime, turnAroundTime;
	
	int j = finishedProcesses.size();
	processor->Delete(0);

	currentTime = max(currentTime, finishedProcesses[j-1].getArrivalTime());
	finishTime = currentTime + finishedProcesses[j-1].getBurstTime();
	turnAroundTime = finishTime - finishedProcesses[j - 1].getArrivalTime();
	
	int temp = currentTime;
	currentTime = finishTime;
	vectorToReadyBox(createdProcessesVector);
	currentTime = temp;

	if (j == 1)
	{
		waitTime = 0;
	}
	else
	{
		waitTime = currentTime - finishedProcesses[j-1].getArrivalTime();
	}

	totalWaitTime += waitTime;
	currentTime = finishTime;
	
	grid->SetCellValue(j, 0, finishedProcesses[j - 1].getId());
	grid->SetCellValue(j, 1, std::to_string(finishedProcesses[j - 1].getArrivalTime()));
	grid->SetCellValue(j, 2, std::to_string(finishedProcesses[j - 1].getBurstTime()));
	grid->SetCellValue(j, 3, std::to_string(finishTime));
	grid->SetCellValue(j, 4, std::to_string(turnAroundTime));
	grid->SetCellValue(j, 5, std::to_string(waitTime));

	AVG_WAIT_TIME = static_cast<double>(totalWaitTime) / static_cast<double>(finishedProcesses.size());
	
	wxString averageWaitMessage = "Average Wait Time: " + wxString::Format("%.3f", AVG_WAIT_TIME);
	averageWait = new wxStaticText(panel, wxID_ANY, averageWaitMessage, wxPoint(450, 550));
}
void MainFrame::finishProcessRR()
{
	int remain = CPUprocess.getRemainingTime() - quantTime->GetValue();
	
	if (remain > 0)
	{
		currentTime += quantTime->GetValue();
		vectorToReadyBox(createdProcessesVector);
		CPUprocess.setRemainingTime(remain);
		sendToReadyBox(CPUprocess);
		processor->Clear();
		finishedProcesses.pop_back();
	}
	else
	{
		int finishTime, waitTime, turnAroundtime;

		int j = finishedProcesses.size();
		processor->Delete(0);

		currentTime = max(currentTime, finishedProcesses[j - 1].getArrivalTime());
		finishTime = currentTime + finishedProcesses[j - 1].getRemainingTime();
		turnAroundtime = finishTime - finishedProcesses[j - 1].getArrivalTime();
		waitTime = turnAroundtime - finishedProcesses[j - 1].getBurstTime();
		
		totalWaitTime += waitTime;
		currentTime = finishTime;
		
		grid->SetCellValue(j, 0, finishedProcesses[j - 1].getId());
		grid->SetCellValue(j, 1, std::to_string(finishedProcesses[j - 1].getArrivalTime()));
		grid->SetCellValue(j, 2, std::to_string(finishedProcesses[j - 1].getBurstTime()));
		grid->SetCellValue(j, 3, std::to_string(finishTime));
		grid->SetCellValue(j, 4, std::to_string(turnAroundtime));
		grid->SetCellValue(j, 5, std::to_string(waitTime));

		AVG_WAIT_TIME = static_cast<double>(totalWaitTime) / static_cast<double>(finishedProcesses.size());

		wxString averageWaitMessage = "Average Wait Time: " + wxString::Format("%.3f", AVG_WAIT_TIME);
		averageWait = new wxStaticText(panel, wxID_ANY, averageWaitMessage, wxPoint(450, 550));
	}
	
}
void MainFrame::finishProcessSF()
{
	int finishTime, waitTime, turnAroundTime;

	int j = finishedProcesses.size();
	processor->Delete(0);
	currentTime = max(currentTime, finishedProcesses[j - 1].getArrivalTime());
	finishTime = currentTime + finishedProcesses[j - 1].getBurstTime();
	turnAroundTime = finishTime - finishedProcesses[j - 1].getArrivalTime();
	
	int temp = currentTime;
	currentTime = finishTime;
	vectorToReadyBox(createdProcessesVector);
	currentTime = temp;
	
	if (j == 1)
	{
		waitTime = 0;
	}
	else
	{
		waitTime = turnAroundTime - finishedProcesses[j - 1].getBurstTime();
	}

	totalWaitTime += waitTime;
	currentTime = finishTime;

	grid->SetCellValue(j, 0, finishedProcesses[j - 1].getId());
	grid->SetCellValue(j, 1, std::to_string(finishedProcesses[j - 1].getArrivalTime()));
	grid->SetCellValue(j, 2, std::to_string(finishedProcesses[j - 1].getBurstTime()));
	grid->SetCellValue(j, 3, std::to_string(finishTime));
	grid->SetCellValue(j, 4, std::to_string(turnAroundTime));
	grid->SetCellValue(j, 5, std::to_string(waitTime));

	AVG_WAIT_TIME = static_cast<double>(totalWaitTime) / static_cast<double>(finishedProcesses.size());

	wxString averageWaitMessage = "Average Wait Time: " + wxString::Format("%.3f", AVG_WAIT_TIME);
	averageWait = new wxStaticText(panel, wxID_ANY, averageWaitMessage, wxPoint(450, 550));
}
void MainFrame::finishProcessPriority()
{
	int finishTime, waitTime, turnAroundTime;

	int j = finishedProcesses.size();
	processor->Delete(0);
	currentTime = max(currentTime, finishedProcesses[j - 1].getArrivalTime());
	finishTime = currentTime + finishedProcesses[j - 1].getBurstTime();
	turnAroundTime = finishTime - finishedProcesses[j - 1].getArrivalTime();

	int temp = currentTime;
	currentTime = finishTime;
	vectorToReadyBox(createdProcessesVector);
	currentTime = temp;

	if (j == 1)
	{
		waitTime = 0;
	}
	else
	{
		waitTime = turnAroundTime - finishedProcesses[j - 1].getBurstTime();
	}
	totalWaitTime += waitTime;
	currentTime = finishTime;

	grid->SetCellValue(j, 0, finishedProcesses[j - 1].getId());
	grid->SetCellValue(j, 1, std::to_string(finishedProcesses[j - 1].getArrivalTime()));
	grid->SetCellValue(j, 2, std::to_string(finishedProcesses[j - 1].getBurstTime()));
	grid->SetCellValue(j, 3, std::to_string(finishTime));
	grid->SetCellValue(j, 4, std::to_string(turnAroundTime));
	grid->SetCellValue(j, 5, std::to_string(waitTime));

	AVG_WAIT_TIME = static_cast<double>(totalWaitTime) / static_cast<double>(finishedProcesses.size());

	wxString averageWaitMessage = "Average Wait Time: " + wxString::Format("%.3f", AVG_WAIT_TIME);
	averageWait = new wxStaticText(panel, wxID_ANY, averageWaitMessage, wxPoint(450, 550));
}

void MainFrame::onePassFCFS()
{
	
	switch (stepNumber)
	{
	case 0:
		std::sort(createdProcessesVector.begin(), createdProcessesVector.end());
		currentTime = createdProcessesVector[0].getArrivalTime();
		sendToReadyBox(createdProcessesVector[0]);
		vectorToReadyBox(createdProcessesVector);
		
		stepNumber++;
		break;
	case 1:
		sendToCPU();
		stepNumber++;
		break;
	case 2:
		if (!checkForIO())
		{
			finishedProcesses.push_back(CPUprocess);
			finishProcessFCFS();
		}
		checkReadyQueue();
		stepNumber = 1;
		break;
	}
}

void MainFrame::onePassRR()
{
	switch (stepNumber)
	{
	case 0:
		std::sort(createdProcessesVector.begin(), createdProcessesVector.end());

		sendToReadyBox(createdProcessesVector[0]);
		currentTime = createdProcessesVector[0].getArrivalTime();
		vectorToReadyBox(createdProcessesVector);
		
		stepNumber++;
		break;
	case  1:
		sendToCPU();
		stepNumber++;
		break;
	case 2:
		if (!checkForIO())
		{
			finishedProcesses.push_back(CPUprocess);
			finishProcessRR();
		}
		
		checkReadyQueue();
		stepNumber = 1;
		break;
	}
}

void MainFrame::onePassSF()
{

	switch (stepNumber)
	{
	case 0:
		std::sort(createdProcessesVector.begin(), createdProcessesVector.end());
		sendToReadyBox(createdProcessesVector[0]);
		currentTime = createdProcessesVector[0].getArrivalTime();
		vectorToReadyBox(createdProcessesVector);
		sortByShortest();
		
		stepNumber++;
		break;
	case  1:
		sortByShortest();
		sendToCPU();
		stepNumber++;
		break;
	case 2:
		if (!checkForIO())
		{
			finishedProcesses.push_back(CPUprocess);
			finishProcessSF();
		}

		checkReadyQueue();
		sortByShortest();
		
		stepNumber = 1;
		break;
	}
}

void MainFrame::onePassPriority()
{
	switch (stepNumber)
	{
	case 0:
		std::sort(createdProcessesVector.begin(), createdProcessesVector.end());
		sendToReadyBox(createdProcessesVector[0]);
		currentTime = createdProcessesVector[0].getArrivalTime();
		vectorToReadyBox(createdProcessesVector);
		sortByPriority();
		
		stepNumber++;
		break;
	case  1:
		sortByPriority();
		sendToCPU();
		stepNumber++;
		break;
	case 2:
		if (!checkForIO())
		{
			finishedProcesses.push_back(CPUprocess);
			finishProcessPriority();
		}

		checkReadyQueue();
		sortByPriority();
		
		stepNumber = 1;
		break;
	}
}

void MainFrame::onAddProcess(wxCommandEvent& evt)
{
	if (createdProcessesVector.size() >= 6)
	{
		wxLogMessage("You have reached the MAX processes (6)");
		return;
	}

	std::string id = processID->GetValue().ToStdString();
	Process process(id, processArrivalTime->GetValue(), processBurstTime->GetValue(), processPriority->GetValue());
	
	if (processInputOutput->IsChecked())
	{
		process.setWaitForIO();
	}
	createdProcessesVector.push_back(process);
	addProcess();
	processInputOutput->SetValue(false);
}

void MainFrame::onRemoveProcess(wxCommandEvent& evt)
{
	removeProcess();
}


void MainFrame::onStart(wxCommandEvent& evt)
{
	if (createdProcessesVector.empty())
	{
		return;
	}
	
	
	switch (algorithms->GetSelection())
	{
	case 0:
		while (finishedProcesses.size() != createdProcessesVector.size())
		{
			onePassFCFS();
		}
		break;
	case 1:
		while (finishedProcesses.size() != createdProcessesVector.size())
		{
			onePassRR();
		}
	case 2:
		while (finishedProcesses.size() != createdProcessesVector.size())
		{
			onePassSF();
		}
		break;
	case 3:
		while (finishedProcesses.size() != createdProcessesVector.size())
		{
			onePassPriority();
		}
		break;
	}
	
	testCurrentTime->SetLabel(std::to_string(currentTime));
	wxString averageWaitMessage = "Average Wait Time: " + wxString::Format("%.3f", AVG_WAIT_TIME);
	averageWait = new wxStaticText(panel, wxID_ANY, averageWaitMessage, wxPoint(450, 550));
}

void MainFrame::onStep(wxCommandEvent& evt)
{
	if (createdProcessesVector.empty())
	{
		return;
	}
	
	disableButtons();
	testCurrentTime->SetLabel(std::to_string(currentTime));
	
	switch (algorithms->GetSelection())
	{
	case 0:
		onePassFCFS();
		break;
	case 1:
		onePassRR();
		break;
	case 2:
		onePassSF();
		break;
	case 3:
		onePassPriority();
		break;
	}
	testCurrentTime->SetLabel(std::to_string(currentTime));
}

void MainFrame::onClear(wxCommandEvent& evt)
{
	enableButtons();
	
	processesBox->Clear();
	readyBox->Clear();
	waitBox->Clear();

	processID->SetValue("Process1");
	processArrivalTime->SetValue(1);
	processBurstTime->SetValue(3);
	processPriority->SetValue(1);
	processInputOutput->SetValue(false);
	quantTime->SetValue(1);

	grid->ClearGrid();
	grid->SetCellValue(0, 0, "Process ID");
	grid->SetCellValue(0, 1, "Arrival Time");
	grid->SetCellValue(0, 2, "Burst Time");
	grid->SetCellValue(0, 3, "Finish Time");
	grid->SetCellValue(0, 4, "TAT");
	grid->SetCellValue(0, 5, "Wait Time");

	createdProcessesVector.clear();
	finishedProcesses.clear();
	tempVector.clear();

	std::queue<Process> tempQ;
	readyQueue = tempQ;
	waitQueue = tempQ;

	processor->Clear();
	stepNumber = 0;
	currentTime = 0;
	totalWaitTime = 0;
	AVG_WAIT_TIME = 0;
	wxString averageWaitMessage = "Average Wait Time: " + wxString::Format("%.3f", AVG_WAIT_TIME);
	averageWait = new wxStaticText(panel, wxID_ANY, averageWaitMessage, wxPoint(450, 550));
	testCurrentTime->SetLabel("0");	
}

void MainFrame::onIObutton(wxCommandEvent& evt)
{
	waitQueue.front().setWaitForIO();
	sendToReadyBox(waitQueue.front());
	waitBox->Delete(0);
	waitQueue.pop();
	IOButton->Hide();
}

int MainFrame::max(int a, int b)
{
	if (a < b)
	{
		return b;
	}
	return a;
}