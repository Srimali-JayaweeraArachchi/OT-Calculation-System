#include <iostream>
#include <string>
#include <sstream>

using namespace std;

class WorkLogBase { 
protected: 
    int clockInHour;
    int clockInMinute;
    int clockOutHour;
    int clockOutMinute;
    bool isWeekend; 

public: //constructor definition
    WorkLogBase(int inHour, int inMinute, int outHour, int outMinute, bool weekend)
        : clockInHour(inHour), clockInMinute(inMinute), clockOutHour(outHour), clockOutMinute(outMinute), isWeekend(weekend) {} 
    //constructor takes five parameters
    
    int getClockInHour() const {
        return clockInHour;
    }

    int getClockInMinute() const {
        return clockInMinute;
    }

    int getClockOutHour() const {
        return clockOutHour;
    }

    int getClockOutMinute() const {
        return clockOutMinute;
    }

    bool getIsWeekend() const { 
        return isWeekend;
    } //const : ensure that when an object of WorkLogBase is const.
};
class WorkLog : public WorkLogBase {
private:
    WorkLog* next; 

//WorkLog is a derived class that inherits from the WorkLogBase class using public inheritance 

//WorkLog has a private member variable next of type WorkLog*. This pointer is used to point to the next WorkLog object in a linked list. */

public:
    WorkLog(int inHour, int inMinute, int outHour, int outMinute, bool weekend)
        : WorkLogBase(inHour, inMinute, outHour, outMinute, weekend), next(nullptr) {}

    WorkLog* getNext() const {
        return next;
    }

    void setNext(WorkLog* nextLog) {
        next = nextLog;
    }

    void setClockInHour(int hour) {
        clockInHour = hour;
    }

    void setClockInMinute(int minute) {
        clockInMinute = minute;
    }

    void setClockOutHour(int hour) {
        clockOutHour = hour;
    }

    void setClockOutMinute(int minute) {
        clockOutMinute = minute;
    }

    void setIsWeekend(bool weekend) {
        isWeekend = weekend;
    }

    ~WorkLog() {   //destructor
        delete next;
    }
};

class Employee {
private:
    string IDNumber;
    string name;
    double basicSalary;
    WorkLog* workLogsHead;

public:
    Employee(string id, string empName, double salary)
        : IDNumber(id), name(empName), basicSalary(salary), workLogsHead(nullptr) {}

    void recordWorkTime(int inHour, int inMinute, int outHour, int outMinute, bool isWeekend) {
        WorkLog* log = new WorkLog(inHour, inMinute, outHour, outMinute, isWeekend);
        log->setNext(workLogsHead);
        workLogsHead = log;
    }

    double calculateOvertimePayment() const {
        double totalOvertimePayment = 0.0;
        double overtimeRate = 1500; // Assuming the overtime rate per hour

        WorkLog* current = workLogsHead;
        while (current != nullptr) {
            double hoursWorked = (current->getClockOutHour() - current->getClockInHour()) +
                (current->getClockOutMinute() - current->getClockInMinute()) / 60.0;

            if (hoursWorked > 8.0 || current->getIsWeekend()) {
                double overtimeHours = (current->getIsWeekend()) ? hoursWorked : hoursWorked - 8.0;
                totalOvertimePayment += overtimeHours * overtimeRate;
            }
            current = current->getNext();
        }

        cout << "Total overtime hours for " << name << ": " << totalOvertimePayment / overtimeRate << endl;

        return totalOvertimePayment;
    }

    double getFullPayment() const {
        return basicSalary + calculateOvertimePayment();
    }

    string getName() const {
        return name;
    }

    double getBasicSalary() const {
        return basicSalary;
    }

    string getIDNumber() const {
        return IDNumber;
    }

    ~Employee() {
        delete workLogsHead;
    }
};

