#include <iostream>
#include <string>
using namespace std;

class Student {
public:
    Student(string name, string branch) : name(name), branch(branch) {}
    virtual ~Student() {}
    virtual void display() const {
        cout << "Name: " << name << ", Branch: " << branch << endl;
    }
    const string& getName() const {
        return name;
    }
    const string& getBranch() const {
        return branch;
    }
private:
    string name;
    string branch;
};

class EngineeringStudent : public Student {
public:
    EngineeringStudent(string name, string branch, string major) : Student(name, branch), major(major) {}
    void display() const override {
        cout << "Name: " << getName() << ", Branch: " << getBranch() << ", Major: " << major << endl;
    }
private:
    string major;
};

class Room {
public:
    Room(int roomNumber) : roomNumber(roomNumber) {
        for (int i = 0; i < 3; i++) {
            beds[i] = nullptr;
        }
    }
    bool addStudent(Student* student) {
        for (int i = 0; i < 3; i++) {
            if (beds[i] == nullptr) {
                beds[i] = student;
                return true;
            }
        }
        return false;
    }
    bool removeStudent(const string& name, const string& branch) {
        for (int i = 0; i < 3; i++) {
            if (beds[i] != nullptr && beds[i]->getName() == name && beds[i]->getBranch() == branch) {
                beds[i] = nullptr;
                return true;
            }
        }
        return false;
    }
    bool isFull() const {
        for (int i = 0; i < 3; i++) {
            if (beds[i] == nullptr) {
                return false;
            }
        }
        return true;
    }
    int getRoomNumber() const {
        return roomNumber;
    }
    const Student* getStudent(int index) const {
        if (index >= 0 && index < 3) {
            return beds[index];
        }
        return nullptr;
    }
private:
    int roomNumber;
    Student* beds[3];
};

class Hostel {
public:
    Hostel() {
        for (int i = 1; i <= 100; i++) {
            rooms[i - 1] = new Room(i);
        }
    }
    ~Hostel() {
        for (int i = 0; i < 100; i++) {
            delete rooms[i];
        }
    }
    void allocateRoom(Student* student) {
        for (int i = 0; i < 100; i++) {
            if (!rooms[i]->isFull()) {
                if (allStudentsSameBranch(rooms[i], student->getBranch())) {
                    if (rooms[i]->addStudent(student)) {
                        cout << student->getBranch() << " student allocated to Room " << rooms[i]->getRoomNumber() << endl;
                        allocatedStudents[allocatedCount++] = student;
                        return;
                    }
                }
            }
        }
        cout << "No suitable room available for " << student->getBranch() << " student." << endl;
    }
    void removeStudent(const string& name, const string& branch) {
        for (int i = 0; i < allocatedCount; i++) {
            if (allocatedStudents[i] != nullptr && allocatedStudents[i]->getName() == name && allocatedStudents[i]->getBranch() == branch) {
                allocatedStudents[i] = nullptr;
                break;
            }
        }
        for (int i = 0; i < 100; i++) {
            if (rooms[i]->removeStudent(name, branch)) {
                cout << "Student removed from Room " << rooms[i]->getRoomNumber() << endl;
                return;
            }
        }
        cout << "Student not found in any room." << endl;
    }
    int getAvailableBeds() const {
        int allocatedBeds = 0;
        for (int i = 0; i < 100; i++) {
            for (int j = 0; j < 3; j++) {
                if (rooms[i]->getStudent(j) != nullptr) {
                    allocatedBeds++;
                }
            }
        }
        return 300 - allocatedBeds;
    }
    void displayStudents() const {
        for (int i = 0; i < 100; i++) {
            for (int j = 0; j < 3; j++) {
                const Student* student = rooms[i]->getStudent(j);
                if (student != nullptr) {
                    cout << "Room " << rooms[i]->getRoomNumber() << ": ";
                    student->display();
                }
            }
        }
    }
private:
    Room* rooms[100];
    Student* allocatedStudents[300];  // Max 300 students can be allocated
    int allocatedCount = 0;
    bool allStudentsSameBranch(Room* room, const string& branch) {
        for (int i = 0; i < 3; i++) {
            const Student* student = room->getStudent(i);
            if (student != nullptr && student->getBranch() != branch) {
                return false;
            }
        }
        return true;
    }
};

int main() {
    Hostel collegeHostel;
    while (true) {
        int choice;
        cout << "\nHostel Accommodation Management System" << endl;
        cout << "1. Allocate Room" << endl;
        cout << "2. Remove Student" << endl;
        cout << "3. Display Students" << endl;
        cout << "4. Total Available Beds" << endl;
        cout << "5. Exit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1: {
                string name, branch, major;
                cout << "Enter student name: ";
                cin >> name;
                cout << "Enter student branch: ";
                cin >> branch;
                if (branch == "Engineering") {
                    cout << "Enter student major: ";
                    cin >> major;
                    Student* student = new EngineeringStudent(name, branch, major);
                    collegeHostel.allocateRoom(student);
                } else {
                    Student* student = new Student(name, branch);
                    collegeHostel.allocateRoom(student);
                }
                break;
            }
            case 2: {
                string name, branch;
                cout << "Enter student name to remove: ";
                cin >> name;
                cout << "Enter student branch: ";
                cin >> branch;
                collegeHostel.removeStudent(name, branch);
                break;
            }
            case 3:
                cout << "Students in the hostel:" << endl;
                collegeHostel.displayStudents();
                break;
            case 4:
                cout << "Total available beds: " << collegeHostel.getAvailableBeds() << endl;
                break;
            case 5:
                return 0;
            default:
                cout << "Invalid choice. Please try again." << endl;
                break;
        }
    }
    return 0;
}
