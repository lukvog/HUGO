/* 
 * *** Sequences.h ****
 *
 * created by Lukas Vogel 15.04.14
 * 
 */
 
#ifndef Sequences_h_
#define Sequences_h_

// class FormantFilterSequence
// {
// public:
	// FormantFilterSequence(

// }


#define a1 0
#define e1 1
#define i1 2
#define o1 3
#define u1 4


int testSeq[10] = { a1, 100, i1, 200};
int testSeqCounter = 0;
int testCounter = 0;
int* pTestSeq = testSeq;
int testSeqLength = 2;
bool testSeqLoop = true;

void testSeqProced()
{
	if (testCounter == 0)
	{
		switch (*pTestSeq++)
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
		testCounter = *pTestSeq++;
		testSeqCounter++;
	}
	else
	{
		testCounter--;
	}

}

#endif