#include "score.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

struct knn_data createModel(int k, char file[])
{
	struct knn_data knn;
	knn.k = k;
	knn.best_voters = (struct samples **) malloc(knn.k * sizeof(struct samples *));
	parseFileToSamples(&knn, file);
	return knn;
}

double predictTestData(struct knn_data knn, char uncategorizedFile[], char categorizedFile[]){
	parseFileToSamples(&knn, uncategorizedFile);
	knnAlgorithm(&knn);
	int correctPredictions = 0, j;
	double accuracyScore;
	FILE *fp;
	char line[30];
	fp = fopen(categorizedFile, "r");
	fseek(fp, 19, SEEK_SET);
	for(int i=0; i<knn.samples_count[1]; i++){
		j = 0;
		while(line[j-1]!='\n'){
			line[j] = getc(fp);
			j++;
			if(i==knn.samples_count[1]-1){
				break;
			}
		}
		line[j] = '\0';
		if(atoi(&line[0])==(knn.samples[1]+i)->group){
			correctPredictions++;
		}
	}
	fclose(fp);
	accuracyScore = (double)correctPredictions/knn.samples_count[1];
	return accuracyScore;
}

int predict(struct knn_data knn, int data, int weightage, char dataFile[]){
	FILE *fptr;

	fptr = fopen(INPUT_SAMPLE, "r+");
	while(getc(fptr)!='\n'){}
	putc('1', fptr);
	fprintf(fptr, "\n%d,%d", data, weightage);
	fclose(fptr);

	parseFileToSamples(&knn, INPUT_SAMPLE);
	knnAlgorithm(&knn);

	return (knn.samples[0])->group;
}

void parseStringToSample(struct sample * sample, char * string, uint32_t max_dimensions, uint8_t has_group)
{
	int tmp_cnt = has_group == 0 ? 0 : 1;
	char * tmp_ptr = strtok(string, ",");
	if (has_group == 0)
		sample->group = atoi(tmp_ptr);
	else
		sample->dim[0] = atof(tmp_ptr);
	while ((tmp_ptr = strtok(NULL, ",")) != NULL)
		sample->dim[tmp_cnt++] = atof(tmp_ptr);
}

void parseFileToSamples(struct knn_data * knn, char * filepath)
{
	int tmp_cnt;
	char line[256];
	FILE *file_pointer;
	file_pointer = fopen(filepath, "r");
	fgets(line, 128, file_pointer);
	tmp_cnt = strstr(line, "uncategorized") == NULL ? 0 : 1;
	fgets(line, 128, file_pointer);
	knn->samples_count[tmp_cnt] = atoi(line);
	knn->samples[tmp_cnt] =
		(struct sample *) malloc(knn->samples_count[tmp_cnt] * sizeof(struct sample));
	fgets(line, 128, file_pointer);
	knn->samples_dimensions[tmp_cnt] = atoi(line);
	for (uint32_t i = 0; i < knn->samples_count[tmp_cnt]; i++)
	{
		(knn->samples[tmp_cnt] + i)->dim =
			(double *)malloc(knn->samples_dimensions[tmp_cnt] * sizeof(double));
		fgets(line, 128, file_pointer);
		parseStringToSample(knn->samples[tmp_cnt] + i, line,
			knn->samples_dimensions[tmp_cnt], tmp_cnt);
	}
}

void knnAlgorithmsSortAscVoters(struct knn_data * knn)
{
	struct sample * tmp_smpl = NULL;
	for (int i = 0; i < knn->k; i++)
		for (int j = 0; j < knn->k - 1; j++)
			if (knn->best_voters[j]->tmp_distance > knn->best_voters[j + 1]->tmp_distance)
			{
				tmp_smpl = knn->best_voters[j];
				knn->best_voters[j] = knn->best_voters[j + 1];
				knn->best_voters[j + 1] = tmp_smpl;
			}
}

