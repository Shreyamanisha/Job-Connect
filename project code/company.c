#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <math.h>
#include "ScoreCalculation/score.h"
#include "validation.h"
#include "applications.h"
#include "sha256.h"
#include "company.h"

void assignJobWeightage(job *jb, int compConst[], int len){
    jb->shift_weightage = DESIRABLE_WEIGHTAGE_COMPANY;
    jb->age_weightage = DESIRABLE_WEIGHTAGE_COMPANY;
    jb->xp_weightage = DESIRABLE_WEIGHTAGE_COMPANY;
    jb->qualification_weightage = DESIRABLE_WEIGHTAGE_COMPANY;
    jb->tenthPercent_weightage = DESIRABLE_WEIGHTAGE_COMPANY;
    jb->twelthPercent_weightage = DESIRABLE_WEIGHTAGE_COMPANY;
    jb->cgpa_weightage = DESIRABLE_WEIGHTAGE_COMPANY;
    for(int i=0; i<len; i++){
        switch(compConst[i]){
            case 1:
                jb->shift_weightage = COMPULSORY_WEIGHTAGE_COMPANY;
                break;
            case 2:
                jb->age_weightage = COMPULSORY_WEIGHTAGE_COMPANY;
                break;
            case 3:
                jb->xp_weightage = COMPULSORY_WEIGHTAGE_COMPANY;
                break;
            case 4:
                jb->qualification_weightage = COMPULSORY_WEIGHTAGE_COMPANY;
                break;
            case 5:
                jb->tenthPercent_weightage = COMPULSORY_WEIGHTAGE_COMPANY;
                break;
            case 6:
                jb->twelthPercent_weightage = COMPULSORY_WEIGHTAGE_COMPANY;
                break;
            case 7:
                jb->cgpa_weightage = COMPULSORY_WEIGHTAGE_COMPANY;
                break;
        }
    }
}

void assignJobScore(job *jb){
    int K, numericData;
    struct knn_data knn;
    //Age
    K = getOptimumKValue(JobAge);
    knn = createModel(K, JOB_TRAIN_AGE);
    jb->age_score = predict(knn, jb->exp_age, jb->age_weightage, JOB_TRAIN_AGE);
    //Experience
    K = getOptimumKValue(JobExperience);
    knn = createModel(K, JOB_TRAIN_XP);
    jb->xp_score = predict(knn, jb->exp_xp, jb->xp_weightage, JOB_TRAIN_XP);
    //Shift
    if(jb->shift == 'D')
        numericData = 1;
    else
        numericData = 2;
    K = getOptimumKValue(JobShift);
    knn = createModel(K, JOB_TRAIN_SHIFT);
    jb->shift_score = predict(knn, numericData, jb->shift_weightage, JOB_TRAIN_SHIFT);
    //Qualification
    if(strcmp(jb->exp_qualification, "BE")==0)
        numericData = 1;
    else if(strcmp(jb->exp_qualification, "BED")==0)
        numericData = 2;
    else if(strcmp(jb->exp_qualification, "MBBS")==0)
        numericData = 3;
    else if(strcmp(jb->exp_qualification, "BCOM")==0)
        numericData = 4;
    else if(strcmp(jb->exp_qualification, "MCOM")==0)
        numericData = 5;
    else if(strcmp(jb->exp_qualification, "PHD")==0)
        numericData = 6;
    else if(strcmp(jb->exp_qualification, "ME")==0)
        numericData = 7;
    else if(strcmp(jb->exp_qualification, "10TH")==0)
        numericData = 8;
    else if(strcmp(jb->exp_qualification, "12th")==0)
        numericData = 9;
    else
        numericData = 10;
    K = getOptimumKValue(JobQualification);
    knn = createModel(K, JOB_TRAIN_QUAL);
    jb->qualification_score = predict(knn, numericData, jb->qualification_weightage, JOB_TRAIN_QUAL);
    //TenthPercent
    K = getOptimumKValue(JobTenthPercent);
    knn = createModel(K, JOB_TRAIN_TENTH);
    jb->tenthPercent_score = predict(knn, (int)round(jb->exp_tenthPercent), jb->tenthPercent_weightage, JOB_TRAIN_TENTH);
    //TwelthPercent
    K = getOptimumKValue(JobTwelthPercent);
    knn = createModel(K, JOB_TRAIN_TWELTH);
    jb->twelthPercent_score = predict(knn, (int)round(jb->exp_twelthPercent), jb->twelthPercent_weightage, JOB_TRAIN_TWELTH);
    //CGPA
    K = getOptimumKValue(JobCGPA);
    knn = createModel(K, JOB_TRAIN_CGPA);
    jb->cgpa_score = predict(knn, (int)round((jb->exp_cgpa)*10), jb->cgpa_weightage, JOB_TRAIN_CGPA);
    printf("\n\nAge: %d\n", jb->age_score);
    printf("Experience: %d\n", jb->xp_score);
    printf("Shift: %d\n", jb->shift_score);
    printf("Qualification: %d\n", jb->qualification_score);
    printf("Tenth Percent: %d\n", jb->tenthPercent_score);
    printf("Twelth Percent: %d\n", jb->twelthPercent_score);
    printf("CGPA: %d\n\n", jb->cgpa_score);
}

