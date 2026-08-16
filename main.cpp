#include <iostream>
#include <chrono>
#include <sstream>
#include <cstring>
#include <fstream>

using namespace std;
using namespace std::chrono;

constexpr int MAX_ID_LENGTH = 20;
constexpr int MAX_NAME_LENGTH= 100;
constexpr int MAX_PROGRAMME_LENGTH = 20;

struct Student {
    char   studentID[MAX_ID_LENGTH]{};
    char   name[MAX_NAME_LENGTH]{};
    char   programme[MAX_PROGRAMME_LENGTH]{};
    int    yearOfStudy{};
    double CGPA{};
    string phoneNumber;
};

struct Node {
    Student data;
    Node*   next{};
};

class SinglyLinkedList {
    Node* head;
    Node* tail;
    int   count;

    static Node* createNode(const Student& student) {
        const auto n = new Node;
        n->data = student;
        n->next = nullptr;
        return n;
    }

public:
    SinglyLinkedList() : head(nullptr), tail(nullptr), count(0) {}
    ~SinglyLinkedList() {
        const Node* current = head;
        while (current) {
            const Node* tmp = current->next;
            delete current;
            current = tmp;
        }
        head  = nullptr;
        tail  = nullptr;
        count = 0;
    }

    [[nodiscard]] bool isEmpty() const { return head == nullptr; }

    void insertAtBeginning(const Student& s) {
        Node* n = createNode(s);
        if (!head) {
            head = tail = n;
        } else {
            n->next = head;
            head    = n;
        }
        ++count;
    }

    void insertAtEnd(const Student& s) {
        Node* n = createNode(s);
        if (!head) {
            head = tail = n;
        } else {
            tail->next = n;
            tail       = n;
        }
        ++count;
    }

    void insertAtPosition(const Student& s, const int pos) {
        if (pos <= 1 || isEmpty()) {
            insertAtBeginning(s);
            return;
        }
        if (pos > count) {
            insertAtEnd(s);
            return;
        }
        Node* current = head;
        for (int i = 1; i < pos - 1; ++i)
            current = current->next;
        Node* n       = createNode(s);
        n->next        = current->next;
        current->next  = n;
        ++count;
    }

    bool deleteByID(const char* id) {
        if (!head) {
            cerr << "[Error] List is empty. Cannot delete.\n";
            return false;
        }
        Node* current = head;
        Node* prev    = nullptr;

        while (current) {
            if (strcmp(current->data.studentID, id) == 0) {
                if (prev)
                    prev->next = current->next;
                else
                    head = current->next;      // removed head

                if (current == tail)
                    tail = prev;               // removed tail

                delete current;
                --count;
                return true;
            }
            prev    = current;
            current = current->next;
        }

        cerr << "[Error] Student ID \"" << id << "\" not found.\n";
        return false;
    }

    Node* linearSearchByID(const char* id) const {
        Node* current = head;
        while (current) {
            if (strcmp(current->data.studentID, id) == 0)
                return current;
            current = current->next;
        }
        return nullptr;
    }

    Node* linearSearchByName(const char* name) const {
        Node* current = head;
        while (current) {
            if (strcmp(current->data.name, name) == 0)
                return current;
            current = current->next;
        }
        return nullptr;
    }

    void display() const {
        if (!head) {
            cout << "[Info] List is empty.\n";
            return;
        }
        const Node* current = head;
        int id = 1;
        cout << "------------------------------------------------------------------------------\n";
        cout << "No.      ID               Name                  Prog    Year   CGPA      Phone\n";
        cout << "------------------------------------------------------------------------------\n";
        while (current) {
            cout << id++ << "\t"
                 << current->data.studentID << "\t"
                 << current->data.name  << "\t"
                 << current->data.programme << "\t"
                 << current->data.yearOfStudy << "\t"
                 << current->data.CGPA      << "\t"
                 << current->data.phoneNumber << "\n";
            current = current->next;
        }
        cout << "------------------------------------------------------------\n";
    }

    int countRecords() const {
        int   c       = 0;
        Node* current = head;
        while (current) {
            ++c;
            current = current->next;
        }
        return c;
    }

    void clear() {
        Node* current = head;
        while (current) {
            Node* tmp = current->next;
            delete current;
            current = tmp;
        }
        head  = nullptr;
        tail  = nullptr;
        count = 0;
    }

