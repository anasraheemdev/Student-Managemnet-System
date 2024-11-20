#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <limits>
#include <iomanip>
using namespace std;

class Student {
private:
    string name;
    string rollNo;
    string department;
    float marks;
    string grade;
    string contact;

public:
    // Constructor
    Student(string n, string r, string d, float m, string c) {
        name = n;
        rollNo = r;
        department = d;
        marks = m;
        contact = c;
        calculateGrade();
    }

    // Calculate grade based on marks
    void calculateGrade() {
        if (marks >= 90) grade = "A+";
        else if (marks >= 80) grade = "A";
        else if (marks >= 70) grade = "B";
        else if (marks >= 60) grade = "C";
        else if (marks >= 50) grade = "D";
        else grade = "F";
    }

    // Getter methods
    string getName() const { return name; }
    string getRollNo() const { return rollNo; }
    string getDepartment() const { return department; }
    float getMarks() const { return marks; }
    string getGrade() const { return grade; }
    string getContact() const { return contact; }

    // Setter methods
    void setName(string n) { name = n; }
    void setDepartment(string d) { department = d; }
    void setMarks(float m) { 
        marks = m; 
        calculateGrade();
    }
    void setContact(string c) { contact = c; }
};

class StudentManagementSystem {
private:
    vector<Student> students;
    const string FILENAME = "students.txt";

