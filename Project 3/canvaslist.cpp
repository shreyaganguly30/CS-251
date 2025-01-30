#include "canvaslist.h"

#include <iostream>

using namespace std;

// Default Constructor
CanvasList::CanvasList() {
  this->list_size = 0;
  this->list_front = nullptr;
}

// Copy Constructor
CanvasList::CanvasList(const CanvasList& object) {
  this->list_size = object.list_size;
  this->list_front = nullptr;
  ShapeNode* existingNode = object.list_front;
  ShapeNode* copiedIntoLinkedList = nullptr;
  // if your input has nothing, return nothing
  if (object.list_front == nullptr) {
    return;
  }
  // if you're copying into itself, return nothing
  if (this == &object) {
    return;
  }
  while (existingNode != nullptr) {
    ShapeNode* nodeToAdd = new ShapeNode();
    nodeToAdd->value = existingNode->value->copy();
    nodeToAdd->next = nullptr;
    if (copiedIntoLinkedList == nullptr) {
      this->list_front = nodeToAdd;
    } else {
      copiedIntoLinkedList->next = nodeToAdd;
    }
    copiedIntoLinkedList = nodeToAdd;
    existingNode = existingNode->next;
  }
}

CanvasList& CanvasList::operator=(const CanvasList& object) {
  // prevent self-assign by returning a pointer to itself
  if (this == &object) {
    return *this;
  }
  // empty the object youre copying into
  this->clear();
  if (object.list_front == nullptr) {
    return *this;
  }
  // set the size
  this->list_size = object.list_size;
  // allocate space for the first node to avoid nullptr issues
  this->list_front = new ShapeNode;
  // set the first values
  this->list_front->value = object.list_front->value->copy();
  this->list_front->next = nullptr;
  // create a new pointer to traverse thru the object parameter
  // use next bc already copied first
  ShapeNode* objectToCopy = object.list_front->next;
  ShapeNode* nodeToEdit = this->list_front;
  while (objectToCopy != nullptr) {
    // retrieve the shape you wanna add
    ShapeNode* newShapeNode = new ShapeNode;
    newShapeNode->value = objectToCopy->value->copy();
    newShapeNode->next = nullptr;
    // set the next node
    nodeToEdit->next = newShapeNode;
    // move to next node
    nodeToEdit = newShapeNode;
    objectToCopy = objectToCopy->next;
  }
  return *this;  // return a pointer to itself
}

CanvasList::~CanvasList() {
  clear();
}

void CanvasList::clear() {
  ShapeNode* curr = this->list_front;
  while (curr != nullptr) {
    ShapeNode* temp = curr;
    Shape* shape = curr->value;
    delete shape;
    curr = curr->next;
    delete temp;
  }
  this->list_front = nullptr;
  this->list_size = 0;
}

void CanvasList::insert_after(int index, Shape* newShape) {
  if (index < 0 || index >= this->list_size) {
    return;  // stop function
  }
  // Go thru the linked list till you get to the index you wanna add after
  ShapeNode* nodeToAddAfter = this->list_front;
  for (int i = 0; i < index; i++) {
    nodeToAddAfter = nodeToAddAfter->next;
  }
  // Create a new ShapeNode* for the rest of the list
  ShapeNode* restOfTheList = new ShapeNode();
  restOfTheList->value = newShape;
  restOfTheList->next = nodeToAddAfter->next;
  // Append that new ShapeNode* to the end of the first half
  nodeToAddAfter->next = restOfTheList;
  // increment the list size
  this->list_size++;
}

void CanvasList::push_front(Shape* newHead) {
  // make a new ShapeNode*
  ShapeNode* newNode = new ShapeNode;
  // Set the head of that node with the value
  newNode->value = newHead;
  // Set the next value to the original linked list
  newNode->next = this->list_front;
  // reset your original linked list with the newNode
  this->list_front = newNode;
  this->list_size++;
}

void CanvasList::push_back(Shape* newNode) {
  // Create new ShapeNode with parameter
  ShapeNode* ourNewNode = new ShapeNode;
  // copy() the Shape value
  ourNewNode->value = newNode;
  // set the next node to null
  ourNewNode->next = nullptr;

  // if your first element is null
  if (this->list_front == nullptr) {
    this->list_front = ourNewNode;
  } else {
    // Go to your current first element
    ShapeNode* shapePtr = this->list_front;
    // While curr is not a null pointer, traverse
    while (shapePtr->next != nullptr) {
      shapePtr = shapePtr->next;
    }
    // now we are at the node that points to nothing
    // set that pointer to the new node
    shapePtr->next = ourNewNode;
  }
  this->list_size++;
}

