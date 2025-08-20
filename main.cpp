#include <bits/stdc++.h>
using namespace std;

struct Patient {
    int id;
    string name;
    int severity; // 1 (low) .. 5 (high)
    long long arrival; // to break ties: smaller = earlier
};

struct CritCmp {
    bool operator()(const Patient& a, const Patient& b) const {
        // higher severity first; if tie, earlier arrival first
        if (a.severity != b.severity) return a.severity < b.severity; // max-heap by severity
        return a.arrival > b.arrival; // earlier arrival has higher priority
    }
};

queue<Patient> regularQ;
priority_queue<Patient, vector<Patient>, CritCmp> criticalQ;
long long globalArrival = 0;
int nextId = 1;

const int MIN_PER_PATIENT = 10;

void addPatient(){
    string name, type; int sev;
    cout << "Name: "; cin.ignore(); getline(cin, name);
    cout << "Type (Critical/Regular): "; cin >> type;
    if (type!="Critical" && type!="Regular"){ cout << "Invalid type.\n"; return; }
    if (type=="Critical"){
        cout << "Severity (1-5): "; cin >> sev;
        Patient p{nextId++, name, max(1,min(5,sev)), globalArrival++};
        criticalQ.push(p);
        cout << "Added to CRITICAL queue with id " << p.id << ".\n";
    } else {
        Patient p{nextId++, name, 0, globalArrival++};
        regularQ.push(p);
        cout << "Added to regular queue with id " << p.id << ".\n";
    }
}

void serveNext(){
    if (!criticalQ.empty()){
        Patient p = criticalQ.top(); criticalQ.pop();
        cout << "Serving CRITICAL patient: [" << p.id << "] " << p.name << " (sev " << p.severity << ")\n";
    } else if (!regularQ.empty()){
        Patient p = regularQ.front(); regularQ.pop();
        cout << "Serving regular patient: [" << p.id << "] " << p.name << "\n";
    } else {
        cout << "No patients waiting.\n";
    }
}

void viewQueues(){
    cout << "\n-- Critical Queue (by priority) --\n";
    vector<Patient> tmp;
    while (!criticalQ.empty()){ tmp.push_back(criticalQ.top()); criticalQ.pop(); }
    for (auto &p: tmp){
        cout << "[" << p.id << "] " << p.name << " sev:" << p.severity << "\n";
    }
    for (auto &p: tmp) criticalQ.push(p);

    cout << "\n-- Regular Queue (FIFO) --\n";
    queue<Patient> rq = regularQ;
    while (!rq.empty()){
        auto p = rq.front(); rq.pop();
        cout << "[" << p.id << "] " << p.name << "\n";
    }
}

int estimateWaitById(){
    int id; cout << "Enter patient id: "; cin >> id;
    // build order: all critical first then regular; but we need position of specific id
    // For critical id: count how many critical before it (with higher priority)
    // For regular id: all critical count + number of regulars before it
    int minutes = 0;
    bool found = false;

    // count critical before target
    vector<Patient> tmp;
    while (!criticalQ.empty()){ tmp.push_back(criticalQ.top()); criticalQ.pop(); }
    for (int i=0;i<tmp.size();++i){
        if (tmp[i].id == id){ 
            minutes = i * MIN_PER_PATIENT; 
            found = true; 
        }
    }
    // restore
    for (auto &p: tmp) criticalQ.push(p);

    if (!found){
        // find position in regular
        int pos = 0; queue<Patient> rq = regularQ;
        while (!rq.empty()){
            auto p = rq.front(); rq.pop();
            if (p.id == id){
                found = true;
                // all critical before + regulars before pos
                minutes = (int(tmp.size()) + pos) * MIN_PER_PATIENT;
                break;
            }
            pos++;
        }
    }
    if (!found){ cout << "ID not found.\n"; return -1; }
    cout << "Estimated wait time: " << minutes << " minutes.\n";
    return minutes;
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    while (true){
        cout << "\n=== Hospital Patient Queue ===\n";
        cout << "1) Add patient\n2) Serve next\n3) View queues\n4) Estimate wait by ID\n5) Exit\nChoice: ";
        int ch; if(!(cin>>ch)) break;
        switch(ch){
            case 1: addPatient(); break;
            case 2: serveNext(); break;
            case 3: viewQueues(); break;
            case 4: estimateWaitById(); break;
            case 5: cout << "Bye\n"; return 0;
            default: cout << "Invalid.\n";
        }
    }
    return 0;
}