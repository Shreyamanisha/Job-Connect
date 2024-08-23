#include <stdio.h>
#include <ctype.h>
#include <string.h>

int checkAlphabetsInIntegers(char stringVal[]){
    int charErr = 0;
    for(int i=0; stringVal[i]!='\0'; i++){
            if(i==0 && stringVal[0]=='-')
                continue;
            if(!isdigit(stringVal[i])){
                charErr = 1;
                break;
            }
        }
    return charErr;
}

int checkAlphabetsInFloat(char stringVal[]){
    int charErr = 0;
    for(int i=0; stringVal[i]!='\0'; i++){
        if(i==0 && stringVal[0]=='-')
            continue;
        if(stringVal[i]=='.')
            continue;
        if(!isdigit(stringVal[i])){
            charErr = 1;
            break;
        }
    }
    return charErr;
}

int validatePassword(char pw[]){
    int hasUpper=0, hasLower=0, hasDigit=0, hasSpecial=0, hasSpace=0;
    if(strlen(pw)<8 || strlen(pw)>15){
        printf("Password length must be between 8 - 15...\n");
        return 0;
    }
    for(int i=0; pw[i]!='\0'; i++){
        if(isupper(pw[i]))
            hasUpper = 1;
        else if(islower(pw[i]))
            hasLower = 1;
        else if(isdigit(pw[i]))
            hasDigit = 1;
        else if(pw[i]==' ')
            hasSpace = 1;
        else
            hasSpecial = 1;
    }
    if(hasSpace){
        printf("Password cannot contain spaces...");
        return 0;
    }
    if(!hasUpper){
        printf("Password must have atleast 1 uppercase character...");
        return 0;
    }
    if(!hasLower){
        printf("Password must have atleast 1 lowercase character...");
        return 0;
    }
    if(!hasDigit){
        printf("Password must have atleast 1 digit...");
        return 0;
    }
    if(!hasSpecial){
        printf("Password must have atleast 1 special character...");
        return 0;
    }
    return 1;
}

int checkPDF(char stringVal[]){
    int len = strlen(stringVal);
    if(stringVal[len-4] != '.')
        return 0;
    if(stringVal[len-3] != 'p')
        return 0;
    if(stringVal[len-2] != 'd')
        return 0;
    if(stringVal[len-1] != 'f')
        return 0;
    return 1;
}