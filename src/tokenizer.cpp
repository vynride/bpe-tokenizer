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

int main(void)
{
    ifstream infile("../alice.txt", ios::in);
    if (!infile.is_open())
    {
        cout << "Could not open input file.\n";
        return 1;
    }

    string str((istreambuf_iterator<char>(infile)), istreambuf_iterator<char>());
    cout << "Finished reading file\n";
    infile.close();

    long long mergeCount = 1e18 * 1LL;
    string inputNumber;
    cout << "No. of merges to perform (press ENTER for maximum): ";
    getline(cin, inputNumber);

    if (!inputNumber.empty())
    {
        mergeCount = stoll(inputNumber);
    }

    vector<long long> tokens;
    for (char ch : str)
    {
        tokens.push_back(static_cast<unsigned char>(ch));
    }

    unordered_map<pair<long long, long long>, long long, HASH> vocab;
    pair<vector<long long>, unordered_map<pair<long long, long long>, long long, HASH>> encodingRes = tokenize(tokens, vocab, mergeCount);
    tokens = encodingRes.first;
    vocab = encodingRes.second;

    ofstream outfile("../output.txt", ios::out);
    if (!outfile.is_open())
    {
        cout << "Could not open output file.\n";
        return 2;
    }

    cout << "Writing Tokenization output\n";
    outfile << "---------- Tokenization ----------\n\n";
    outfile << "[";

    for (size_t i = 0; i < tokens.size(); i++)
    {
        outfile << tokens[i];

        if (i != tokens.size() - 1)
        {
            outfile << ", ";
        }
    }

    outfile << "]\n\n";
    cout << "Writing Detokenization output\n";
    outfile << "---------- Detokenization ----------\n\n";

    // reverse vocab
    unordered_map<long long, pair<long long, long long>> lookupTable;
    for (auto it = vocab.begin(); it != vocab.end(); it++)
    {
        lookupTable[it->second] = it->first;

        cout << it->second << " -> (" << it->first.first << ", " << it->first.second << ")" << endl;
    }

    string detokenizedText = detokenize(tokens, lookupTable);
    outfile << detokenizedText << '\n';
    cout << "\nOutput for Tokenization & Detokenization written to output.txt\n";

    outfile.close();

    return 0;
}

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