#ifndef SCORE_CALCULATION
#define SCORE_CALCULATION

#include <inttypes.h>
#define SEEKER_TRAIN_SALARY "ScoreCalculation\\LabelledSeekerConstraints\\Training Data\\SeekerSalary.txt"
#define SEEKER_TRAIN_JOBTYPE "ScoreCalculation\\LabelledSeekerConstraints\\Training Data\\SeekerJobType.txt"
#define SEEKER_TRAIN_SHIFT "ScoreCalculation\\LabelledSeekerConstraints\\Training Data\\SeekerShift.txt"
#define SEEKER_TRAIN_WORKFROMHOME "ScoreCalculation\\LabelledSeekerConstraints\\Training Data\\SeekerWorkFromHome.txt"
#define SEEKER_TRAIN_COMPANYTYPE "ScoreCalculation\\LabelledSeekerConstraints\\Training Data\\SeekerCompanyType.txt"
#define SEEKER_TRAIN_LOCATION "ScoreCalculation\\LabelledSeekerConstraints\\Training Data\\SeekerLocation.txt"
#define SEEKER_TEST_C_SALARY "ScoreCalculation\\LabelledSeekerConstraints\\Testing Data\\CategorizedSeekerSalary.txt"
#define SEEKER_TEST_C_JOBTYPE "ScoreCalculation\\LabelledSeekerConstraints\\Testing Data\\CategorizedSeekerJobType.txt"
#define SEEKER_TEST_C_SHIFT "ScoreCalculation\\LabelledSeekerConstraints\\Testing Data\\CategorizedSeekerShift.txt"
#define SEEKER_TEST_C_WORKFROMHOME "ScoreCalculation\\LabelledSeekerConstraints\\Testing Data\\CategorizedSeekerWorkFromHome.txt"
#define SEEKER_TEST_C_COMPANYTYPE "ScoreCalculation\\LabelledSeekerConstraints\\Testing Data\\CategorizedSeekerCompanyType.txt"
#define SEEKER_TEST_C_LOCATION "ScoreCalculation\\LabelledSeekerConstraints\\Testing Data\\CategorizedSeekerLocation.txt"
#define SEEKER_TEST_U_SALARY "ScoreCalculation\\LabelledSeekerConstraints\\Testing Data\\UncategorizedSeekerSalary.txt"
#define SEEKER_TEST_U_JOBTYPE "ScoreCalculation\\LabelledSeekerConstraints\\Testing Data\\UncategorizedSeekerJobType.txt"
#define SEEKER_TEST_U_SHIFT "ScoreCalculation\\LabelledSeekerConstraints\\Testing Data\\UncategorizedSeekerShift.txt"
#define SEEKER_TEST_U_WORKFROMHOME "ScoreCalculation\\LabelledSeekerConstraints\\Testing Data\\UncategorizedSeekerWorkFromHome.txt"
#define SEEKER_TEST_U_COMPANYTYPE "ScoreCalculation\\LabelledSeekerConstraints\\Testing Data\\UncategorizedSeekerCompanyType.txt"
#define SEEKER_TEST_U_LOCATION "ScoreCalculation\\LabelledSeekerConstraints\\Testing Data\\UncategorizedSeekerLocation.txt"

#define JOB_TRAIN_AGE "ScoreCalculation\\LabelledJobConstraints\\Training Data\\JobAge.txt"
#define JOB_TRAIN_XP "ScoreCalculation\\LabelledJobConstraints\\Training Data\\JobExperience.txt"
#define JOB_TRAIN_QUAL "ScoreCalculation\\LabelledJobConstraints\\Training Data\\JobQualification.txt"
#define JOB_TRAIN_SHIFT "ScoreCalculation\\LabelledJobConstraints\\Training Data\\JobShift.txt"
#define JOB_TRAIN_TENTH "ScoreCalculation\\LabelledJobConstraints\\Training Data\\JobTenthPercent.txt"
#define JOB_TRAIN_TWELTH "ScoreCalculation\\LabelledJobConstraints\\Training Data\\JobTwelthPercent.txt"
#define JOB_TRAIN_CGPA "ScoreCalculation\\LabelledJobConstraints\\Training Data\\JobCGPA.txt"
#define JOB_TEST_C_AGE "ScoreCalculation\\LabelledJobConstraints\\Testing Data\\CategorizedJobAge.txt"
#define JOB_TEST_C_XP "ScoreCalculation\\LabelledJobConstraints\\Testing Data\\CategorizedJobExperience.txt"
#define JOB_TEST_C_QUAL "ScoreCalculation\\LabelledJobConstraints\\Testing Data\\CategorizedJobQualification.txt"
#define JOB_TEST_C_SHIFT "ScoreCalculation\\LabelledJobConstraints\\Testing Data\\CategorizedJobShift.txt"
#define JOB_TEST_C_TENTH "ScoreCalculation\\LabelledJobConstraints\\Testing Data\\CategorizedJobTenthPercent.txt"
#define JOB_TEST_C_TWELTH "ScoreCalculation\\LabelledJobConstraints\\Testing Data\\CategorizedJobTwelthPercent.txt"
#define JOB_TEST_C_CGPA "ScoreCalculation\\LabelledJobConstraints\\Testing Data\\CategorizedJobCGPA.txt"
#define JOB_TEST_U_AGE "ScoreCalculation\\LabelledJobConstraints\\Testing Data\\UncategorizedJobAge.txt"
#define JOB_TEST_U_XP "ScoreCalculation\\LabelledJobConstraints\\Testing Data\\UncategorizedJobExperience.txt"
#define JOB_TEST_U_QUAL "ScoreCalculation\\LabelledJobConstraints\\Testing Data\\UncategorizedJobQualification.txt"
#define JOB_TEST_U_SHIFT "ScoreCalculation\\LabelledJobConstraints\\Testing Data\\UncategorizedJobShift.txt"
#define JOB_TEST_U_TENTH "ScoreCalculation\\LabelledJobConstraints\\Testing Data\\UncategorizedJobTenthPercent.txt"
#define JOB_TEST_U_TWELTH "ScoreCalculation\\LabelledJobConstraints\\Testing Data\\UncategorizedJobTwelthPercent.txt"
#define JOB_TEST_U_CGPA "ScoreCalculation\\LabelledJobConstraints\\Testing Data\\UncategorizedJobCGPA.txt"

#define INPUT_SAMPLE "ScoreCalculation\\Sample.txt"


struct sample{
	double * dim;
	uint32_t group;
	double tmp_distance;
};

struct knn_data{
	uint32_t k;
	struct sample ** best_voters;
	struct sample * samples[2];
	uint32_t samples_count[2];
	uint32_t samples_dimensions[2];
};

enum Constraints{
	JobAge, JobExperience, JobQualification, JobShift, JobTenthPercent, JobTwelthPercent, JobCGPA, SeekerCompanyType, SeekerJobType, SeekerSalary, SeekerShift, SeekerWorkFromHome, SeekerLocation
};

void parseStringToSample(struct sample *, char *, uint32_t, uint8_t);
void parseFileToSamples(struct knn_data *, char *);
void knnAlgorithmsSortAscVoters(struct knn_data * knn);
void knnAlgorithm(struct knn_data *);
struct knn_data createModel(int k, char file[]);
double predictTestData(struct knn_data knn, char uncategorizedFile[], char categorizedFile[]);
int getOptimumKValue(enum Constraints constraint);
int predict(struct knn_data knn, int data, int weightage, char dataFile[]);
void uploadTrainingData(int score, int data, int weightage, char dataFile[]);

#endif