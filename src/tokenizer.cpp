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

unordered_map<pair<int, int>, int, HASH> pairGen(vector<int> tokens);
void printVocab(unordered_map<pair<int, int>, int, HASH> vocab);
pair<int, int> mostFreq(unordered_map<pair<int, int>, int, HASH> vocab);
vector<int> mergeTokens(vector<int>tokens, pair<int, int>mostFreqPair, int newPairsCount);

int main(void)
{
    string str;
    cout << "Enter String: ";

    getline(cin, str);
    // cout << "\nProvided String: " << str << "\n\n";

    int mergeCount;
    cout << "Enter number of merges to perform: ";
    cin >> mergeCount;

    vector<int> tokens;
    for (char ch : str)
    {
        tokens.push_back(static_cast<unsigned char>(ch));
    }

    cout << "Initial Tokens: ";
    for (int i : tokens)
    {
        cout << i << " ";
    }

    cout << '\n';
    
    unordered_map<pair<int, int>, int, HASH> freqMap;

    freqMap = pairGen(tokens);
    printVocab(freqMap);

    int newPairsCount = 0;
    unordered_map<pair<int, int>, string, HASH> vocab;

    while (newPairsCount < mergeCount)
    {
        pair<int, int> mostFreqPair = mostFreq(freqMap);
        cout << "Found Most Freq: (" << mostFreqPair.first << "," << mostFreqPair.second << ")\n";

        // early stop
        if (freqMap[mostFreqPair] == 1)
        {
            cout << "Stopping Early as Max Frequency is 1\n";
            break;
        }

        newPairsCount++;
        vocab[mostFreqPair] = 255 + newPairsCount;

        tokens = mergeTokens(tokens, mostFreqPair, newPairsCount);
        cout << "New Tokens: ";
        for (auto tok : tokens) {
            cout << tok << " ";
        }

        cout << endl; 
        
        freqMap = pairGen(tokens);
        printVocab(freqMap);
    }
}

unordered_map<pair<int, int>, int, HASH> pairGen(vector<int> tokens)
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

vector<int> mergeTokens(vector<int>tokens, pair<int, int>mostFreqPair, int newPairsCount) {

    for (size_t i = 0; i < tokens.size() - 1; i++) {
        if (tokens[i] == mostFreqPair.first && tokens[i+1] == mostFreqPair.second) {
            tokens[i] = 255 + newPairsCount;

            tokens.erase(tokens.begin() + i + 1);
            i--;
        }
    }

    return tokens;
}