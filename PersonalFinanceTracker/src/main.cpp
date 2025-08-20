#include <bits/stdc++.h>
using namespace std;

struct Transaction {
    string date;     // YYYY-MM-DD
    string type;     // Income or Expense
    string category; // e.g., Food, Rent
    string note;     // free text
    double amount;   // positive
};

// Using a fixed-size array to align with requirement "arrays of structures"
const int MAX_TX = 1000;
Transaction tx[MAX_TX];
int txCount = 0;

bool isValidDate(const string& d) {
    // Simple check: YYYY-MM-DD with minimal validation
    if (d.size() != 10 || d[4] != '-' || d[7] != '-') return false;
    for (int i : {0,1,2,3,5,6,8,9}) if (!isdigit(d[i])) return false;
    return true;
}

void addTransaction() {
    if (txCount >= MAX_TX) { cout << "Storage full!\n"; return; }
    Transaction t;
    cout << "Enter date (YYYY-MM-DD): "; cin >> t.date;
    if (!isValidDate(t.date)) { cout << "Invalid date format.\n"; return; }
    cout << "Type (Income/Expense): "; cin >> t.type;
    if (t.type != "Income" && t.type != "Expense") { cout << "Type must be Income or Expense.\n"; return; }
    cout << "Category: "; cin.ignore(); getline(cin, t.category);
    cout << "Note: "; getline(cin, t.note);
    cout << "Amount: "; cin >> t.amount;
    if (t.amount <= 0) { cout << "Amount must be positive.\n"; return; }
    tx[txCount++] = t;
    cout << "Added.\n";
}

void listTransactions() {
    if (txCount == 0) { cout << "No transactions.\n"; return; }
    cout << left << setw(4) << "#" << setw(12) << "Date" << setw(10) << "Type" 
         << setw(14) << "Category" << setw(10) << "Amount" << "Note\n";
    cout << string(70, '-') << "\n";
    for (int i = 0; i < txCount; ++i) {
        cout << left << setw(4) << (i+1) << setw(12) << tx[i].date << setw(10) << tx[i].type
             << setw(14) << tx[i].category << setw(10) << fixed << setprecision(2) << tx[i].amount
             << tx[i].note << "\n";
    }
}

void filterExpensesOver() {
    double thr; cout << "Show expenses over amount: "; cin >> thr;
    bool any=false;
    cout << "Expenses > " << thr << ":\n";
    for (int i = 0; i < txCount; ++i) {
        if (tx[i].type=="Expense" && tx[i].amount > thr) {
            any=true;
            cout << tx[i].date << " | " << tx[i].category << " | " << tx[i].amount << " | " << tx[i].note << "\n";
        }
    }
    if(!any) cout << "None.\n";
}

void searchByKeyword() {
    cout << "Keyword in category/note: ";
    string key; cin.ignore(); getline(cin, key);
    string keylow = key; transform(keylow.begin(), keylow.end(), keylow.begin(), ::tolower);
    bool any=false;
    for (int i=0;i<txCount;++i){
        string c = tx[i].category, n = tx[i].note;
        transform(c.begin(), c.end(), c.begin(), ::tolower);
        transform(n.begin(), n.end(), n.begin(), ::tolower);
        if (c.find(keylow)!=string::npos || n.find(keylow)!=string::npos){
            any=true;
            cout << tx[i].date << " | " << tx[i].type << " | " << tx[i].category << " | " 
                 << tx[i].amount << " | " << tx[i].note << "\n";
        }
    }
    if(!any) cout << "No matches.\n";
}

int dateToInt(const string& d){ // YYYYMMDD as int for sorting
    string s = d.substr(0,4)+d.substr(5,2)+d.substr(8,2);
    return stoi(s);
}

