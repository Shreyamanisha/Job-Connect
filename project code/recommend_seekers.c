#include "recommend_seekers.h"
#include <stdio.h>
#include <string.h>

int getSeekersCount(){
    int count = 0;
    FILE *fptr;
    seeker sk;
    fptr = fopen("Seekers.dat", "rb");
    if(fptr==NULL)
        printf("\n\n------------Something Gone Wrong Internally--------------\n");
    else{
        while(fread(&sk, sizeof(sk), 1, fptr)!=0)
            count++;
        fclose(fptr);
    }
    return count;
}


void getSeekers(seeker *seekers){
    seeker sk;
    FILE *fptr;
    fptr = fopen("Seekers.dat", "rb");
    if(fptr == NULL)
        printf("\n\n------------Something Gone Wrong Internally--------------\n");
    else{
        while(fread(&sk, sizeof(sk), 1, fptr)!=0){
            *seekers = sk;
            seekers++;
        }
        fclose(fptr);
    }
}

int checkSeekerMatching(job jb, seeker *sk){
    if(jb.shift_weightage == COMPULSORY_WEIGHTAGE_COMPANY){
        if(sk->exp_shift != jb.shift){
            sk->recommendationPercentage = 0;
            return 0;
        }
    }
    if(jb.age_weightage == COMPULSORY_WEIGHTAGE_COMPANY){
        if(sk->age<jb.exp_age){
            sk->recommendationPercentage = 0;
            return 0;
        }
    }
    if(jb.xp_weightage == COMPULSORY_WEIGHTAGE_COMPANY){
        if(sk->xp < jb.exp_xp){
            sk->recommendationPercentage = 0;
            return 0;
        }
    }
    if(jb.qualification_weightage == COMPULSORY_WEIGHTAGE_COMPANY){
        if(strcmp(sk->qualification,jb.exp_qualification)!=0){
            sk->recommendationPercentage = 0;
            return 0;
        }
    }
    if(jb.tenthPercent_weightage == COMPULSORY_WEIGHTAGE_COMPANY){
        if(sk->tenthPercentage < jb.exp_tenthPercent){
            sk->recommendationPercentage = 0;
            return 0;
        }
    }
    if(jb.twelthPercent_weightage == COMPULSORY_WEIGHTAGE_COMPANY){
        if(sk->twelthPercentage < jb.exp_twelthPercent){
            sk->recommendationPercentage = 0;
            return 0;
        }
    }
    if(jb.cgpa_weightage == COMPULSORY_WEIGHTAGE_COMPANY){
        if(sk->cgpa < jb.exp_cgpa){
            sk->recommendationPercentage = 0;
            return 0;
        }
    }
    return 1;
}

float getSeekerRecommendationPercentage(seeker sk, job jb, int calledInsideFunc){
    float seekerToJobRecPercent, jobToSeekerRecPercent, netRecPercent;
    int numSeekerToJob=0;
    int denSeekerToJob = jb.shift_weightage*jb.shift_score + jb.age_weightage*jb.age_score + jb.xp_weightage*jb.xp_score + jb.qualification_weightage*jb.qualification_score + jb.tenthPercent_weightage*jb.tenthPercent_score + jb.twelthPercent_weightage*jb.twelthPercent_score + jb.cgpa_weightage*jb.cgpa_score;
    //Seeker to Job Satisfaction
    if(sk.exp_shift == jb.shift)
        numSeekerToJob+=(jb.shift_weightage*jb.shift_score);
    if(sk.age >= jb.exp_age)
        numSeekerToJob+=(jb.age_weightage*jb.age_score);
    if(sk.xp >= jb.exp_xp)
        numSeekerToJob+=(jb.xp_weightage*jb.xp_score);
    if(strcmp(jb.exp_qualification, sk.qualification)==0)
        numSeekerToJob+=(jb.qualification_weightage*jb.qualification_score);
    if(sk.tenthPercentage >= jb.exp_tenthPercent)
        numSeekerToJob+=(jb.tenthPercent_weightage*jb.tenthPercent_score);
    if(sk.twelthPercentage >= jb.exp_twelthPercent)
        numSeekerToJob+=(jb.twelthPercent_weightage*jb.twelthPercent_score);
    if(sk.cgpa >= jb.exp_cgpa)
        numSeekerToJob+=(jb.cgpa_weightage*jb.cgpa_score);
    seekerToJobRecPercent = (float)numSeekerToJob/denSeekerToJob*100;

    //Job To Seeker Satisfaction - Good Chance of the Seeker choosing the job
    if(calledInsideFunc==1)
        return seekerToJobRecPercent;

    jobToSeekerRecPercent = getJobRecommendationPercentage(jb, sk, 1);
    netRecPercent = 0.7*seekerToJobRecPercent + 0.3*jobToSeekerRecPercent;
    return netRecPercent;
}

void sortSeekers(seeker *seekers, int skrlen){
    seeker skTemp;
    for(int i=0; i<skrlen; i++){
        for(int j=i+1; j<skrlen; j++){
            if((*(seekers+j)).recommendationPercentage > (*(seekers+i)).recommendationPercentage){
                skTemp = *(seekers+i);
                *(seekers+i) = *(seekers+j);
                *(seekers+j) = skTemp;
            }
        }
    }
}

void printRecommendedSeekers(seeker matchedSkr[], int len){
    printf("\n\n-----------------------------------------------------------------------------RECOMMENDED SEEKERS-------------------------------------------------------------------------\n\n");
    printf("    %-10s%-30s%-20s%-35s%s\n", "Sl.No", "Seeker", "Phone Number", "Email Id", "Recommendation Percent");
    for(int i=0; i<len; i++){
        printf("    %-10d%-30s%-20lli%-35s%.2f%%\n", i+1, matchedSkr[i].seekerName, matchedSkr[i].phoneNumber, matchedSkr[i].email, matchedSkr[i].recommendationPercentage);
    }
}

void recommendSeekers(job jb){
    int seekersCount = getSeekersCount();
    int matchedSeekersCount=0;
    seeker seekers[seekersCount];
    getSeekers(seekers);
    for(int i=0; i<seekersCount; i++){
        if(checkSeekerMatching(jb, &seekers[i])==1){
            seekers[i].recommendationPercentage = getSeekerRecommendationPercentage(seekers[i], jb, 0);
            matchedSeekersCount++;
        }
    }
    if(matchedSeekersCount==0){
        printf("\n\nSorry. Currently we do not have any Seekers that would match your interests\n");
        return;
    }
    seeker matchedSeekers[matchedSeekersCount];
    int j=0;
    for(int i=0; i<seekersCount; i++){
        if(seekers[i].recommendationPercentage!=0){
            matchedSeekers[j] = seekers[i];
            j++;
        }
    }
    printRecommendedSeekers(matchedSeekers, matchedSeekersCount);
}