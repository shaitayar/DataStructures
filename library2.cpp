#include "library2.h"
#include "EmployeeManager.h"

void *Init(int k) {
    EmployeeManager *DS = new EmployeeManager(k);
    return (void *) DS;
}

StatusType AddEmployee(void *DS, int employeeID, int companyID, int grade) {
    if (DS == NULL || employeeID <= 0 || companyID <= 0 || grade < 0) return INVALID_INPUT;
    try {
        ((EmployeeManager *) DS)->AddEmployee(employeeID, companyID, grade);
    }
    catch (EmployeeManager::EmInvalidInput &e) {
        return INVALID_INPUT;
    }
    catch (EmployeeManager::EmAllocationError &e) {
        return ALLOCATION_ERROR;
    }
    catch (EmployeeManager::EmFailure &e) {
        return FAILURE;
    }
    return SUCCESS;


}

StatusType RemoveEmployee(void *DS, int employeeID) {
    if (DS == NULL || employeeID <= 0) return INVALID_INPUT;
    try {
        ((EmployeeManager *) DS)->RemoveEmployee(employeeID);
    }
    catch (EmployeeManager::EmInvalidInput &e) {
        return INVALID_INPUT;
    }
    catch (EmployeeManager::EmAllocationError &e) {
        return ALLOCATION_ERROR;
    }
    catch (EmployeeManager::EmFailure &e) {
        return FAILURE;
    }
    return SUCCESS;
}


StatusType AcquireCompany(void *DS, int companyID1, int companyID2, double factor) {
    if (DS == NULL || companyID1 <= 0 || companyID2 <= 0 || factor <= 0.00 || companyID2 == companyID1)
        return INVALID_INPUT;
    try {
        ((EmployeeManager *) DS)->AcquireCompany(companyID1, companyID2, factor);
    }
    catch (EmployeeManager::EmInvalidInput &e) {
        return INVALID_INPUT;
    }
    catch (EmployeeManager::EmAllocationError &e) {
        return ALLOCATION_ERROR;
    }
    catch (EmployeeManager::EmFailure &e) {
        return FAILURE;
    }
    return SUCCESS;

}

StatusType EmployeeSalaryIncrease(void *DS, int employeeID, int salaryIncrease) {
    if (DS == NULL || employeeID <= 0 || salaryIncrease <= 0)
        return INVALID_INPUT;
    try {
        ((EmployeeManager *) DS)->EmployeeSalaryIncrease(employeeID, salaryIncrease);
    }
    catch (EmployeeManager::EmInvalidInput &e) {
        return INVALID_INPUT;
    }
    catch (EmployeeManager::EmAllocationError &e) {
        return ALLOCATION_ERROR;
    }
    catch (EmployeeManager::EmFailure &e) {
        return FAILURE;
    }
    return SUCCESS;

}


StatusType PromoteEmployee(void *DS, int employeeID, int bumpGrade) {
    if (DS == NULL || employeeID <= 0)
        return INVALID_INPUT;
    try {
        ((EmployeeManager *) DS)->PromoteEmployee(employeeID, bumpGrade);
    }
    catch (EmployeeManager::EmInvalidInput &e) {
        return INVALID_INPUT;
    }
    catch (EmployeeManager::EmAllocationError &e) {
        return ALLOCATION_ERROR;
    }
    catch (EmployeeManager::EmFailure &e) {
        return FAILURE;
    }
    return SUCCESS;

}

StatusType SumOfBumpGradeBetweenTopWorkersByGroup(void *DS, int companyID, int m) {
    if (DS == NULL || companyID < 0 || m <= 0)
        return INVALID_INPUT;
    try {
        long long x=0;
        long long* sumBumpGrade=&x ;
        ((EmployeeManager *) DS)->SumOfBumpGradeBetweenTopWorkersByGroup(companyID, m, (void**)&sumBumpGrade);

        printf("SumOfBumpGradeBetweenTopWorkersByGroup: %lld\n", x);

    }
    catch (EmployeeManager::EmInvalidInput &e) {
        return INVALID_INPUT;
    }
    catch (EmployeeManager::EmAllocationError &e) {
        return ALLOCATION_ERROR;
    }
    catch (EmployeeManager::EmFailure &e) {
        return FAILURE;
    }
    return SUCCESS;
}

//#include <cmath>
StatusType AverageBumpGradeBetweenSalaryByGroup(void *DS, int companyID, int lowerSalary, int higherSalary){
    if (DS == NULL || companyID < 0 || lowerSalary < 0 || higherSalary<0 || lowerSalary>higherSalary)
        return INVALID_INPUT;
    try {
        double x=0.0;
        double* averageBumpGrade=&x ;
        ((EmployeeManager *) DS)->AverageBumpGradeBetweenSalaryByGroup(companyID, lowerSalary, higherSalary, (void**)&averageBumpGrade);

/*        int temp = (int)(x * 10);
        double average = ((double)temp)/10;
        if(fabs(average + 0.1 - ((double)x)) <= 0.0000000001){
            average += 0.1;
        }*/

        //printf("AverageBumpGradeBetweenSalaryByGroup: %.1f\n", floor(10 * x + 0.5f) / 10);

        printf("AverageBumpGradeBetweenSalaryByGroup: %.1f\n", x);
    }
    catch (EmployeeManager::EmInvalidInput &e) {
        return INVALID_INPUT;
    }
    catch (EmployeeManager::EmAllocationError &e) {
        return ALLOCATION_ERROR;
    }
    catch (EmployeeManager::EmFailure &e) {
        return FAILURE;
    }
    return SUCCESS;
}


StatusType CompanyValue(void *DS, int companyID){
    if (DS == NULL || companyID <= 0)
        return INVALID_INPUT;
    try {
        double x=0.0;
        double* standing=&x ;
        ((EmployeeManager *) DS)->CompanyValue(companyID, (void**)&standing);

        printf("CompanyValue: %.1f\n", x);

    }
    catch (EmployeeManager::EmInvalidInput &e) {
        return INVALID_INPUT;
    }
    catch (EmployeeManager::EmAllocationError &e) {
        return ALLOCATION_ERROR;
    }
    catch (EmployeeManager::EmFailure &e) {
        return FAILURE;
    }
    return SUCCESS;
}

void Quit(void **DS) {
    if (DS == NULL) return;
    delete ((EmployeeManager *) *DS);
    *DS = NULL;
}