company createCompany(int *alreadyPresent){
    company cp;
    *alreadyPresent = 0;
    cp.noOfJobs = 0;
    char createJbString[30], passwordString[30], confirmPassword[30];
    char createJb;
    int companyExists, passwordVal;
    //Get Company Name
    fflush(stdin);
    printf("Enter Company Name: ");
    scanf("%[^\n]", cp.companyName);
    *alreadyPresent = checkCompanyExistence(cp.companyName);
    if(*alreadyPresent == 1){
        return cp;
    }
    //Get Password
    printf("\n\nPassword\n");
    printf("\t- Password must be 8 - 15 characters long\n\t- Password must have atleast 1 special character\n\t- Password must have at least 1 digit\n\t- Password must have at least 1 lowercase character\n\t- Password must have atleast 1 uppercase character\n\t- Password cannot any spaces");
    do{
        fflush(stdin);
        printf("\n\nEnter password: ");
        scanf("%[^\n]", passwordString);
        passwordVal = validatePassword(passwordString);
    }while(passwordVal == 0);
    //ConfirmPassword
    do{
        fflush(stdin);
        printf("\nConfirm Password: ");
        scanf("%[^\n]", confirmPassword);
        if(strcmp(confirmPassword, passwordString)==0)
            strcpy(cp.password, SHA256(passwordString));
        else
            printf("Passwords do not match...\n"); 
    }while(strcmp(confirmPassword, passwordString) != 0);
    //Create Job?
    do{
        fflush(stdin);
        printf("\n\nDo you want to create a job now(Y/N): ");
        scanf("%[^\n]", createJbString);
        if(strlen(createJbString)!=1){
            printf("Invalid Input. Enter Y or N\n");
            continue;
        }
        else{
            createJbString[0] = toupper(createJbString[0]);
            if(createJbString[0]!='Y'&&createJbString[0]!='N'){
                printf("Invalid Input. Enter Y or N\n");
                continue;
            }
        }
    }while(createJbString[0]!='Y'&&createJbString[0]!='N');
    switch(createJbString[0]){
        case 'Y':
            cp = createJob(cp);
            break;
        case 'N':
            break;
    }
    return cp;
}

