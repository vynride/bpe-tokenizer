#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

using namespace std;

/*
    unordered_map does not provide a hash function for pair<long long, long long>, so we have to implement
    our own hash function

    Reference: https://www.geeksforgeeks.org/cpp/how-to-create-an-unordered_map-of-pairs-in-c/
*/
struct HASH
{
    template <class T1, class T2>
    size_t operator()(const pair<T1, T2> &p) const
    {
        size_t hash1 = hash<T1>{}(p.first);
        size_t hash2 = hash<T2>{}(p.second);

        return hash1 ^ (hash2 + 0x9e3779b9 + (hash1 << 6) + (hash1 >> 2));
    }
};

string detokenize(vector<long long> &tokens, unordered_map<long long, pair<long long, long long>> lookupTable);
pair<vector<long long>, unordered_map<pair<long long, long long>, long long, HASH>> tokenize(vector<long long> &tokens, unordered_map<pair<long long, long long>, long long, HASH> vocab, long long mergeCount);
unordered_map<pair<long long, long long>, long long, HASH> pairGen(const vector<long long> &tokens);
pair<long long, long long> mostFreq(unordered_map<pair<long long, long long>, long long, HASH> vocab);
vector<long long> mergeTokens(vector<long long> &tokens, pair<long long, long long> mostFreqPair, long long newPairsCount);

#endif
