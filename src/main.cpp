#include "tokenizer.h"

using namespace std;

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