void knnAlgorithm(struct knn_data * knn)
{
	double euclidean_distance;
	uint32_t * most_common[2], selected_group_pos;
	most_common[0] = (uint32_t *)malloc(knn->k * sizeof(uint32_t));
	most_common[1] = (uint32_t *)malloc(knn->k * sizeof(uint32_t));
	for (int i = 0; i < knn->samples_count[1]; i++)
	{
		for (int q = 0; q < knn->k; q++)
			knn->best_voters[q] = NULL;
		for (int j = 0; j < knn->samples_count[0]; j++)
		{
			euclidean_distance = 0;
			for (int q = 0; q < knn->samples_dimensions[0]; q++)
				euclidean_distance += pow(
				(knn->samples[0] + j)->dim[q] - (knn->samples[1] + i)->dim[q]
					, 2);
			(knn->samples[0] + j)->tmp_distance = sqrt(euclidean_distance);
			if (j < knn->k)
			{
				knn->best_voters[j] = (knn->samples[0] + j);
			}
			else
			{
				if (j == knn->k)
					knnAlgorithmsSortAscVoters(knn);
				for (int q = 0; q < knn->k; q++)
					if (knn->best_voters[q]->tmp_distance > (knn->samples[0] + j)->tmp_distance)
					{
						for (int z = knn->k - 1; z >= q + 1; z--)
							knn->best_voters[z] = knn->best_voters[z - 1];
						knn->best_voters[q] = (knn->samples[0] + j);
						break;
					}
			}
		}

		memset(most_common[0], 0, knn->k * sizeof(uint32_t));
		memset(most_common[1], 0, knn->k * sizeof(uint32_t));

		for (int j = 0; j < knn->k; j++)
		{
			for (int q = 0; q < knn->k; q++)
				if (*(most_common[0] + q) == knn->best_voters[j]->group)
				{
					*(most_common[1] + q) += 1;
					break;
				}
				else if (*(most_common[0] + q) == 0)
				{
					*(most_common[0] + q) = knn->best_voters[j]->group;
					*(most_common[1] + q) += 1;
					break;
				}
		}

		selected_group_pos = 0;

		for (int j = 1; j < knn->k; j++)
		{
			if (*(most_common[1] + j) == 0)
				break;
			if (*(most_common[0] + j) > *(most_common[0] + selected_group_pos))
				selected_group_pos = j;
		}
		(knn->samples[1] + i)->group = *(most_common[0] + selected_group_pos);
	}
}


int getOptimumKValue(enum Constraints constraint){
	char uncategorizedTestFile[100], categorizedtestFile[100], trainFile[100];
	switch(constraint){
		case JobAge:
			strcpy(uncategorizedTestFile, JOB_TEST_U_AGE);
			strcpy(categorizedtestFile, JOB_TEST_C_AGE);
			strcpy(trainFile, JOB_TRAIN_AGE);
			break;
		case JobExperience:
			strcpy(uncategorizedTestFile, JOB_TEST_U_XP);
			strcpy(categorizedtestFile, JOB_TEST_C_XP);
			strcpy(trainFile, JOB_TRAIN_XP);
			break;
		case JobQualification:
			strcpy(uncategorizedTestFile, JOB_TEST_U_QUAL);
			strcpy(categorizedtestFile, JOB_TEST_C_QUAL);
			strcpy(trainFile, JOB_TRAIN_QUAL);
			break;
		case JobShift:
			strcpy(uncategorizedTestFile, JOB_TEST_U_SHIFT);
			strcpy(categorizedtestFile, JOB_TEST_C_SHIFT);
			strcpy(trainFile, JOB_TRAIN_SHIFT);
			break;
		case JobTenthPercent:
			strcpy(uncategorizedTestFile, JOB_TEST_U_TENTH);
			strcpy(categorizedtestFile, JOB_TEST_C_TENTH);
			strcpy(trainFile, JOB_TRAIN_TENTH);
			break;
		case JobTwelthPercent:
			strcpy(uncategorizedTestFile, JOB_TEST_U_TWELTH);
			strcpy(categorizedtestFile, JOB_TEST_C_TWELTH);
			strcpy(trainFile, JOB_TRAIN_TWELTH);
			break;
		case JobCGPA:
			strcpy(uncategorizedTestFile, JOB_TEST_U_CGPA);
			strcpy(categorizedtestFile, JOB_TEST_C_CGPA);
			strcpy(trainFile, JOB_TRAIN_CGPA);
			break;
		case SeekerCompanyType:
			strcpy(uncategorizedTestFile, SEEKER_TEST_U_COMPANYTYPE);
			strcpy(categorizedtestFile, SEEKER_TEST_C_COMPANYTYPE);
			strcpy(trainFile, SEEKER_TRAIN_COMPANYTYPE);
			break;
		case SeekerJobType:
			strcpy(uncategorizedTestFile, SEEKER_TEST_U_JOBTYPE);
			strcpy(categorizedtestFile, SEEKER_TEST_C_JOBTYPE);
			strcpy(trainFile, SEEKER_TRAIN_JOBTYPE);
			break;
		case SeekerSalary:
			strcpy(uncategorizedTestFile, SEEKER_TEST_U_SALARY);
			strcpy(categorizedtestFile, SEEKER_TEST_C_SALARY);
			strcpy(trainFile, SEEKER_TRAIN_SALARY);
			break;
		case SeekerShift:
			strcpy(uncategorizedTestFile, SEEKER_TEST_U_SHIFT);
			strcpy(categorizedtestFile, SEEKER_TEST_C_SHIFT);
			strcpy(trainFile, SEEKER_TRAIN_SHIFT);
			break;
		case SeekerWorkFromHome:
			strcpy(uncategorizedTestFile, SEEKER_TEST_U_WORKFROMHOME);
			strcpy(categorizedtestFile, SEEKER_TEST_C_WORKFROMHOME);
			strcpy(trainFile, SEEKER_TRAIN_WORKFROMHOME);
			break;
		case SeekerLocation:
			strcpy(uncategorizedTestFile, SEEKER_TEST_U_LOCATION);
			strcpy(categorizedtestFile, SEEKER_TEST_C_LOCATION);
			strcpy(trainFile, SEEKER_TRAIN_LOCATION);
			break;
	}

	int optK=1;
	double maxAcc = 0;
	double kAccuracy;
	for(int i=1; i<=25; i+=2){
		struct knn_data knn = createModel(i, trainFile);
		kAccuracy = predictTestData(knn, uncategorizedTestFile, categorizedtestFile);
		if(kAccuracy>maxAcc){
			maxAcc = kAccuracy;
			optK = i;
		}
	}
	return optK;
}

