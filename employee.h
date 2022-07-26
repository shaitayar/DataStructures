#ifndef WET2_EMPLOYEE_H
#define WET2_EMPLOYEE_H
#include <iostream>

class Company;

class Employee{
    int ID;
    long long salary;
    long long grade;
    Company *  p_company;

public:
    Employee(int ID, int grade, Company* p_company);
    Employee(int ID, int salary, int grade, Company* p_company);

    ~Employee()=default;
    int getID() const;
    long long getSalary() const;
    long long getGrade() const;
    Company* getCompany() const;

    void setSalary(int SalaryIncrease);

    void setGrade(int bumpGrade);

    void setCompany(Company* new_company){this->p_company = new_company;}

    void print() const{
        std::cout<<"Print Employee, ID: "<<ID<<std::endl;
    }

    long long getRank() const;

};

class CompEmployeeById {
public:
    int operator()(Employee * e1, Employee * e2) const{
        int id1 = e1->getID();
        int id2 = e2->getID();
        if (id1 > id2) return 1;
        else if (id1 < id2) return -1;
        else return 0;
    }

    int operator()(const Employee * e1, int id2) const{
        int id1 = e1->getID();
        if (id1 > id2) return 1;
        else if (id1 < id2) return -1;
        else return 0;
    }

};

class CompEmployeeBySalary{
public:
    int operator()(Employee * e1, Employee * e2) const{
        long long s1 = e1->getSalary();
        long long s2 = e2->getSalary();
        if (s1 == s2){
            CompEmployeeById c;
            return c(e1,e2);
        }
        else if(s1>s2) return 1;
        else return -1;
    }
};

#endif //WET2_EMPLOYEE_H
