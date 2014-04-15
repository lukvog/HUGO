/* 
 * *** FormantFilter.h ****
 *
 * created by Lukas Vogel 15.04.14
 * 
 */
 
#ifndef FormantFilter_h_
#define FormantFilter_h_


int ToneFilter[]={0,0,0,0,0,0,0,0x80000000,0,0,0,0,0,0,0,0x80000000,0,0,0,0,0,0,0,0}; 	// defines 2 sets of coefficients, not sure max possible in
																						// time frame but probably quite a few.


int updateFilter[5];

extern AudioFilterBiquad formantFilter;

// sopran a
void setSopranA() {
	// calcBiquad(FilterType,FrequencyC,dBgain,BW,QuantizationUnit,SampleRate,int*);
	calcBiquad(FILTER_BANDPASS,450,0,0.5,2147483648,44100,updateFilter);
	formantFilter.updateCoefs(updateFilter); // default set updateCoefs(0,updateFilter);
	calcBiquad(FILTER_BANDPASS,800,-11,0.4,2147483648,44100,updateFilter);
	formantFilter.updateCoefs(1,updateFilter);
	calcBiquad(FILTER_BANDPASS,2830,-22,0.3,2147483648,44100,updateFilter);
	formantFilter.updateCoefs(2,updateFilter);	
}

// alt i
void setAltI() {
	// calcBiquad(FilterType,FrequencyC,dBgain,BW,QuantizationUnit,SampleRate,int*);
	calcBiquad(FILTER_BANDPASS,350,0,0.5,2147483648,44100,updateFilter);
	formantFilter.updateCoefs(updateFilter); // default set updateCoefs(0,updateFilter);
	calcBiquad(FILTER_BANDPASS,1700,-20,0.4,2147483648,44100,updateFilter);
	formantFilter.updateCoefs(1,updateFilter);
	calcBiquad(FILTER_BANDPASS,2700,-30,0.3,2147483648,44100,updateFilter);
	formantFilter.updateCoefs(2,updateFilter);	
}

#endif