// int main(){
// 	int choice;
// 	printf("Seeker(1) or Company(2): ");
// 	scanf("%d", &choice);
// 	if(choice == 1){
// 		int salary, salary_score, salary_weightage, location, location_weightage, location_score, jobType, jobType_score, jobType_weightage, companyType, companyType_score, companyType_weightage, shift, shift_score, shift_weightage, workFromHome, workFromHome_score, workFromHome_weightage;
// 		int rank[6], score[6];
// 		printf("1. Enter Salary: ");
// 		scanf("%d", &salary);
// 		printf("   Enter Salary Weightage: ");
// 		scanf("%d", &salary_weightage);
// 		printf("2. Enter Location: ");
// 		scanf("%d", &location);
// 		printf("   Enter Location Weightage: ");
// 		scanf("%d", &location_weightage);
// 		printf("3. Enter Shift(D-1/N-2): ");
// 		scanf("%d", &shift);
// 		printf("   Enter Shift weightage: ");
// 		scanf("%d", &shift_weightage);
// 		printf("4. Enter jobType (Full Time-1/Part Time-2): ");
// 		scanf("%d", &jobType);
// 		printf("   Enter jobType weightage: ");
// 		scanf("%d", &jobType_weightage);
// 		printf("5. Enter companyType (Startup-1/Small business-2/Corporation-3): ");
// 		scanf("%d", &companyType);
// 		printf("   Enter companyType weightage: ");
// 		scanf("%d", &companyType_weightage);
// 		printf("6. Enter workFromHome (Y-1/N-2): ");
// 		scanf("%d", &workFromHome);
// 		printf("   Enter workFromHome weightaage: ");
// 		scanf("%d", &workFromHome_weightage);

// 		printf("\nEnter preference order: ");
// 		scanf("%d %d %d %d %d %d", &rank[0], &rank[1], &rank[2], &rank[3], &rank[4], &rank[5]);

// 		for(int i=0; i<6; i++){
// 			switch(rank[i]){
// 				case 1:
// 					score[0] = 6-i;
// 					break;
// 				case 2:
// 					score[1] = 6-i;
// 					break;
// 				case 3:
// 					score[2] = 6-i;
// 					break;
// 				case 4:
// 					score[3] = 6-i;
// 					break;
// 				case 5:
// 					score[4] = 6-i;
// 					break;
// 				case 6:
// 					score[5] = 6-i;
// 					break;
// 			}
// 		}

