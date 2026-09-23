# C Document Parser

This project is a local implementation of the HackerRank challenge titled "Structuring the Document". The goal is to parse a raw text document into a hierarchical data structure made of documents, paragraphs, sentences, and words, and then answer queries about specific elements in that structure.

The program uses C structs, dynamic memory allocation, and pointer-based indexing to represent the document as nested arrays. It reads input from standard input, builds the document internally, and prints the requested paragraph, sentence, or word based on user queries.

---

## Problem Summary

The challenge models a document in terms of four nested levels:

- A document contains paragraphs
- A paragraph contains sentences
- A sentence contains words
- A word is a string of characters

The document is provided as flat text, where:

- paragraphs are separated by newline characters (`\n`)
- sentences are separated by periods (`.`)
- words are separated by spaces (` `)

The program must convert the raw input text into a structured representation that can be queried efficiently.

---

## Data Structures

The core structures are defined as follows:

```c
struct word {
    char* data;
};

struct sentence {
    struct word* data;
    int word_count;
};

struct paragraph {
    struct sentence* data;
    int sentence_count;
};

struct document {
    struct paragraph* data;
    int paragraph_count;
};
```

These types mirror the HackerRank model and allow the program to build a nested document tree using pointer-based arrays.

---

## How the Program Works

### 1. Input Collection

The program reads the number of paragraphs, then reads each paragraph as a single string, and finally reads the number of queries.

Each query is one of the following forms:

- `1 k` -> retrieve the `k`th paragraph
- `2 k m` -> retrieve the `k`th sentence of the `m`th paragraph
- `3 k m n` -> retrieve the `k`th word of the `m`th sentence of the `n`th paragraph

### 2. Building the Document

The function `get_document(char* text)` walks the input string and:

- counts the number of paragraphs
- allocates an array of paragraph structs
- counts the number of sentences in each paragraph
- allocates each paragraph's sentence array
- counts the number of words in each sentence
- allocates each sentence's word array
- copies each word into its own dynamically allocated string

This results in a deeply nested structure that mirrors the structure of the original text.

### 3. Query Handling

The program exposes helper functions such as:

- `kth_paragraph(struct document Doc, int k)`
- `kth_sentence_in_mth_paragraph(struct document Doc, int k, int m)`
- `kth_word_in_mth_sentence_of_nth_paragraph(struct document Doc, int k, int m, int n)`

These return the relevant struct instance from the nested document data.

### 4. Output

The code then prints the selected object using helper functions like:

- `print_word()`
- `print_sentence()`
- `print_paragraph()`

This lets the program output the exact paragraph, sentence, or word requested by a query.

---

## Input Format and Query Semantics

The program expects input in this order:

1. The number of paragraphs in the document
2. Each paragraph on its own line
3. The number of queries to execute
4. One query per line

The query formats are:

- `1 k` -> return paragraph `k`
- `2 k m` -> return sentence `k` from paragraph `m`
- `3 k m n` -> return word `k` from sentence `m` of paragraph `n`

The indexing is 1-based, just like the HackerRank problem statement.

### Example Input

```text
2
Alpha beta gamma. Delta epsilon.
Red blue green. Yellow.
6
1 2
2 1 1
2 2 1
3 1 1 1
3 2 1 1
3 2 2 1
```

### How to Read That Input

- `2` means there are 2 paragraphs
- Paragraph 1: `Alpha beta gamma. Delta epsilon.`
- Paragraph 2: `Red blue green. Yellow.`
- `6` means there are 6 queries

The queries mean:

- `1 2` -> print paragraph 2
- `2 1 1` -> print sentence 1 of paragraph 1
- `2 2 1` -> print sentence 2 of paragraph 1
- `3 1 1 1` -> print word 1 of sentence 1 of paragraph 1
- `3 2 1 1` -> print word 2 of sentence 1 of paragraph 1
- `3 2 2 1` -> print word 2 of sentence 2 of paragraph 1

### Example Output

```text
Red blue green. Yellow.
Alpha beta gamma
 Delta epsilon
Alpha
beta
Delta
```

### Why This Output Makes Sense

- The first result is paragraph 2:
  `Red blue green. Yellow.`
- The second result is sentence 1 of paragraph 1:
  `Alpha beta gamma`
- The third result is sentence 2 of paragraph 1:
  ` Delta epsilon`
- The fourth result is the first word in sentence 1 of paragraph 1:
  `Alpha`
- The fifth result is the second word in sentence 1 of paragraph 1:
  `beta`
- The sixth result is the second word in sentence 2 of paragraph 1:
  `Delta`

Notice that the output is not prefixed with labels; it simply prints the selected struct contents in the exact format expected by the challenge.

---

## Local Build and Run

Compile the C source file:

```bash
gcc -o c-doc-parser c-doc-parser.c
```

Run it with input redirected from a file:

```bash
./c-doc-parser < tests/input/input1.txt
```

You can also run it interactively:

```bash
./c-doc-parser
```

---

## Project Files

```text
.
├── c-doc-parser.c
├── README.md
└── tests/
    ├── input/
    └── output/
```

- `c-doc-parser.c` contains the full parser and query logic
- `README.md` explains the challenge and the implementation
- `tests/input/` contains sample inputs for local testing

---

## Notes

This project is a strong example of:

- struct composition in C
- dynamic memory allocation with `malloc`
- parsing raw text using delimiters
- pointer arithmetic and nested data structures
- building a custom in-memory document model

It is a good exercise in both data modeling and memory management in C.

---

## Reference

This implementation follows the HackerRank challenge:

https://www.hackerrank.com/challenges/structuring-the-document/problem