    bool loadFromCSV(const char* filename) {
        ifstream file(filename);

        if (!file.is_open()) {
            cerr << "[Error] Cannot open file: " << filename << "\n";
            return false;
        }

        clear();

        string line;
        int loaded     = 0;
        int lineNumber = 0;

        while (getline(file, line)) {
            lineNumber++;

            if (line.empty()) continue;

            if (!line.empty() && line.back() == '\r')
                line.pop_back();

            // Remove UTF-8 BOM (Excel CSV issue)
            if (lineNumber == 1 && line.size() >= 3 &&
                static_cast<unsigned char>(line[0]) == 0xEF &&
                static_cast<unsigned char>(line[1]) == 0xBB &&
                static_cast<unsigned char>(line[2]) == 0xBF) {
                line.erase(0, 3);
            }

            istringstream ss(line);
            string id, name, prog, yearStr, cgpaStr, phoneStr;

            if (!getline(ss, id,      ',')) { cerr << "[Line " << lineNumber << "] Invalid ID column\n";        continue; }
            if (!getline(ss, name,    ',')) { cerr << "[Line " << lineNumber << "] Invalid Name column\n";      continue; }
            if (!getline(ss, prog,    ',')) { cerr << "[Line " << lineNumber << "] Invalid Programme column\n"; continue; }
            if (!getline(ss, yearStr, ',')) { cerr << "[Line " << lineNumber << "] Invalid Year column\n";      continue; }
            if (!getline(ss, cgpaStr, ',')) { cerr << "[Line " << lineNumber << "] Invalid CGPA column\n";      continue; }
            if (!getline(ss, phoneStr,',')) { cerr << "[Line " << lineNumber << "] Invalid Phone column\n";     continue; }

            auto trim = [](string& s) {
                const size_t start = s.find_first_not_of(" \t\r\n");
                const size_t end   = s.find_last_not_of(" \t\r\n");
                s = (start == string::npos) ? "" : s.substr(start, end - start + 1);
            };

            trim(id); trim(name); trim(prog);
            trim(yearStr); trim(cgpaStr); trim(phoneStr);

            if (id == "StudentID" || id == "studentID" || id == "student_id")
                continue;

            if (id.empty() || name.empty()) {
                cerr << "[Line " << lineNumber << "] Empty required fields\n";
                continue;
            }

            Student s;
            strncpy(s.studentID,  id.c_str(),   MAX_ID_LENGTH - 1);
            s.studentID[MAX_ID_LENGTH - 1] = '\0';
            strncpy(s.name,       name.c_str(),  MAX_NAME_LENGTH - 1);
            s.name[MAX_NAME_LENGTH - 1] = '\0';
            strncpy(s.programme,  prog.c_str(),  MAX_PROGRAMME_LENGTH - 1);
            s.programme[MAX_PROGRAMME_LENGTH - 1] = '\0';

            try {
                s.yearOfStudy = stoi(yearStr);
                s.CGPA        = stod(cgpaStr);
            } catch (...) {
                cerr << "[Line " << lineNumber << "] Number conversion error\n";
                continue;
            }

            if (s.CGPA < 0.0 || s.CGPA > 4.0) {
                cerr << "[Line " << lineNumber << "] Invalid CGPA: " << s.CGPA << "\n";
                continue;
            }

            s.phoneNumber = phoneStr;
            insertAtEnd(s);
            loaded++;
        }

        file.close();
        cout << "[Info] Successfully loaded " << loaded
             << " records from " << filename << "\n";
        return loaded > 0;
    }

    void bubbleSort(const bool ascending = true) {
        if (!head || !head->next) return;

        bool swapped;
        do {
            swapped      = false;
            Node* current    = head;
            const Node* endPtr = nullptr;

            while (current->next != endPtr) {
                const Student& a = current->data;
                const Student& b = current->next->data;

                bool doSwap = ascending ? (a.CGPA > b.CGPA) : (a.CGPA < b.CGPA);

                if (doSwap) {
                    swap(current->data, current->next->data);
                    swapped = true;
                }
                current = current->next;
            }
            endPtr = current;
        } while (swapped);

        Node* t = head;
        while (t->next) t = t->next;
        tail = t;
    }
};

