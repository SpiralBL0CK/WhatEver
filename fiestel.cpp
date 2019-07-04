#include <stdio.h>      /* printf, scanf, puts, NULL */
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */


void PrimeRandomNumberPump()
{
	for (unsigned int index = 0; index < 20; ++index)
        rand();
}

unsigned char RoundFunction(unsigned char value,unsigned char key)
{
	return value+key | (value * key) + 3;
}

void Encrypt(unsigned char &left,unsigned char &right,unsigned char key)
{
	for(unsigned int idx = 0;idx < 4;++idx)
	{
		// Feistel Network Encryption:
        //  Left[i+1]  = Right[i];
        //  Right[i+1] = Left[i] ^ RoundFunction(Right[i], key);
        unsigned char oldLeft = left;
        left = right;
        right = oldLeft ^ RoundFunction(right, key);
	}
}

int main()
{
	
}
