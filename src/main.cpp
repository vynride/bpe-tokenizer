#include "tokenizer.h"
#include <limits>

using namespace std;

void printMenu();

int main(void)
{
    bool isTrained = false;
    unordered_map<pair<long long, long long>, long long, HASH> vocab;
    unordered_map<long long, pair<long long, long long>> lookupTable;
    vector<long long> encodedNewTokens;
    string fpath, str;
    ofstream outfile("../output.txt", ios::out);
    
    if (!outfile.is_open())
    {
        cout << "Could not open output file.\n";
        return 2;
    }

    while (1) {
        int choice;
        printMenu();
        cin >> choice;
        cin.ignore();

        switch(choice) {
            case 1:
            {
                cout << "---------- Training Tokenizer ----------\n\n";

                cout << "Enter path of file to read: ";
                getline(cin, fpath);

                ifstream infile(fpath);

                if (!infile.is_open())
                {
                    cout << "Could not open input file.\n";
                    cout << "Provided path: " << fpath << '\n';
                    break;
                }

                str.assign((istreambuf_iterator<char>(infile)), istreambuf_iterator<char>());
                cout << "Finished reading file\n";

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

                pair<vector<long long>, unordered_map<pair<long long, long long>, long long, HASH>> encodingRes = tokenize(tokens, vocab, mergeCount);
                vocab = encodingRes.second;

                cout << "---------- Finished Training tokenizer  ----------\n";

                outfile << "---------- Learnt Vocabulary ----------\n\n";

                for (auto it = vocab.begin(); it != vocab.end(); it++)
                {
                    outfile << "(" << it->first.first << ", " << it->first.second << ") -> " << it->second << "\n";
                }

                // reverse vocab
                for (auto it = vocab.begin(); it != vocab.end(); it++)
                {
                    lookupTable[it->second] = it->first;
                }
                
                isTrained = true;
                infile.close();
                break;
            }
            
            case 2:
            {
                if (!isTrained) {
                    cout << "Use [1] to train the Tokenizer first\n";
                    break;
                }

                string newString;
                cout << "\nEnter string to tokenize: ";
                getline(cin, newString);

                encodedNewTokens.clear();
                for (char ch : newString)
                {
                    encodedNewTokens.push_back(static_cast<unsigned char>(ch));
                }

                cout << "Encoding the string\n";

                while (true)
                {
                    unordered_map<pair<long long, long long>, long long, HASH> freqMap = pairGen(encodedNewTokens);
                    if (freqMap.empty()) 
                        break;

                    pair<long long, long long> mostFreqPair = mostFreq(freqMap);

                    if (vocab.count(mostFreqPair))
                    {
                        long long newToken = vocab[mostFreqPair];
                        encodedNewTokens = mergeTokens(encodedNewTokens, mostFreqPair, newToken);
                    }
                    else
                    {
                        break;
                    }
                }

                cout << "Encoded tokens: [";
                outfile << "\n\n---------- Encoding the string ----------\n\n";
                outfile << "Original: " << newString << "\n";
                outfile << "[";
                for (size_t i = 0; i < encodedNewTokens.size(); i++)
                {
                    cout << encodedNewTokens[i];
                    outfile << encodedNewTokens[i];

                    if (i != encodedNewTokens.size() - 1)
                    {
                        cout << ", ";
                        outfile << ", ";
                    }
                }
                cout << "]\n";
                outfile << "]\n";

                cout << "\n\n---------- Finished Encoding the string ----------\n\n";
                break;
            }

            case 3:
            {
                if (!isTrained) {
                    cout << "Use [1] to train the Tokenizer first\n";
                    break;
                }
                if (encodedNewTokens.empty()) {
                    cout << "Use [2] to encode a string first\n";
                    break;
                }
                cout << "---------- Decoding the tokens ---------\n";
                string detokenizedNewText = detokenize(encodedNewTokens, lookupTable);

                cout << "Decoded text: " << detokenizedNewText << "\n";
                outfile << "\n\n---------- Detokenization of string ----------\n\n";
                outfile << "Decoded: " << detokenizedNewText << '\n';

                cout << "\nOutput for Tokenization & Detokenization written to output.txt\n";
                break;
            }
            case 4:
                cout << "Exiting...\n";
                outfile.close();
                return 0;

            default:
                cout << "Invalid choice. Please try again.\n";
                break;
        }
    }

    outfile.close();
    return 0; 
}

void printMenu() {
    cout << "\n---------- BPE Tokenizer ----------\n\n";
    cout << "[1] Train tokenizer\n";
    cout << "[2] Encode\n";
    cout << "[3] Decode\n";
    cout << "[4] Exit\n\n";

    cout << "Enter choice index: "; 
}