void sortMenu(){
    cout << "Sort by: 1) Amount asc 2) Amount desc 3) Date asc 4) Date desc : ";
    int c; cin >> c;
    vector<int> idx(txCount);
    iota(idx.begin(), idx.end(), 0);
    auto byAmtAsc = [&](int a,int b){return tx[a].amount < tx[b].amount;};
    auto byAmtDesc = [&](int a,int b){return tx[a].amount > tx[b].amount;};
    auto byDateAsc = [&](int a,int b){return dateToInt(tx[a].date) < dateToInt(tx[b].date);};
    auto byDateDesc= [&](int a,int b){return dateToInt(tx[a].date) > dateToInt(tx[b].date);};
    if(c==1) stable_sort(idx.begin(), idx.end(), byAmtAsc);
    else if(c==2) stable_sort(idx.begin(), idx.end(), byAmtDesc);
    else if(c==3) stable_sort(idx.begin(), idx.end(), byDateAsc);
    else stable_sort(idx.begin(), idx.end(), byDateDesc);
    cout << left << setw(4) << "#" << setw(12) << "Date" << setw(10) << "Type" 
         << setw(14) << "Category" << setw(10) << "Amount" << "Note\n";
    cout << string(70, '-') << "\n";
    for (int k=0;k<idx.size();++k){
        int i = idx[k];
        cout << left << setw(4) << (k+1) << setw(12) << tx[i].date << setw(10) << tx[i].type
             << setw(14) << tx[i].category << setw(10) << fixed << setprecision(2) << tx[i].amount
             << tx[i].note << "\n";
    }
}

void saveToFile(){
    ofstream out("finance_data.txt");
    if(!out){ cout << "Failed to open file.\n"; return; }
    for (int i=0;i<txCount;++i){
        out << tx[i].date << "|" << tx[i].type << "|" << tx[i].category << "|" 
            << tx[i].note << "|" << fixed << setprecision(2) << tx[i].amount << "\n";
    }
    cout << "Saved to finance_data.txt\n";
}

void loadFromFile(){
    ifstream in("finance_data.txt");
    if(!in){ cout << "No file found.\n"; return; }
    txCount = 0;
    string line;
    while (getline(in, line)){
        if(line.empty()) continue;
        stringstream ss(line);
        string part;
        Transaction t;
        getline(ss, t.date, '|');
        getline(ss, t.type, '|');
        getline(ss, t.category, '|');
        getline(ss, t.note, '|');
        ss >> t.amount;
        if (txCount < MAX_TX) tx[txCount++] = t;
    }
    cout << "Loaded " << txCount << " transactions.\n";
}

void asciiMonthlyChart(){
    // Sum expenses by month (1..12)
    double sumMonth[13]={0};
    for (int i=0;i<txCount;++i){
        if (tx[i].type!="Expense") continue;
        int m = stoi(tx[i].date.substr(5,2));
        sumMonth[m] += tx[i].amount;
    }
    double maxv=0; for (int m=1;m<=12;++m) maxv=max(maxv, sumMonth[m]);
    if (maxv==0){ cout << "No expense data to chart.\n"; return; }
    cout << "ASCII Bar Chart (Expense by Month)\n";
    for (int m=1;m<=12;++m){
        int bars = (int) round((sumMonth[m]/maxv)*50); // up to 50 chars
        cout << setw(2) << m << " | " << string(bars,'#') << " " << fixed << setprecision(2) << sumMonth[m] << "\n";
    }
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    while (true){
        cout << "\n==== Personal Finance Tracker ====\n";
        cout << "1) Add Transaction\n2) List All\n3) Filter: Expenses over amount\n4) Search by keyword\n";
        cout << "5) Sort & Show\n6) Save to file\n7) Load from file\n8) ASCII monthly chart\n9) Exit\nChoice: ";
        int ch; if(!(cin>>ch)) break;
        switch(ch){
            case 1: addTransaction(); break;
            case 2: listTransactions(); break;
            case 3: filterExpensesOver(); break;
            case 4: searchByKeyword(); break;
            case 5: sortMenu(); break;
            case 6: saveToFile(); break;
            case 7: loadFromFile(); break;
            case 8: asciiMonthlyChart(); break;
            case 9: cout << "Bye!\n"; return 0;
            default: cout << "Invalid.\n";
        }
    }
    return 0;
}