Student inputStudent() {
    Student student;
    cout << "Student ID   : "; cin >> student.studentID;
    cin.ignore();
    cout << "Full Name    : "; cin.getline(student.name, MAX_NAME_LENGTH);
    cout << "Programme    : "; cin >> student.programme;
    cout << "Year of Study: "; cin >> student.yearOfStudy;
    cout << "CGPA (0-4)   : "; cin >> student.CGPA;
    cout << "Phone Number  : "; cin >> student.phoneNumber;

    if (student.CGPA < 0.0 || student.CGPA > 4.0) {
        cerr << "[Error] Invalid CGPA. Setting to 0.0.\n";
        student.CGPA = 0.0;
    }
    return student;
}

int main() {
    SinglyLinkedList sll;
    int choice;

    while (true) {
        cout << "\n=== Singly Linked List - Student Record System ===\n"
             << "1. Insert at Beginning\n"
             << "2. Insert at End\n"
             << "3. Insert at Position\n"
             << "4. Delete by Student ID\n"
             << "5. Search by Student ID\n"
             << "6. Search by Name\n"
             << "7. Display all Records\n"
             << "8. Count all Records\n"
             << "9. Load from CSV file\n"
             << "10. Bubble Sort\n"
             << "0. Exit\n"
             << "Enter Your Choice: ";

        cin >> choice;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "\nInvalid input! Please enter a number only.\n";
            continue;
        }

        switch (choice) {
            // <========== INSERT AT BEGINNING ==========>
            case 1: {
                if (sll.isEmpty()) {
                    cerr << "[Error] Empty list required fields\n";
                    break;
                }

                Student student = inputStudent();
                if (sll.linearSearchByID(student.studentID)) {
                    cerr << "[Error] Duplicate Student ID.\n";
                } else {
                    auto start = high_resolution_clock::now();
                    sll.insertAtBeginning(student);
                    auto end = high_resolution_clock::now();
                    cout << "[OK] Inserted. Time: "
                         << duration_cast<microseconds>(end - start).count()
                         << " microseconds\n";
                }
                break;
            }

            // <========== INSERT AT END ==========>
            case 2: {
                if (sll.isEmpty()) {
                    cerr << "[Error] Empty list required fields\n";
                    break;
                }

                Student student = inputStudent();
                if (sll.linearSearchByID(student.studentID)) {
                    cerr << "[Error] Duplicate Student ID.\n";
                } else {
                    auto start = high_resolution_clock::now();
                    sll.insertAtEnd(student);
                    auto end = high_resolution_clock::now();
                    cout << "[OK] Inserted. Time: "
                         << duration_cast<microseconds>(end - start).count()
                         << " microseconds\n";
                }
                break;
            }

            // <========== INSERT AT POSITION ==========>
            case 3: {
                if (sll.isEmpty()) {
                    cerr << "[Error] Empty list required fields\n";
                    break;
                }

                int position;
                cout << "Position: "; cin >> position; cin.ignore();

                Student student = inputStudent();
                if (sll.linearSearchByID(student.studentID)) {
                    cerr << "[Error] Duplicate Student ID.\n";
                } else {
                    auto start = high_resolution_clock::now();
                    sll.insertAtPosition(student, position);
                    auto end = high_resolution_clock::now();
                    cout << "[OK] Inserted. Time: "
                         << duration_cast<microseconds>(end - start).count()
                         << " microseconds\n";
                }
                break;
            }

            // <========== DELETE BY STUDENT ID ==========>
            case 4: {
                if (sll.isEmpty()) {
                    cerr << "[Error] Empty list required fields\n";
                    break;
                }

                char ID[MAX_ID_LENGTH];
                cout << "Enter Student ID to delete: ";
                cin >> ID; cin.ignore();

                auto start    = high_resolution_clock::now();
                const bool ok = sll.deleteByID(ID);
                auto end      = high_resolution_clock::now();
                if (ok) cout << "[OK] Deleted. Time: "
                             << duration_cast<microseconds>(end - start).count()
                             << " microseconds\n";
                break;
            }

            // <========== SEARCH BY STUDENT ID ==========>
            case 5: {
                if (sll.isEmpty()) {
                    cerr << "[Error] Empty list required fields\n";
                    break;
                }

                char ID[MAX_ID_LENGTH];
                cout << "Enter Student ID to search: ";
                cin >> ID; cin.ignore();

                auto start        = high_resolution_clock::now();
                const Node* result = sll.linearSearchByID(ID);
                auto end          = high_resolution_clock::now();

                if (result) {
                    cout << "[Found] "
                         << "ID: "          << result->data.studentID   << " | "
                         << "Name: "        << result->data.name        << " | "
                         << "Programme: "   << result->data.programme   << " | "
                         << "Year: "        << result->data.yearOfStudy << " | "
                         << "CGPA: "        << result->data.CGPA        << " | "
                         << "Phone Number: "<< result->data.phoneNumber << "\n";
                } else {
                    cout << "[Not Found]\n";
                }
                cout << "Search time: "
                     << duration_cast<microseconds>(end - start).count()
                     << " microseconds\n";
                break;
            }

            // <========== SEARCH BY STUDENT NAME ==========>
            case 6: {
                if (sll.isEmpty()) {
                    cerr << "[Error] Empty list required fields\n";
                    break;
                }

                char name[MAX_NAME_LENGTH];
                cout << "Enter Student Name to search: ";
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cin.getline(name, MAX_NAME_LENGTH);

                int len = strlen(name);
                while (len > 0 && (name[len-1] == ' '  ||
                                   name[len-1] == '\t' ||
                                   name[len-1] == '\r' ||
                                   name[len-1] == '\n'))
                    name[--len] = '\0';

                auto start        = high_resolution_clock::now();
                const Node* result = sll.linearSearchByName(name);
                auto end          = high_resolution_clock::now();

                if (result) {
                    cout << "[Found] "
                         << "ID: "          << result->data.studentID   << " | "
                         << "Name: "        << result->data.name        << " | "
                         << "Programme: "   << result->data.programme   << " | "
                         << "Year: "        << result->data.yearOfStudy << " | "
                         << "CGPA: "        << result->data.CGPA        << " | "
                         << "Phone Number: "<< result->data.phoneNumber << "\n";
                } else {
                    cout << "[Not Found]\n";
                }
                cout << "Search time: "
                     << duration_cast<microseconds>(end - start).count()
                     << " microseconds\n";
                break;
            }

            // <========== DISPLAY ALL RECORDS ==========>
            case 7: {
                auto start = high_resolution_clock::now();
                sll.display();
                auto end      = high_resolution_clock::now();
                auto duration = duration_cast<microseconds>(end - start);
                cout << "\nDisplay Time: " << duration.count() << " microseconds\n";
                break;
            }

            // <========== COUNT ALL RECORDS ==========>
            case 8: {
                if (sll.isEmpty()) {
                    cerr << "[Error] Empty list required fields\n";
                    break;
                }

                auto start    = high_resolution_clock::now();
                const int c   = sll.countRecords();
                auto end      = high_resolution_clock::now();
                auto duration = duration_cast<microseconds>(end - start);

                cout << "\nTotal Records: " << c << endl;
                cout << "Counting Time: "  << duration.count() << " microseconds\n";
                break;
            }

            // <========== LOAD CSV FILE ==========>
            case 9: {
                char file_name[256];
                cout << "Enter the name of the file (students_500.csv / students_2000.csv / students_8000.csv / students_30000.csv): ";
                cin >> file_name; cin.ignore();
                sll.loadFromCSV(file_name);
                break;
            }

            // <========== BUBBLE SORT (ASC / DESC) ==========>
            case 10: {
                if (sll.isEmpty()) { cerr << "[Error] List is empty.\n"; break; }

                int order = 0;
                while (true) {
                    cout << "Order (1 = Ascending, 2 = Descending): ";
                    cin >> order;

                    if (cin.fail()) {
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        cerr << "[Error] Invalid input. Please enter 1 or 2.\n";
                        continue;
                    }
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');

                    if (order == 1 || order == 2) break;
                    cerr << "[Error] Invalid choice. Please enter 1 (Ascending) or 2 (Descending).\n";
                }

                bool asc = (order == 1);

                auto s = high_resolution_clock::now();
                sll.bubbleSort(asc);
                auto e = high_resolution_clock::now();

                cout << "[OK] Merge Sort by CGPA ("
                     << (asc ? "Ascending" : "Descending")
                     << ") done. Time: "
                     << duration_cast<microseconds>(e - s).count() << " microseconds\n";
                sll.display();
                break;
            }

            // <========== EXIT PROGRAM ==========>
            case 0:
                cout << "Exiting program...\n";
                return 0;

            default:
                cout << "\nInvalid Choice. Please try again.\n";
        }
    }
}
