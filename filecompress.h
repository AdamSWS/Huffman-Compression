// Adam Shaar
// ashaar2
//
// filecompress.h
//
// the declaration and implementation of functions(and the helper functions)
// called in filecompress.cpp

#pragma once

#include "BinaryTree.h"
#include "PriorityQueue.h"
#include <bitset>
#include <cctype>
#include <cstring>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using std::cin;
using std::cout;
using std::string;

// function declarations
void readFileFrequencies(std::string input, std::vector<int> &frequencies);
BinaryTree *createHuffmanTree(std::vector<int> frequencies);
void generateHuffmanCodes(const TreeNode *node, std::vector<std::string> &codes,
                          std::string currentCode);
void readHuffmanCodesFromFile(const std::string &filename,
                              std::vector<std::string> &huffmanCodes);
TreeNode *rebuildHuffmanTree(const std::vector<std::string> &huffmanCodes);
int writeBit(std::ofstream &outputFile, bool bit, int &bitBuffer,
             int &bitCount);
int flushBitBuffer(std::ofstream &outputFile, int &bitBuffer, int &bitCount);
void readBit(std::istream &in, bool &bit, int &bitBuffer, int &bitCount);
void decompressFile(const std::string &inputFilename,
                    const std::string &outputFilename,
                    const BinaryTree *huffmanTree);
void createHuffmanInfoFile(const std::string &input);
void loadHuffmanInfoFile(const std::string &input,
                         std::vector<std::string> &huffmanCodes,
                         BinaryTree *&huffmanTree);
void compressFile(const std::string &input, const BinaryTree *huffmanTree,
                  const std::vector<std::string> huffmanCodes);
void decompressFile(const std::string &input, const BinaryTree *huffmanTree);

//
//  readFileFrequencies
//
//  Function finds the frequency of each ASCII character and returns a frequency
void readFileFrequencies(string fname, std::vector<int> &freq) {
  // Initialize the frequency vector with 128 0's (0 frequency = default)
  for (int i = 0; i < 128; i++) {
    freq.push_back(0);
  }
  // unsigned char ch to store current character from file
  unsigned char ch;
  std::fstream infile;
  infile.open(fname, std::fstream::in);
  // returns error message if original file DNE
  if (!infile.is_open()) {
    cout << "could not open file: " << fname << std::endl;
    exit(0);
  }
  // total characters in file
  int charCount;
  // read the file character by character
  while (infile >> std::noskipws >> ch) {
    charCount++;
    // convert char to int ASCII Value
    int chint = ch;
    // check to see if char is an ASCII char
    if (chint > 127)
      cout << "Non ASCII Character: " << chint << std::endl;
    else
      // adds to freq vector
      freq[chint]++;
  }
}

//
// createHuffmanTree
//
// Function builds a Huffman Tree based on the algorithm from 10.4.1 of CS351
// Zybooks
BinaryTree *createHuffmanTree(std::vector<int> frequencies) {
  PriorityQueue Q;
  // Initialize a priority queue Q.
  for (int i = 0; i < 128; i++) {
    unsigned char c = i;
    TreeNode *TN = new TreeNode(c, frequencies[i]);
    BinaryTree *T = new BinaryTree(TN);
    // a single node TN with f(c)
    // Insert T and freq[i] in Q
    Q.insert(T, frequencies[i]);
  }
  while (Q.size() > 1) {
    int F1 = Q.min();
    BinaryTree *T1(Q.removeMin());
    int F2 = Q.min();
    BinaryTree *T2(Q.removeMin());
    // Create a new binary tree T with left subtree T1 and right subtree T2
    TreeNode *mergedNode = new TreeNode('\0', F1 + F2);
    mergedNode->left = T1->getRoot();
    mergedNode->right = T2->getRoot();
    BinaryTree *mergedTree = new BinaryTree(mergedNode);
    // insert T into Q with key F1 + F2
    Q.insert(mergedTree, F1 + F2);
  }
  // return the last remaining tree in the Q (the Huffman Tree)
  return Q.removeMin();
}

//
// generateHuffmanCodes
//
// Function generates Huffman codes for each ASCII character by depth-first
// traversal through Huffman tree
void generateHuffmanCodes(const TreeNode *node, std::vector<std::string> &codes,
                          std::string currentCode) {
  // return if node is null
  if (node == nullptr) {
    return;
  }
  // if the current node is a leaf node, store the code in the codes vector at
  // the respective index
  if (node->left == nullptr && node->right == nullptr) {
    codes[node->character] = currentCode;
  }
  // recursively generate the code for left and right children
  generateHuffmanCodes(node->left, codes, currentCode + "0");
  generateHuffmanCodes(node->right, codes, currentCode + "1");
}

//
// readHuffmanCodesFromFile
//
// Function reads Huffman Codes from a .hi file and stores it in a vector of
// strings
void readHuffmanCodesFromFile(const std::string &filename,
                              std::vector<std::string> &huffmanCodes) {
  // opening .hi file for reading
  std::ifstream hiFile(filename);

  if (!hiFile.is_open()) {
    std::cout << "Error: Unable to open Huffman Information file." << std::endl;
    return;
  }

  std::string line;
  // Skip the first line
  std::getline(hiFile, line);

  int asciiValue;
  std::string code;
  // Read the file line by line, extract the ascii value and code and store it
  // in the huffmanCodes vector
  while (hiFile >> asciiValue >> code) {
    huffmanCodes[asciiValue] = code;
  }
  // close file
  hiFile.close();
}

