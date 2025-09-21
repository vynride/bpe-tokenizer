#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

using namespace std;

/*
    unordered_map does not provide a hash function for pair<int, int>, so we have to implement
    our own hash function

    Reference: https://codeforces.com/blog/entry/21853
*/
struct HASH
{
    size_t operator()(const pair<int, int> &x) const
    {
        return hash<long long>()(((long long)x.first) ^ (((long long)x.second) << 32));
    }
};

string detokenize(vector<int> &tokens, unordered_map<int, pair<int, int>> lookupTable);
pair<vector<int>, unordered_map<pair<int, int>, int, HASH>> tokenize(vector<int> &tokens, unordered_map<pair<int, int>, int, HASH> vocab, long long mergeCount);
unordered_map<pair<int, int>, int, HASH> pairGen(const vector<int> &tokens);
pair<int, int> mostFreq(unordered_map<pair<int, int>, int, HASH> vocab);
vector<int> mergeTokens(vector<int> &tokens, pair<int, int> mostFreqPair, int newPairsCount);

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

    long long mergeCount;
    cout << "No. of merges to perform: (press ENTER for maximum)";
    cin >> mergeCount;

    vector<int> tokens;
    for (char ch : str)
    {
        tokens.push_back(static_cast<unsigned char>(ch));
    }

    unordered_map<pair<int, int>, int, HASH> vocab;
    pair<vector<int>, unordered_map<pair<int, int>, int, HASH>> encodingRes = tokenize(tokens, vocab, mergeCount);
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
    unordered_map<int, pair<int, int>> lookupTable;
    for (auto it = vocab.begin(); it != vocab.end(); it++)
    {
        lookupTable[it->second] = it->first;

        cout << it->second << " -> (" << it->first.first << ", " << it->first.second << ")" << endl;
    }

    string detokenizedText = detokenize(tokens, lookupTable);
    outfile << detokenizedText << '\n';
    cout << "Output for Tokenization & Detokenization written to output.txt\n";

    outfile.close();
}

unordered_map<pair<int, int>, int, HASH> pairGen(const vector<int> &tokens)
{
    int len = tokens.size();
    unordered_map<pair<int, int>, int, HASH> vocab;

    for (int i = 0; i < len - 1; i++)
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

pair<int, int> mostFreq(unordered_map<pair<int, int>, int, HASH> vocab)
{
    int maxFreq = 0;
    pair<int, int> maxFreqItem = make_pair(0, 0);

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

vector<int> mergeTokens(vector<int> &tokens, pair<int, int> mostFreqPair, int newToken)
{
    // avoid having to modify original vector in loop
    vector<int> newTokens;
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

pair<vector<int>, unordered_map<pair<int, int>, int, HASH>> tokenize(vector<int> &tokens, unordered_map<pair<int, int>, int, HASH> vocab, long long mergeCount)
{
    long long newPairsCount = 1LL;
    unordered_map<pair<int, int>, int, HASH> freqMap;
    freqMap = pairGen(tokens);

    while (newPairsCount < mergeCount)
    {
        pair<int, int> mostFreqPair = mostFreq(freqMap);

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

string detokenize(vector<int> &tokens, unordered_map<int, pair<int, int>> lookupTable)
{
    bool foundMergedToken = false;

    do
    {
        foundMergedToken = false;
        vector<int> unmergedTokens;
        // reserve enough space to avoid resizing unmergedTokens many times
        unmergedTokens.reserve(tokens.size());

        size_t i = 0;
        while (i < tokens.size())
        {
            if (tokens[i] > 255)
            {
                foundMergedToken = true;

                pair<int, int> unmergedPair = lookupTable[tokens[i]];
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

    string detokenizedText = string(tokens.begin(), tokens.end());
    return detokenizedText;
}