company createJob(company cp){

    if(cp.noOfJobs==15){
        printf("\nYou have reached the maximum no of jobs(15) for you company...\n\n");
        return cp;
    }


    int noOfCompulsoryConstraints;
    char createJb;
    char qualifications[10][10] = {"BE", "BED", "MBBS", "BCOM", "MCOM", "PHD", "ME", "10TH", "12TH", "DIPLOMA"};
    char locations[11][30] = {"Chennai", "Mumbai", "Delhi", "New Delhi", "Kolkata", "Bangalore", "Hyderabad", "Kerala", "Coimbatore", "Gujarat", "Pune"};
    int qualFound=0, locationFound = 0;
    char salaryString[30], jobTypeString[30], companyTypeString[30], expAgeString[30], expXpString[30], shiftString[30], workFromHomeString[30], tenthString[30], twelthString[30], cgpaString[30];
    int salaryInt, jobTypeInt, companyTypeInt, expAgeInt, expXpInt;
    float tenthFloat, twelthFloat, cgpaFloat;
    int charError;
    char noOfCompulsoryConstraintsString[30], compulsoryConstraintString[30];

    //Get Job Name
    fflush(stdin);
    printf("\n\nEnter Job: ");
    scanf("%[^\n]", cp.jobs[cp.noOfJobs].job);
    //Get Job Salary
    while(1){
    fflush(stdin);
        printf("\n\nEnter Monthly Salary(5000-1000000): ");
        scanf("%[^\n]", salaryString);
        charError = checkAlphabetsInIntegers(salaryString);
        if(charError == 1){
            printf("Invalid Input. Salary cannot contain alphabets or special characters. Please enter a salary between in the range Rs.5000 - Rs.1000000\n");
            continue;
        }
        else{
            salaryInt = atoi(salaryString);
            if(salaryInt<5000||salaryInt>1000000){
                printf("Invalid Input. Please enter a salary between in the range Rs.5000 - Rs.1000000\n");
                continue;
            }
        }
        break;
    }
    cp.jobs[cp.noOfJobs].salary = salaryInt;
    //Get Job Location
    do{
        fflush(stdin);
        printf("\n\nEnter location of workplace: ");
        scanf("%[^\n]", &cp.jobs[cp.noOfJobs].location);
        cp.jobs[cp.noOfJobs].location[0] = toupper(cp.jobs[cp.noOfJobs].location[0]);
        for(int i=1; cp.jobs[cp.noOfJobs].location[i]!='\0'; i++){
            if(cp.jobs[cp.noOfJobs].location[i-1]==' ')
                cp.jobs[cp.noOfJobs].location[i] = toupper(cp.jobs[cp.noOfJobs].location[i]);
            else
                cp.jobs[cp.noOfJobs].location[i] = tolower(cp.jobs[cp.noOfJobs].location[i]);
        }
        for(int i=0; i<11; i++){
            if(strcmp(cp.jobs[cp.noOfJobs].location, locations[i])==0)
                locationFound = 1;
        }
        if(locationFound == 0)
            printf("Location out of range of the application...\n");
    }while(locationFound == 0);
    //Get Job Type
    printf("\n\nEnter Job Type: \n");
    printf("\t1. Full Time\t2. Part Time\n");
    do{
        fflush(stdin);
        printf("\nEnter 1 or 2: ");
        scanf("%[^\n]", jobTypeString);
        charError = checkAlphabetsInIntegers(jobTypeString);
        if(charError == 1){
            printf("Invalid Input. Please fill with 1 or 2...\n");
            continue;
        }
        else{
            jobTypeInt = atoi(jobTypeString);
            switch(jobTypeInt){
                case 1:
                    strcpy(cp.jobs[cp.noOfJobs].jobType, "Full Time");
                    break;
                case 2:
                    strcpy(cp.jobs[cp.noOfJobs].jobType, "Part Time");
                    break;
                default:
                    printf("Invalid Input. Please fill with 1 or 2...\n");
                    break;
            }
        }
    }while((jobTypeInt!=1)&&(jobTypeInt!=2));
    //Get Company Type
    printf("\n\nEnter Companny Type: \n");
    printf("\t1. Startup\t2. Small Business\t3. Corporation\n");
    do{
        fflush(stdin);
        printf("Enter 1, 2 or 3: ");
        scanf("%[^\n]", companyTypeString);
        charError = checkAlphabetsInIntegers(companyTypeString);
        if(charError == 1){
            printf("Invalid Input. Please fill with 1, 2 or 3...\n");
            continue;
        }
        else{
            companyTypeInt = atoi(companyTypeString);
            switch(companyTypeInt){
                case 1:
                    strcpy(cp.jobs[cp.noOfJobs].companyType, "Startup");
                    break;
                case 2:
                    strcpy(cp.jobs[cp.noOfJobs].companyType, "Small Business");
                    break;
                case 3:
                    strcpy(cp.jobs[cp.noOfJobs].companyType, "Corporation");
                    break;
                default:
                    printf("Invalid Input. Please fill with 1,2 or 3...\n");
                    break;
            }
        }
    }while((companyTypeInt!=1)&&(companyTypeInt!=2)&&(companyTypeInt!=3));
    //Get Work From Home
    do{
        fflush(stdin);
        printf("\n\nIs the job Work From Home (Y/N): ");
        scanf("%[^\n]", workFromHomeString);
        if(strlen(workFromHomeString) != 1){
            printf("Invalid Input. Please fill with Y or N...\n");
            continue;
        }
        else{
            workFromHomeString[0] = toupper(workFromHomeString[0]);
            if((workFromHomeString[0]!='Y')&&(workFromHomeString[0]!='N'))
                printf("Invalid Input. Please fill with Y or N...\n");
        }
    }while((workFromHomeString[0]!='Y')&&(workFromHomeString[0]!='N'));
    cp.jobs[cp.noOfJobs].workFromHome = workFromHomeString[0];
    //--------Constraints-------------
    //Get Shift
    do{
        fflush(stdin);
        printf("\n\nEnter Shift (D-Day/N-Night): ");
        scanf("%[^\n]", shiftString);
        if(strlen(shiftString) != 1){
            printf("Invalid Input. Please fill with D or N...\n");
            continue;
        }
        else{
            shiftString[0] = toupper(shiftString[0]);
            if((shiftString[0]!='D')&&(shiftString[0]!='N'))
                printf("Invalid Input. Please fill with D or N...\n");
        }
    }while((shiftString[0]!='D')&&(shiftString[0]!='N'));
    cp.jobs[cp.noOfJobs].shift = shiftString[0];
    //Get Expected Age
    while(1){
        fflush(stdin);
        printf("\n\nEnter Expected Age (15 - 70): ");
        scanf("%[^\n]", expAgeString);
        charError = checkAlphabetsInIntegers(expAgeString);
        if(charError == 1){
            printf("Invalid Input. Age cannot contain alphabets or special characters. Please enter a number between 15 - 70...\n");
            continue;
        }
        else{
            expAgeInt = atoi(expAgeString);
            if(expAgeInt<0){
                printf("Invalid Input. Age cannot be neagtive...\n");
                continue;
            }
            else if(expAgeInt<15){
                printf("Your expected age is too young for an employee. A valid age ranges between 15 - 70...\n");
                continue;
            }
            else if(expAgeInt>70){
                printf("Your expected age is too old for an employee. A valid age ranges between 15-70...\n");
                continue;
            }
            else{
                break;
            }
        }
    }
    cp.jobs[cp.noOfJobs].exp_age = expAgeInt;
    //Get Expected Experience
    while(1){
        fflush(stdin);
        printf("\n\nEnter Expected Experience in years: ");
        scanf("%[^\n]", expXpString);
        charError = checkAlphabetsInIntegers(expXpString);
        if(charError == 1){
            printf("Invalid Input. Experience cannot contain alphabets and special characters...\n");
            continue;
        }
        else{
            expXpInt = atoi(expXpString);
            if(expXpInt>=cp.jobs[cp.noOfJobs].exp_age){
                printf("Expected Experience is greater than Age. Please enter a valid experience...\n");
                continue;
            }
            else if(expXpInt<0){
                printf("Invalid Input. Experience cannot be neagtive...\n");
                continue;
            }
            break;
        }
    }
    cp.jobs[cp.noOfJobs].exp_xp = expXpInt;
    //Get Expected Qualification
    do{
        fflush(stdin);
        printf("\n\nEnter Qualification: ");
        scanf("%s", cp.jobs[cp.noOfJobs].exp_qualification);
        for(int i=0; cp.jobs[cp.noOfJobs].exp_qualification[i]!='\0'; i++)
            cp.jobs[cp.noOfJobs].exp_qualification[i] = toupper(cp.jobs[cp.noOfJobs].exp_qualification[i]);
        for(int i=0; i<10; i++){
            if(strcmp(cp.jobs[cp.noOfJobs].exp_qualification, qualifications[i])==0){
                qualFound = 1;
            }
        }
        if(qualFound==0)
            printf("Invalid Qualification. Please enter again...\n");
    }while(qualFound==0);
    //Get Expected 10th Percentage
    while(1){
        fflush(stdin);
        printf("\n\nEnter Expected 10th Percentage: ");
        scanf("%[^\n]", tenthString);
        charError = checkAlphabetsInFloat(tenthString);
        if(charError == 1){
            printf("Invalid Input. 10th Percentage cannot contain alphabets or special characters...\n");
            continue;
        }
        else{
            tenthFloat = atof(tenthString);
            if(tenthFloat<0){
                printf("Invalid Input. 10th Percentage cannot be negative...\n");
                continue;
            }
            else if(tenthFloat>100){
                printf("Invalid Input. 10th Percentage cannot be greater than 100...\n");
            }
            else{
                break;
            }
        }
    }
    cp.jobs[cp.noOfJobs].exp_tenthPercent = atof(tenthString);
    //Get Expected 12th Percentage
    while(1){
        fflush(stdin);
        printf("\n\nEnter Expected 12th Percentage: ");
        scanf("%[^\n]", twelthString);
        charError = checkAlphabetsInFloat(twelthString);
        if(charError == 1){
            printf("Invalid Input. 12th Percentage cannot contain alphabets or special characters...\n");
            continue;
        }
        else{
            twelthFloat = atof(twelthString);
            if(twelthFloat<0){
                printf("Invalid Input. 12th Percentage cannot be negative...\n");
                continue;
            }
            else if(twelthFloat>100){
                printf("Invalid Input. 12th Percentage cannot be greater than 100...\n");
            }
            else{
                break;
            }
        }
    }
    cp.jobs[cp.noOfJobs].exp_twelthPercent = atof(twelthString);
    //Get Expected CGPA
    while(1){
        fflush(stdin);
        printf("\n\nEnter Expected CGPA: ");
        scanf("%[^\n]", cgpaString);
        charError = checkAlphabetsInFloat(cgpaString);
        if(charError == 1){
            printf("Invalid Input. CGPA cannot contain alphabets or special characters...\n");
            continue;
        }
        else{
            cgpaFloat = atof(cgpaString);
            if(cgpaFloat<0){
                printf("Invalid Input. CGPA cannot be negative...\n");
                continue;
            }
            else if(cgpaFloat>10){
                printf("Invalid Input. CGPA cannot be greater than 10...\n");
            }
            else{
                break;
            }
        }
    }
    cp.jobs[cp.noOfJobs].exp_cgpa = atof(cgpaString);
    printf("\n\n");
    //------------End of Getting Inputs-----------------

    //-----------Get Compulosry Constraints------------
    printJobConstraints(cp.jobs[cp.noOfJobs]);
    //Get number of compulsory contraints
    while(1){
        fflush(stdin);
        printf("\nEnter your no of Compulsory Constraints (Max limit is %d): ", MAX_COMP_CONST_COMPANY);
        scanf("%[^\n]", noOfCompulsoryConstraintsString);
        charError = checkAlphabetsInIntegers(noOfCompulsoryConstraintsString);
        if(charError == 1){
            printf("Invalid Input. Please enter a number between 0 - %d...\n", MAX_COMP_CONST_COMPANY);
            continue;
        }
        else{
            noOfCompulsoryConstraints = atoi(noOfCompulsoryConstraintsString);
            if((noOfCompulsoryConstraints>MAX_COMP_CONST_COMPANY)||(noOfCompulsoryConstraints<0)){
                printf("Please enter a number between 0 - %d...\n", MAX_COMP_CONST_COMPANY);
                continue;
            }
            break;
        }
    }
    if(noOfCompulsoryConstraints>0){
        //Get the compulsory contraints
        int compulsoryConstraints[noOfCompulsoryConstraints];
        printf("\n");
        for(int i=0; i<noOfCompulsoryConstraints; i++){
            while(1){
                fflush(stdin);
                printf("Enter the Sl. No of your Compulsory Contraint(1 - %d): ", NO_OF_CONSTRAINTS_COMPANY);
                scanf("%[^\n]", compulsoryConstraintString);
                charError = checkAlphabetsInIntegers(compulsoryConstraintString);
                if(charError == 1){
                    printf("Invalid Input. Please enter a number between 1 - %d\n\n", NO_OF_CONSTRAINTS_COMPANY);
                    continue;
                }
                else{
                    compulsoryConstraints[i] = atoi(compulsoryConstraintString);
                    if((compulsoryConstraints[i]>NO_OF_CONSTRAINTS_COMPANY)||(compulsoryConstraints[i]<1)){
                        printf("Input number out of range. Please enter a number between 1 - %d\n\n", NO_OF_CONSTRAINTS_COMPANY);
                        continue;
                    }
                    break;
                }
            }
        }
        assignJobWeightage(&cp.jobs[cp.noOfJobs], compulsoryConstraints, noOfCompulsoryConstraints);
    }
    else{
        cp.jobs[cp.noOfJobs].shift_weightage = DESIRABLE_WEIGHTAGE_COMPANY;
        cp.jobs[cp.noOfJobs].age_weightage = DESIRABLE_WEIGHTAGE_COMPANY;
        cp.jobs[cp.noOfJobs].xp_weightage = DESIRABLE_WEIGHTAGE_COMPANY;
        cp.jobs[cp.noOfJobs].qualification_weightage = DESIRABLE_WEIGHTAGE_COMPANY;
        cp.jobs[cp.noOfJobs].tenthPercent_weightage = DESIRABLE_WEIGHTAGE_COMPANY;
        cp.jobs[cp.noOfJobs].twelthPercent_weightage = DESIRABLE_WEIGHTAGE_COMPANY;
        cp.jobs[cp.noOfJobs].cgpa_weightage = DESIRABLE_WEIGHTAGE_COMPANY;
    }
    //-----------End of Getting Compulsory Constraints------------

    //Assing Score and Weightage
    assignJobScore(&cp.jobs[cp.noOfJobs]);

    cp.noOfJobs++;
    
    return cp;
}

