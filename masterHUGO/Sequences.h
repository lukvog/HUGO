/* 
 * *** Sequences.h ****
 *
 * created by Lukas Vogel 15.04.14
 * 
 */
 
#ifndef Sequences_h_
#define Sequences_h_

#include <vector>

extern RF24Network network;

class MasterSeq
{
public:
	MasterSeq(int _nodeNr, int _paramNr, int* _seq, int _seqLength)
		: nodeNr(_nodeNr),
		seq(_seq),
		pSeq(_seq),
		seqLength(_seqLength)
	{
		switch(_paramNr)
		{
		case 0: type = 'S';
				break;
		case 1: type = 'F';
				break;
		case 2: type = 'G';
				break;
		case 3: type = 'H';
				break;
		case 4: type = 'I';
				break;
		default: Serial.print("paramNr not valid");
				break;
		}
	}
	~MasterSeq() {}
	
	void seqProceed()
	{	
		if (stepCounter == 0)
		{	
			int message = *pSeq++;
			RF24NetworkHeader header(/*to node*/ node_address_set[nodeNr], /*type*/ type /*Time*/);
			network.write(header,&message,sizeof(int));
			Serial.printf_P(PSTR("%lu: APP Send Value %lu to 0%o\n\r"),millis(),message,node_address_set[nodeNr]);
			stepCounter = ((*pSeq++) / (MOD_RATE)) * (1.0/speed);
		}
		else
		{		
			stepCounter--;
			if(stepCounter == 0)
				seqCounter++;
		}
	}
	
	void reset()
	{
		pSeq = seq;
		seqCounter = 0;
		stepCounter = 0;	
	}
	int nodeNr;
	char type;
	float speed = 1.0;
	int stepCounter = 0;
	int seqCounter = 0;
	int* seq;
	int* pSeq;
	int* pSeqNext;
	int seqLength = 0;
	bool loop = true;
};

struct MasterNode
{
	MasterNode(int _nodeNr,
			   int* _seqNr, int seqNr_length,
			   int* _Mod1, int seqNr_Mod1,
			   int* _Mod2, int seqNr_Mod2,
			   int* _Mod3, int seqNr_Mod3,
			   int* _Mod4, int seqNr_Mod4)
	: SeqNr(_nodeNr, 0, _seqNr, seqNr_length),
		Mod1(_nodeNr, 1, _Mod1, seqNr_Mod1),
		Mod2(_nodeNr, 2, _Mod2, seqNr_Mod2),
		Mod3(_nodeNr, 3, _Mod3, seqNr_Mod3),
		Mod4(_nodeNr, 4, _Mod4, seqNr_Mod4)
	{}
	MasterSeq SeqNr;
	MasterSeq Mod1;
	MasterSeq Mod2;
	MasterSeq Mod3;
	MasterSeq Mod4;
};

int node1_seqNr[8] = { 1, 100, 2, 100, 3, 100, 4, 100};
int node1_Mod1[8] = { 1, 1000, -10, 1000, 1, 1000, -40, 1000};
int node1_Mod2[8] = { 1, 1000, -10, 1000, 1, 1000, -40, 1000};
int node1_Mod3[8] = { 1, 1000, -10, 1000, 1, 1000, -40, 1000};
int node1_Mod4[8] = { 1, 1000, -10, 1000, 1, 1000, -40, 1000};

MasterNode masterNode1(1, node1_seqNr, 4, node1_Mod1, 4, node1_Mod2, 4, node1_Mod3, 4, node1_Mod4, 4);



#endif