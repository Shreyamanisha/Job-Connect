#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "seeker.h"
#include "ScoreCalculation/score.h"
#include "validation.h"
#include "sha256.h"

void assignSeekerWeightage(seeker *sk, int compConst[], int len){
    sk->salary_weightage = DESIRABLE_WEIGHTAGE_SEEKER;
    sk->location_weightage = DESIRABLE_WEIGHTAGE_SEEKER;
    sk->jobType_weightage = DESIRABLE_WEIGHTAGE_SEEKER;
    sk->companyType_weightage = DESIRABLE_WEIGHTAGE_SEEKER;
    sk->shift_weightage = DESIRABLE_WEIGHTAGE_SEEKER;
    sk->workFromHome_weightage = DESIRABLE_WEIGHTAGE_SEEKER;
    for(int i=0; i<len; i++){
        switch(compConst[i]){
            case 1:
                sk->salary_weightage = COMPULSORY_WEIGHTAGE_SEEKER;
                break;
            case 2:
                sk->location_weightage = COMPULSORY_WEIGHTAGE_SEEKER;
                break;
            case 3:
                sk->jobType_weightage = COMPULSORY_WEIGHTAGE_SEEKER;            
                break;
            case 4:
                sk->companyType_weightage= COMPULSORY_WEIGHTAGE_SEEKER;
                break;
            case 5:
                sk->shift_weightage = COMPULSORY_WEIGHTAGE_SEEKER;
                break;
            case 6:
                sk->workFromHome_weightage = COMPULSORY_WEIGHTAGE_SEEKER;
                break;
        }
    }
}

void assignSeekerScore(seeker *sk){
    int K, numericData;
    struct knn_data knn;
    //Salary
    K = getOptimumKValue(SeekerSalary);
    knn = createModel(K, SEEKER_TRAIN_SALARY);
    sk->salary_score = predict(knn, sk->exp_salary, sk->salary_weightage, SEEKER_TRAIN_SALARY);
    //JobType
    if(strcmp(sk->exp_jobType, "Full Time")==0)
        numericData = 1;
    else
        numericData = 2;
    K = getOptimumKValue(SeekerJobType);
    knn = createModel(K, SEEKER_TRAIN_JOBTYPE);
    sk->jobType_score = predict(knn, numericData, sk->jobType_weightage, SEEKER_TRAIN_JOBTYPE);
    //CompanyType
    if(strcmp(sk->exp_companyType, "Startup")==0)
        numericData = 1;
    else if(strcmp(sk->exp_companyType, "Small Business")==0)
        numericData = 2;
    else
        numericData = 3;
    K = getOptimumKValue(SeekerCompanyType);
    knn = createModel(K, SEEKER_TRAIN_COMPANYTYPE);
    sk->companyType_score = predict(knn, numericData, sk->companyType_weightage, SEEKER_TRAIN_COMPANYTYPE);
    //Shift
    if(sk->exp_shift == 'D')
        numericData = 1;
    else
        numericData = 2;
    K = getOptimumKValue(SeekerShift);
    knn = createModel(K, SEEKER_TRAIN_SHIFT);
    sk->shift_score = predict(knn, numericData, sk->shift_weightage, SEEKER_TRAIN_SHIFT);
    sk->location_score = 2;
    //WorkFromHome
    if(sk->workFromHome == 'Y')
        numericData = 1;
    else
        numericData = 2;
    K = getOptimumKValue(SeekerWorkFromHome);
    knn = createModel(K, SEEKER_TRAIN_WORKFROMHOME);
    sk->workFromHome_score = predict(knn, numericData, sk->workFromHome_weightage, SEEKER_TRAIN_WORKFROMHOME);
    //Location
    if(strcmp(sk->exp_location, "Chennai")==0)
        numericData = 1;
    if(strcmp(sk->exp_location, "Mumbai")==0)
        numericData = 2;
    if(strcmp(sk->exp_location, "Delhi")==0)
        numericData = 3;
    if(strcmp(sk->exp_location, "New Delhi")==0)
        numericData = 4;
    if(strcmp(sk->exp_location, "Kolkata")==0)
        numericData = 5;
    if(strcmp(sk->exp_location, "Bangalore")==0)
        numericData = 6;
    if(strcmp(sk->exp_location, "Hyderabad")==0)
        numericData = 7;
    if(strcmp(sk->exp_location, "Kerala")==0)
        numericData = 8;
    if(strcmp(sk->exp_location, "Coimbatore")==0)
        numericData = 9;
    if(strcmp(sk->exp_location, "Gujarat")==0)
        numericData = 10;
    if(strcmp(sk->exp_location, "Pune")==0)
        numericData = 11;
    K = getOptimumKValue(SeekerLocation);
    knn = createModel(K, SEEKER_TRAIN_LOCATION);
    sk->location_score = predict(knn, numericData, sk->location_weightage, SEEKER_TRAIN_LOCATION);
    printf("\n\nSalary: %d\n", sk->salary_score);
    printf("Location: %d\n", sk->location_score);
    printf("Shift: %d\n", sk->shift_score);
    printf("Job Type: %d\n", sk->jobType_score);
    printf("Company Type: %d\n", sk->companyType_score);
    printf("Work From Home: %d\n\n", sk->workFromHome_score);
}


