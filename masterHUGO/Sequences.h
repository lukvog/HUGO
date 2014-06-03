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

/////////////////////
//change array length and step length
/////////////////////

int node1_seqNr[18] = { 0, 1000, 2, 20, 1, 1000, 2, 20, 3, 1000, 2, 20, 4, 1000, 5, 1000, 2, 20};
int node1_Mod1[2] = { 1, 10};
int node1_Mod2[2] = { 1, 10};
int node1_Mod3[2] = { 1, 10};
int node1_Mod4[2] = { 1, 10};

MasterNode* MasterNode1 = new MasterNode(1, node1_seqNr, 9, node1_Mod1, 1, node1_Mod2, 1, node1_Mod3, 1, node1_Mod4, 1);

int node2_seqNr[18] = { 0, 1000, 2, 20, 1, 1000, 2, 20, 3, 1000, 2, 20, 4, 1000, 5, 1000, 2, 20};
int node2_Mod1[2] = { 1, 10};
int node2_Mod2[2] = { 1, 10};
int node2_Mod3[2] = { 1, 10};
int node2_Mod4[2] = { 1, 10};

MasterNode* MasterNode2 = new MasterNode(2, node2_seqNr, 9, node2_Mod1, 1, node2_Mod2, 1, node2_Mod3, 1, node2_Mod4, 1);

int node3_seqNr[18] = { 0, 1000, 2, 20, 1, 1000, 2, 20, 3, 1000, 2, 20, 4, 1000, 5, 1000, 2, 20};
int node3_Mod1[2] = { 1, 10};
int node3_Mod2[2] = { 1, 10};
int node3_Mod3[2] = { 1, 10};
int node3_Mod4[2] = { 1, 10};

MasterNode* MasterNode3 = new MasterNode(3, node3_seqNr, 9, node3_Mod1, 1, node3_Mod2, 1, node3_Mod3, 1, node3_Mod4, 1);

int node4_seqNr[18] = { 0, 1000, 2, 20, 1, 1000, 2, 20, 3, 1000, 2, 20, 4, 1000, 5, 1000, 2, 20};
int node4_Mod1[2] = { 1, 10};
int node4_Mod2[2] = { 1, 10};
int node4_Mod3[2] = { 1, 10};
int node4_Mod4[2] = { 1, 10};

MasterNode* MasterNode4 = new MasterNode(4, node4_seqNr, 9, node4_Mod1, 1, node4_Mod2, 1, node4_Mod3, 1, node4_Mod4, 1);

int node5_seqNr[18] = { 0, 1000, 2, 20, 1, 1000, 2, 20, 3, 1000, 2, 20, 4, 1000, 5, 1000, 2, 20};
int node5_Mod1[2] = { 1, 10};
int node5_Mod2[2] = { 1, 10};
int node5_Mod3[2] = { 1, 10};
int node5_Mod4[2] = { 1, 10};

MasterNode* MasterNode5 = new MasterNode(5, node5_seqNr, 9, node5_Mod1, 1, node5_Mod2, 1, node5_Mod3, 1, node5_Mod4, 1);

int node6_seqNr[18] = { 0, 1000, 2, 20, 1, 1000, 2, 20, 3, 1000, 2, 20, 4, 1000, 5, 1000, 2, 20};
int node6_Mod1[2] = { 1, 10};
int node6_Mod2[2] = { 1, 10};
int node6_Mod3[2] = { 1, 10};
int node6_Mod4[2] = { 1, 10};

MasterNode* MasterNode6 = new MasterNode(6, node6_seqNr, 9, node6_Mod1, 1, node6_Mod2, 1, node6_Mod3, 1, node6_Mod4, 1);

int node7_seqNr[18] = { 0, 1000, 2, 20, 1, 1000, 2, 20, 3, 1000, 2, 20, 4, 1000, 5, 1000, 2, 20};
int node7_Mod1[2] = { 1, 10};
int node7_Mod2[2] = { 1, 10};
int node7_Mod3[2] = { 1, 10};
int node7_Mod4[2] = { 1, 10};

MasterNode* MasterNode7 = new MasterNode(7, node7_seqNr, 9, node7_Mod1, 1, node7_Mod2, 1, node7_Mod3, 1, node7_Mod4, 1);

int node8_seqNr[18] = { 0, 1000, 2, 20, 1, 1000, 2, 20, 3, 1000, 2, 20, 4, 1000, 5, 1000, 2, 20};
int node8_Mod1[2] = { 1, 10};
int node8_Mod2[2] = { 1, 10};
int node8_Mod3[2] = { 1, 10};
int node8_Mod4[2] = { 1, 10};

MasterNode* MasterNode8 = new MasterNode(8, node8_seqNr, 9, node8_Mod1, 1, node8_Mod2, 1, node8_Mod3, 1, node8_Mod4, 1);

int node9_seqNr[18] = { 0, 1000, 2, 20, 1, 1000, 2, 20, 3, 1000, 2, 20, 4, 1000, 5, 1000, 2, 20};
int node9_Mod1[2] = { 1, 10};
int node9_Mod2[2] = { 1, 10};
int node9_Mod3[2] = { 1, 10};
int node9_Mod4[2] = { 1, 10};

MasterNode* MasterNode9 = new MasterNode(9, node9_seqNr, 9, node9_Mod1, 1, node9_Mod2, 1, node9_Mod3, 1, node9_Mod4, 1);

int node10_seqNr[18] = { 0, 1000, 2, 20, 1, 1000, 2, 20, 3, 1000, 2, 20, 4, 1000, 5, 1000, 2, 20};
int node10_Mod1[2] = { 1, 10};
int node10_Mod2[2] = { 1, 10};
int node10_Mod3[2] = { 1, 10};
int node10_Mod4[2] = { 1, 10};

MasterNode* MasterNode10 = new MasterNode(10, node10_seqNr, 9, node10_Mod1, 1, node10_Mod2, 1, node10_Mod3, 1, node10_Mod4, 1);

int node11_seqNr[18]= { 0, 1000, 2, 20, 1, 1000, 2, 20, 3, 1000, 2, 20, 4, 1000, 5, 1000, 2, 20};
int node11_Mod1[2] = { 1, 10};
int node11_Mod2[2] = { 1, 10};
int node11_Mod3[2] = { 1, 10};
int node11_Mod4[2] = { 1, 10};

MasterNode* MasterNode11 = new MasterNode(11, node11_seqNr, 9, node11_Mod1, 1, node11_Mod2, 1, node11_Mod3, 1, node11_Mod4, 1);

int node12_seqNr[18] = { 0, 1000, 2, 20, 1, 1000, 2, 20, 3, 1000, 2, 20, 4, 1000, 5, 1000, 2, 20};
int node12_Mod1[2] = { 1, 10};
int node12_Mod2[2] = { 1, 10};
int node12_Mod3[2] = { 1, 10};
int node12_Mod4[2] = { 1, 10};

MasterNode* MasterNode12 = new MasterNode(12, node12_seqNr, 9, node12_Mod1, 1, node12_Mod2, 1, node12_Mod3, 1, node12_Mod4, 1);


#endif