void CanvasList::remove_at(int val) {
  // If index is invalid, stop
  if (val < 0 || val >= this->list_size) {
    return;  // stop function
  }
  if (val == 0) {
    ShapeNode* linkedList = this->list_front;
    this->list_front = linkedList->next;
    // deallocate the Shape* and the ShapeNode* to prevent memory leaks
    delete linkedList->value;
    delete linkedList;
    this->list_size--;
    return;  // stop function
  }
  // Go thru the linked list till you get to the index you wanna remove
  ShapeNode* lastNodeToKeep = nullptr;
  ShapeNode* nodeToRemove = this->list_front;
  for (int i = 0; i < val; i++) {
    // current node
    lastNodeToKeep = nodeToRemove;
    // next node (aka the node when your index == val)
    nodeToRemove = nodeToRemove->next;
  }
  if (nodeToRemove != nullptr) {
    lastNodeToKeep->next = nodeToRemove->next;
  } else {
    this->list_front = nodeToRemove->next;
  }
  // deallocate the Shape* and the ShapeNode* to prevent memory leaks
  this->list_size--;
  delete nodeToRemove->value;
  delete nodeToRemove;
}

// remove at odd indicies {0, 1, 2, 3} -> {0, 2}
void CanvasList::remove_every_other() {
  // get the front of the list
  ShapeNode* nodeToKeep = this->list_front;
  // while the next 2 elements exist
  while (nodeToKeep != nullptr && nodeToKeep->next != nullptr) {
    // make the next node (odd index) your removal node
    ShapeNode* nodeToRemove = nodeToKeep->next;
    // point the next of the current element to the next even element
    nodeToKeep->next = nodeToRemove->next;
    // move to the next even element
    nodeToKeep = nodeToRemove->next;
    // delete the nodeToRemove
    delete nodeToRemove->value;
    delete nodeToRemove;
    // decrement the list size
    this->list_size--;
  }
}

Shape* CanvasList::pop_front() {
  // If the list is empty, return nullptr
  if (this->list_front == nullptr) {
    return nullptr;
  }
  // go to the start of the linked list
  ShapeNode* linkedList = this->list_front;
  // get the first shape
  Shape* firstShape = linkedList->value;
  // make the original front the next node
  this->list_front = list_front->next;
  // delete the original front
  delete linkedList;
  // increment the size
  this->list_size--;
  // return the first shape
  return firstShape;
}

Shape* CanvasList::pop_back() {
  // If the list is empty, return nullptr
  if (this->list_front == nullptr) {
    return nullptr;
  }
  // If the list has one item
  if (this->list_front->next == nullptr) {
    Shape* shape = this->list_front->value;
    delete this->list_front;
    this->list_front = nullptr;
    (this->list_size)--;
    return shape;
  }
  ShapeNode* linkedList = this->list_front;
  // while the next to next node is not null, move forward
  while (linkedList->next->next != nullptr) {
    linkedList = linkedList->next;
  }
  Shape* lastShape = linkedList->next->value;
  delete linkedList->next;
  linkedList->next = nullptr;
  (this->list_size)--;
  return lastShape;
}

ShapeNode* CanvasList::front() const {
  return this->list_front;
}

bool CanvasList::empty() const {
  return list_size == 0;
}

int CanvasList::size() const {
  return this->list_size;
}

int CanvasList::find(int x, int y) const {
  ShapeNode* shape = this->list_front;
  int index = 0;
  while (shape != nullptr) {
    int currX = shape->value->getX();
    int currY = shape->value->getY();
    if (currX == x && currY == y) {
      return index;
    }
    index++;
    shape = shape->next;
  }
  return -1;
}

Shape* CanvasList::shape_at(int val) const {
  int currentIndex = 0;
  ShapeNode* current = this->front();
  while (current != nullptr) {
    if (currentIndex == val) {
      return current->value;
    }
    currentIndex++;
    current = current->next;
  }
  return nullptr;
}

void CanvasList::draw() const {
  ShapeNode* currentNode = this->front();
  while (currentNode != nullptr) {
    Shape* currentShape = currentNode->value;
    cout << currentShape->as_string() << endl;
    currentNode = currentNode->next;
  }
}

void CanvasList::print_addresses() const {
  ShapeNode* currentNode = this->front();
  while (currentNode != nullptr) {
    Shape* currentShape = currentNode->value;
    cout << "Node Address: " << currentNode
         << "\tShape Address: " << currentShape << endl;
    currentNode = currentNode->next;
  }
}
