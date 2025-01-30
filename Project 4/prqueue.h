#pragma once

#include <iostream>  // For debugging
#include <sstream>   // For as_string

using namespace std;

template <typename T>
class prqueue {
 public:
  struct NODE {
    int priority;
    T value;
    NODE* parent;
    NODE* left;
    NODE* right;
    NODE* link;
  };

 private:
  NODE* root;
  size_t sz;

  // Utility pointers for begin and next.
  NODE* curr;
  NODE* temp;

  // recursive function to clear the tree (post-order)
  void clearChildren(NODE* parent) {
    // if parent is empty, stop
    if (parent == nullptr) {
      return;
    }
    // else, resursively call to free its children and delete the orignal parent
    // postorder
    else {
      clearChildren(parent->left);
      clearChildren(parent->right);
      clearChildren(parent->link);
      delete parent;
    }
  }

  // recursive function to print priority queue (in-order)
  // NOTE - function is const when it doesnt modify the object's state
  void printChildren(NODE* parent, ostringstream& pqInfo) const {
    // parent is empty, stop
    if (parent == nullptr) {
      return;
    }
    // else, recursively call the left node, concat to the string, call right
    // node
    else {
      printChildren(parent->left, pqInfo);
      pqInfo << parent->priority << " value: " << parent->value << "\n";
      NODE* parse = parent;
      while (parse->link != nullptr) {
        pqInfo << parse->link->priority << " value: " << parse->link->value
               << "\n";
        parse = parse->link;
      }
      printChildren(parent->right, pqInfo);
    }
  }

  NODE* copyHelper(NODE* original, NODE* parent) const {
    // if you've reached the end, return to stop
    if (original == nullptr) {
      return nullptr;
    }

    // otherwise, create a new node
    NODE* headNode = new NODE{
        original->priority, original->value, parent, nullptr, nullptr, nullptr};

    // call the function recursively for the left and right
    // and set those values
    headNode->left = copyHelper(original->left, headNode);
    headNode->right = copyHelper(original->right, headNode);

    // if a link exists
    if (original->link != nullptr) {
      // add the first link
      NODE* originalLink = original->link;
      NODE* linkNode = new NODE{originalLink->priority,
                                originalLink->value,
                                nullptr,
                                nullptr,
                                nullptr,
                                nullptr};
      headNode->link = linkNode;

      // while there is another link
      while (originalLink->link != nullptr) {
        // go to the next link node
        originalLink = originalLink->link;
        // make and attach the next link node
        linkNode->link = new NODE{originalLink->priority,
                                  originalLink->value,
                                  nullptr,
                                  nullptr,
                                  nullptr,
                                  nullptr};
        linkNode->link->parent = headNode;
        // move forward in the link part of the tree
        linkNode = linkNode->link;
      }
    }

    // return the head node of your copied tree
    return headNode;
  }

  // recursive function to find the smallest priority and it's value (pre-order)
  void findSmallestPriorityAndValue(NODE* parent, int& smallestPriority,
                                    T& valueOfSmallestPriority) const {
    // parent is empty, stop
    if (parent == nullptr) {
      return;
    }
    // if your current priority is smaller than the current smallest,
    // set the smallestPriority and valueOfSmallestPriority
    if (parent->priority < smallestPriority) {
      smallestPriority = parent->priority;
      valueOfSmallestPriority = parent->value;
    }
    // recursively call the function
    findSmallestPriorityAndValue(parent->left, smallestPriority,
                                 valueOfSmallestPriority);
    findSmallestPriorityAndValue(parent->right, smallestPriority,
                                 valueOfSmallestPriority);
  }

  bool bothEqual(NODE* first, NODE* second) const {
    // check if both nodes are equal
    if (first == second) {
      return true;
    }

    // check if they have different values or priorities
    if (first->priority != second->priority || first->value != second->value) {
      return false;
    }

    // check if the links match
    NODE* firstLink = first->link;
    NODE* secondLink = second->link;

    // while both links have something
    while (firstLink != nullptr && secondLink != nullptr) {
      // check if the links have different values or priorities
      if (firstLink->priority != secondLink->priority ||
          firstLink->value != secondLink->value) {
        return false;
      }
      // move to next link
      firstLink = firstLink->link;
      secondLink = secondLink->link;
    }

    // check if they're at the same spot after while loop
    if (firstLink != secondLink) {
      return false;
    }

    return bothEqual(first->left, second->left) &&
           bothEqual(first->right, second->right);
  }

 public:
  prqueue() {
    this->root = nullptr;
    this->sz = 0;
    this->curr = nullptr;
    this->temp = nullptr;
  }

  size_t size() const {
    return this->sz;
  }

  NODE* getRoot() {
    return this->root;
  }

  // Adds another node given the value and priority
  void enqueue(T value, int priority) {
    // create a new node based on parameters
    NODE* newNode =
        new NODE{priority, value, nullptr, nullptr, nullptr, nullptr};

    // if the root is empty, set the new node as the root
    if (root == nullptr) {
      root = newNode;
    }
    // if the root is not empty, we have to find the right spot for the new node
    else {
      // create new nodes for traversal starting at root
      NODE* current = this->root;
      NODE* parent = nullptr;

      // while your current node is not empty
      while (current != nullptr) {
        // let parent hold the value of current
        parent = current;

        // If priority is greater, go left
        if (current->priority > priority) {
          current = current->left;
        }
        // If the priority is smaller, go right
        else if (current->priority < priority) {
          current = current->right;
        }
        // If the priorities are the same, go to linked
        else {
          current = current->link;
        }
      }

      // now current holds the correct location
      // since parent points to current, parent has the right location

      // Set the parent pointer of the new node
      newNode->parent = parent;

      // Insert the new node as a left or right child of the parent
      // If the priority of the node is less than its parent's priority,
      // make newNode the left sub node
      if (priority < parent->priority) {
        parent->left = newNode;
      }
      // If is greater than its parent's priority, make right subnode
      else if (priority > parent->priority) {
        parent->right = newNode;
      }
      // If the priority is same, add to linked
      else {
        parent->link = newNode;
      }
    }

    // increment size bc we added a new node
    sz++;
  }

