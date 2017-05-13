#include <iostream>
#include <cstring>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <bitset>
using namespace std;

class bf_t
{
	public:
		bitset<2000000> hash[8];
};
uint32_t primeNumbers[8] = {6291469,410393,540559,12582917,658303,67867979,777277,86028157};

//Reference Link: https://en.wikipedia.org/wiki/MurmurHash
uint32_t murmur3(const uint8_t* key, size_t len, uint32_t seed) {
	uint32_t h = seed;
	if (len > 3) {
		const uint32_t* key_x4 = (const uint32_t*) key;
		size_t i = len >> 2;
		do {
			uint32_t k = *key_x4++;
			k *= 0xcc9e2d51;
			k = (k << 15) | (k >> 17);
			k *= 0x1b873593;
			h ^= k;
			h = (h << 13) | (h >> 19);
			h += (h << 2) + 0xe6546b64;
		} while (--i);
		key = (const uint8_t*) key_x4;
	}

	if (len & 3) {
		size_t i = len & 3;
		uint32_t k = 0;
		key = &key[i - 1];
		do {
			k <<= 8;
			k |= *key--;
		} while (--i);
		k *= 0xcc9e2d51;
		k = (k << 15) | (k >> 17);
		k *= 0x1b873593;
		h ^= k;
	}

	h ^= len;
	h ^= h >> 16;
	h *= 0x85ebca6b;
	h ^= h >> 13;
	h *= 0xc2b2ae35;
	h ^= h >> 16;
	return h;
}

//Method to Create the bloom filters
bf_t *create_bf()
{
	bf_t *bloomfilter = new bf_t();
	return bloomfilter;
}

//Method to insert elements at certain index based on hash functions
void insert_bf(bf_t *b, char *s)
{
	for (int i = 0; i < 8; i++)
	{
		uint32_t temp = murmur3((uint8_t *) s, strlen(s), primeNumbers[i]);
		uint32_t index = (temp % 2000000);
		b->hash[i].set(index, 1);
	}
}

//Method to check whether the elements exists at that index based on the hash functions
int is_element(bf_t *b, char *q)
{
	for (int i = 0; i < 8; i++)
	{
		uint32_t temp = murmur3((uint8_t *) q, strlen(q), primeNumbers[i]);
		uint32_t index = (temp % 2000000);
		if (b->hash[i].test(index) == false)
			return 0;
	}
	return 1;
}