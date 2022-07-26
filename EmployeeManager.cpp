#include "EmployeeManager.h"

EmployeeManager::~EmployeeManager() {
    employees_by_id.DestroyHashTable(false);
    employees_by_salary.DestroyTree(false);
    companies.DestroyUF();

}

void EmployeeManager::AddEmployee(int employeeID, int companyID, int grade) {
    if (companyID > company_num) throw EmInvalidInput();
    if (employees_by_id.Get(employeeID)) throw EmFailure();

    Company *company = companies.Find(companyID);
    Employee *employee = new Employee(employeeID, grade, company);
    if (!employee) throw EmAllocationError();

    //add to EM hash
    employees_by_id.Add(employee);
    employee_num++;

    num_zero_sal++;
    grades_zero_sal += grade;
    //add to company
    company->addEmployee(employee);
}


void EmployeeManager::RemoveEmployee(int employeeID) {
    Employee *employee = employees_by_id.Get(employeeID);
    if (!employee) throw EmFailure();
    Company *company = employee->getCompany();

    //delete from EM
    employees_by_id.Remove(employeeID, false);
    if (employee->getSalary() > 0) employees_by_salary.deleteNode(employee, false);

        //"delete" from zero salary
    else {
        num_zero_sal--;
        grades_zero_sal -= employee->getGrade();
    }

    //delete from company
    company->removeEmployee(employee, true);
    employee_num--;
}
template <class L>
void merge(Employee **X, int x_size, Employee **Y, int y_size, Employee **result, L compare) {
    int i = 0, j = 0, k = 0;
    while ((i < x_size) && (j < y_size)) {
        if (compare(X[i], Y[j]) > 0) {
            result[k] = (Y[j]);
            k++;
            j++;
        } else {
            result[k] = (X[i]);
            k++;
            i++;
        }
    }
    while (i < x_size) {
        result[k] = (X[i]);
        k++;
        i++;
    }
    while (j < y_size) {
        result[k] = (Y[j]);
        k++;
        j++;
    }
}

void update_company(Employee ** arr,int size, Company * comp){
    for (int i = 0; i < size; ++i) {
        arr[i]->setCompany(comp);
    }
}


//union company B into company A
void EmployeeManager::union_companies(Company *a, Company *b) {
    //combine trees
    int bsize = b->getNotZeroSize();
    int asize = a->getNotZeroSize();
    Employee **empBySalaryB = (Employee **) malloc(sizeof(**empBySalaryB) * bsize);
    Employee **empBySalaryA = (Employee **) malloc(sizeof(**empBySalaryA) * asize);
    Employee **combinedSalary = (Employee **) malloc(sizeof(**combinedSalary) * (bsize + asize));

    if(!empBySalaryB||!empBySalaryA ||!combinedSalary){
        free(empBySalaryB);
        free(empBySalaryA);
        free(combinedSalary);
        throw EmAllocationError();
    }

    b->TreeToArray(empBySalaryB);
    a->TreeToArray(empBySalaryA);

    CompEmployeeBySalary cs;
    merge<CompEmployeeBySalary>(empBySalaryB, bsize, empBySalaryA, asize, combinedSalary, cs);

    a->emptyCompanyTree();
    b->emptyCompanyTree();
    a->ArrayToTree(combinedSalary,bsize+asize);

    //free all allocated memory
    free(empBySalaryB);
    free(empBySalaryA);
    free(combinedSalary);

    //combine Hash Tables
    int all_b = b->getSize();
    Employee **empByIDB = (Employee **) malloc(sizeof(**empByIDB) * all_b);

    if(!empByIDB){
        throw EmAllocationError();
    }

    b->HashToArr(empByIDB);
    update_company(empByIDB,all_b, a);
    for (int i = 0; i < all_b; ++i) {
        a->addEmployee(empByIDB[i]);
    }
    free(empByIDB);

}