  T peek() const {
    if (this->root == nullptr) {
      return T{};  // return default value for 'T'
    }
    // make a new node to parse thru
    NODE* parse = this->root;
    // set the current node's priority to smallest priority
    int smallestPriority = this->root->priority;
    // get the value associated with the smallest priority
    T valueOfSmallestPriority = this->root->value;
    // resursively parse thru to find the smallest priority's value
    findSmallestPriorityAndValue(parse, smallestPriority,
                                 valueOfSmallestPriority);
    return valueOfSmallestPriority;
  }

  // Remove the smallest priority, which is bottom left
  T dequeue() {
    if (this->root == nullptr) {
      return T{};  // return default value for 'T'
    }
    // find the smallest priority
    NODE* parse = this->root;
    NODE* parent = nullptr;

    // go to the leftmost node (smallest priority)
    while (parse->left != nullptr) {
      parent = parse;
      parse = parse->left;
    }

    // store value of the smalllest priority
    T valueOfSmallestPriority = parse->value;

    if (parse->link != nullptr) {
      NODE* linkedDuplicateNode = parse->link;
      parse->value = linkedDuplicateNode->value;
      parse->priority = linkedDuplicateNode->priority;
      parse->link = linkedDuplicateNode->link;
      delete linkedDuplicateNode;
      this->sz--;
      return valueOfSmallestPriority;
    }

    // we have parsed to the bottom left, but our node can have a right child...
    // if the root is the only node, aka the parent is nullptr,
    // set the root to be the right subtree of our last left tree
    if (parent == nullptr) {
      this->root = parse->right;

      // If the right subtree is added as the new root,
      // set its parent pointer to null bc it's the root
      if (this->root != nullptr) {
        this->root->parent = nullptr;
      }
    }
    // if your smallest priority node is not your root node,
    // set the removal node's parent's left to the removal node's right
    else {
      // Otherwise, reattach the parent's left pointer to parse's right subtree
      parent->left = parse->right;

      // If the right subtree is added,
      // set it's parent to the removal node's parent
      if (parse->right != nullptr) {
        parse->right->parent = parent;
      }
    }

    // delete the node with the smallest priority
    delete parse;
    // decrement the size of the tree
    this->sz--;
    // return the value of the node with the smallest priority
    return valueOfSmallestPriority;
  }

  string as_string() const {
    // use ostringstream to concatentate strings and variables for output
    ostringstream pqInfo;
    // call recursive function to get all priority queue info
    printChildren(this->root, pqInfo);
    // return the output string stream's buffer as a single string
    return pqInfo.str();
  }

  void clear() {
    // call a resursive function to clear the children of the parent
    clearChildren(this->root);
    // reset all other variables
    this->root = nullptr;
    this->sz = 0;
  }

  ~prqueue() {
    // resursively clear all children
    clearChildren(this->root);
  }

  prqueue(const prqueue& other) {
    // set to inital values
    this->root = nullptr;
    this->sz = 0;

    // if your paramter is not empty, reset those vars
    if (other.root != nullptr) {
      this->root = copyHelper(other.root, nullptr);
      this->sz = other.sz;
    }
  }

  prqueue& operator=(const prqueue& other) {
    // dont continue if youre setting equal to yourself
    if (this == &other) {
      return *this;
    }

    // clears this tree
    clearChildren(this->root);

    // set to inital values
    this->root = nullptr;
    this->sz = 0;

    // if your paramter is not empty, reset those vars
    if (other.root != nullptr) {
      this->root = copyHelper(other.root, nullptr);
      this->sz = other.sz;
    }

    return *this;
  }

  bool operator==(const prqueue& other) const {
    // check if both are equal by calling helper function
    return bothEqual(this->root, other.root);
  }

  void begin() {
    // set curr to the root
    curr = root;

    // make curr your smallest node
    while (curr != nullptr && curr->left != nullptr) {
      curr = curr->left;
    }
  }

  bool next(T& value, int& priority) {
    // if your current node is empty, return false
    if (curr == nullptr) {
      return false;
    };

    // get the current value and priority
    value = curr->value;
    priority = curr->priority;

    // go to the link if there is one, return true
    if (curr->link != nullptr) {
      curr = curr->link;
      return true;
    }

    // go right if there is one
    if (curr->right != nullptr) {
      curr = curr->right;
      // then go as left as you can so you're at the smallest node
      while (curr->left != nullptr) {
        curr = curr->left;
      }
    }
    // if there's no right
    else {
      // while there's a parent and the current priority matches the parent
      // priority, go to the parent
      while (curr->parent && curr->priority == curr->parent->priority) {
        curr = curr->parent;
      }
      // once youre at the parent, if you can go right, go right
      if (curr->right != nullptr) {
        curr = curr->right;
        // then go to the leftmost node
        while (curr->left != nullptr) {
          curr = curr->left;
        }
      }
      // if there's no right
      else {
        while (curr->parent != nullptr) {
          // while there's another parent,
          // if your current node isnt your parent's right node, return true
          if (curr != curr->parent->right) {
            curr = curr->parent;
            return true;
          }
          // go to the parent
          curr = curr->parent;
        }
        // go to the parent
        curr = curr->parent;
      }
    }
    // if you get to the end without returning, return true
    return true;
  }
};