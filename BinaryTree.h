// Adam Shaar
// ashaar2
//
// BinaryTree.h
//
// custom implementation of BinaryTree Class
#pragma once

// TreeNode struct represents a node in the Huffman tree.
struct TreeNode {
  unsigned char character;
  int frequency;
  TreeNode *left;
  TreeNode *right;

  TreeNode(unsigned char c, int f) {
    character = c;
    frequency = f;
    left = right = nullptr;
  }
};

class BinaryTree {
public:
  BinaryTree();
  ~BinaryTree();
  BinaryTree(const BinaryTree &other);
  BinaryTree &operator=(const BinaryTree &other);
  BinaryTree(TreeNode *root);

  TreeNode *getRoot() const;

private:
  TreeNode *root;
  void copyTree(TreeNode *&dest, TreeNode *src);
  void deleteTree(TreeNode *node);
};

// Default constructor initializes the root to nullptr.
BinaryTree::BinaryTree() { root = nullptr; }
// Destructor calls the deleteTree function to free memory.
BinaryTree::~BinaryTree() { deleteTree(root); }

BinaryTree::BinaryTree(const BinaryTree &other) {
  root = nullptr;
  copyTree(root, other.root);
}

// Copy assignment operator checks for self-assignment, frees memory, and
// deep-copies the source tree.
BinaryTree &BinaryTree::operator=(const BinaryTree &other) {
  if (this == &other) {
    return *this;
  }

  deleteTree(root);
  copyTree(root, other.root);

  return *this;
}

// Constructor with TreeNode root initializes the root to the provided TreeNode.
BinaryTree::BinaryTree(TreeNode *root) : root(root) {}

// Returns the root node of the binary tree.
TreeNode *BinaryTree::getRoot() const { return root; }

// Recursively copies the source tree into the destination tree.
void BinaryTree::copyTree(TreeNode *&dest, TreeNode *src) {
  if (src == nullptr) {
    dest = nullptr;
  } else {
    dest = new TreeNode(src->character, src->frequency);
    copyTree(dest->left, src->left);
    copyTree(dest->right, src->right);
  }
}

// Recursively deletes the tree, starting from the given node.
void BinaryTree::deleteTree(TreeNode *node) {
  if (node != nullptr) {
    deleteTree(node->left);
    deleteTree(node->right);
    delete node;
  }
}