seeker createSeeker(int *alreadyPresent){
    seeker sk;
    *alreadyPresent = 0;
    int seekerExists;
    int noOfCompulsoryConstraints;
    int jobTypeRef, companyTypeRef;
    char qualifications[10][10] = {"DIPLOMA", "10TH", "12TH", "BE", "BCOM", "BED", "ME", "MCOM", "MBBS", "PHD"};
    char locations[11][30] = {"Chennai", "Mumbai", "Delhi", "New Delhi", "Kolkata", "Bangalore", "Hyderabad", "Kerala", "Coimbatore", "Gujarat", "Pune"};
    int qualFound=0, locationFound = 0;
    char ageString[30], xpString[30], expSalaryString[30], expJobTypeString[30], expCompanyTypeString[30], shiftString[30], workFromHomeString[30], passwordString[30], confirmPassword[30], tenthString[30], twelthString[30], cgpaString[30];
    int ageInt, xpInt, expSalaryInt, expJobTypeInt, expCompanyTypeInt;
    float tenthFloat, twelthFloat, cgpaFloat;
    char noOfCompulsoryConstraintsString[30], compulsoryConstraintString[30];
    char tenthCertificate[1000], twelthCertificate[1000], ugCertificate[1000], aadhar[1000];
    int charError, passwordVal, pdfError, fileChar;

    sk.noOfAppliedJobs = 0;
    sk.noOfCalls = 0;
    sk.newlyAppeared = 0;

    //------------Getting Inputs Form the Job Seeker------------
    //Get Seeker Name
    fflush(stdin);
    printf("\nEnter Name: ");
    scanf("%[^\n]", sk.seekerName);
    *alreadyPresent = checkSeekerExistence(sk.seekerName);
    if(*alreadyPresent==1){
        return sk;
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
        printf("\n\nConfirm Password: ");
        scanf("%[^\n]", confirmPassword);
        if(strcmp(confirmPassword, passwordString)==0){
            strcpy(sk.password, SHA256(passwordString));
        }
        else
            printf("Passwords do not match...\n"); 
    }while(strcmp(confirmPassword, passwordString) != 0);
    //Get age
    while(1){
        fflush(stdin);
        printf("\n\nEnter Age(15-70): ");
        scanf("%[^\n]", ageString);
        charError = checkAlphabetsInIntegers(ageString);
        if(charError==1)
            printf("Invalid Input. Age cannot contain an alphabet or special character. Please enter a number between 15 - 70...\n");
        else{
            ageInt = atoi(ageString);
            if(ageInt<0){
                printf("Invalid Input. Age cannot be neagtive...\n");
            }
            else if(ageInt<15){
                printf("Your age is too young for an employee. A valid age ranges between 15 - 70...\n");
            }
            else if(ageInt>70){
                printf("Your age is too old for an employee. A valid age ranges between 15-70...\n");
            }
            else{
                sk.age = ageInt;
                break;
            }
        }    
    }
    //Get phone number
    while(1){
        fflush(stdin);
        printf("\n\nEnter Phone Number: ");
        if(scanf("%lli", &sk.phoneNumber)==0){
            printf("Invalid Phone Number. Phone number cannot contain alphabets...\n");
            continue;
        }
        else{
            if((sk.phoneNumber<1000000000)||(sk.phoneNumber>9999999999)){
                printf("Invalid Phone Number. Please enter a 10 digit Phone number...\n");
                continue;
            }
        }
        break;
    }
    //Get email
    fflush(stdin);
    printf("\n\nEnter email Id: ");
    scanf("%s", &sk.email);
    //Get 10th percentage
    while(1){
        fflush(stdin);
        printf("\n\nEnter 10th Percentage: ");
        scanf("%[^\n]", tenthString);
        charError = checkAlphabetsInFloat(tenthString);
        if(charError == 1){
            printf("Invalid Input. 10th Percentage cannot contain alphabets or special characters...\n");
            continue;
        }
        else{
            tenthFloat = atof(tenthString);
            if(tenthFloat<0){
                printf("Invalid Input. 10th Percentage cannot be neagtive...\n");
                continue;
            }
            else if(tenthFloat>100){
                printf("Invalid Input. 10th Percentage cannot be greater than 100...\n");
                continue;
            }
            break;
        }
    }
    sk.tenthPercentage = atof(tenthString);
    //Get 10th Document
    while(1){
        fflush(stdin);
        printf("\n\nEnter location of 10th Certificate (*PDF ONLY): ");
        scanf("%[^\n]", tenthCertificate);
        pdfError = checkPDF(tenthCertificate);
        if(pdfError == 0){
            printf("Invalid file format. Please upload a PDF file...\n");
            continue;
        }
        else{
            char outName[100] = "Documents\\10th\\";
            strcat(outName, sk.seekerName);
            strcat(outName, ".pdf");
            pdfError = copyDocument(tenthCertificate, outName);
            if(pdfError == 0)
                continue;
            break;
        }
    }
    //Get 12th percentage
    while(1){
        fflush(stdin);
        printf("\n\nEnter 12th Percentage (Enter -1 if not attended): ");
        scanf("%[^\n]", twelthString);
        charError = checkAlphabetsInFloat(twelthString);
        if(charError == 1){
            printf("Invalid Input. 12th Percentage cannot contain alphabets or special characters...\n");
            continue;
        }
        else{
            twelthFloat = atof(twelthString);
            if(twelthFloat == -1)
                break;
            if(twelthFloat<0){
                printf("Invalid Input. 12th Percentage cannot be neagtive...\n");
                continue;
            }
            else if(twelthFloat>100){
                printf("Invalid Input. 12th Percentage cannot be greater than 100...\n");
                continue;
            }
            break;
        }
    }
    sk.twelthPercentage = atof(twelthString);
    //Get 12th Certificate
    if(twelthFloat != -1){
        while(1){
            fflush(stdin);
            printf("\n\nEnter location of 12th Certificate (*PDF ONLY): ");
            scanf("%[^\n]", twelthCertificate);
            pdfError = checkPDF(twelthCertificate);
            if(pdfError == 0){
                printf("Invalid file format. Please upload a PDF file...\n");
                continue;
            }
            else{
                char outName[100] = "Documents\\12th\\";
                strcat(outName, sk.seekerName);
                strcat(outName, ".pdf");
                pdfError = copyDocument(twelthCertificate, outName);
                if(pdfError == 0)
                    continue;
                break;
            }
        }
    }
    //Get qualification
    do{
        fflush(stdin);
        printf("\n\nEnter Qualification: ");
        scanf("%s", sk.qualification);
        for(int i=0; sk.qualification[i]!='\0'; i++)
            sk.qualification[i] = toupper(sk.qualification[i]);
        for(int i=0; i<10; i++){
            if(strcmp(sk.qualification, qualifications[i])==0){
                qualFound = 1;
            }
        }
        if(qualFound==0)
            printf("Invalid Qualification. Please enter again...\n");
    }while(qualFound==0);
    if(sk.twelthPercentage!=-1){
        //Get college
        fflush(stdin);
        printf("\n\nEnter College Name(None if not attended): ");
        scanf("%[^\n]", sk.college);
        sk.college[0] = toupper(sk.college[0]);
        for(int i=0; sk.college[i]!='\0'; i++){
            if(sk.college[i]==' '){
                sk.college[i+1] = toupper(sk.college[i+1]);
            }
        }
        if(strcmp(sk.college, "None")!=0){
            //Get CGPA
            while(1){
                fflush(stdin);
                printf("\n\nEnter College CGPA (Enter -1 if not attended): ");
                scanf("%[^\n]", cgpaString);
                charError = checkAlphabetsInFloat(cgpaString);
                if(charError == 1){
                    printf("Invalid Input. CGPA cannot contain alphabets or special characters...\n");
                    continue;
                }
                else{
                    cgpaFloat = atof(cgpaString);
                    if(cgpaFloat == -1)
                        break;
                    if(cgpaFloat<0){
                        printf("Invalid Input. CGPA cannot be neagtive...\n");
                        continue;
                    }
                    else if(cgpaFloat>10){
                        printf("Invalid Input. 12th cannot be greater than 10...\n");
                        continue;
                    }
                    break;
                }
            }
            sk.cgpa = atof(cgpaString);
            //Get degree certificate
            if(cgpaFloat != -1){
                while(1){
                    fflush(stdin);
                    printf("\n\nEnter location of Degree Certificate (*PDF ONLY): ");
                    scanf("%[^\n]", ugCertificate);
                    pdfError = checkPDF(ugCertificate);
                    if(pdfError == 0){
                        printf("Invalid file format. Please upload a PDF file...\n");
                        continue;
                    }
                    else{
                        char outName[100] = "Documents\\Degree\\";
                        strcat(outName, sk.seekerName);
                        strcat(outName, ".pdf");
                        pdfError = copyDocument(ugCertificate, outName);
                        if(pdfError == 0)
                            continue;
                        break;
                    }
                }
            }
        }
        else
            sk.cgpa = -1;
    }
    else{
        sk.cgpa = -1;
        strcpy(sk.college, "None");
    }
    //Get Aadhar Card
    while(1){
        fflush(stdin);
        printf("\n\nEnter location of Aadhar Card (*PDF ONLY): ");
        scanf("%[^\n]", aadhar);
        pdfError = checkPDF(aadhar);
        if(pdfError == 0){
            printf("Invalid file format. Please upload a PDF file...\n");
            continue;
        }
        else{
            char outName[100] = "Documents\\Aadhar\\";
            strcat(outName, sk.seekerName);
            strcat(outName, ".pdf");
            pdfError = copyDocument(aadhar, outName);
            if(pdfError == 0)
                continue;
            break;
        }
    }
    //Get Experience
    while(1){
        fflush(stdin);
        printf("\n\nEnter Experience in years: ");
        scanf("%[^\n]", xpString);
        charError = checkAlphabetsInIntegers(xpString);
        if(charError == 1)
            printf("Invalid Input. Experience cannot contain an alphabet or special character...\n");
        else{
            xpInt = atoi(xpString);
            if(xpInt>=sk.age){
                printf("Experience is greater than Age. Please enter a valid experience...\n");
                continue;
            }
            else if(xpInt<0){
                printf("Invalid Input. Experience cannot be neagtive...\n");
                continue;
            }
            break;
        }
    }
    sk.xp = xpInt;
    //-------Constraints------
    //Get Expected Salary
    while(1){
        fflush(stdin);
        printf("\n\nEnter Expected Monthly Salary(Rs.5000 - Rs.1000000): ");
        scanf("%[^\n]", expSalaryString);
        charError = checkAlphabetsInIntegers(expSalaryString);
        if(charError==1)
            printf("Invalid Input. Salary cannot contain an alphabet or special character. Please enter a number between 5000 - 1000000...\n");
        else{
            expSalaryInt = atoi(expSalaryString);
            if(expSalaryInt<5000||expSalaryInt>1000000){
                printf("Invalid Input. Please enter a salary between in the range Rs.5000 - Rs.1000000\n");
                continue;
            }
            break;
        }
    }
    sk.exp_salary = expSalaryInt;
    //Get Expected Location
    do{
        fflush(stdin);
        printf("\n\nEnter expected location of workplace: ");
        scanf("%[^\n]", &sk.exp_location);
        sk.exp_location[0] = toupper(sk.exp_location[0]);
        for(int i=1; sk.exp_location[i]!='\0'; i++){
            if(sk.exp_location[i-1]==' ')
                sk.exp_location[i] = toupper(sk.exp_location[i]);
            else
                sk.exp_location[i] = tolower(sk.exp_location[i]);
        }
        for(int i=0; i<11; i++){
            if(strcmp(sk.exp_location, locations[i])==0)
                locationFound = 1;
        }
        if(locationFound == 0)
            printf("Location out of range of the application...\n");
    }while(locationFound==0);
    //Get Expected JobType
    printf("\n\nEnter Expected Job Type: \n");
    printf("\t1. Full Time\t2. Part Time\n");
    do{
        fflush(stdin);
        printf("\nEnter 1 or 2: ");
        scanf("%[^\n]", expJobTypeString);
        charError = checkAlphabetsInIntegers(expJobTypeString);
        if(charError == 1){
            printf("Invalid Input. Job Type cannot contain an alphabet or special character. Please enter 1 or 2...\n");
            continue;
        }
        else{
            expJobTypeInt = atoi(expJobTypeString);
            switch(expJobTypeInt){
                case 1:
                    strcpy(sk.exp_jobType, "Full Time");
                    break;
                case 2:
                    strcpy(sk.exp_jobType, "Part Time");
                    break;
                default:
                    printf("Invalid Input. Please fill with 1 or 2\n");
                    break;
            }
        }
    }while((expJobTypeInt!=1)&&(expJobTypeInt!=2));
    //Get Expected Company Type
    printf("\n\nEnter Company Type: \n");
    printf("\t1. Startup\t2. Small Business\t3. Corporation\n");
    do{
        fflush(stdin);
        printf("\nEnter 1, 2 or 3: ");
        scanf("%[^\n]", expCompanyTypeString);
        charError = checkAlphabetsInIntegers(expCompanyTypeString);
        if(charError == 1)
            printf("Invalid Input. Company Type cannot contain an alphabet or special character. Please enter 1, 2, or 3...\n");
        else{
            expCompanyTypeInt = atoi(expCompanyTypeString);
            switch(expCompanyTypeInt){
                case 1:
                    strcpy(sk.exp_companyType, "Startup");
                    break;
                case 2:
                    strcpy(sk.exp_companyType, "Small Business");
                    break;
                case 3:
                    strcpy(sk.exp_companyType, "Corporation");
                    break;
                default:
                    printf("Invalid Input. Please fill with 1,2 or 3\n");
                    break;
            }
        }
    }while((expCompanyTypeInt!=1)&&(expCompanyTypeInt!=2)&&(expCompanyTypeInt!=3));
    ///Get Expected Shift
    do{
    fflush(stdin);
    printf("\n\nEnter Shift (D-Day/N-Night): ");
    scanf("%[^\n]", shiftString);
    if(strlen(shiftString)!=1){
        printf("Invalid Input. Please fill with D or N...\n");
        continue;
    }
    else{
        shiftString[0] = toupper(shiftString[0]);
        if((shiftString[0]!='D')&&(shiftString[0]!='N')){
            printf("Invalid Input. Please fill with D or N...\n");
            continue;
        }
    }
    }while((shiftString[0]!='D')&&(shiftString[0]!='N'));
    sk.exp_shift = shiftString[0];
    //Get Work From Home
    do{
    fflush(stdin);
    printf("\n\nDo you prefer a Work From Home job (Y/N): ");
    scanf("%[^\n]", workFromHomeString);
    if(strlen(workFromHomeString)!=1){
        printf("Invalid Input. Please fill with Y or N...\n");
        continue;
    }
    else{
        workFromHomeString[0] = toupper(workFromHomeString[0]);
        if((workFromHomeString[0]!='Y')&&(workFromHomeString[0]!='N')){
            printf("Invalid Input. Please fill with Y or N...\n");
            continue;
        }
    }
    }while((workFromHomeString[0]!='Y')&&(workFromHomeString[0]!='N'));
    sk.workFromHome = workFromHomeString[0];
    printf("\n\n");
    //-----------------End of Getting Inputs------------------


    //----------------Get Compulsory Constraints--------------------
    printSeekerConstraints(sk);
    //Get number of compulsory contraints
    while(1){
        fflush(stdin);
        printf("\nEnter your no of Compulsory Constraints (Max limit is %d): ", MAX_COMP_CONST_SEEKER);
        scanf("%[^\n]", noOfCompulsoryConstraintsString);
        charError = checkAlphabetsInIntegers(noOfCompulsoryConstraintsString);
        if(charError==1){
                printf("Please enter a number between 0 - %d\n", MAX_COMP_CONST_SEEKER);
                continue;
        }
        else{
            noOfCompulsoryConstraints = atoi(noOfCompulsoryConstraintsString);
            if((noOfCompulsoryConstraints>MAX_COMP_CONST_SEEKER)||(noOfCompulsoryConstraints<0)){
                printf("Please enter a number between 0 - %d\n", MAX_COMP_CONST_SEEKER);
                continue;
            }
        }
        break;
    }
    if(noOfCompulsoryConstraints>0){
        //Get the compulsory contraints
        int compulsoryConstraints[noOfCompulsoryConstraints];
        printf("\n");
        for(int i=0; i<noOfCompulsoryConstraints; i++){
            while(1){
                fflush(stdin);
                printf("Enter the Sl. No of your Compulsory Contraint(1 - %d): ", NO_OF_CONSTRAINTS_SEEKER);
                scanf("%[^\n]", compulsoryConstraintString);
                charError = checkAlphabetsInIntegers(compulsoryConstraintString);
                if(charError == 1){
                    printf("Input number out of range. Please enter a number between 1 - %d\n\n", NO_OF_CONSTRAINTS_SEEKER);
                    continue;
                }
                else{
                    compulsoryConstraints[i] = atoi(compulsoryConstraintString);
                    if((compulsoryConstraints[i]>NO_OF_CONSTRAINTS_SEEKER)||(compulsoryConstraints[i]<1)){
                        printf("Input number out of range. Please enter a number between 1 - %d\n\n", NO_OF_CONSTRAINTS_SEEKER);
                        continue;
                    }
                }
                break;
            }
        }
        assignSeekerWeightage(&sk, compulsoryConstraints, noOfCompulsoryConstraints);
    }
    else if(noOfCompulsoryConstraints == 0){
        sk.salary_weightage = DESIRABLE_WEIGHTAGE_SEEKER;
        sk.location_weightage = DESIRABLE_WEIGHTAGE_SEEKER;
        sk.jobType_weightage = DESIRABLE_WEIGHTAGE_SEEKER;
        sk.companyType_weightage = DESIRABLE_WEIGHTAGE_SEEKER;
        sk.shift_weightage = DESIRABLE_WEIGHTAGE_SEEKER;
        sk.workFromHome_weightage = DESIRABLE_WEIGHTAGE_SEEKER;   
    }
    //-----------End of Getting Compulsory Constraints------------

    //Assing Score and Weightage
    assignSeekerScore(&sk);
    
    return sk;
}