void writeCompanyFile(company cp_write){
    company cp_read;
    int flag = 0;
    FILE *fptr;
    fptr = fopen("Companies.dat", "rb+");
    if(fptr==NULL)
        printf("\n\n------------Something Gone Wrong Internally--------------\n");
    else{
        if(cp_write.noOfJobs == 0){
            fseek(fptr, 0, SEEK_END);
            fwrite(&cp_write, sizeof(cp_write), 1, fptr);
            allocateCompanyApplications(cp_write);
        }
        else if(cp_write.noOfJobs == 1){
            while(fread(&cp_read, sizeof(cp_read), 1, fptr)){
                if(strcmp(cp_read.companyName, cp_write.companyName)==0){
                    flag = 1;
                    fseek(fptr, -sizeof(cp_read), SEEK_CUR);
                    fwrite(&cp_write, sizeof(cp_write), 1, fptr);
                    break;
                }
            }
            if(flag == 0){
                fseek(fptr, 0, SEEK_END);
                fwrite(&cp_write, sizeof(cp_write), 1, fptr);
                allocateCompanyApplications(cp_write);
            }
        }
        else{
            while(fread(&cp_read, sizeof(cp_read), 1, fptr)!=0){
                if(strcmp(cp_read.companyName, cp_write.companyName)==0){
                    fseek(fptr, -sizeof(cp_read), SEEK_CUR);
                    fwrite(&cp_write, sizeof(cp_write), 1, fptr);
                    break;
                }
            }
        }
        fclose(fptr);
    }
}

