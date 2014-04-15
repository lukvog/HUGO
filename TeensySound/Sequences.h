/* 
 * *** Sequences.h ****
 *
 * created by Lukas Vogel 15.04.14
 * 
 */
 
#ifndef Sequences_h_
#define Sequences_h_

#define a1 0
#define e1 1
#define i1 2
#define o1 3
#define u1 4

class FormantFilterSequence
{
public:
	FormantFilterSequence(int* _seq, int _seqLength)
		:seq(_seq),
		pSeq(_seq),
		seqLength(_seqLength)
	{}
	~FormantFilterSequence() {}
	
	void seqProceed()
	{
		if (stepCounter == 0)
		{
			switch (*pSeq++)
			{
				case a1: 	setSopranA();
							break;
				case e1: 	break;
				case i1: 	setAltI();
							break;
				case o1: 	break;
				case u1:	break;
				default:	break;
			}
			stepCounter = ((*pSeq++) / (MOD_RATE)) * (1.0/speed);
			seqCounter++;
		}
		else
		{
			stepCounter--;
		}
	}
	
	void reset()
	{
		pSeq = seq;
		seqCounter = 0;
		stepCounter = 0;	
	}
	float speed = 1.0;
	int stepCounter = 0;
	int seqCounter = 0;
	int* seq;
	int* pSeq;
	int seqLength = 0;
	bool loop = true;
};

int testSeq[4] = { a1, 1000, i1, 2000};

int testSeq2[8] = { a1, 1000, i1, 2000, a1, 500, i1, 500};

FormantFilterSequence firstFormantSeq(testSeq, 2);

FormantFilterSequence secondFormantSeq(testSeq2, 4);

#endif