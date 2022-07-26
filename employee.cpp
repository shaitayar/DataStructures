#include "employee.h"
#include <iostream>


Employee::Employee(int ID, int grade, Company* p_company): ID(ID), salary(0), grade(grade), p_company(p_company) {}

Employee::Employee(int ID, int salary, int grade, Company* p_company): ID(ID), salary(salary), grade(grade), p_company(p_company) {}

int Employee::getID() const {
    return this->ID;
}

long long Employee::getSalary() const {
    return this->salary;
}

long long Employee::getGrade() const {
    return this->grade;
}

long long Employee::getRank() const{
    return getGrade();
}

Company* Employee::getCompany() const{
    return p_company;
}

void Employee::setSalary(int SalaryIncrease) {
    salary+=SalaryIncrease;
}

void Employee::setGrade(int bumpGrade){
    grade+=bumpGrade;
}