int checkCompanyExistence(char companyName[]){
    company companyData;
    FILE *fptr;
    fptr = fopen("Companies.dat", "rb");
    if(fptr==NULL)
        printf("\n\n------------Something Gone Wrong Internally--------------\n");\
    else{
        while(fread(&companyData, sizeof(companyData), 1, fptr) != 0){
            if(strcmp(companyName, companyData.companyName)==0){
                fclose(fptr);
                return 1;
            }
        }
        fclose(fptr);
    }
    return 0;
}

company loginCompany(char loginCompanyName[], char password[], int *correctPw){
    *correctPw = 0;
    company loginRecord;
    FILE *fptr;
    fptr = fopen("Companies.dat", "rb");
    if(fptr==NULL)
        printf("\n\n------------Something Gone Wrong Internally--------------\n");
    else{
        while(fread(&loginRecord, sizeof(loginRecord), 1, fptr) != 0){
            if(strcmp(loginCompanyName, loginRecord.companyName)==0){
                if(strcmp(SHA256(password), loginRecord.password)==0){
                    *correctPw = 1;
                    fclose(fptr);
                    return loginRecord;
                }
            }
        }
        fclose(fptr);
    }
}


void printJobs(company cp){
    printf("\n%-10s%s\n", "Sl. No", "Job");
    for(int i=0; i<cp.noOfJobs; i++){
        printf("%-10d%s\n", i+1, cp.jobs[i].job);
    }
    printf("\n");
}

