#include <bits/stdc++.h>
using namespace std;

struct Book {
    int id;
    string title, author;
    int copies;
    Book* next;
    Book(int i, string t, string a, int c): id(i), title(std::move(t)), author(std::move(a)), copies(c), next(nullptr) {}
};

enum ActionType { ADD, DEL, BORROW, RET };
struct Action {
    ActionType type;
    Book snapshot; // snapshot of book state (id/title/author/copies), next ignored
    // for delete/add we need position; keep id sufficient for re-locating
    Action(ActionType t, const Book& b): type(t), snapshot(b) {}
};

Book* head = nullptr;
stack<Action> undoStack;

Book* findById(int id) {
    Book* cur = head;
    while (cur) {
        if (cur->id == id) return cur;
        cur = cur->next;
    }
    return nullptr;
}

void addBook() {
    int id, copies; string title, author;
    cout << "ID: "; cin >> id; cin.ignore();
    cout << "Title: "; getline(cin, title);
    cout << "Author: "; getline(cin, author);
    cout << "Copies: "; cin >> copies;
    // insert at head for simplicity (could be alphabetical insertion as extension)
    Book* b = new Book(id, title, author, copies);
    b->next = head; head = b;
    undoStack.emplace(ADD, *b);
    cout << "Book added.\n";
}

void deleteBook(){
    int id; cout << "Delete ID: "; cin >> id;
    Book* cur = head; Book* prev = nullptr;
    while (cur && cur->id != id){ prev = cur; cur = cur->next; }
    if (!cur){ cout << "Not found.\n"; return; }
    undoStack.emplace(DEL, *cur);
    if (!prev) head = cur->next;
    else prev->next = cur->next;
    delete cur;
    cout << "Deleted.\n";
}

void borrowBook(){
    int id; cout << "Borrow ID: "; cin >> id;
    Book* b = findById(id);
    if (!b){ cout << "Not found.\n"; return; }
    if (b->copies <= 0){ cout << "No copies available.\n"; return; }
    undoStack.emplace(BORROW, *b);
    b->copies--;
    cout << "Borrowed. Remaining copies: " << b->copies << "\n";
}

void returnBook(){
    int id; cout << "Return ID: "; cin >> id;
    Book* b = findById(id);
    if (!b){ cout << "Not found.\n"; return; }
    undoStack.emplace(RET, *b);
    b->copies++;
    cout << "Returned. Copies: " << b->copies << "\n";
}

void search(){
    cout << "Search by (1) Title (2) Author: ";
    int c; cin >> c; cin.ignore();
    cout << "Keyword: ";
    string key; getline(cin, key);
    string low = key; transform(low.begin(), low.end(), low.begin(), ::tolower);
    Book* cur = head; bool any=false;
    while (cur){
        string t = cur->title, a = cur->author;
        transform(t.begin(), t.end(), t.begin(), ::tolower);
        transform(a.begin(), a.end(), a.begin(), ::tolower);
        if ((c==1 && t.find(low)!=string::npos) || (c==2 && a.find(low)!=string::npos)){
            cout << "ID:" << cur->id << " | " << cur->title << " by " << cur->author << " | copies: " << cur->copies << "\n";
            any=true;
        }
        cur = cur->next;
    }
    if(!any) cout << "No matches.\n";
}

void listAll(){
    cout << "Inventory:\n";
    Book* cur = head;
    while (cur){
        cout << "ID:" << cur->id << " | " << cur->title << " by " << cur->author << " | copies: " << cur->copies << "\n";
        cur = cur->next;
    }
}

void undo(){
    if (undoStack.empty()){ cout << "Nothing to undo.\n"; return; }
    Action a = undoStack.top(); undoStack.pop();
    if (a.type == ADD){
        // undo ADD -> remove that id
        int id = a.snapshot.id;
        Book* cur = head; Book* prev=nullptr;
        while (cur && cur->id != id){ prev=cur; cur=cur->next; }
        if (cur){
            if (!prev) head = cur->next;
            else prev->next = cur->next;
            delete cur;
            cout << "Undo: removed book added.\n";
        }
    } else if (a.type == DEL){
        // undo DEL -> reinsert snapshot at head
        Book* b = new Book(a.snapshot.id, a.snapshot.title, a.snapshot.author, a.snapshot.copies);
        b->next = head; head = b;
        cout << "Undo: restored deleted book.\n";
    } else if (a.type == BORROW){
        // undo BORROW -> increment copies
        Book* b = findById(a.snapshot.id);
        if (b){ b->copies++; cout << "Undo: borrow reverted.\n"; }
    } else if (a.type == RET){
        // undo RETURN -> decrement copies (not below 0)
        Book* b = findById(a.snapshot.id);
        if (b && b->copies>0){ b->copies--; cout << "Undo: return reverted.\n"; }
    }
}

void cleanUp(){
    Book* cur = head;
    while (cur){ Book* nxt = cur->next; delete cur; cur = nxt; }
    head = nullptr;
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    while (true){
        cout << "\n=== E-Library Book Management ===\n";
        cout << "1) Add Book\n2) Delete Book\n3) Borrow\n4) Return\n5) Search\n6) List All\n7) Undo\n8) Exit\nChoice: ";
        int ch; if(!(cin>>ch)) break;
        switch(ch){
            case 1: addBook(); break;
            case 2: deleteBook(); break;
            case 3: borrowBook(); break;
            case 4: returnBook(); break;
            case 5: search(); break;
            case 6: listAll(); break;
            case 7: undo(); break;
            case 8: cleanUp(); cout << "Bye\n"; return 0;
            default: cout << "Invalid.\n";
        }
    }
    cleanUp();
    return 0;
}