    // Utility function to clear input buffer
    void clearInputBuffer() {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    // Input validation functions
    bool isValidMarks(float marks) {
        return marks >= 0 && marks <= 100;
    }

    bool isValidContact(string contact) {
        return contact.length() == 10 && 
               contact.find_first_not_of("0123456789") == string::npos;
    }

public:
    StudentManagementSystem() {
        loadFromFile();
    }

    // Load data from file
    void loadFromFile() {
        ifstream file(FILENAME);
        if (!file) return;

        string name, rollNo, department, contact;
        float marks;
        
        while (getline(file, name, ',')) {
            getline(file, rollNo, ',');
            getline(file, department, ',');
            file >> marks;
            file.ignore();
            getline(file, contact);
            
            students.push_back(Student(name, rollNo, department, marks, contact));
        }
        file.close();
    }

    // Save data to file
    void saveToFile() {
        ofstream file(FILENAME);
        for (const auto& student : students) {
            file << student.getName() << ","
                 << student.getRollNo() << ","
                 << student.getDepartment() << ","
                 << student.getMarks() << ","
                 << student.getContact() << "\n";
        }
        file.close();
    }

    // Add new student
    void addStudent() {
        string name, rollNo, department, contact;
        float marks;

        clearInputBuffer();
        cout << "\nEnter Student Details\n";
        cout << "Name: ";
        getline(cin, name);

        // Check if roll number already exists
        do {
            cout << "Roll No: ";
            getline(cin, rollNo);
            if (findStudent(rollNo) != -1) {
                cout << "Roll No already exists! Try again.\n";
                continue;
            }
            break;
        } while (true);

        cout << "Department: ";
        getline(cin, department);

        do {
            cout << "Marks (0-100): ";
            cin >> marks;
            if (cin.fail() || !isValidMarks(marks)) {
                cout << "Invalid marks! Please enter marks between 0 and 100.\n";
                clearInputBuffer();
                continue;
            }
            break;
        } while (true);

        clearInputBuffer();
        do {
            cout << "Contact (10 digits): ";
            getline(cin, contact);
            if (!isValidContact(contact)) {
                cout << "Invalid contact number! Please enter 10 digits.\n";
                continue;
            }
            break;
        } while (true);

        students.push_back(Student(name, rollNo, department, marks, contact));
        saveToFile();
        cout << "\nStudent added successfully!\n";
    }

    // Display all students
    void displayAllStudents() {
        if (students.empty()) {
            cout << "\nNo students in database!\n";
            return;
        }

        cout << "\nStudent Details:\n";
        cout << setw(20) << left << "Name"
             << setw(10) << "Roll No"
             << setw(15) << "Department"
             << setw(10) << "Marks"
             << setw(10) << "Grade"
             << setw(15) << "Contact" << endl;
        cout << string(80, '-') << endl;

        for (const auto& student : students) {
            cout << setw(20) << left << student.getName()
                 << setw(10) << student.getRollNo()
                 << setw(15) << student.getDepartment()
                 << setw(10) << student.getMarks()
                 << setw(10) << student.getGrade()
                 << setw(15) << student.getContact() << endl;
        }
    }

    // Find student by roll number
    int findStudent(string rollNo) {
        for (size_t i = 0; i < students.size(); i++) {
            if (students[i].getRollNo() == rollNo) {
                return i;
            }
        }
        return -1;
    }

    // Search student
    void searchStudent() {
        if (students.empty()) {
            cout << "\nNo students in database!\n";
            return;
        }

        string rollNo;
        clearInputBuffer();
        cout << "\nEnter Roll No to search: ";
        getline(cin, rollNo);

        int index = findStudent(rollNo);
        if (index == -1) {
            cout << "Student not found!\n";
            return;
        }

        cout << "\nStudent Details:\n";
        cout << "Name: " << students[index].getName() << endl
             << "Roll No: " << students[index].getRollNo() << endl
             << "Department: " << students[index].getDepartment() << endl
             << "Marks: " << students[index].getMarks() << endl
             << "Grade: " << students[index].getGrade() << endl
             << "Contact: " << students[index].getContact() << endl;
    }

    // Update student
    void updateStudent() {
        if (students.empty()) {
            cout << "\nNo students in database!\n";
            return;
        }

        string rollNo;
        clearInputBuffer();
        cout << "\nEnter Roll No to update: ";
        getline(cin, rollNo);

        int index = findStudent(rollNo);
        if (index == -1) {
            cout << "Student not found!\n";
            return;
        }

        int choice;
        do {
            cout << "\nWhat would you like to update?\n"
                 << "1. Name\n"
                 << "2. Department\n"
                 << "3. Marks\n"
                 << "4. Contact\n"
                 << "5. Go Back\n"
                 << "Enter your choice: ";
            cin >> choice;

            string newValue;
            float newMarks;

            switch (choice) {
                case 1:
                    clearInputBuffer();
                    cout << "Enter new name: ";
                    getline(cin, newValue);
                    students[index].setName(newValue);
                    break;
                case 2:
                    clearInputBuffer();
                    cout << "Enter new department: ";
                    getline(cin, newValue);
                    students[index].setDepartment(newValue);
                    break;
                case 3:
                    do {
                        cout << "Enter new marks (0-100): ";
                        cin >> newMarks;
                        if (cin.fail() || !isValidMarks(newMarks)) {
                            cout << "Invalid marks! Please enter marks between 0 and 100.\n";
                            clearInputBuffer();
                            continue;
                        }
                        break;
                    } while (true);
                    students[index].setMarks(newMarks);
                    break;
                case 4:
                    clearInputBuffer();
                    do {
                        cout << "Enter new contact (10 digits): ";
                        getline(cin, newValue);
                        if (!isValidContact(newValue)) {
                            cout << "Invalid contact number! Please enter 10 digits.\n";
                            continue;
                        }
                        break;
                    } while (true);
                    students[index].setContact(newValue);
                    break;
                case 5:
                    break;
                default:
                    cout << "Invalid choice!\n";
            }
        } while (choice != 5);

        saveToFile();
        cout << "\nStudent information updated successfully!\n";
    }

    // Delete student
    void deleteStudent() {
        if (students.empty()) {
            cout << "\nNo students in database!\n";
            return;
        }

        string rollNo;
        clearInputBuffer();
        cout << "\nEnter Roll No to delete: ";
        getline(cin, rollNo);

        int index = findStudent(rollNo);
        if (index == -1) {
            cout << "Student not found!\n";
            return;
        }

        char confirm;
        cout << "Are you sure you want to delete student " << students[index].getName() 
             << "? (y/n): ";
        cin >> confirm;

        if (confirm == 'y' || confirm == 'Y') {
            students.erase(students.begin() + index);
            saveToFile();
            cout << "\nStudent deleted successfully!\n";
        }
    }
};

int main() {
    StudentManagementSystem sms;
    int choice;

    do {
        cout << "\nStudent Management System\n"
             << "1. Add Student\n"
             << "2. Display All Students\n"
             << "3. Search Student\n"
             << "4. Update Student\n"
             << "5. Delete Student\n"
             << "6. Exit\n"
             << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                sms.addStudent();
                break;
            case 2:
                sms.displayAllStudents();
                break;
            case 3:
                sms.searchStudent();
                break;
            case 4:
                sms.updateStudent();
                break;
            case 5:
                sms.deleteStudent();
                break;
            case 6:
                cout << "Exiting...\n";
                break;
            default:
                cout << "Invalid choice!\n";
        }
    } while (choice != 6);

    return 0;
}