void printJobProfile(job jb){
    printf("\n\n----------------------%s-----------------------\n\n", jb.job);
    //Print details
    printf("    %-25s:   %d\n", "Salary", jb.salary);
    printf("    %-25s:   %s\n", "Location", jb.location);
    printf("    %-25s:   %s\n", "Job Type", jb.jobType);
    printf("    %-25s:   %s\n", "Company Type", jb.companyType);
    printf("    %-25s:   %s\n\n", "Work From Home", jb.workFromHome=='Y'?"Yes":"No");
    //Print constraints
    printf("    %-30s%-10s%s\n", "Constraints", "Values", "Complusory Constraint");
    printf("    %-30s%-10c%s\n", "Shift", jb.shift, jb.shift_weightage==COMPULSORY_WEIGHTAGE_COMPANY?"Yes":"No");
    printf("    %-30s%-10d%s\n", "Age", jb.exp_age, jb.age_weightage==COMPULSORY_WEIGHTAGE_COMPANY?"Yes":"No");
    printf("    %-30s%-10d%s\n", "Experience", jb.exp_xp, jb.xp_weightage==COMPULSORY_WEIGHTAGE_COMPANY?"Yes":"No");
    printf("    %-30s%-10s%s\n", "Qualification", jb.exp_qualification, jb.qualification_weightage==COMPULSORY_WEIGHTAGE_COMPANY?"Yes":"No");
    printf("    %-30s%-10.2f%s\n", "10th Percentage", jb.exp_tenthPercent, jb.tenthPercent_weightage==COMPULSORY_WEIGHTAGE_COMPANY?"Yes":"No");
    printf("    %-30s%-10.2f%s\n", "12th Percentage", jb.exp_twelthPercent, jb.twelthPercent_weightage==COMPULSORY_WEIGHTAGE_COMPANY?"Yes":"No");
    printf("    %-30s%-10.2f%s\n\n", "CGPA", jb.exp_cgpa, jb.cgpa_weightage==COMPULSORY_WEIGHTAGE_COMPANY?"Yes":"No");
    printf("-------------------------------------------------\n\n");
}

