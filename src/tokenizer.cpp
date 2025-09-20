#include <iostream>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

using namespace std;

/*
    unordered_map does not provide a hash function for pair<int, int>, so we have to implement
    our own hash function

    Reference: https://codeforces.com/blog/entry/21853#comment-265285
*/

struct HASH
{
    size_t operator()(const pair<int, int> &x) const
    {
        return hash<long long>()(((long long)x.first) ^ (((long long)x.second) << 32));
    }
};

pair<vector<int>, unordered_map<pair<int, int>, string, HASH>> encode(vector<int> tokens, unordered_map<pair<int, int>, string, HASH> vocab, int mergeCount);
unordered_map<pair<int, int>, int, HASH> pairGen(const vector<int> &tokens);
void printVocab(unordered_map<pair<int, int>, int, HASH> vocab);
pair<int, int> mostFreq(unordered_map<pair<int, int>, int, HASH> vocab);
vector<int> mergeTokens(vector<int> tokens, pair<int, int> mostFreqPair, int newPairsCount);

int main(void)
{
    string str;
    cout << "Enter String: ";

    getline(cin, str);

    int mergeCount;
    cout << "Enter number of merges to perform: ";
    cin >> mergeCount;

    vector<int> tokens;
    for (char ch : str)
    {
        tokens.push_back(static_cast<unsigned char>(ch));
    }

    cout << "\nInitial Tokens: [";
    for (int i : tokens)
    {
        cout << i << ", ";
    }

    cout << "]\n";

    unordered_map<pair<int, int>, string, HASH> vocab;

    pair<vector<int>, unordered_map<pair<int, int>, string, HASH>> encodingRes = encode(tokens, vocab, mergeCount);
    tokens = encodingRes.first;
    vocab = encodingRes.second;
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

// TODO: this isn't printing the vocab but the number of times a pair occurs
void printVocab(unordered_map<pair<int, int>, int, HASH> vocab)
{
    for (const auto &item : vocab)
    {
        cout << "(" << item.first.first << "," << item.first.second << ") - " << item.second << '\n';
    }
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

vector<int> mergeTokens(vector<int> tokens, pair<int, int> mostFreqPair, int newToken)
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

pair<vector<int>, unordered_map<pair<int, int>, string, HASH>> encode(vector<int> tokens, unordered_map<pair<int, int>, string, HASH> vocab, int mergeCount)
{
    int newPairsCount = 0;
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

        cout << "\nSelected Most Frequent Pair: (" << mostFreqPair.first << ", " << mostFreqPair.second << ")";

        int newToken = 255 + newPairsCount;
        newPairsCount++;
        vocab[mostFreqPair] = to_string(newToken);

        tokens = mergeTokens(tokens, mostFreqPair, newToken);
        cout << "\nTokens after merging: [";
        for (auto tok : tokens)
        {
            cout << tok << ", ";
        }

        cout << "]\n";

        freqMap = pairGen(tokens);
    }

    return make_pair(tokens, vocab);
}
