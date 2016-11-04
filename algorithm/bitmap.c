#include <stdio.h>

#define SHIFT 5
#define MASK 0x1F

void set_bit(unsigned int *bitmap, const int bitn)
{
    bitmap[bitn >> SHIFT] |= (1 << (bitn & MASK));
}

int get_bit(unsigned int *bitmap, const int bitn)
{
    return bitmap[bitn >> SHIFT] & (1 << (bitn & MASK));
}

void clr_bit(unsigned int *bitmap, const int bitn)
{
    bitmap[bitn >> SHIFT] &= ~(1 << (bitn & MASK));
}

int main(int argc, char *argv[])
{
    printf("");
}
