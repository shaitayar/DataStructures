#ifndef WET2_EMPLOYEEMANAGER_H
#define WET2_EMPLOYEEMANAGER_H

#include "company.h"
#include "employee.h"
#include "RankTree.h"
#include "HashTable.h"
#include "UnionFind.h"
#include <stdio.h>
#include "exception"

class EmployeeManager {
    int company_num;
    int employee_num;
    long long grades_zero_sal;
    int num_zero_sal;


    CompEmployeeById ed;
    CompEmployeeBySalary es;

    UnionFind<Company> companies;
    RankTree<Employee, CompEmployeeBySalary> employees_by_salary;
    HashTable<Employee, CompEmployeeById> employees_by_id;

public:
    EmployeeManager(int k): company_num(k), employee_num(0), grades_zero_sal(0), num_zero_sal(0), ed(), es(), companies(k), employees_by_salary(es), employees_by_id(ed){};

    ~EmployeeManager();

    void AddEmployee( int employeeID, int companyID, int grade);

    void RemoveEmployee(int employeeID);

    void AcquireCompany(int AcquirerID, int TargetID, double Factor);

    void EmployeeSalaryIncrease(int employeeID, int salaryIncrease);

    void PromoteEmployee(int employeeID, int bumpGrade);

    void SumOfBumpGradeBetweenTopWorkersByGroup(int companyID, int m, void ** sumBumpGrade);

    void AverageBumpGradeBetweenSalaryByGroup(int companyID, int lowerSalary, int higherSalary, void ** averageBumpGrade);

    void CompanyValue(int companyID, void ** standing);

    void union_companies(Company *a, Company *b);

    class EmException : public std::exception {};

    class EmFailure : public EmException {};

    class EmInvalidInput : public EmException {};

    class EmAllocationError : public EmException {};

};


#endif //WET2_EMPLOYEEMANAGER_H