int copyDocument(char inName[], char outName[]){
    int fileChar;
    FILE *in;
    FILE *out;
    in = fopen(inName, "rb");
    if(in == NULL){
        printf("File location doesn't exists. Please check for the file location...\n");
        return 0;
    }
    out = fopen(outName, "wb");
    while((fileChar = getc(in)) != EOF){
        putc(fileChar, out);
    }
    fclose(out);
    fclose(in);
    return 1;
}

void writeSeekerFile(seeker sk){
    FILE *fptr;
    fptr = fopen("Seekers.dat", "rb+");
    if(fptr==NULL)
        printf("\n\n------------Something Gone Wrong Internally--------------\n");
    else{
        fseek(fptr, 0, SEEK_END);
        fwrite(&sk, sizeof(sk), 1, fptr);
        fclose(fptr);
    }
}

void updateSeekerProfile(seeker sk_write){
    seeker sk_read;
    FILE *fptr;
    fptr = fopen("Seekers.dat", "rb+");
    if(fptr==NULL)
        printf("\n\n------------Something Gone Wrong Internally--------------\n");
    else{
        while(fread(&sk_read, sizeof(sk_read), 1, fptr) != 0){
            if(strcmp(sk_read.seekerName, sk_write.seekerName) == 0){
                fseek(fptr, -sizeof(sk_read), SEEK_CUR);
                fwrite(&sk_write, sizeof(sk_write), 1, fptr);
                fclose(fptr);
                return;
            }
        }
        fclose(fptr);
    }
}

