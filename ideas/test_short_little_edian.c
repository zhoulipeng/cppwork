#include <stdio.h>

typedef union sequence{
	short s;
	unsigned short us;
	unsigned char uc[2];
}Seq;

int main(int argc, char *argv[])
{
	printf("sizeof(short) = %lu, sizeof(union sequence) = %lu\n", sizeof(short), sizeof(union sequence));
	Seq seq1;
	seq1.uc[0] = 0x00;
	seq1.uc[1] = 0x80;
	printf("%04hx, %04hx, %d, %u, ", seq1.s, seq1.us, seq1.s, seq1.us);
	seq1.uc[0] = seq1.uc[0]^seq1.uc[1];
        seq1.uc[1] = seq1.uc[0]^seq1.uc[1];
        seq1.uc[0] = seq1.uc[0]^seq1.uc[1];
	short sh1 = -32768;
	printf("%04hx, %04hx, %d, %u\n", sh1, seq1.us, seq1.s, seq1.us);
	
	return 0;
}
