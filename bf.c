#include "bf.h"
#include "bv.h"
#include "city.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

struct BloomFilter
{
    uint64_t salts[N_HASHES];
    uint32_t n_keys;
    uint32_t n_hits;
    uint32_t n_misses;
    uint32_t n_bits_examined;
    BitVector *filter;
};

static uint64_t default_salts[] = {0x5adf08ae86d36f21, 0x419d292ea2ffd49e, 0x50d8bb08de3818df, 0x272347aea4045dd5, 0x7c8e16f768811a21};

BloomFilter *bf_create(uint32_t size)
{
    BloomFilter *bf = (BloomFilter *)malloc(sizeof(BloomFilter));
    if (bf)
    {
        bf->n_keys = 0;
        bf->n_hits = 0;
        bf->n_misses = 0;
        bf->n_bits_examined = 0;
        for (int i = 0; i < N_HASHES; i++)
        {
            bf->salts[i] = default_salts[i];
        }
        bf->filter = bv_create(size);
        if (bf->filter == NULL)
        {
            free(bf);
            bf = NULL;
        }
    }
    return bf;
}

void bf_delete(BloomFilter **bf)
{
    if ((*bf) != NULL && (bf) != NULL)
    {
        bv_delete(&(*bf)->filter);
        free((*bf));
        (*bf) = NULL;
    }
}

uint32_t bf_size(BloomFilter *bf)
{
    if (bf)
    {
        return (bv_length(bf->filter));
    }
}

void bf_insert(BloomFilter *bf, char *oldspeak)
{
    if (bf)
    {
        bv_set_bit(bf->filter, (hash(default_salts[0], oldspeak) % bf_size(bf)));
        bv_set_bit(bf->filter, (hash(default_salts[1], oldspeak) % bf_size(bf)));
        bv_set_bit(bf->filter, (hash(default_salts[2], oldspeak) % bf_size(bf)));
        bv_set_bit(bf->filter, (hash(default_salts[3], oldspeak) % bf_size(bf)));
        bv_set_bit(bf->filter, (hash(default_salts[4], oldspeak) % bf_size(bf)));
        bf->n_keys++;
    }
}

bool bf_probe(BloomFilter *bf, char *oldspeak)
{
    if (bf)
    {
        for (uint32_t i = 0; i < N_HASHES; i++)
        {
            bf->n_bits_examined += 1;
            if (bv_get_bit(bf->filter, hash(default_salts[i], oldspeak) % bf_size(bf)) == 0)
            {
                bf->n_misses += 1;
                return 0;
            }
            // bf->n_hits++;
        }
        bf->n_hits++;
        return 1;
    }
}

uint32_t bf_count(BloomFilter *bf)
{
    if (bf)
    {
        uint32_t count = 0;
        for (uint32_t i = 0; i < bv_length(bf->filter); i++)
        {
            if (bv_get_bit(bf->filter, i))
            {
                count++;
            }
        }
        return count;
    }
}

void bf_print(BloomFilter *bf)
{
    if (bf)
    {
        bv_print(bf->filter);
    }
}

void bf_stats(BloomFilter *bf, uint32_t *nk, uint32_t *nh, uint32_t *nm, uint32_t *ne)
{
    *nk = bf->n_keys;
    *nh = bf->n_hits;
    *nm = bf->n_misses;
    *ne = bf->n_bits_examined;
}
