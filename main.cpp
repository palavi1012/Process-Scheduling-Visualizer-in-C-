#include <iostream>
#include <iomanip>
#include <vector>
#include <queue>
#include <algorithm>
#include <climits>

using namespace std;

class Process {
public:
    int pid, at, bt, pr;
    int ct, wt, tat;
    Process(int id, int arrival, int burst, int priority) {
        pid = id;
        at = arrival;
        bt = burst;
        pr = priority;
        ct = wt = tat = 0;
    }
};

void printResults(vector<Process> processes, vector<int> gantt) {
    cout << "\nProcess Table:\n";
    cout << "PID\tAT\tBT\tPR\tCT\tWT\tTAT\n";

    double totalWT = 0, totalTAT = 0;
    for (auto &p : processes) {
        cout << p.pid << "\t" << p.at << "\t" << p.bt << "\t" << p.pr
             << "\t" << p.ct << "\t" << p.wt << "\t" << p.tat << "\n";
        totalWT += p.wt;
        totalTAT += p.tat;
    }

    cout << fixed << setprecision(2);
    cout << "\nAverage Waiting Time: " << (totalWT / processes.size());
    cout << "\nAverage Turnaround Time: " << (totalTAT / processes.size()) << "\n";

    cout << "\nGantt Chart:\n";
    for (int pid : gantt) cout << "| P" << pid << " ";
    cout << "|\n0 ";
    int time = 0;
    for (int pid : gantt) {
        for (auto &p : processes) {
            if (p.pid == pid) {
                time += p.bt;
                cout << setw(3) << time << " ";
                break;
            }
        }
    }
    cout << "\n";
}


void fcfs(vector<Process> processes) {
    sort(processes.begin(), processes.end(),
         [](Process &a, Process &b) { return a.at < b.at; });

    int time = 0;
    vector<int> gantt;
    for (auto &p : processes) {
        time = max(time, p.at) + p.bt;
        p.ct = time;
        p.tat = p.ct - p.at;
        p.wt = p.tat - p.bt;
        gantt.push_back(p.pid);
    }
    printResults(processes, gantt);
}

void sjf(vector<Process> processes) {
    int time = 0, completed = 0;
    vector<int> gantt;
    vector<bool> done(processes.size(), false);

    while (completed < processes.size()) {
        int idx = -1, mn = INT_MAX;
        for (int i = 0; i < processes.size(); i++) {
            if (!done[i] && processes[i].at <= time && processes[i].bt < mn) {
                mn = processes[i].bt;
                idx = i;
            }
        }
        if (idx == -1) {
            time++;
            continue;
        }
        time += processes[idx].bt;
        processes[idx].ct = time;
        processes[idx].tat = processes[idx].ct - processes[idx].at;
        processes[idx].wt = processes[idx].tat - processes[idx].bt;
        gantt.push_back(processes[idx].pid);
        done[idx] = true;
        completed++;
    }
    printResults(processes, gantt);
}

void roundRobin(vector<Process> processes, int quantum) {
    queue<int> q;
    vector<int> remBT(processes.size());
    vector<int> gantt;

    for (int i = 0; i < processes.size(); i++) remBT[i] = processes[i].bt;

    int time = 0, completed = 0;
    q.push(0);
    vector<bool> inQueue(processes.size(), false);
    inQueue[0] = true;

    while (!q.empty()) {
        int i = q.front(); q.pop();
        gantt.push_back(processes[i].pid);

        if (remBT[i] > quantum) {
            time += quantum;
            remBT[i] -= quantum;
        } else {
            time += remBT[i];
            remBT[i] = 0;
            processes[i].ct = time;
            processes[i].tat = processes[i].ct - processes[i].at;
            processes[i].wt = processes[i].tat - processes[i].bt;
            completed++;
        }

        for (int j = 0; j < processes.size(); j++) {
            if (!inQueue[j] && processes[j].at <= time && remBT[j] > 0) {
                q.push(j);
                inQueue[j] = true;
            }
        }
        if (remBT[i] > 0) q.push(i);
    }
    printResults(processes, gantt);
}

void priorityScheduling(vector<Process> processes) {
    int time = 0, completed = 0;
    vector<int> gantt;
    vector<bool> done(processes.size(), false);

    while (completed < processes.size()) {
        int idx = -1, bestPr = INT_MIN;
        for (int i = 0; i < processes.size(); i++) {
            if (!done[i] && processes[i].at <= time && processes[i].pr > bestPr) {
                bestPr = processes[i].pr;
                idx = i;
            }
        }
        if (idx == -1) {
            time++;
            continue;
        }
        time += processes[idx].bt;
        processes[idx].ct = time;
        processes[idx].tat = processes[idx].ct - processes[idx].at;
        processes[idx].wt = processes[idx].tat - processes[idx].bt;
        gantt.push_back(processes[idx].pid);
        done[idx] = true;
        completed++;
    }
    printResults(processes, gantt);
}

int main() {
    vector<Process> processes;
    int choice;

    do {
        cout << "\n==== CPU Scheduling Simulator ====\n";
        cout << "1. Input Process Details\n";
        cout << "2. Run FCFS\n";
        cout << "3. Run SJF\n";
        cout << "4. Run Round Robin\n";
        cout << "5. Run Priority Scheduling\n";
        cout << "6. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;

        if (choice == 1) {
            processes.clear();
            int n;
            cout << "Enter number of processes: ";
            cin >> n;
            for (int i = 0; i < n; i++) {
                int at, bt, pr;
                cout << "Enter details for Process " << i+1 << ":\n";
                cout << "Arrival Time: ";
                cin >> at;
                cout << "Burst Time: ";
                cin >> bt;
                cout << "Priority: ";
                cin >> pr;
                processes.push_back(Process(i+1, at, bt, pr));
            }
        } else if (choice == 2) {
            if (processes.empty()) cout << "No process details entered!\n";
            else fcfs(processes);
        } else if (choice == 3) {
            if (processes.empty()) cout << "No process details entered!\n";
            else sjf(processes);
        } else if (choice == 4) {
            if (processes.empty()) cout << "No process details entered!\n";
            else {
                int quantum;
                cout << "Enter Time Quantum: ";
                cin >> quantum;
                roundRobin(processes, quantum);
            }
        } else if (choice == 5) {
            if (processes.empty()) cout << "No process details entered!\n";
            else priorityScheduling(processes);
        } else if (choice == 6) {
            cout << "Exiting...\n";
        } else {
            cout << "Invalid choice! Try again.\n";
        }

    } while (choice != 6);

    return 0;
}