int main() {
    cout << "Please enter time in the format HH.MM (24-hour clock)." << endl;
    cout << "Example: For 8.30 AM, enter as 08.30. For 4.15 PM, enter as 16.15." << endl;
    cout << "Employee ID should be between 001 and 005." << endl << endl;

    Employee employees[] = { //initializes an array of Employee objects named employees
        Employee("001", "Mr.A.B.Perera", 150000), //these are array elements
        Employee("002", "Mr.M.L.Sunil", 80000),
        Employee("003", "Ms.D.A.Pieris", 80000),
        Employee("004", "Ms.A.P.Seetha", 100000),
        Employee("005", "Mr.B.P.Nimal", 70000)
        // Add more employees here
    };

    string searchID;
    bool found = false;

    while (!found) {
        cout << "Enter employee ID (001-005): ";
        cin >> searchID;

        for (auto& emp : employees) {
            if (emp.getIDNumber() == searchID) {
                found = true;
                int weekdaysWorked, weekendDaysWorked;

                cout << "Enter number of weekdays worked for " << emp.getName() << " (0-20): ";
                cin >> weekdaysWorked;

                while (weekdaysWorked < 0 || weekdaysWorked > 20) {
                    cout << "Invalid input. Please enter a number between 0 and 20: ";
                    cin >> weekdaysWorked;
                }

                cout << "Enter number of weekend days worked for " << emp.getName() << " (0-8): ";
                cin >> weekendDaysWorked;

                while (weekendDaysWorked < 0 || weekendDaysWorked > 8) {
                    cout << "Invalid input. Please enter a number between 0 and 8: ";
                    cin >> weekendDaysWorked;
                }

                for (int i = 1; i <= weekdaysWorked + weekendDaysWorked; ++i) {
                    int clockInHour, clockInMinute, clockOutHour, clockOutMinute;
                    bool isWeekend = (i > weekdaysWorked);

                    string clockInTime;
                    string clockOutTime;
                    char dot;

                    while (true) {
                        if (isWeekend) {
                            cout << "Enter clock-in time for weekend day " << i - weekdaysWorked << " for " << emp.getName() << " (HH.MM): ";
                        }
                        else {
                            cout << "Enter clock-in time for weekday " << i << " for " << emp.getName() << " (HH.MM): ";
                        }
                        cin >> clockInTime;
                        istringstream clockInStream(clockInTime);
                        clockInStream >> clockInHour >> dot >> clockInMinute;

                        if (dot != '.') {
                            cout << "Invalid format! Please use HH.MM format." << endl;
                            continue;
                        }
                        break;
                    }

                    while (true) {
                        if (isWeekend) {
                            cout << "Enter clock-out time for weekend day " << i - weekdaysWorked << " for " << emp.getName() << " (HH.MM): ";
                        }
                        else {
                            cout << "Enter clock-out time for weekday " << i << " for " << emp.getName() << " (HH.MM): ";
                        }
                        cin >> clockOutTime;
                        istringstream clockOutStream(clockOutTime);
                        clockOutStream >> clockOutHour >> dot >> clockOutMinute;

                        if (dot != '.') {
                            cout << "Invalid format! Please use HH.MM format." << endl;
                            continue;
                        }
                        break;
                    }

                    emp.recordWorkTime(clockInHour, clockInMinute, clockOutHour, clockOutMinute, isWeekend);
                }

                cout << "ID Number for " << emp.getName() << ": " << emp.getIDNumber() << endl;
                cout << "Basic Salary for " << emp.getName() << ": LKR " << emp.getBasicSalary() << endl;
                double overtimePayment = emp.calculateOvertimePayment();
                cout << "Overtime payment for " << emp.getName() << ": LKR " << overtimePayment << endl;
                double fullPayment = emp.getFullPayment();
                cout << "Full Payment for " << emp.getName() << ": LKR " << fullPayment << endl;
            }
        }

        if (!found) {
            cout << "Employee ID not found! Please try again." << endl;
        }
    }

    return 0;

}
