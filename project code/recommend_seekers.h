#ifndef RECOMMEND_SEEKERS
#define RECOMMEND_SEEKERS

#include "seeker.h"
#include "company.h"
#include "recommend_jobs.h"


int getSeekersCount();
void getSeekers(seeker *seekers);
int checkSeekerMatching(job jb, seeker *sk);
void recommendSeekers(job jb);
float getSeekerRecommendationPercentage(seeker skr, job jb, int calledInsideFunc);
void printRecommendedSeekers(seeker matchedSkr[], int len);
void sortSeekers(seeker *seekers, int skrlen);

#endif