void EmployeeManager::AcquireCompany(int AcquirerID, int TargetID, double Factor) {
    if (AcquirerID > company_num || TargetID > company_num || companies.isSameGroup(AcquirerID, TargetID))
        throw EmInvalidInput();
    Company *acquirer = companies.Find(AcquirerID);
    Company *target = companies.Find(TargetID);
    int x=0;
    int * bigger = &x;

    companies.Union(AcquirerID, TargetID, Factor, bigger);

    //Acquirer is bigger
    if (x == acquirer->getID()) {
        union_companies(acquirer, target);
    }
        //Target is bigger
    else {
        union_companies(target, acquirer);
    }
}

void EmployeeManager::EmployeeSalaryIncrease(int employeeID, int salaryIncrease) {
    Employee *employee = employees_by_id.Get(employeeID);
    if (!employee) throw EmFailure();
    Company *company = employee->getCompany();

    //if in tree, remove than add after update
    if (employee->getSalary() > 0) {
        employees_by_salary.deleteNode(employee, false);
        company->removeEmployeeFromTreeOnly(employee, false);
    }
        //if zero sal
    else {
        num_zero_sal--;
        grades_zero_sal -= employee->getGrade();
        company->decrease_zero(employee);
    }

    employee->setSalary(salaryIncrease);
    employees_by_salary.insert(employee);
    company->addEmployeeToTreeOnly(employee);

}

void EmployeeManager::PromoteEmployee(int employeeID, int bumpGrade) {
    Employee *employee = employees_by_id.Get(employeeID);
    if (!employee) throw EmFailure();

    if (bumpGrade <= 0) return;

    Company *company = employee->getCompany();

    if (employee->getSalary()<=0) {
        employee->setGrade(bumpGrade);
        company->addGradeToZeroes(bumpGrade);
        grades_zero_sal+=bumpGrade;
        return;
    }
    //update trees
    employees_by_salary.deleteNode(employee, false);
    company->removeEmployeeFromTreeOnly(employee, false);

    employee->setGrade(bumpGrade);
    employees_by_salary.insert(employee);
    company->addEmployeeToTreeOnly(employee);

}

void EmployeeManager::SumOfBumpGradeBetweenTopWorkersByGroup(int companyID, int m, void **sumBumpGrade) {
    if (companyID > company_num) throw EmInvalidInput();
    //for all EM
    if (companyID == 0) {
        if (employees_by_salary.getSize() < m) throw EmFailure();
        employees_by_salary.SumOfBumpGradeBetweenTopWorkers(m,sumBumpGrade);
    }
    //for specific company
    else {
        Company *company = companies.Find(companyID);
        if (company->getSizeOfSalaryWorkers() < m) throw EmFailure();
        company->SumOfBumpGradeBetweenTopWorkersC(m, sumBumpGrade);
    }
}

void EmployeeManager::AverageBumpGradeBetweenSalaryByGroup(int companyID, int lowerSalary, int higherSalary,
                                                           void **averageBumpGrade) {
    if (companyID > company_num) throw EmInvalidInput();

    //for all EM
    if (companyID == 0) {
        //both 0
        if (lowerSalary == 0) {
            if (higherSalary == 0) {
                if (num_zero_sal == 0) throw EmFailure();
                **(double **) averageBumpGrade = double(grades_zero_sal) / num_zero_sal;
                return;
            }

            else {
                int index;
                long long grades;
                employees_by_salary.find_rank_index(higherSalary, &grades, &index);

                if (num_zero_sal+index ==0) throw EmFailure();
                **(double **) averageBumpGrade = double(grades_zero_sal + grades) / (num_zero_sal + index);
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

            if (index_h-index_l <=0) throw EmFailure();

            **(double **) averageBumpGrade = double(grades_h-grades_l) / (index_h- index_l);

        }
    }
    //for specific company
    else {
        Company * company = companies.Find(companyID);
        try{
            company->AverageBumpGradeBetweenSalaryByGroupC(lowerSalary, higherSalary, averageBumpGrade);
        }
        catch (Company::CFailure & e){
            throw EmFailure();
        }
    }
}

void EmployeeManager::CompanyValue(int companyID, void **standing) {
    if (companyID>company_num) throw EmInvalidInput();
    **(double**)standing = companies.getValue(companyID);
}