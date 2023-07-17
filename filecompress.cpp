// Adam Shaar
// ashaar2
//
// filecompress.cpp
//
// the main file of the program 
// g++ filecompress.cpp + ./a.out to run
// create Huffman information files, load Huffman information files, compress files with Huffman information, and decompress files with Huffman information

#include "filecompress.h"
#include "BinaryTree.h"
#include "PriorityQueue.h"
#include <bitset>
#include <cctype>
#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using std::cin;
using std::cout;
using std::string;

//
// displayCommands()
//
// Display each of the function operations and take a filename as an input
void displayCommands() {
  cout << "\nOperation are given by digits 1 through 5\n\n";
  cout << "  1 <filename> - create a new Huffman Information file from an "
          "original file\n";
  cout << "  2 <filename> - load a Huffman Information file \n";
  cout << "  3 <filename> - compress a file using the current Huffman "
          "Information file\n";
  cout << "  4 <filename> - decompress a file using the current Huffman "
          "Information file\n";
  cout << "  5            - quit the program\n\n";
}

int main(int argc, char **argv) {

  cout << "Welcome to File Compression program\n";

  displayCommands();

  bool done = false;
  string line;
  string input;

  std::vector<std::string> huffmanCodes(128, "");
  BinaryTree *huffmanTree = nullptr;

  do {
    cout << "cmd> ";

    // read a complete line
    std::getline(std::cin, line);

    // now create a "stringstream" on the line just read
    std::stringstream ss(line);

    // clear input from any previous value
    input = "";

    // get command character
    char command;
    ss >> command;

    if (command == '1') {
      ss >> input;
      // create a .hi file that contains:
      //   - 128 lines of information
      //   - decimal value of ASCII characters in sorted order from 0 to 127
      //   - binary string representing ASCII characters with Huffman encoding
      createHuffmanInfoFile(input);
    }

    if (command == '2') {
      ss >> input;
      // store Huffman code information into the huffmanCodes vector
      // store Huffman tree into huffmanTree BinaryTree Structure
      loadHuffmanInfoFile(input, huffmanCodes, huffmanTree);
    }

    if (command == '3') {
      // verifying that operation 2 was performed
      if (huffmanTree == nullptr) {
        std::cout << "Error: Huffman Information file not loaded. Please load "
                     "a .hi file using operation 2."
                  << std::endl;
      } else {
        ss >> input;
        // reading in the file character by character based on user input
        // For each character, write out a bit string for the character's Huffman Code
        // create a .hc file that contains the bit string for the character's Huffman code
        // display the following statistics 
        //   - # of char/bytes in the original file
        //   - # of bytes in the compressed file
        //   - The compression ratio printed out to 5 decimal places
        //   - % of saved space printed out to 5 decimal places
        compressFile(input, huffmanTree, huffmanCodes);
      }
    }

    if (command == '4') {
      // verifying that operation 2 was performed
      if (huffmanTree == nullptr) {
        std::cout << "Error: Huffman Information file not loaded. Please load "
                     "a .hi file using operation 2."
                  << std::endl;
      } else {
        ss >> input;
        // Read-in file bit-by-bit and traverse down the HuffmanTree structure one level for each bit
        // If the bit is 0, traverse to the left child. If the bit is a 1, traverse to the right child.
        // When a leaf node is encountered, output the ASCII character associated with the leaf node and begin the next traversal from the top of the tree
        // creates the src of the original file that created the .hc
        decompressFile(input, huffmanTree);
      }
    }

    if (command == '5' || command == 'q') {
      // end program
      done = true;
    }

  } while (!done && !cin.eof());

  return 0;
}