// 		FILE *fptr;
// 		fptr = fopen(SEEKER_TEST_C_SALARY, "a");
// 		fprintf(fptr, "%d,%d,%d\n", score[0], salary, salary_weightage);
// 		fclose(fptr);
// 		fptr = fopen(SEEKER_TEST_C_LOCATION, "a");
// 		fprintf(fptr, "%d,%d,%d\n", score[1], location, location_weightage);
// 		fclose(fptr);
// 		fptr = fopen(SEEKER_TEST_C_SHIFT, "a");
// 		fprintf(fptr, "%d,%d,%d\n", score[2], shift, shift_weightage);
// 		fclose(fptr);
// 		fptr = fopen(SEEKER_TEST_C_JOBTYPE, "a");
// 		fprintf(fptr, "%d,%d,%d\n", score[3], jobType, jobType_weightage);
// 		fclose(fptr);
// 		fptr = fopen(SEEKER_TEST_C_COMPANYTYPE, "a");
// 		fprintf(fptr, "%d,%d,%d\n", score[4], companyType, companyType_weightage);
// 		fclose(fptr);
// 		fptr = fopen(SEEKER_TEST_C_WORKFROMHOME, "a");
// 		fprintf(fptr, "%d,%d,%d\n", score[5], workFromHome, workFromHome_weightage);
// 		fclose(fptr);
// 	}
// 	else if(choice == 2){
// 		int age, age_weightage, cgpa, cgpa_weightage, xp, xp_weightage, qual, qual_weightage, shift, shift_weightage, tenth, tenth_weightage, twelth, twelth_weightage;
// 		int rank[7], score[7];
// 		printf("1. Enter Age: ");
// 		scanf("%d", &age);
// 		printf("   Enter Age Weightage: ");
// 		scanf("%d", &age_weightage);
// 		printf("2. Enter CGPA: ");
// 		scanf("%d", &cgpa);
// 		printf("   Enter CGPA Weightage: ");
// 		scanf("%d", &cgpa_weightage);
// 		printf("3. Enter experience: ");
// 		scanf("%d", &xp);
// 		printf("   Enter exp weightage: ");
// 		scanf("%d", &xp_weightage);
// 		printf("4. Enter qualification: ");
// 		scanf("%d", &qual);
// 		printf("   Enter qualification weightage: ");
// 		scanf("%d", &qual_weightage);
// 		printf("5. Enter Shift(D-1/N-2): ");
// 		scanf("%d", &shift);
// 		printf("   Enter shift weightage: ");
// 		scanf("%d", &shift_weightage);
// 		printf("6. Enter tenth percent: ");
// 		scanf("%d", &tenth);
// 		printf("   Enter tenth percent weightaage: ");
// 		scanf("%d", &tenth_weightage);
// 		printf("7. Enter twelth percent: ");
// 		scanf("%d", &twelth);
// 		printf("   Enter tenth percent weightaage: ");
// 		scanf("%d", &twelth_weightage);

// 		printf("\nEnter preference order: ");
// 		scanf("%d %d %d %d %d %d %d", &rank[0], &rank[1], &rank[2], &rank[3], &rank[4], &rank[5], &rank[6]);

// 		for(int i=0; i<7; i++){
// 			switch(rank[i]){
// 				case 1:
// 					score[0] = 7-i;
// 					break;
// 				case 2:
// 					score[1] = 7-i;
// 					break;
// 				case 3:
// 					score[2] = 7-i;
// 					break;
// 				case 4:
// 					score[3] = 7-i;
// 					break;
// 				case 5:
// 					score[4] = 7-i;
// 					break;
// 				case 6:
// 					score[5] = 7-i;
// 					break;
// 				case 7:
// 					score[6] = 7-i;
// 					break;
// 			}
// 		}

// 		FILE *fptr;
// 		fptr = fopen(JOB_TEST_C_AGE, "a");
// 		fprintf(fptr, "%d,%d,%d\n", score[0], age, age_weightage);
// 		fclose(fptr);
// 		fptr = fopen(JOB_TEST_C_CGPA, "a");
// 		fprintf(fptr, "%d,%d,%d\n", score[1], cgpa, cgpa_weightage);
// 		fclose(fptr);
// 		fptr = fopen(JOB_TEST_C_XP, "a");
// 		fprintf(fptr, "%d,%d,%d\n", score[2], xp, xp_weightage);
// 		fclose(fptr);
// 		fptr = fopen(JOB_TEST_C_QUAL, "a");
// 		fprintf(fptr, "%d,%d,%d\n", score[3], qual, qual_weightage);
// 		fclose(fptr);
// 		fptr = fopen(JOB_TEST_C_SHIFT, "a");
// 		fprintf(fptr, "%d,%d,%d\n", score[4], shift, shift_weightage);
// 		fclose(fptr);
// 		fptr = fopen(JOB_TEST_C_TENTH, "a");
// 		fprintf(fptr, "%d,%d,%d\n", score[5], tenth, tenth_weightage);
// 		fclose(fptr);
// 		fptr = fopen(JOB_TEST_C_TWELTH, "a");
// 		fprintf(fptr, "%d,%d,%d\n", score[6], twelth, twelth_weightage);
// 		fclose(fptr);
// 	}

// 	return 0;
// }