void printJobConstraints(job jb){
    printf("----------------------------------\n");
    printf("               %s                 \n\n", jb.job);
    printf("%-6s%-30s%s\n", "SlNo", "Constraints", "Values");
    printf("1.    %-30s%c\n", "Shift", jb.shift);
    printf("2.    %-30s%d\n", "Age", jb.exp_age);
    printf("3.    %-30s%d\n", "Experience", jb.exp_xp);
    printf("4.    %-30s%s\n", "Qualification", jb.exp_qualification);
    printf("5.    %-30s%.2f\n", "10th Percentage", jb.exp_tenthPercent);
    printf("6.    %-30s%.2f\n", "12th Percentage", jb.exp_twelthPercent);
    printf("7.    %-30s%.2f\n\n", "CGPA", jb.exp_cgpa);
}

void printJobDetails(job jb){
    printf("\n\n----------------------%s-----------------------\n\n", jb.job);
    printf("    %-25s:   %d\n", "Salary", jb.salary);
    printf("    %-25s:   %s\n", "Location", jb.location);
    printf("    %-25s:   %s\n", "Job Type", jb.jobType);
    printf("    %-25s:   %s\n", "Company Type", jb.companyType);
    printf("    %-25s:   %s\n\n", "Work From Home", jb.workFromHome=='Y'?"Yes":"No");
    printf("-------------------------------------------------\n\n");
}