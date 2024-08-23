#ifndef APPLICATIONS
#define APPLICATIONS

#include "company.h"
#include "seeker.h"

typedef struct Applications{
    char applicant[30];
    char jobId[30];
}applications;

typedef struct CompanyApplications{
    char company[50];
    int noOfApplicants;
    int newlyAppeared;
    applications appl[100];
}companyApplications;


void allocateCompanyApplications(company cp);
void updateCompanyApplications(seeker sk, job jb, company cp);
companyApplications getApplications(company cp);
void resetNewlyAppeared(companyApplications cpApp_write);
void updateSeekerApplication(char seekerName[], char job[], char company[], int status);
void printApplicants(companyApplications cpApp);
void printApplicantDetails(char seekerName[]);
void printAppliedJobsStatus(seeker sk);

#endif