//
// rebuildHuffmanTree
//
// Function rebuilds Huffman Tree from the huffmanCodes vector and returns a
// vpointer to the root node of the treew
TreeNode *rebuildHuffmanTree(const std::vector<std::string> &huffmanCodes) {
  // Create a new root node with null character and 0 frequency
  TreeNode *root = new TreeNode('\0', 0);
  // Iterate through the huffmanCodes Vector
  for (int index = 0; index < huffmanCodes.size(); ++index) {
    // get the character at the current index
    unsigned char character = index;
    // Get the Huffman code for the index
    const std::string &code = huffmanCodes[index];
    // if the Huffman code is blank, continue to the next character
    if (code.empty()) {
      continue;
    }
    // iterate through the huffman code, creating the tree as needed
    TreeNode *currentNode = root;
    for (const char c : code) {
      if (c == '0') {
        // if there isn't a left child, create a new node with a null character
        if (currentNode->left == nullptr) {
          currentNode->left = new TreeNode('\0', 0);
        }
        // Move left
        currentNode = currentNode->left;
      } else if (c == '1') {
        // If there isn't a right child, create a new node with a null character
        if (currentNode->right == nullptr) {
          currentNode->right = new TreeNode('\0', 0);
        }
        // Move right
        currentNode = currentNode->right;
      } else {
        // If character is invalid, exit with error
        cout << "Invalid character in Huffman code" << std::endl;
        exit(0);
      }
    }
    // Set the character of the current node to charcter correstponding
    currentNode->character = character;
  }
  // Return root of the rebuilt Huffman tree
  return root;
}

//
// writeBit
//
// Function wites a isngle bit to the output file and returns the number of bits
// written
int writeBit(std::ofstream &outputFile, bool bit, int &bitBuffer,
             int &bitCount) {
  if (bit) {
    bitBuffer |= (1 << (7 - bitCount));
  }
  bitCount++;
  // If the bitBuffer is full (contains 8 bits), write it to the outputFile
  if (bitCount == 8) {
    outputFile.put(bitBuffer);
    // Reset the bitCount and bitBuffer for the next group of bits
    bitCount = 0;
    bitBuffer = 0;
    return 1;
  }
  return 0;
}
//
// flushBitBuffer
//
// Function flushes the remaining bits in the bitBuffer to the output file and
// returns the number of bytes written
int flushBitBuffer(std::ofstream &outputFile, int &bitBuffer, int &bitCount) {
  // If there are bits remaining in the bitBuffer, write them to the outputFile
  if (bitCount > 0) {
    outputFile.put(bitBuffer);
    return 1;
  }
  return 0;
}

//
// findLongHuffmanCode
//
// Function searchest for the first Huffman code in the vector and returns the
// corresponding character
unsigned char
findLongHuffmanCode(const std::vector<std::string> &huffmanCodes) {
  // Iterate through the huffmanCodes vector
  for (int i = 0; i < huffmanCodes.size(); ++i) {
    // If the current Huffman code has a length greater than 8, return the
    // corresponding character
    if (huffmanCodes[i].length() > 8) {
      unsigned char ch = i;
      return ch;
    }
  }
  // return null char
  return '\0';
}

//
// readBit
//
// Function reads a single bit from the input stream and stores the result
void readBit(std::istream &in, bool &bit, int &bitBuffer, int &bitCount) {
  // If bitCount zero, read the next byte
  if (bitCount == 0) {
    bitBuffer = in.get();
    bitCount = 8;
  }
  // Check if the most significant bit is 0 or 1
  bit = (bitBuffer & (1 << (bitCount - 1))) != 0;
  // Decrement the bit count, since we just read a bit
  --bitCount;
}

//
// decompressFile
//
// Function decompresses a .hc file and writes the new contents to a file
void decompressFile(const std::string &inputFilename,
                    const std::string &outputFilename,
                    const BinaryTree *huffmanTree) {
  // Open input and output files in binary mode
  std::ifstream inputFile(inputFilename, std::ios::binary);
  std::ofstream outputFile(outputFilename, std::ios::binary);
  // Check if input and output files are open
  if (!inputFile.is_open()) {
    std::cout << "Error: Unable to open input file." << std::endl;
    return;
  }

  if (!outputFile.is_open()) {
    std::cout << "Error: Unable to open output file." << std::endl;
    return;
  }

  int bitBuffer = 0;
  int bitCount = 0;
  bool bit;
  TreeNode *currentNode = huffmanTree->getRoot();
  // Continue reading bits from the input file until EOF is reached
  while (inputFile.peek() != std::ifstream::traits_type::eof()) {
    readBit(inputFile, bit, bitBuffer, bitCount);
    // Move to the right child if bit is 1, otherwise move to the left child
    if (bit) {
      currentNode = currentNode->right;
    } else {
      currentNode = currentNode->left;
    }
    // If currentNode is a leaf node, we have found a character
    if (currentNode->left == nullptr && currentNode->right == nullptr) {
      outputFile.put(currentNode->character);
      currentNode = huffmanTree->getRoot();
    }
  }
  // Close input and output files
  inputFile.close();
  outputFile.close();
}

