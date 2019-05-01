#ifndef BITMANIPULATION_H
#define BITMANIPULATION_H

#include<vector>

unsigned long numbits(unsigned long generator);
unsigned long bitsequence_from_vector(std::vector<char> seq);
char xor_all_bits(unsigned long val);
std::vector<char> takeskip(std::vector<char> &vec, unsigned long take=1, unsigned long skip=1, unsigned long offset=0);

#endif // BITMANIPULATION_H
