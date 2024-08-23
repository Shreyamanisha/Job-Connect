#ifndef TEAM
#define TEAM

#define COMPULSORY_WEIGHTAGE_COMPANY 15
#define DESIRABLE_WEIGHTAGE_COMPANY 8
#define MAX_COMP_CONST_COMPANY 3
#define NO_OF_CONSTRAINTS_COMPANY 7


typedef struct Job{
    //details
    char job[30];
    int salary;
    char location[30];
    char jobType[20];
    char companyType[20];
    char workFromHome;
    //constraints
    char shift;
    int shift_weightage, shift_score;
    int exp_age, age_weightage, age_score;
    int exp_xp, xp_weightage, xp_score;
    char exp_qualification[10];
    int qualification_weightage, qualification_score;
    float exp_tenthPercent;
    int tenthPercent_weightage, tenthPercent_score;
    float exp_twelthPercent;
    int twelthPercent_weightage, twelthPercent_score;
    float exp_cgpa;
    int cgpa_weightage, cgpa_score;
    float recommendationPercentage;
}job;

typedef struct Company{
    char companyName[50];
    char password[64];
    int noOfJobs;
    job jobs[15];
}company;




int checkCompanyExistence(char companyName[]);
company loginCompany(char loginCompany[], char password[], int *correctPw);
company createCompany(int *alreadyPresent);
company createJob(company cp);
void printJobConstraints(job jb);
void assignJobWeightage(job *jb, int compConst[], int len);
void assignJobScore(job *jb);
void writeCompanyFile(company cp_write);
void printJobs(company cp);
void printJobProfile(job jb);
void printJobDetails(job jb);   

#endif