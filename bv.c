#include "bv.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

struct BitVector
{
    uint32_t length;
    uint64_t *vector;
};

BitVector *bv_create(uint32_t length)
{
    BitVector *bv = (BitVector *)malloc(sizeof(BitVector));
    if (bv)
    {
        bv->length = length;
        bv->vector = (uint64_t *)calloc((length / 64) + 1, sizeof(uint64_t));
        if (!(bv->vector))
        {
            fprintf(stderr, "Insufficient Memory\n");
            free(bv);
            bv = NULL;
            return NULL;
        }
        return bv;
    }
    else
    {
        bv = NULL;
        return bv;
    }
}

void bv_delete(BitVector **bv)
{
    if ((*bv) != NULL && bv != NULL)
    {
        free((*bv)->vector);
        (*bv)->vector = NULL;
        free(*bv);
        *bv = NULL;
    }
}

uint32_t bv_length(BitVector *bv)
{
    if (bv)
    {
        return bv->length;
    }
}

void bv_set_bit(BitVector *bv, uint32_t i)
{
    if (bv)
    {
        bv->vector[i / 64] |= (1UL << (i % 64));
    }
}

void bv_clr_bit(BitVector *bv, uint32_t i)
{
    if (bv)
    {
        bv->vector[i / 64] &= ~(1UL << (i % 64));
    }
}

uint8_t bv_get_bit(BitVector *bv, uint32_t i)
{
    if (bv)
    {
        return (bv->vector[i / 64] >> (i % 64)) & 1UL;
    }
}

void bv_print(BitVector *bv)
{
    for (uint32_t i = 0; i < bv->length; i++)
    {
        if (bv_get_bit(bv, i))
        {
            printf("1");
        }
        else
        {
            printf("0");
        }
    }
    printf("\n");
}