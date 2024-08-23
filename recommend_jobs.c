#include "recommend_jobs.h"
#include <stdio.h>
#include <string.h>

int getCompaniesCount(){
    int count=0;
    company cp;
    FILE *fptr;
    fptr = fopen("Companies.dat", "rb");
    if(fptr==NULL)
        printf("\n\n------------Something Gone Wrong Internally--------------\n");
    else{
        while(fread(&cp, sizeof(cp), 1, fptr)!=0){
            count++;
        }
        fclose(fptr);
    }
    return count;
}


void getCompanies(company *companies){
    company cp;
    FILE *fptr;
    fptr = fopen("Companies.dat", "rb");
    if(fptr == NULL)
        printf("\n\n------------Something Gone Wrong Internally--------------\n");
    else{
        while(fread(&cp, sizeof(cp), 1, fptr)!=0){
            *companies = cp;
            companies++;
        }
        fclose(fptr);
    }
}

int checkJobMatching(seeker sk, job *jb){
    if(sk.salary_weightage == COMPULSORY_WEIGHTAGE_SEEKER){
        if(jb->salary<sk.exp_salary){
            jb->recommendationPercentage = 0;
            return 0;
        }
    }
    if(sk.location_weightage == COMPULSORY_WEIGHTAGE_SEEKER){
        if(strcmp(jb->location, sk.exp_location)!=0){
            jb->recommendationPercentage = 0;
            return 0;
        }
    }
    if(sk.jobType_weightage == COMPULSORY_WEIGHTAGE_SEEKER){
        if(strcmp(jb->jobType, sk.exp_jobType)!=0){
            jb->recommendationPercentage = 0;
            return 0;
        }
    }
    if(sk.companyType_weightage == COMPULSORY_WEIGHTAGE_SEEKER){
        if(strcmp(jb->companyType, sk.exp_companyType)!=0){
            jb->recommendationPercentage = 0;
            return 0;
        }
    }
    if(sk.shift_weightage == COMPULSORY_WEIGHTAGE_SEEKER){
        if(jb->shift != sk.exp_shift){
            jb->recommendationPercentage = 0;
            return 0;
        }
    }
    if(sk.workFromHome_weightage == COMPULSORY_WEIGHTAGE_SEEKER){
        if(jb->workFromHome != sk.workFromHome){
            jb->recommendationPercentage = 0;
            return 0;
        }
    }
    return 1;
}

float getJobRecommendationPercentage(job jb, seeker sk, int calledInsideFunc){
    float jobToSeekerRecPercent, seekertoJobRecPercent, netRecPercent;
    int numJobToSeeker=0;
    int denJobToSeeker = sk.salary_weightage*sk.salary_score + sk.location_weightage*sk.location_score + sk.jobType_weightage*sk.jobType_score + sk.companyType_weightage*sk.companyType_score + sk.shift_weightage*sk.shift_score + sk.workFromHome_weightage*sk.workFromHome_score;
    //Job to Seeker Satisfaction
    if(jb.salary>=sk.exp_salary)
        numJobToSeeker += (sk.salary_weightage*sk.salary_score);
    if(strcmp(jb.location, sk.exp_location)==0)
        numJobToSeeker += (sk.location_weightage*sk.location_score);
    if(strcmp(jb.jobType, sk.exp_jobType)==0)
        numJobToSeeker += (sk.jobType_weightage*sk.jobType_score);
    if(strcmp(jb.companyType, sk.exp_companyType)==0)
        numJobToSeeker += (sk.companyType_weightage*sk.companyType_score);
    if(jb.shift == sk.exp_shift)
        numJobToSeeker += (sk.shift_weightage*sk.shift_score);
    if(jb.workFromHome == sk.workFromHome)
        numJobToSeeker += (sk.workFromHome_weightage*sk.workFromHome_score);
    jobToSeekerRecPercent = (float)numJobToSeeker/denJobToSeeker*100;

    //Seeker to Job Satisfaction - Good chance of getting hired
    if(calledInsideFunc==1)
        return jobToSeekerRecPercent;

    seekertoJobRecPercent = getSeekerRecommendationPercentage(sk, jb, 1);
    netRecPercent = 0.7*jobToSeekerRecPercent + 0.3*seekertoJobRecPercent;
    return netRecPercent;
}

void printRecommendedJobs(company companies[], job jobs[], int len){
    printf("\n\n--------------------------------------------------------------RECOMMENDED JOBS---------------------------------------------------------\n\n");
    printf("    %-8s%-30s%-30s%-10s%-25s%s\n", "Sl.No", "Company", "Job", "Salary", "Location", "Recommendation Percent");    
    for(int i=0; i<len; i++){
        printf("    %-8d%-30s%-30s%-10d%-25s%.2f%%\n", i+1, companies[i].companyName, jobs[i].job, jobs[i].salary, jobs[i].location, jobs[i].recommendationPercentage);
    }
}

void sortJobs(company *cp, job *jb, int len){
    job jobTemp;
    company companyTemp;
    for(int i=0; i<len; i++){
        for(int j=i+1; j<len; j++){
            if((*(jb+j)).recommendationPercentage > (*(jb+i)).recommendationPercentage){
                jobTemp = *(jb+i);
                *(jb+i) = *(jb+j);
                *(jb+j) = jobTemp;

                companyTemp = *(cp+i);
                *(cp+i) = *(cp+j);
                *(cp+j) = companyTemp;
            }
        }
    }
}

void recommendJobs(seeker sk, company *recCp, job *recJb, int *len){
    *len=0;
    int companiesCount = getCompaniesCount();
    int internallyMatchedJobsCount; 
    int matchedJobsCount=0, start = 1;
    company companies[companiesCount];
    getCompanies(companies);
    for(int i=0; i<companiesCount; i++){
        internallyMatchedJobsCount = 0;
        for(int j=0; j<companies[i].noOfJobs; j++){
            if(checkJobMatching(sk, &companies[i].jobs[j])==1){
                companies[i].jobs[j].recommendationPercentage = getJobRecommendationPercentage(companies[i].jobs[j], sk, 0);
                internallyMatchedJobsCount++;
                matchedJobsCount++;
            }
        }
        if(internallyMatchedJobsCount>0){
            job matchedJobs[internallyMatchedJobsCount];
            int b=0;
            for(int a=0; a<companies[i].noOfJobs; a++){
                if(companies[i].jobs[a].recommendationPercentage!=0){
                    matchedJobs[b] = companies[i].jobs[a];
                    b++;
                    *recCp = companies[i];
                    *recJb = companies[i].jobs[a];
                    recCp++;
                    recJb++;
                    (*len)++;
                }
            }

            // printRecommendedJobs(companies[i], matchedJobs, internallyMatchedJobsCount, start, matchedJobsCount-internallyMatchedJobsCount+1);
            // start = 0;
        }    
    }
    if(matchedJobsCount==0){
        printf("\n\nSorry. Currently we do not have any jobs that would match your interests\n");
    }
}