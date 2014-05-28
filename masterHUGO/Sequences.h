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
		seqLength(_seqLength),
		header(/*to node*/ node_address_set[nodeNr], /*type*/ type[_paramNr] /*Time*/)
	{}
	~MasterSeq() {}
	
	void seqProceed()
	{	
		if (stepCounter == 0)
		{	
			int message = *pSeq++;
			int ok = network.write(header,&message,sizeof(int));
			// // Notify us of the result
		   if (ok)
		   {
		    Serial.printf_P(PSTR("%lu: Master Send ok: Value %lu to 0%o\n\r"),millis(),message,node_address_set[nodeNr]);
		   }
		   else
		   {
		    Serial.printf_P(PSTR("%lu: Master Send faild: Value %lu to 0%o\n\r"),millis(),message,node_address_set[nodeNr]);
		   }

			stepCounter = *pSeq++ * (1.0/speed);
			stepCounter--;
			if(stepCounter == 0)
				seqCounter++;
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
	
	void Process()
	{
		if (seqCounter >= seqLength)
		{
			reset();
			seqProceed();	
		}
		else
		{		
			seqProceed();
		}
	}
		
	int nodeNr;
	float speed = 1.0;
	int stepCounter = 0;
	int seqCounter = 0;
	int* seq;
	int* pSeq;
	int* pSeqNext;
	int seqLength = 0;
	bool loop = true;
	char type[5] = {'S', 'F', 'G', 'H', 'I'};
	
	RF24NetworkHeader header;
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

int node1_seqNr[8] = { 0, 100, 1, 100, 0, 100, 1, 100};
int node1_Mod1[2] = { 1, 10};
int node1_Mod2[2] = { 1, 10};
int node1_Mod3[2] = { 1, 10};
int node1_Mod4[2] = { 1, 10};

MasterNode MasterNode1(1, node1_seqNr, 4, node1_Mod1, 1, node1_Mod2, 1, node1_Mod3, 1, node1_Mod4, 1);

int node2_seqNr[8] = { 0, 100, 1, 100, 0, 100, 1, 100};
int node2_Mod1[2] = { 1, 10};
int node2_Mod2[2] = { 1, 10};
int node2_Mod3[2] = { 1, 10};
int node2_Mod4[2] = { 1, 10};

MasterNode MasterNode2(2, node2_seqNr, 4, node2_Mod1, 1, node2_Mod2, 1, node2_Mod3, 1, node2_Mod4, 1);

int node3_seqNr[8] = { 0, 100, 1, 100, 0, 100, 1, 100};
int node3_Mod1[2] = { 1, 1000};
int node3_Mod2[2] = { 1, 1000};
int node3_Mod3[2] = { 1, 1000};
int node3_Mod4[2] = { 1, 1000};

MasterNode MasterNode3(3, node3_seqNr, 4, node3_Mod1, 1, node3_Mod2, 1, node3_Mod3, 1, node3_Mod4, 1);

int node4_seqNr[8] = { 0, 100, 1, 100, 0, 100, 1, 100}};
int node4_Mod1[2] = { 1, 1000};
int node4_Mod2[2] = { 1, 1000};
int node4_Mod3[2] = { 1, 1000};
int node4_Mod4[2] = { 1, 1000};

MasterNode MasterNode4(4, node4_seqNr, 4, node4_Mod1, 1, node4_Mod2, 1, node4_Mod3, 1, node4_Mod4, 1);

int node5_seqNr[8] = { 0, 100, 1, 100, 0, 100, 1, 100};
int node5_Mod1[2] = { 1, 1000};
int node5_Mod2[2] = { 1, 1000};
int node5_Mod3[2] = { 1, 1000};
int node5_Mod4[2] = { 1, 1000};

MasterNode MasterNode5(5, node5_seqNr, 4, node5_Mod1, 1, node5_Mod2, 1, node5_Mod3, 1, node5_Mod4, 1);

int node6_seqNr[8] = { 0, 100, 1, 100, 0, 100, 1, 100};
int node6_Mod1[2] = { 1, 1000};
int node6_Mod2[2] = { 1, 1000};
int node6_Mod3[2] = { 1, 1000};
int node6_Mod4[2] = { 1, 1000};

MasterNode MasterNode6(6, node6_seqNr, 4, node6_Mod1, 1, node6_Mod2, 1, node6_Mod3, 1, node6_Mod4, 1);

int node7_seqNr[8] = { 0, 100, 1, 100, 0, 100, 1, 100};
int node7_Mod1[2] = { 1, 1000};
int node7_Mod2[2] = { 1, 1000};
int node7_Mod3[2] = { 1, 1000};
int node7_Mod4[2] = { 1, 1000};

MasterNode MasterNode7(7, node7_seqNr, 4, node7_Mod1, 1, node7_Mod2, 1, node7_Mod3, 1, node7_Mod4, 1);

int node8_seqNr[8] = { 0, 100, 1, 100, 0, 100, 1, 100};
int node8_Mod1[2] = { 1, 1000};
int node8_Mod2[2] = { 1, 1000};
int node8_Mod3[2] = { 1, 1000};
int node8_Mod4[2] = { 1, 1000};

MasterNode MasterNode8(8, node8_seqNr, 4, node8_Mod1, 1, node8_Mod2, 1, node8_Mod3, 1, node8_Mod4, 1);

int node9_seqNr[8] = { 0, 100, 1, 100, 0, 100, 1, 100};
int node9_Mod1[2] = { 1, 1000};
int node9_Mod2[2] = { 1, 1000};
int node9_Mod3[2] = { 1, 1000};
int node9_Mod4[2] = { 1, 1000};

MasterNode MasterNode9(9, node9_seqNr, 4, node9_Mod1, 1, node9_Mod2, 1, node9_Mod3, 1, node9_Mod4, 1);

int node10_seqNr[8] = { 0, 100, 1, 100, 0, 100, 1, 100};
int node10_Mod1[2] = { 1, 1000};
int node10_Mod2[2] = { 1, 1000};
int node10_Mod3[2] = { 1, 1000};
int node10_Mod4[2] = { 1, 1000};

MasterNode MasterNode10(10, node10_seqNr, 4, node10_Mod1, 1, node10_Mod2, 1, node10_Mod3, 1, node10_Mod4, 1);

int node11_seqNr[8] = { 0, 100, 1, 100, 0, 100, 1, 100};
int node11_Mod1[2] = { 1, 1000};
int node11_Mod2[2] = { 1, 1000};
int node11_Mod3[2] = { 1, 1000};
int node11_Mod4[2] = { 1, 1000};

MasterNode MasterNode11(11, node11_seqNr, 4, node11_Mod1, 1, node11_Mod2, 1, node11_Mod3, 1, node11_Mod4, 1);

int node12_seqNr[8] = { 0, 100, 1, 100, 0, 100, 1, 100};
int node12_Mod1[2] = { 1, 1000};
int node12_Mod2[2] = { 1, 1000};
int node12_Mod3[2] = { 1, 1000};
int node12_Mod4[2] = { 1, 1000};

MasterNode MasterNode12(12, node12_seqNr, 4, node12_Mod1, 1, node12_Mod2, 1, node12_Mod3, 1, node12_Mod4, 1);


#endif
