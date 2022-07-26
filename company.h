
#ifndef WET2_COMPANY_H
#define WET2_COMPANY_H

#include "HashTable.h"
#include "employee.h"
#include "RankTree.h"

class Company {
    int ID;
    int num_employees;
    int num_zero;
    long long grades_zero;
    CompEmployeeBySalary es;
    CompEmployeeById ed;

    HashTable<Employee, CompEmployeeById> employees;
    RankTree<Employee, CompEmployeeBySalary> employees_by_salary;

public:
    explicit Company(int ID) : ID(ID), num_employees(0), num_zero(0), grades_zero(0), es(), ed(), employees(ed),
                               employees_by_salary(es) {};

    ~Company();

    int getID() const { return ID; }

    int getRank() const { return ID; }

    int getSize() const { return num_employees; }

    void addEmployee(Employee *employee);

    void removeEmployee(Employee *employee, bool is_obj);

    void removeEmployeeFromTreeOnly(Employee *employee, bool is_obj);

    void decrease_zero(Employee *employee);

    void addEmployeeToTreeOnly(Employee *employee);

    int getSizeOfSalaryWorkers() const { return employees_by_salary.getSize(); }

    void SumOfBumpGradeBetweenTopWorkersC(int m, void **sumBumpGrade);

    void AverageBumpGradeBetweenSalaryByGroupC(int lowerSalary, int higherSalary, void **averageBumpGrade);

    void TreeToArray(Employee **empBySalaryTarget);

    void ArrayToTree(Employee ** combinedSalary, int size);

    void emptyCompanyTree();

    void emptyCompanyHash();

    //void combineHash(Company * other);

    void HashToArr(Employee ** arr);

    int getNotZeroSize() const {return num_employees-num_zero;}

    void addGradeToZeroes(int bump_grade) {grades_zero+=bump_grade;}

    void print2d(){
        employees_by_salary.print2D();
    }

    class CException : public std::exception {
    };

    class CFailure : public CException {
    };

};


#endif //WET2_COMPANY_H
