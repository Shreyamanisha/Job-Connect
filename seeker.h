#ifndef SEEKER
#define SEEKER

#define COMPULSORY_WEIGHTAGE_SEEKER 15
#define DESIRABLE_WEIGHTAGE_SEEKER 8
#define MAX_COMP_CONST_SEEKER 3
#define NO_OF_CONSTRAINTS_SEEKER 6


typedef struct AppliedJob{
    char company[50];
    char job[30];
    int status; //0->Applied 1->Selected -1->Rejected
}appliedJob;

typedef struct Job_seeker {
    //details
    char seekerName[30];
    char password[64];
    int age;
    long long int phoneNumber;
    char email[50];
    char qualification[10];
    int xp;
    char college[100];
    float tenthPercentage, twelthPercentage, cgpa;
    //constraints
    int exp_salary, salary_weightage, salary_score;
    char exp_location[30];
    int location_weightage, location_score;
    char exp_jobType[20];
    int jobType_weightage, jobType_score;
    char exp_companyType[20];
    int companyType_weightage, companyType_score;
    char exp_shift;
    int shift_weightage, shift_score;
    char workFromHome;
    int workFromHome_weightage, workFromHome_score;
    float recommendationPercentage;
    //company calls
    int noOfCalls;
    int noOfAppliedJobs;
    int newlyAppeared;
    appliedJob appliedJobs[15];
} seeker;


int checkSeekerExistence(char seekerName[]);
seeker loginSeeker(char loginSeeker[], char password[], int *correctPw);
seeker createSeeker(int *alreadyPresent);
void printSeekerConstraints(seeker sk);
void assignSeekerWeightage(seeker *sk, int compConst[], int len);
void assignSeekerScore(seeker *sk);
void writeSeekerFile(seeker sk);
void printSeekerProfile(seeker sk);
void printSeekerDetails(seeker sk);
void updateSeekerProfile(seeker sk);
int copyDocument(char inName[], char outName[]);

#endif