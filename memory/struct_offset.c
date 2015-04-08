#include <stdio.h>
struct student 
{ 
	int a; 
	char b[20]; 
	double ccc; 
};
#define OFFSET( struc, e)(size_t)&(((struc *)0)->e) 
int main(int argc, char *argv[])
{
	printf("%lu ", OFFSET(struct student, ccc));
	printf("%lu ", OFFSET(struct student, b));
	printf("%lu ", OFFSET(struct student, a));
}
