#include <iostream>
#include <queue>
#include <vector>
#include <string>
#include <algorithm> // For sorting

using namespace std;

// Process Control Block (PCB) Structure
struct PCB {
    int pid; // Process ID
    string processName;
    int priority; // Used for priority scheduling
    int burstTime; // Total burst time required by the process
    int arrivalTime; // Time at which the process arrives
    int remainingTime; // Remaining burst time (used for Round Robin)
    bool completed; // Flag to indicate if the process is completed
};

// Process Scheduler Class
class ProcessScheduler {
private:
    queue<PCB> readyQueue; // Queue for FCFS & Round Robin scheduling
    vector<PCB> processTable; // Stores all processes
    int pidCounter;
    int timeQuantum; // For Round Robin Scheduling
    int contextSwitchTime; // Time taken to perform context switching

public:
    ProcessScheduler(int tq = 4, int cst = 1) : pidCounter(0), timeQuantum(tq), contextSwitchTime(cst) {}

    int createProcess(string name, int priority, int burstTime, int arrivalTime) {
        PCB process = {pidCounter++, name, priority, burstTime, arrivalTime, burstTime, false};
        processTable.push_back(process);
        readyQueue.push(process);
        return process.pid;
    }

    void terminateProcess(int pid) {
        for (auto it = processTable.begin(); it != processTable.end(); ++it) {
            if (it->pid == pid) {
                processTable.erase(it);
                break;
            }
        }
    }

    void contextSwitch() {
        cout << "Context Switching... Taking " << contextSwitchTime << " units of time." << endl;
    }

    void FCFS() {
        cout << "\nFCFS Scheduling:\n";
        int currentTime = 0;
        for (auto &process : processTable) {
            if (process.arrivalTime <= currentTime && !process.completed) {
                cout << "Running Process " << process.processName << " (PID: " << process.pid << ")\n";
                currentTime += process.burstTime;
                contextSwitch();
                currentTime += contextSwitchTime;
                process.completed = true;
            }
        }
    }

    void RoundRobin() {
        cout << "\nRound Robin Scheduling (Time Quantum = " << timeQuantum << "):\n";
        int currentTime = 0;
        while (!readyQueue.empty()) {
            PCB process = readyQueue.front();
            readyQueue.pop();

            if (process.remainingTime > 0) {
                int execTime = min(timeQuantum, process.remainingTime);
                cout << "Running Process " << process.processName << " (PID: " << process.pid << ") for " << execTime << " units\n";
                process.remainingTime -= execTime;
                currentTime += execTime;
                contextSwitch();
                currentTime += contextSwitchTime;

                if (process.remainingTime > 0) {
                    readyQueue.push(process); // Reinsert into queue if not finished
                }
            }
        }
    }

    void PriorityScheduling() {
        cout << "\nPriority Scheduling:\n";
        sort(processTable.begin(), processTable.end(), [](const PCB &a, const PCB &b) {
            return a.priority < b.priority; // Lower priority value means higher priority
        });

        int currentTime = 0;
        for (auto &process : processTable) {
            if (!process.completed) {
                cout << "Running Process " << process.processName << " (PID: " << process.pid << ") with Priority: " << process.priority << "\n";
                currentTime += process.burstTime;
                contextSwitch();
                currentTime += contextSwitchTime;
                process.completed = true;
            }
        }
    }

    void displayProcesses() {
        cout << "\nProcess Table:\n";
        for (const auto& process : processTable) {
            cout << 
            "PID: " << process.pid << ", Name: " << process.processName
            << ", Priority: " << process.priority << ", Burst Time: " 
            << process.burstTime << ", Arrival Time: " << process.arrivalTime 
            << endl;
        }
    }
};

int main() {
    ProcessScheduler scheduler(4, 1); // Time Quantum = 4, Context Switch Time = 1

    // Creating sample processes
    scheduler.createProcess("Process1", 1, 10, 0);
    scheduler.createProcess("Process2", 2, 5, 2);
    scheduler.createProcess("Process3", 0, 8, 4); // Higher priority (0 means highest)

    scheduler.displayProcesses();

    scheduler.FCFS();
    scheduler.RoundRobin();
    scheduler.PriorityScheduling();

    return 0;
}