//
// createHuffmanInfoFile
//
// Function creates a .hi files containing huffman codes from input file
void createHuffmanInfoFile(const string &input) {
  // build a new .hi file using the information in the file: input
  std::vector<int> frequencies;
  readFileFrequencies(input, frequencies);

  // Generate the Huffman codes for the characters using the temporary Huffman
  // tree
  BinaryTree *huffmanTreeTemp = createHuffmanTree(frequencies);
  std::vector<std::string> huffmanCodesTemp(128, "");
  generateHuffmanCodes(huffmanTreeTemp->getRoot(), huffmanCodesTemp, "");

  // Save the frequency information to a .hi file
  std::string hiFilename = input + ".hi";
  std::ofstream hiFile(hiFilename);
  // Check if the .hi file is open and ready for writing
  if (hiFile.is_open()) {
    hiFile << std::endl;
    // Write the Huffman codes for each character to the .hi file
    for (int i = 0; i < 128; i++) {
      hiFile << i << "    " << huffmanCodesTemp[i] << std::endl;
    }
    // Write the Huffman codes for each character to the .hi file
    hiFile.close();
    cout << "Huffman Information file created: " << hiFilename << std::endl;
  } else {
    cout << "Error: Unable to create Huffman Information file." << std::endl;
  }
}

//
// loadHuffmanInfoFile
//
// Function loads the Huffman codes from a .hi file and creates a Huffman tree
// from the loadedd codes
void loadHuffmanInfoFile(const string &input,
                         std::vector<std::string> &huffmanCodes,
                         BinaryTree *&huffmanTree) {
  // Load a .hi file to later perform compression and decompression
  readHuffmanCodesFromFile(input, huffmanCodes);
  // If there is an existing Huffman tree, delete it to avoid memory leaks
  if (huffmanTree != nullptr) {
    delete huffmanTree;
  }
  // Rebuild the Huffman tree using the loaded Huffman codes
  TreeNode *root = rebuildHuffmanTree(huffmanCodes);
  huffmanTree = new BinaryTree(root);

  std::cout << "Huffman Information file loaded: " << input << std::endl;
}

//
// compressFile
//
// Function compresses the input file with saved HuffmanTree and Codes, and
// creates a .hc file with the new data
void compressFile(const string &input, const BinaryTree *huffmanTree,
                  const std::vector<std::string> huffmanCodes) {
  // Create the output file name by appending the ".hc" extension to the input
  // file name
  std::string outputFilename = input + ".hc";
  // Open the input and output files
  std::ifstream inputFile(input, std::ios::binary);
  std::ofstream outputFile(outputFilename, std::ios::binary);

  if (!inputFile.is_open()) {
    std::cout << "Error: Unable to open input file." << std::endl;
  } else if (!outputFile.is_open()) {
    std::cout << "Error: Unable to open output file." << std::endl;
  } else {
    // Initialize the bit buffer, bit count, input file size, and output file
    // size
    int bitBuffer = 0;
    int bitCount = 0;
    int inputSize = 0;
    int outputSize = 0;
    // Read the input file byte by byte
    unsigned char byte;
    while (inputFile.read((char *)&byte, 1)) {
      inputSize++;
      // Write the Huffman code for each byte to the output file using the
      // writeBit function
      for (char bit : huffmanCodes[byte]) {
        outputSize += writeBit(outputFile, bit == '1', bitBuffer, bitCount);
      }
    }

    // Flush any remaining bits in the bit buffer to the output file
    outputSize += flushBitBuffer(outputFile, bitBuffer, bitCount);
    // Close the input and output files
    inputFile.close();
    outputFile.close();
    // Calculate and display the compression statistics
    double compressionRatio = (double)(inputSize) / outputSize;
    double spaceSaving = (1.0 - (double)(outputSize) / inputSize) * 100.0;

    std::cout << "Input file size: " << inputSize << " bytes" << std::endl;
    std::cout << "Compressed file size: " << outputSize << " bytes"
              << std::endl;
    std::cout << "Compression ratio: " << std::fixed << std::setprecision(5)
              << compressionRatio << std::endl;
    std::cout << "Space saving: " << std::fixed << std::setprecision(5)
              << spaceSaving << " %" << std::endl;
  }
}

//
// decompressFile
//
// Function decompresses the input file with the saved huffmanTree and makes a
// decompressed file with the next data
void decompressFile(const string &input, const BinaryTree *huffmanTree) {
  // Create the output file name by removing the ".hc" extension from the input
  // file name
  std::string outputFilename = input.substr(0, input.size() - 3);
  // Call the decompressFile function with the input and output file names, and
  // the Huffman tree
  decompressFile(input, outputFilename, huffmanTree);
  std::cout << "Decompressed file: " << outputFilename << std::endl;
}