#include "tokenizer.h"

using namespace std;

string detokenize(vector<long long> &tokens, unordered_map<long long, pair<long long, long long>> lookupTable);
pair<vector<long long>, unordered_map<pair<long long, long long>, long long, HASH>> tokenize(vector<long long> &tokens, unordered_map<pair<long long, long long>, long long, HASH> vocab, long long mergeCount);
unordered_map<pair<long long, long long>, long long, HASH> pairGen(const vector<long long> &tokens);
pair<long long, long long> mostFreq(unordered_map<pair<long long, long long>, long long, HASH> vocab);
vector<long long> mergeTokens(vector<long long> &tokens, pair<long long, long long> mostFreqPair, long long newPairsCount);

unordered_map<pair<long long, long long>, long long, HASH> pairGen(const vector<long long> &tokens)
{
    size_t len = tokens.size();
    unordered_map<pair<long long, long long>, long long, HASH> vocab;

    for (size_t i = 0; i < len - 1; i++)
    {
        if (vocab.find(make_pair(tokens[i], tokens[i + 1])) != vocab.end())
        {
            vocab[make_pair(tokens[i], tokens[i + 1])]++;
        }

        else
        {
            vocab[make_pair(tokens[i], tokens[i + 1])] = 1;
        }
    }

    return vocab;
}

pair<long long, long long> mostFreq(unordered_map<pair<long long, long long>, long long, HASH> vocab)
{
    long long maxFreq = 0;
    pair<long long, long long> maxFreqItem = make_pair(0, 0);

    for (const auto &item : vocab)
    {
        if (item.second > maxFreq)
        {
            maxFreq = item.second;
            maxFreqItem = item.first;
        }
    }

    return maxFreqItem;
}

vector<long long> mergeTokens(vector<long long> &tokens, pair<long long, long long> mostFreqPair, long long newToken)
{
    // avoid having to modify original vector in loop
    vector<long long> newTokens;
    size_t i = 0;

    while (i < tokens.size())
    {
        if (i < tokens.size() - 1 && tokens[i] == mostFreqPair.first && tokens[i + 1] == mostFreqPair.second)
        {
            newTokens.push_back(newToken);
            i += 2;
        }

        else
        {
            newTokens.push_back(tokens[i]);
            i++;
        }
    }

    return newTokens;
}

pair<vector<long long>, unordered_map<pair<long long, long long>, long long, HASH>> tokenize(vector<long long> &tokens, unordered_map<pair<long long, long long>, long long, HASH> vocab, long long mergeCount)
{
    long long newPairsCount = 1LL;
    unordered_map<pair<long long, long long>, long long, HASH> freqMap;
    freqMap = pairGen(tokens);

    while (newPairsCount < mergeCount)
    {
        pair<long long, long long> mostFreqPair = mostFreq(freqMap);

        // early stop
        if (freqMap[mostFreqPair] == 1)
        {
            cout << "\nStopping as Maximum Frequency among Pairs is 1\n";
            break;
        }

        long long newToken = 255 * 1LL + newPairsCount;
        newPairsCount++;
        vocab[mostFreqPair] = newToken;
        cout << "\nSelected Most Frequent Pair: (" << mostFreqPair.first << ", " << mostFreqPair.second << ") -> " << newToken;

        tokens = mergeTokens(tokens, mostFreqPair, newToken);

        freqMap = pairGen(tokens);
    }
    cout << "\nEncoding Completed\n";

    return make_pair(tokens, vocab);
}

string detokenize(vector<long long> &tokens, unordered_map<long long, pair<long long, long long>> lookupTable)
{
    bool foundMergedToken = false;

    do
    {
        foundMergedToken = false;
        vector<long long> unmergedTokens;
        // reserve enough space to avoid resizing unmergedTokens many times
        unmergedTokens.reserve(tokens.size());

        size_t i = 0;
        while (i < tokens.size())
        {
            if (tokens[i] > 255)
            {
                foundMergedToken = true;

                pair<long long, long long> unmergedPair = lookupTable[tokens[i]];
                unmergedTokens.push_back(unmergedPair.first);
                unmergedTokens.push_back(unmergedPair.second);
            }

            else
            {
                unmergedTokens.push_back(tokens[i]);
            }

            i++;
        }

        tokens = unmergedTokens;

    } while (foundMergedToken);

    string detokenizedText;
    detokenizedText.reserve(tokens.size());

    for (long long tok : tokens)
    {
        detokenizedText += static_cast<char>(tok);
    }

    return detokenizedText;
}