/* 
 * *** FormantFilter.h ****
 *
 * created by Lukas Vogel 15.04.14
 * 
 */
 
#ifndef FormantFilter_h_
#define FormantFilter_h_


int ToneFilter1[]={0,0,0,0,0,0,0};
int ToneFilter2[]={0,0,0,0,0,0,0};
int ToneFilter3[]={0,0,0,0,0,0,0};

int updateFilter[5];

extern AudioFilterBiquad formantFilter1;
extern AudioFilterBiquad formantFilter2;
extern AudioFilterBiquad formantFilter3;


#endif