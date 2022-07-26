#include "company.h"

Company::~Company(){
    employees_by_salary.DestroyTree(false);
    employees.DestroyHashTable(true);
}

void Company::addEmployee(Employee * employee){
    //add to hash table
    employees.Add(employee);
    num_employees++;
    //update zero
    if (employee->getSalary()==0){
        num_zero++;
        grades_zero+=employee->getGrade();
    }
}
void Company::removeEmployee(Employee * employee, bool is_obj){
    //remove from tree
    if (employee->getSalary()>0){
        employees_by_salary.deleteNode(employee, false);
    }
    //remove from zeroes
    else{
        grades_zero -= employee->getGrade();
        num_zero--;
    }

    //remove from hash
    employees.Remove(employee->getID(), is_obj);
    num_employees--;
}

void Company::removeEmployeeFromTreeOnly(Employee * employee, bool is_obj){
    employees_by_salary.deleteNode(employee, is_obj);
}

void Company::decrease_zero(Employee * employee){
    num_zero--;
    grades_zero-=employee->getGrade();
}
void Company::addEmployeeToTreeOnly(Employee * employee){
    employees_by_salary.insert(employee);
}

void Company::SumOfBumpGradeBetweenTopWorkersC(int m, void ** sumBumpGrade){
    return employees_by_salary.SumOfBumpGradeBetweenTopWorkers(m,sumBumpGrade);
}

void Company::AverageBumpGradeBetweenSalaryByGroupC(int lowerSalary, int higherSalary, void ** averageBumpGrade){

    //both 0
    if (lowerSalary == 0) {
        if (higherSalary == 0) {
            if (num_zero == 0) throw CFailure();
            **(double **) averageBumpGrade = double(grades_zero) / num_zero;
            return;
        }

        else {
            int index;
            long long grades;

            employees_by_salary.find_rank_index(higherSalary, &grades, &index);
            if (num_zero+index ==0) throw CFailure();
            **(double **) averageBumpGrade = double(grades_zero + grades) / (num_zero + index);
        }
    }
        //None is 0
    else{
        int index_l;
        long long grades_l;
        int index_h;
        long long grades_h;

        employees_by_salary.find_rank_index(lowerSalary-1, &grades_l, &index_l);
        employees_by_salary.find_rank_index(higherSalary, &grades_h, &index_h);

        if (index_h-index_l <=0) throw CFailure();

        **(double **) averageBumpGrade = double(grades_h-grades_l) / (index_h- index_l);

    }
}

void Company::TreeToArray(Employee **empBySalaryTarget){
    employees_by_salary.treeToArr(empBySalaryTarget);

}
void Company::ArrayToTree(Employee ** combinedSalary, int size){
    employees_by_salary.arrToTree(combinedSalary, size);
}

void Company::emptyCompanyTree(){
    employees_by_salary.DestroyTree(false);
}

void Company::emptyCompanyHash(){
    employees.DestroyHashTable(false);
    this->num_employees=0;
    this->num_zero=0;
    this->grades_zero=0;


}

void Company::HashToArr(Employee ** arr){
    employees.HashToArray(arr);
    this->emptyCompanyHash();
}
