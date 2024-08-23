#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <conio.h>
#include "company.h"
#include "seeker.h"
#include "recommend_jobs.h"
#include "recommend_seekers.h"
#include "applications.h"
#include "validation.h"

int main(){
    system("cls");
    char title[] = "\n\n=======================================================JOB RECOMMENDATION SYSTEM=======================================================\n\n";
    int userType, user, userExists, editUser;
    char editConstraint[30];
    char continueApp, userExistsLogin;
    char loginUser[30], loginPassword[30];
    int isCorrectPassword, charError;
    program_start: //label
    do{
        userType = 0;
        printf("%s", title);
        do{
            user = 0;
            fflush(stdin);
            printf("\nAre you a\n\t=> 1. Job Seeker\n\t=> 2. Talent Acquisition Manager\n\n");
            printf("Enter 1 or 2: ");
            if(scanf("%d", &userType)==0)
                printf("Invalid Input. Enter 1 or 2...\n\n");
            else{
                if(userType==1){
                    seeker_ui:
                    do{
                        seeker loggedInSeeker, seekerData;
                        fflush(stdin);
                        printf("\n\nDo you want to\n\t=> 1.Create a new Seeker account\n\t=> 2.Login\n");
                        printf("\nEnter -1 to go back\n");
                        printf("Enter 1, 2 or -1: ");  
                                                                                         
                        if(scanf("%d", &user)==0){
                            printf("Invalid Input. Enter 1,2 or -1...\n\n");
                            continue;
                        }
                        else{
                            if(user==1){
                                system("cls");
                                printf("%s", title);
                                printf("\n\n-------------------------------------------------------Create A New Seeker Account-----------------------------------------------------\n\n");
                                seekerData = createSeeker(&userExists);
                                int jobSlNo;
                                if(userExists==0){
                                    int recommendationLength, slNoInt, seekerMenuChoice;
                                    char slNoString[100], apply, applyConfirm;
                                    company recommendedCompanies[100];
                                    job recommendedJobs[100];
                                    writeSeekerFile(seekerData);
                                    printf("%d", seekerData.noOfAppliedJobs);
                                    recommendJobs(seekerData, recommendedCompanies, recommendedJobs, &recommendationLength);
                                    if(recommendationLength>0){
                                        sortJobs(recommendedCompanies, recommendedJobs, recommendationLength);
                                        printRecommendedJobs(recommendedCompanies, recommendedJobs, recommendationLength);
                                        while(1){
                                            fflush(stdin);
                                            printf("\nEnter -1 to go back");
                                            printf("\nEnter the Sl.No of the Job you want to explore: ");
                                            scanf("%[^\n]", slNoString);
                                            charError = checkAlphabetsInIntegers(slNoString);
                                            if(charError == 1){
                                                printf("Invalid Input. Please Enter a number between 1 - %d\n", recommendationLength);
                                                continue;
                                            }
                                            else{
                                                slNoInt = atoi(slNoString);
                                                if(slNoInt == -1){
                                                    system("cls");
                                                    printf("%s", title);
                                                    goto seeker_ui;
                                                }
                                                if(slNoInt<1 || slNoInt>recommendationLength){
                                                    printf("Invalid Input. Please Enter a number between 1 - %d\n", recommendationLength);
                                                    continue;
                                                }
                                                printJobDetails(recommendedJobs[slNoInt-1]);
                                                do{
                                                    fflush(stdin);
                                                    printf("Do you want to apply for this Job(Y/N): ");
                                                    scanf("%c", &apply);
                                                    apply = toupper(apply);
                                                    switch(apply){
                                                        case 'Y':
                                                            do{
                                                                fflush(stdin);
                                                                printf("Are you sure(Y/N): ");
                                                                scanf("%c", &applyConfirm);
                                                                applyConfirm = toupper(applyConfirm);
                                                                switch(applyConfirm){
                                                                    case 'Y':
                                                                        updateCompanyApplications(seekerData, recommendedJobs[slNoInt-1], recommendedCompanies[slNoInt-1]);
                                                                        strcpy(seekerData.appliedJobs[loggedInSeeker.noOfAppliedJobs].company, recommendedCompanies[slNoInt-1].companyName);
                                                                        strcpy(seekerData.appliedJobs[loggedInSeeker.noOfAppliedJobs].job, recommendedJobs[slNoInt-1].job);
                                                                        seekerData.appliedJobs[loggedInSeeker.noOfAppliedJobs].status = 0;
                                                                        seekerData.noOfAppliedJobs++;
                                                                        updateSeekerProfile(seekerData);
                                                                        break;
                                                                    case 'N':
                                                                        break;
                                                                    default:
                                                                        printf("Invalid Input. Please fill with Y or N...\n");
                                                                        break;
                                                                }
                                                            }while(applyConfirm!='Y' && applyConfirm!='N');
                                                            break;
                                                        case 'N':
                                                            break;
                                                        default:
                                                            printf("Invalid Input. Please fill with Y or N...\n");
                                                            break;
                                                    }
                                                }while(apply!='N' && apply!='Y');
                                            }
                                            break;
                                        }
                                    }
                                }
                                else{
                                    printf("Seeker name already exists...\n\n");
                                    do{
                                        fflush(stdin);
                                        printf("\nDo you want to login instead(Y/N): ");
                                        scanf("%c", &userExistsLogin);
                                        userExistsLogin = toupper(userExistsLogin);
                                        if(userExistsLogin=='Y'){
                                            do{
                                                printf("Enter Password: ");
                                                fflush(stdin);
                                                scanf("%[^\n]", loginPassword);
                                                loggedInSeeker = loginSeeker(seekerData.seekerName, loginPassword, &isCorrectPassword);
                                                if(strcmp(loginPassword, "-1")==0){
                                                    system("cls");
                                                    printf("%s", title);
                                                    goto seeker_ui;
                                                }
                                                if(isCorrectPassword){
                                                    goto logged_in_seeker;
                                                }
                                                else{
                                                    printf("Incorrect Password...\n");
                                                    printf("\nEnter -1 to go back\n");
                                                }
                                            }while(!isCorrectPassword || strcmp(loginPassword, "-1")==0);
                                        }
                                        else if(userExistsLogin=='N'){
                                            system("cls");
                                            printf("%s", title);
                                            goto seeker_ui;
                                        }
                                        else{
                                            printf("Invalid Input. Please Enter Y or N...\n");
                                        }
                                    }while(userExistsLogin!='Y'&&userExistsLogin!='N');
                                }
                            }
                            else if(user==2){
                                system("cls");
                                printf("%s", title);
                                printf("\n\n------------------------------------------------------------Login Seeker---------------------------------------------------------------\n\n");
                                do{
                                    fflush(stdin);
                                    printf("Enter -1 to go back\n");
                                    printf("Enter username: ");
                                    scanf("%[^\n]", &loginUser);
                                    if(strcmp(loginUser, "-1")==0){
                                        system("cls");
                                        printf("%s", title);
                                        goto seeker_ui;
                                    }
                                    userExists = checkSeekerExistence(loginUser);
                                    if(userExists==0){
                                        printf("\nThe given Seeker does not exist. Try again with a differenet seeker name\n\n");
                                    }
                                }while(userExists==0);
                                do{
                                printf("Enter Password: ");
                                fflush(stdin);
                                scanf("%[^\n]", loginPassword);
                                loggedInSeeker = loginSeeker(loginUser, loginPassword, &isCorrectPassword);
                                if(strcmp(loginPassword, "-1")==0){
                                    system("cls");
                                    printf("%s", title);
                                    goto seeker_ui;
                                }
                                if(isCorrectPassword){
                                    logged_in_seeker: //label
                                    system("cls");
                                    printf("%s", title);
                                    if(loggedInSeeker.newlyAppeared>0){
                                        printf("\n***You have %d new Interview Calls from Companies...***\n", loggedInSeeker.newlyAppeared);
                                        loggedInSeeker.newlyAppeared = 0;
                                        updateSeekerProfile(loggedInSeeker);
                                    }
                                    printSeekerProfile(loggedInSeeker);
                                    int recommendationLength, slNoInt, seekerMenuChoice;
                                    char slNoString[100], apply, applyConfirm;
                                    company recommendedCompanies[100];
                                    job recommendedJobs[100];
                                    printf("\nDo you want to\n\t=> 1.Check Recommended Jobs\n\t=> 2.Check Appliead Jobs\n");
                                    while(1){
                                        printf("\nEnter -1 to logout");
                                        printf("\nEnter 1,2 or -1: ");
                                        if(scanf("%d", &seekerMenuChoice)==0){
                                            printf("Invalid Input. Please Enter 1,2, or -1...\n");
                                            continue;
                                        }
                                        else{
                                            if(seekerMenuChoice == -1){
                                                system("cls");
                                                printf("%s", title);
                                                goto seeker_ui;
                                            }
                                            else if(seekerMenuChoice == 1){
                                                system("cls");
                                                printf("%s", title);
                                                recommendJobs(loggedInSeeker, recommendedCompanies, recommendedJobs, &recommendationLength);
                                                if(recommendationLength>0){
                                                    sortJobs(recommendedCompanies, recommendedJobs, recommendationLength);
                                                    printRecommendedJobs(recommendedCompanies, recommendedJobs, recommendationLength);
                                                    while(1){
                                                        fflush(stdin);
                                                        printf("\nEnter -1 to go back");
                                                        printf("\nEnter the Sl.No of the Job you want to explore: ");
                                                        scanf("%[^\n]", slNoString);
                                                        charError = checkAlphabetsInIntegers(slNoString);
                                                        if(charError == 1){
                                                            printf("Invalid Input. Please Enter a number between 1 - %d\n", recommendationLength);
                                                            continue;
                                                        }
                                                        else{
                                                            slNoInt = atoi(slNoString);
                                                            if(slNoInt == -1){
                                                                system("cls");
                                                                printf("%s", title);
                                                                goto logged_in_seeker;
                                                            }
                                                            if(slNoInt<1 || slNoInt>recommendationLength){
                                                                printf("Invalid Input. Please Enter a number between 1 - %d\n", recommendationLength);
                                                                continue;
                                                            }
                                                            printJobDetails(recommendedJobs[slNoInt-1]);
                                                            do{
                                                                fflush(stdin);
                                                                printf("Do you want to apply for this Job(Y/N): ");
                                                                scanf("%c", &apply);
                                                                apply = toupper(apply);
                                                                switch(apply){
                                                                    case 'Y':
                                                                        do{
                                                                            fflush(stdin);
                                                                            printf("Are you sure(Y/N): ");
                                                                            scanf("%c", &applyConfirm);
                                                                            applyConfirm = toupper(applyConfirm);
                                                                            switch(applyConfirm){
                                                                                case 'Y':
                                                                                    updateCompanyApplications(loggedInSeeker, recommendedJobs[slNoInt-1], recommendedCompanies[slNoInt-1]);
                                                                                    strcpy(loggedInSeeker.appliedJobs[loggedInSeeker.noOfAppliedJobs].company, recommendedCompanies[slNoInt-1].companyName);
                                                                                    strcpy(loggedInSeeker.appliedJobs[loggedInSeeker.noOfAppliedJobs].job, recommendedJobs[slNoInt-1].job);
                                                                                    loggedInSeeker.appliedJobs[loggedInSeeker.noOfAppliedJobs].status = 0;
                                                                                    loggedInSeeker.noOfAppliedJobs++;
                                                                                    updateSeekerProfile(loggedInSeeker);
                                                                                    break;
                                                                                case 'N':
                                                                                    break;
                                                                                default:
                                                                                    printf("Invalid Input. Please fill with Y or N...\n");
                                                                                    break;
                                                                            }
                                                                        }while(applyConfirm!='Y' && applyConfirm!='N');
                                                                        break;
                                                                    case 'N':
                                                                        goto logged_in_seeker;
                                                                        break;
                                                                    default:
                                                                        printf("Invalid Input. Please fill with Y or N...\n");
                                                                        break;
                                                                }
                                                            }while(apply!='N' && apply!='Y');
                                                        }
                                                        break;
                                                    }
                                                }
                                                printf("\n\n\nPress any button to go back...\n");
                                                getch();
                                                system("cls");
                                                printf("%s", title);
                                                goto logged_in_seeker;
                                            }
                                            else if(seekerMenuChoice == 2){
                                                system("cls");
                                                printf("%s", title);
                                                if(loggedInSeeker.noOfAppliedJobs>0)
                                                    printAppliedJobsStatus(loggedInSeeker);
                                                else
                                                    printf("\n\nYou haven't applied for any jobs yet...\n");
                                                printf("\n\n\nPress any button to go back...\n");
                                                getch();
                                                system("cls");
                                                printf("%s", title);
                                                goto logged_in_seeker;
                                            }
                                            else{
                                                printf("Invalid Input. Please Enter 1,2, or -1...\n");
                                                continue;
                                            }
                                            break;
                                        }
                                    }
                                    
                                }
                                else{
                                    printf("Incorrect Password...\n");
                                    printf("\nEnter -1 to go back\n");
                                }
                                }while(!isCorrectPassword || strcmp(loginPassword, "-1")==0);
                            }
                            else if(user == -1){
                                system("cls");
                                goto program_start;
                            }
                            else
                                printf("Invalid Input. Enter 1,2 or -1\n\n");
                            }
                    }while((user!=1)&&(user!=2)&&(user!=-1));
                }
                else if(userType==2){
                    company_ui: //label
                    do{
                        company loggedInCompany, companyData;
                        fflush(stdin);
                        printf("\nDo you want to\n\t=> 1.Create a new Company account\n\t=> 2.Login\n");
                        printf("\nEnter -1 to go back\n");
                        printf("Enter 1,2 or -1: ");
                        if(scanf("%d", &user)==0){
                            printf("Invalid Input. Enter 1,2 or -1...\n\n");
                            continue;
                        }
                        else{
                            if(user==1){
                                system("cls");
                                printf("%s", title);
                                printf("\n\n------------------------------------------------------Create A New Company Account-----------------------------------------------------\n\n");
                                companyData = createCompany(&userExists);
                                if(userExists==0){
                                    writeCompanyFile(companyData);
                                    recommendSeekers(companyData.jobs[0]);
                                }
                                else{
                                    printf("Company name already exists...\n\n");
                                    do{
                                        fflush(stdin);
                                        printf("\nDo you want to login instead(Y/N): ");
                                        scanf("%c", &userExistsLogin);
                                        userExistsLogin = toupper(userExistsLogin);
                                        if(userExistsLogin=='Y'){
                                            do{
                                                printf("Enter Password: ");
                                                fflush(stdin);
                                                scanf("%[^\n]", loginPassword);
                                                loggedInCompany = loginCompany(companyData.companyName, loginPassword, &isCorrectPassword);
                                                if(strcmp(loginPassword, "-1")==0){
                                                    system("cls");
                                                    printf("%s", title);
                                                    goto company_ui;
                                                }
                                                if(isCorrectPassword){
                                                    goto logged_in_company;
                                                }
                                                else{
                                                    printf("Incorrect Password...\n");
                                                    printf("\nEnter -1 to go back\n");
                                                }
                                            }while(!isCorrectPassword || strcmp(loginPassword, "-1")==0);
                                        }
                                        else if(userExistsLogin=='N'){
                                            system("cls");
                                            printf("%s", title);
                                            goto company_ui;
                                        }
                                        else{
                                            printf("Invalid Input. Please Enter Y or N...\n");
                                        }
                                    }while(userExistsLogin!='Y'&&userExistsLogin!='N');
                                }
                            }
                            else if(user==2){
                                int acqTeamChoice, jobProfile, applicantSlNoInt;
                                char seeRecommendedSeekers, applicantSlNoString[30], callApplicant, confirmCallApplicant;
                                system("cls");
                                printf("%s", title);
                                printf("\n\n------------------------------------------------------------Login Company--------------------------------------------------------------\n\n");
                                do{
                                    fflush(stdin);
                                    printf("Enter -1 to exit\n");
                                    printf("Enter Company Name: ");
                                    scanf("%[^\n]", &loginUser);
                                    if(strcmp(loginUser, "-1")==0){
                                        system("cls");
                                        printf("%s", title);
                                        goto company_ui;
                                    }
                                    userExists = checkCompanyExistence(loginUser);
                                    if(userExists==0)
                                        printf("The given Company does not exist. Try again with a differenet Company name\n\n");
                                }while(userExists==0);
                                if(userExists == 1){
                                    do{
                                        printf("Enter Password: ");
                                        fflush(stdin);
                                        scanf("%[^\n]", loginPassword);
                                        loggedInCompany = loginCompany(loginUser, loginPassword, &isCorrectPassword);
                                        if(strcmp(loginPassword, "-1")==0){
                                            system("cls");
                                            printf("%s", title);
                                            goto company_ui;
                                        }
                                        if(isCorrectPassword){
                                            logged_in_company: //label
                                            system("cls");
                                            printf("%s", title);
                                            printf("\n\n---------------------------------------%s-------------------------------------\n\n", loggedInCompany.companyName);
                                            companyApplications cpApp = getApplications(loggedInCompany);
                                            if(cpApp.newlyAppeared>0){
                                                printf("\n***You have %d new Applications...***\n\n", cpApp.newlyAppeared);
                                                resetNewlyAppeared(cpApp);
                                            }
                                            do{
                                                acqTeamChoice = 0;
                                                fflush(stdin);
                                                printf("\t=> 1.View Jobs\n\t=> 2.Add Jobs\n\t=> 3.View Applicants\n");
                                                printf("\nEnter -1 to go back");
                                                printf("\nEnter 1,2,3 or -1: ");
                                                if(scanf("%d", &acqTeamChoice) == 0){
                                                    printf("Invalid Input. Please Enter 1 or 2...\n\n");
                                                    continue;
                                                }
                                                else{
                                                    switch(acqTeamChoice){
                                                        case 1:
                                                            printJobs(loggedInCompany);
                                                            printf("\nEnter -1 to logout\n");
                                                            printf("Enter the Sl. No of Job to view its Profile: ");
                                                            scanf("%d", &jobProfile);
                                                            if(jobProfile == -1){
                                                                system("cls");
                                                                printf("%s", title);
                                                                goto logged_in_company;
                                                            }
                                                            system("cls");
                                                            printf("%s", title);
                                                            printJobProfile(loggedInCompany.jobs[jobProfile-1]);
                                                            do{
                                                                fflush(stdin);
                                                                printf("Do you want to see the Recommended Seekers for this Job(Y/N): ");
                                                                scanf("%c", &seeRecommendedSeekers);
                                                                seeRecommendedSeekers = toupper(seeRecommendedSeekers);
                                                                switch(seeRecommendedSeekers){
                                                                    case 'Y':
                                                                        recommendSeekers(loggedInCompany.jobs[jobProfile-1]);
                                                                        printf("\n\n\nPress any button to go back...\n");
                                                                        getch();
                                                                        system("cls");
                                                                        printf("%s", title);
                                                                        goto logged_in_company;
                                                                        break;
                                                                    case 'N':  
                                                                        goto logged_in_company;
                                                                        break;
                                                                    default:
                                                                        printf("Invalid Input. Please Enter Y or N...\n\n");
                                                                        break;
                                                                }
                                                            }while((seeRecommendedSeekers!='Y')&&(seeRecommendedSeekers!='N'));
                                                            break;
                                                        case 2:
                                                            printf("\n\n");
                                                            company cpAdd = createJob(loggedInCompany);
                                                            writeCompanyFile(cpAdd);
                                                            recommendSeekers(cpAdd.jobs[cpAdd.noOfJobs-1]);
                                                            printf("\n\n\nPress any button to continue...\n");
                                                            getch();
                                                            system("cls");
                                                            printf("%s", title);
                                                            loggedInCompany = cpAdd;
                                                            goto logged_in_company;
                                                            break;
                                                        case 3:
                                                            system("cls");
                                                            printf("%s", title);
                                                            if(cpApp.noOfApplicants>0){
                                                                printApplicants(cpApp);
                                                                printf("\n");
                                                                while(1){
                                                                    fflush(stdin);
                                                                    printf("\nEnter -1 to go back");
                                                                    printf("\nEnter the Sl No of the Applicant(1 - %d): ", cpApp.noOfApplicants);
                                                                    scanf("%[^\n]", applicantSlNoString);
                                                                    charError = checkAlphabetsInIntegers(applicantSlNoString);
                                                                    if(charError == 1){
                                                                        printf("Invalid Input. Please enter a number between 1 - %d\n", cpApp.noOfApplicants);
                                                                        continue;
                                                                    }
                                                                    else{
                                                                        applicantSlNoInt = atoi(applicantSlNoString);
                                                                        if(applicantSlNoInt == -1){
                                                                            system("cls");
                                                                            printf("%s", title);
                                                                            goto logged_in_company;
                                                                        }
                                                                        if(applicantSlNoInt<1 || applicantSlNoInt>cpApp.noOfApplicants){
                                                                            printf("Invalid Input. Please enter a number between 1 - %d\n", cpApp.noOfApplicants);
                                                                            continue;
                                                                        }
                                                                        else{
                                                                            printApplicantDetails(cpApp.appl[applicantSlNoInt-1].applicant);
                                                                            do{
                                                                                fflush(stdin);
                                                                                printf("\nDo you want to Accept or Reject the Application(Accept-A/Reject-R): ");
                                                                                scanf("%c", &callApplicant);
                                                                                callApplicant = toupper(callApplicant);
                                                                                switch(callApplicant){
                                                                                    case 'A':
                                                                                        do{
                                                                                            fflush(stdin);
                                                                                            printf("Are you sure you want to Accept the Application(Y/N): ");
                                                                                            scanf("%c", &confirmCallApplicant);
                                                                                            confirmCallApplicant = toupper(confirmCallApplicant);
                                                                                            switch(confirmCallApplicant){
                                                                                                case 'Y':
                                                                                                    updateSeekerApplication(cpApp.appl[applicantSlNoInt-1].applicant, cpApp.appl[applicantSlNoInt-1].jobId, cpApp.company, 1);
                                                                                                    printf("\n\n\nPress any button to continue...\n");
                                                                                                    getch();
                                                                                                    system("cls");
                                                                                                    printf("%s", title);
                                                                                                    goto logged_in_company;
                                                                                                    break;
                                                                                                case 'N':
                                                                                                    goto logged_in_company;
                                                                                                    break;
                                                                                                default:
                                                                                                    printf("Invalid Input. Please fill with A or R...\n");
                                                                                                    break;
                                                                                            }
                                                                                        }while(confirmCallApplicant!='A' && confirmCallApplicant!='R');
                                                                                        break;
                                                                                    case 'R':
                                                                                        do{
                                                                                            fflush(stdin);
                                                                                            printf("Are you sure you want to Reject the Application(Y/N): ");
                                                                                            scanf("%c", &confirmCallApplicant);
                                                                                            confirmCallApplicant = toupper(confirmCallApplicant);
                                                                                            switch(confirmCallApplicant){
                                                                                                case 'Y':
                                                                                                    updateSeekerApplication(cpApp.appl[applicantSlNoInt-1].applicant, cpApp.appl[applicantSlNoInt-1].jobId, cpApp.company, -1);
                                                                                                    printf("\n\n\nPress any button to continue...\n");
                                                                                                    getch();
                                                                                                    system("cls");
                                                                                                    printf("%s", title);
                                                                                                    goto logged_in_company;
                                                                                                    break;
                                                                                                case 'N':
                                                                                                    goto logged_in_company;
                                                                                                    break;
                                                                                                default:
                                                                                                    printf("Invalid Input. Please fill with A or R...\n");
                                                                                                    break;
                                                                                            }
                                                                                        }while(confirmCallApplicant!='A' && confirmCallApplicant!='R');
                                                                                        break;
                                                                                    default:
                                                                                        printf("Invalid Input. Please fill with Y or N...\n");
                                                                                        break;
                                                                                }
                                                                            }while(callApplicant!='Y' && callApplicant!='N');
                                                                            break;
                                                                        }
                                                                    }
                                                                }
                                                            }
                                                            else{
                                                                printf("\n\nNo Applications have been made to this Job...\n");
                                                                printf("\n\n\nPress any button to continue...\n");
                                                                getch();
                                                                system("cls");
                                                                printf("%s", title);
                                                                goto logged_in_company;
                                                                break;   
                                                            }
                                                            break;
                                                        case -1:
                                                            system("cls");
                                                            printf("%s", title);
                                                            goto company_ui;
                                                            break;
                                                        default:
                                                            printf("Invalid Input. Please Enter 1 or 2...\n\n");
                                                            break;
                                                    }
                                                }
                                            }while((acqTeamChoice!=1)&&(acqTeamChoice!=2)&&(acqTeamChoice!=3)&&(acqTeamChoice!=-1));
                                        }
                                        else{
                                            printf("Incorrect Password...\n");
                                            printf("\nEnter -1 to go back\n");
                                        }
                                    }while(!isCorrectPassword || strcmp(loginPassword, "-1")==0);
                                }
                            }
                            else if(user == -1){
                                system("cls");
                                goto program_start;
                            }
                            else
                                printf("Invalid Input. Enter 1, 2 or -1\n\n");
                        }
                    }while((user!=1)&&(user!=2)&&(user!=-1));
                }
                else{
                    printf("Invalid Input.Please Enter 1 or 2...\n\n");
                }
            } 
        }while((userType!=1)&&(userType!=2));
        printf("\n");
        do{
            fflush(stdin);
            printf("\nDo you want to continue.......(Y/N): ");
            scanf("%c", &continueApp);
            continueApp = toupper(continueApp);
            if((continueApp!='Y')&&(continueApp!='N'))
                printf("Invalid Input. Enter Y or N...\n");
        }while((continueApp!='Y')&&(continueApp!='N'));
        system("cls");
    }while(continueApp=='Y');
    printf("\n\n");

    return 0;
}