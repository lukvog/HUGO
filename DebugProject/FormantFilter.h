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

// sopran a
void setSopranA() {
	// calcBiquad(FilterType,FrequencyC,dBgain,BW,QuantizationUnit,SampleRate,int*);
	calcBiquad(FILTER_BANDPASS,800,0,0,0.5,2147483648,44100,updateFilter);
	formantFilter1.updateCoefs(updateFilter);
	calcBiquad(FILTER_BANDPASS,1150,-6,0,0.4,2147483648,44100,updateFilter);
	formantFilter2.updateCoefs(updateFilter);
	calcBiquad(FILTER_BANDPASS,2900,-32,0,0.3,2147483648,44100,updateFilter);
	formantFilter3.updateCoefs(updateFilter);	
}

// sopran e
void setSopranE() {
	// calcBiquad(FilterType,FrequencyC,dBgain,BW,QuantizationUnit,SampleRate,int*);
	calcBiquad(FILTER_BANDPASS,350,0,0,0.5,2147483648,44100,updateFilter);
	formantFilter1.updateCoefs(updateFilter);
	calcBiquad(FILTER_BANDPASS,2000,-20,0,0.4,2147483648,44100,updateFilter);
	formantFilter2.updateCoefs(updateFilter);
	calcBiquad(FILTER_BANDPASS,2800,-15,0,0.3,2147483648,44100,updateFilter);
	formantFilter3.updateCoefs(updateFilter);	
}

// sopran i
void setSopranI() {
	// calcBiquad(FilterType,FrequencyC,dBgain,BW,QuantizationUnit,SampleRate,int*);
	calcBiquad(FILTER_BANDPASS,270,0,0,0.5,2147483648,44100,updateFilter);
	formantFilter1.updateCoefs(updateFilter);
	calcBiquad(FILTER_BANDPASS,2140,-10,0,0.4,2147483648,44100,updateFilter);
	formantFilter2.updateCoefs(updateFilter);
	calcBiquad(FILTER_BANDPASS,2950,-26,0,0.3,2147483648,44100,updateFilter);
	formantFilter3.updateCoefs(updateFilter);	
}

// sopran o
void setSopranO() {
	// calcBiquad(FilterType,FrequencyC,dBgain,BW,QuantizationUnit,SampleRate,int*);
	calcBiquad(FILTER_BANDPASS,450,0,0,0.5,2147483648,44100,updateFilter);
	formantFilter1.updateCoefs(updateFilter);
	calcBiquad(FILTER_BANDPASS,800,-11,0,0.4,2147483648,44100,updateFilter);
	formantFilter2.updateCoefs(updateFilter);
	calcBiquad(FILTER_BANDPASS,2830,-22,0,0.3,2147483648,44100,updateFilter);
	formantFilter3.updateCoefs(updateFilter);	
}

// sopran u
void setSopranU() {
	// calcBiquad(FilterType,FrequencyC,dBgain,BW,QuantizationUnit,SampleRate,int*);
	calcBiquad(FILTER_BANDPASS,325,0,0,0.5,2147483648,44100,updateFilter);
	formantFilter1.updateCoefs(updateFilter);
	calcBiquad(FILTER_BANDPASS,700,-16,0,0.4,2147483648,44100,updateFilter);
	formantFilter2.updateCoefs(updateFilter);
	calcBiquad(FILTER_BANDPASS,2700,-35,0,0.3,2147483648,44100,updateFilter);
	formantFilter3.updateCoefs(updateFilter);	
}


#endif