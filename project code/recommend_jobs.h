#ifndef RECOMMEND_JOBS
#define RECOMMEND_JOBS

#include "seeker.h"
#include "company.h"
#include "recommend_seekers.h"


int getCompaniesCount();
void getCompanies(company *companies);
int checkJobMatching(seeker sk, job *jb);
void recommendJobs(seeker sk, company *recCp, job *recJb, int *len);
float getJobRecommendationPercentage(job jb, seeker sk, int calledInsideFunc);
void printRecommendedJobs(company companies[], job jobs[], int len);
void sortJobs(company *cp, job *jb, int len);

#endif