int checkSeekerExistence(char seekerName[]){
    seeker seekerData;
    FILE *fptr;
    fptr = fopen("Seekers.dat", "rb");
    if(fptr==NULL)
        printf("\n\n------------Something Gone Wrong Internally--------------\n");\
    else{
        while(fread(&seekerData, sizeof(seekerData), 1, fptr) != 0){
            if(strcmp(seekerName, seekerData.seekerName)==0){
                fclose(fptr);
                return 1;
            }
        }
        fclose(fptr);
    }
    return 0;
}

seeker loginSeeker(char loginSeekerName[], char password[], int *correctPw){
    *correctPw = 0;
    seeker loginRecord;
    FILE *fptr;
    fptr = fopen("Seekers.dat", "rb");
    if(fptr==NULL)
        printf("\n\n------------Something Gone Wrong Internally--------------\n");
    else{
        while(fread(&loginRecord, sizeof(loginRecord), 1, fptr) != 0){
            if(strcmp(loginSeekerName, loginRecord.seekerName)==0){
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

void printSeekerProfile(seeker sk){
    printf("\n\n---------------------------------------------------------%s----------------------------------------------------------\n\n", sk.seekerName);
    //Print details
    printf("    %-25s:   %d\n", "Age", sk.age);
    printf("    %-25s:   %lli\n", "Phone No", sk.phoneNumber);
    printf("    %-25s:   %s\n", "Email Id", sk.email);
    printf("    %-25s:   %s\n", "Qualification", sk.qualification);
    printf("    %-25s:   %.2f\n", "10th Percentage", sk.tenthPercentage);
    if(sk.twelthPercentage==-1)
        printf("    %-25s:   %s\n", "12th Percentage", "Not Attended");
    else
        printf("    %-25s:   %.2f\n", "12th Percentage", sk.twelthPercentage);
    printf("    %-25s:   %d years\n", "Experience", sk.xp);
    printf("    %-25s:   %s\n", "College", sk.college);
    if(sk.cgpa==-1)
        printf("    %-25s:   %s\n\n", "CGPA", "Not Attended");
    else
        printf("    %-25s:   %.2f\n\n", "CGPA", sk.cgpa);
    //Print constraints
    printf("    %-30s%-20s%s\n", "Constraints", "Values", "Complusory Constraint");
    printf("    %-30s%-20d%s\n", "Salary", sk.exp_salary, sk.salary_weightage==COMPULSORY_WEIGHTAGE_SEEKER?"Yes":"No");
    printf("    %-30s%-20s%s\n", "Location", sk.exp_location, sk.location_weightage==COMPULSORY_WEIGHTAGE_SEEKER?"Yes":"No");
    printf("    %-30s%-20s%s\n", "Job Type", sk.exp_jobType, sk.jobType_weightage==COMPULSORY_WEIGHTAGE_SEEKER?"Yes":"No");
    printf("    %-30s%-20s%s\n", "Company Type", sk.exp_companyType, sk.companyType_weightage==COMPULSORY_WEIGHTAGE_SEEKER?"Yes":"No");
    printf("    %-30s%-20c%s\n", "Shift", sk.exp_shift, sk.shift_weightage==COMPULSORY_WEIGHTAGE_SEEKER?"Yes":"No");
    printf("    %-30s%-20c%s\n\n", "Work From Home", sk.workFromHome, sk.workFromHome_weightage==COMPULSORY_WEIGHTAGE_SEEKER?"Yes":"No");
}

void printSeekerConstraints(seeker sk){      
    printf("------------------------------------------------------------\n");
    printf("                  %s                    \n\n", sk.seekerName);
    printf("%-6s%-30s%s\n", "SlNo", "Constraints", "Values");
    printf("1.    %-30s%d\n", "Salary", sk.exp_salary);
    printf("2.    %-30s%s\n", "Location", sk.exp_location);
    printf("3.    %-30s%s\n", "Job Type", sk.exp_jobType);
    printf("4.    %-30s%s\n", "Company Type", sk.exp_companyType);
    printf("5.    %-30s%c\n", "Shift", sk.exp_shift);
    printf("6.    %-30s%c\n", "Work From Home", sk.workFromHome);
}

void printSeekerDetails(seeker sk){
    printf("\n\n---------------------------%s----------------------------\n\n", sk.seekerName);
    printf("    %-25s:   %d\n", "Age", sk.age);
    printf("    %-25s:   %lli\n", "Phone No", sk.phoneNumber);
    printf("    %-25s:   %s\n", "Email Id", sk.email);
    printf("    %-25s:   %s\n", "Qualification", sk.qualification);
    printf("    %-25s:   %.2f\n", "10th Percentage", sk.tenthPercentage);
    if(sk.twelthPercentage==-1)
        printf("    %-25s:   %s\n", "12th Percentage", "Not Attended");
    else
        printf("    %-25s:   %.2f\n", "12th Percentage", sk.twelthPercentage);
    printf("    %-25s:   %d years\n", "Experience", sk.xp);
    printf("    %-25s:   %s\n", "College", sk.college);
    if(sk.cgpa==-1)
        printf("    %-25s:   %s\n\n", "CGPA", "Not Attended");
    else
        printf("    %-25s:   %.2f\n\n", "CGPA", sk.cgpa);
}