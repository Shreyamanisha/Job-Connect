#include "applications.h"
#include <stdio.h>
#include <string.h>

void allocateCompanyApplications(company cp){
    companyApplications cpApp;
    strcpy(cpApp.company, cp.companyName);
    cpApp.noOfApplicants = 0;
    cpApp.newlyAppeared = 0;
    FILE *fptr;
    fptr = fopen("Applications.dat", "rb+");
    if(fptr==NULL)
        printf("\n\n------------Something Gone Wrong Internally--------------\n");
    else{
        fseek(fptr, 0, SEEK_END);
        fwrite(&cpApp, sizeof(cpApp), 1, fptr);
        fclose(fptr);
    }
}

void updateCompanyApplications(seeker sk, job jb, company cp){
    companyApplications cpApp;
    FILE *fptr;
    fptr = fopen("Applications.dat", "rb+");
    if(fptr==NULL)
        printf("\n\n------------Something Gone Wrong Internally--------------\n");
    else{
        while(fread(&cpApp, sizeof(cpApp), 1, fptr) != 0){
            if(strcmp(cpApp.company, cp.companyName)==0){
                strcpy(cpApp.appl[cpApp.noOfApplicants].applicant, sk.seekerName);
                strcpy(cpApp.appl[cpApp.noOfApplicants].jobId, jb.job);
                // cpApp.appl[cpApp.noOfApplicants-1].jobId = jb.jobId;
                cpApp.noOfApplicants++;
                cpApp.newlyAppeared++;
                fseek(fptr, -sizeof(cpApp), SEEK_CUR);
                fwrite(&cpApp, sizeof(cpApp), 1, fptr);
            }
        }
        fclose(fptr);
    }
}

void resetNewlyAppeared(companyApplications cpApp_write){
    companyApplications cpApp_read;
    FILE *fptr;
    fptr = fopen("Applications.dat", "rb+");
    if(fptr==NULL)
        printf("\n\n------------Something Gone Wrong Internally--------------\n");
    else{
        while(fread(&cpApp_read, sizeof(cpApp_read), 1, fptr) != 0){
            if(strcmp(cpApp_write.company, cpApp_read.company)==0){
                cpApp_write.newlyAppeared = 0;
                fseek(fptr, -sizeof(cpApp_read), SEEK_CUR);
                fwrite(&cpApp_write, sizeof(cpApp_write), 1, fptr);
            }
        }
        fclose(fptr);
    }
}

companyApplications getApplications(company cp){
    companyApplications cpApp;
    FILE *fptr;
    fptr = fopen("Applications.dat", "rb");
    if(fptr==NULL)
        printf("\n\n------------Something Gone Wrong Internally--------------\n");
    else{
        while(fread(&cpApp, sizeof(cpApp), 1, fptr) != 0){
            if(strcmp(cpApp.company, cp.companyName) == 0){
                fclose(fptr);
                return cpApp;
            }
        }
        fclose(fptr);
    }
}

void updateSeekerApplication(char seekerName[], char job[], char company[], int status){
    seeker sk;
    FILE *fptr;
    fptr = fopen("Seekers.dat", "rb+");
    if(fptr==NULL)
        printf("\n\n------------Something Gone Wrong Internally--------------\n");
    else{
        while(fread(&sk, sizeof(sk), 1, fptr) != 0){
            if(strcmp(seekerName, sk.seekerName)==0){
                for(int i=0; i<sk.noOfAppliedJobs; i++){
                    if(strcmp(company, sk.appliedJobs[i].company)==0 && strcmp(job, sk.appliedJobs[i].job)==0){
                        if(status == 1)
                            sk.newlyAppeared++;
                        sk.appliedJobs[i].status = status;
                        fseek(fptr, -sizeof(sk), SEEK_CUR);
                        fwrite(&sk, sizeof(sk), 1, fptr);
                        break;
                    }
                }
            }
        }
        fclose(fptr);
    }
}

void printApplicants(companyApplications cpApp){
    seeker sk;
    FILE *fptr;
    fptr = fopen("Seekers.dat", "rb");
    printf("\n\n--------------------------------------APPLICATIONS----------------------------------\n\n");
    printf("    %-10s%-25s%-25s%-15s%-20s%s\n", "Sl.No", "Applicant", "Job Applied", "Age", "Experience", "Qualification");
    for(int i=0; i<cpApp.noOfApplicants; i++){
        fseek(fptr, 0, SEEK_SET);
        while(fread(&sk, sizeof(sk), 1, fptr) != 0){
            if(strcmp(sk.seekerName, cpApp.appl[i].applicant) == 0){
                printf("    %-10d%-25s%-25s%-15d%-3d years           %s\n", i+1, cpApp.appl[i].applicant, cpApp.appl[i].jobId, sk.age, sk.xp, sk.qualification);
            }
        }
    }
    fclose(fptr);
}

void printApplicantDetails(char seekerName[]){
    seeker sk;
    FILE *fptr;
    fptr = fopen("Seekers.dat", "rb");
    if(fptr==NULL)
        printf("\n\n------------Something Gone Wrong Internally--------------\n");
    else{
        while(fread(&sk, sizeof(sk), 1, fptr) != 0){
            if(strcmp(seekerName, sk.seekerName)==0){
                printSeekerDetails(sk);
                fclose(fptr);
                return;
            }
        }
        fclose(fptr);
    }
}

void printAppliedJobsStatus(seeker sk){
    char statusString[15];
    printf("\n\n--------------------------------------APPLIED JOBS----------------------------------\n\n");
    printf("    %-10s%-25s%-25s%s\n", "Sl.No", "Company Name", "Job Applied", "Status of the Application");
    for(int i=0; i<sk.noOfAppliedJobs; i++){
        switch(sk.appliedJobs[i].status){
            case -1:
                strcpy(statusString, "Rejected");
                break;
            case 0:
                strcpy(statusString, "Not Chekced");
                break;
            case 1:
                strcpy(statusString, "Accepted");
                break;

        }
        printf("    %-10d%-25s%-25s%s\n", i+1, sk.appliedJobs[i].company, sk.appliedJobs[i].job, statusString);
    }
}