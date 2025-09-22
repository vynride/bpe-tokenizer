# Byte Pair Encoding Tokenizer  
---
An implementation in C++ of the Byte Pair Encoding (BPE) algorithm for text tokenization.
## 📜 Table of Contents
- [✨ Description](#-description)
- [🧠 How it Works](#-how-it-works)
- [📂 File Structure](#-file-structure)
- [🛠️ Core Components](#️-core-components)
  - [`tokenizer.h`](#tokenizerh)
  - [`tokenizer.cpp`](#tokenizercpp)
  - [`main.cpp`](#maincpp)
- [🔄 Workflow](#-workflow)
  - [1. Training](#1-training)
  - [2. Encoding](#2-encoding)
  - [3. Decoding](#3-decoding)
- [📚 Training Data](#-training-data)
- [🚀 How to Use](#-how-to-use)

## ✨ Description

This project provides a C++ implementation of the Byte Pair Encoding (BPE) algorithm for tokenization. It features a robust command-line interface (CLI) that allows you to train a BPE tokenizer on a text corpus, and then use the trained model to encode new strings into tokens and decode them back to text.

## 🧠 How it Works

Byte-pair encoding is a data compression technique that iteratively replaces the most frequent pair of consecutive bytes in a sequence with a new byte that is not present in the data. A vocabulary of these replacements is built during the training process, which is then used to rebuild the original data.

For a deep dive into the algorithm, check out the [Wikipedia article](https://en.wikipedia.org/wiki/Byte-pair_encoding).

###  Illustrative Example

Let's say we want to encode the string: `aaabdaaabac`

1.  The pair **"aa"** is the most frequent. We replace it with a new token, "Z".
    - **Data:** `ZabdZabac`
    - **Vocabulary:** `Z = aa`

2.  Now, the pair **"ab"** is the most frequent. We replace it with "Y".
    - **Data:** `ZYdZYac`
    - **Vocabulary:** `Y = ab`, `Z = aa`

3.  Next, **"ZY"** is the most frequent pair. We replace it with "X".
    - **Data:** `XdXac`
    - **Vocabulary:** `X = ZY`, `Y = ab`, `Z = aa`

At this stage, no pair appears more than once, so the process stops.

## 📂 File Structure

```
bpe-tokenizer/
├── src/
│   ├── main.cpp        # 🖥️ Menu-driven CLI for interaction
│   ├── tokenizer.cpp   # ⚙️ Implementation of the BPE functions
│   └── tokenizer.h     # 📋 Header file for tokenizer declarations
├── README.md           # 📄 This file LOL!
├── alice.txt           # 📖 Training corpus
└── output.txt          # 📝 Logs for vocabulary, tokenization, and detokenization
```

## 🛠️ Core Components

### `tokenizer.h`
This header file defines the blueprint for our tokenizer, declaring all necessary functions and data structures.

-   **`struct HASH`**: A custom hash function to enable the use of `std::pair` as a key in `std::unordered_map`.
-   **`detokenize(...)`**: Converts a vector of tokens back into a human-readable string.
-   **`tokenize(...)`**: The core training function that builds the vocabulary from a text corpus.
-   **`pairGen(...)`**: Scans the token sequence and generates a frequency map of adjacent pairs.
-   **`mostFreq(...)`**: Finds the most frequently occurring pair of tokens in the frequency map.
-   **`mergeTokens(...)`**: Merges the most frequent pair of tokens into a single new token.

### `tokenizer.cpp`
This file contains the logic and implementation of the functions declared in `tokenizer.h`.

-   **`pairGen(...)`**: Iterates through the tokens to count the occurrences of each adjacent pair.
-   **`mostFreq(...)`**: Finds the pair with the highest count in the frequency map.
-   **`mergeTokens(...)`**: Creates a new, shorter vector of tokens by replacing instances of the most frequent pair with a new token ID.
-   **`tokenize(...)`**: The main training loop. It repeatedly finds the most frequent pair, merges them, and updates the vocabulary.
-   **`detokenize(...)`**: Reverses the process by iteratively unpacking merged tokens using a lookup table until only the original bytes remain.

### `main.cpp`
This file serves as the user-facing entry point, providing a simple CLI to interact with the tokenizer.

-   **`printMenu()`**: Displays the main menu with options for the user.
-   **`main()`**: The main program loop that handles user input and orchestrates the calls to the tokenizer functions for training, encoding, and decoding.

## ✨ Bonus Features
- Unicode corner cases 
- Handling of special tokens
- [Demo Video](https://drive.google.com/file/d/17nE_Pq25MQSZE1vpHiNglW3P9FawqQZx/view?usp=drive_link).

## 🔄 Workflow
The application guides you through a simple train-encode-decode process.

### 1. Training
-   The user selects the "Train tokenizer" option and provides the path to a text file (like `alice.txt`).
-   The program reads the text and converts it into a sequence of integer tokens, where each token represents the byte value of a character.
-   It then iteratively finds the most frequent pair of adjacent tokens and merges them into a new, higher-valued token.
-   This new merge rule is stored in a vocabulary map (e.g., `(97, 97) -> 256`).
-   This process repeats for a specified number of merges, building a rich vocabulary. The learned vocabulary is logged in `output.txt`.

### 2. Encoding
-   After training, the user can encode any string.
-   The input string is first converted into a sequence of base tokens.
-   The tokenizer then applies the learned merge rules from the vocabulary to this sequence until no more merges are possible.
-   The final sequence of tokens is the compressed, encoded representation of the string.

### 3. Decoding
-   Once a string has been encoded, the user can decode the resulting tokens.
-   Using a reverse lookup table created from the vocabulary, the program unpacks the merged tokens back into their constituent pairs.
-   This process is repeated until all tokens are broken down into their original byte values, which are then converted back into a string.

## 📚 Training Data

The `alice.txt` file is provided as a sample training corpus. It is a plain text version of Lewis Carroll's "Alice's Adventures in Wonderland," sourced from [UMass Trace Repository](https://gaia.cs.umass.edu/wireshark-labs/alice.txt).

## 🚀 How to Use

1.  **Clone the repository:**
    ```bash
    git clone https://github.com/vynride/bpe-tokenizer
    ```

2.  **Navigate to the source directory:**
    ```bash
    cd src/
    ```

3.  **Compile the source code:**
    (Requires a C++ compiler like g++)
    ```bash
    g++ main.cpp tokenizer.cpp -o bpe_tokenizer
    ```

4.  **Run the application:**
    ```bash
    ./bpe_tokenizer
    ```

5.  **Follow the on-screen menu:**

    ```
    ---------- BPE Tokenizer ----------

    [1] Train tokenizer
    [2] Encode
    [3] Decode
    [4] Exit

    Enter choice index: 1
    ```

    -   **Train:** Enter `1` and provide the path to a training file (e.g., `../alice.txt`).
    -   **Encode:** After training, enter `2` and input the string you want to tokenize.
    -   **Decode:** After encoding, enter `3` to see the original string restored.
    -   **Exit:** Enter `4` to close the program.
      
## 🤖 AI Usage Disclaimer
- Generating edge cases
- Help in writing this `README.md` :P
