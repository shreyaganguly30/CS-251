// Why the angle brackets? Grader reasons.
// Don't change this or you will be sad :(
#include <prqueue.h>

#include "gmock/gmock.h"
#include "gtest/gtest.h"

using namespace std;
using namespace testing;

TEST(PRQueueCore, Declare_and_Initalize) {
  prqueue<int> head;
  // inital size should be 0 and root should be nullptr
  EXPECT_THAT(head.size(), Eq(0));
  EXPECT_THAT(head.getRoot(), Eq(nullptr));

  // Add values to nodes and ensure nodes are not empty after initalized
  head.enqueue(5, 1);  // head
  EXPECT_THAT(head.getRoot(), Ne(nullptr));
  head.enqueue(2, 0);  // left
  EXPECT_THAT(head.getRoot()->left, Ne(nullptr));
  head.enqueue(7, 2);  // right
  EXPECT_THAT(head.getRoot()->right, Ne(nullptr));

  // Check to see if the left and right values are correctly initalized
  EXPECT_THAT(head.size(), Eq(3));
  EXPECT_THAT(head.getRoot()->value, Eq(5));
  EXPECT_THAT(head.getRoot()->left->value, Eq(2));
  EXPECT_THAT(head.getRoot()->right->value, Eq(7));

  // clear and check that size is returned to 0
  head.clear();
  EXPECT_THAT(head.size(), Eq(0));
}

TEST(PRQueueUsing, Adding_and_Removing) {
  prqueue<string> pq;
  // value, priority
  pq.enqueue("fourth", 4);
  pq.enqueue("third", 3);
  pq.enqueue("zero", 0);
  pq.enqueue("second", 2);
  pq.enqueue("first", 1);
  // Prints... "priority" value: "data"\n
  EXPECT_THAT(pq.as_string(), Eq("0 value: zero\n1 value: first\n2 value: "
                                 "second\n3 value: third\n4 value: fourth\n"));
  EXPECT_THAT(pq.peek(), Eq("zero"));
  EXPECT_THAT(pq.size(), Eq(5));
  // dequeue removes the highest priority item (smallest priority number)
  EXPECT_THAT(pq.dequeue(), Eq("zero"));
  EXPECT_THAT(pq.size(), Eq(4));
  // new start will be "first"
  EXPECT_THAT(pq.peek(), Eq("first"));
}

TEST(PRQueueDuplicates, Duplicates_Allowed) {
  prqueue<string> pq;
  // value, priority

  pq.enqueue("zero", 0);
  pq.enqueue("first", 1);
  pq.enqueue("second", 2);

  EXPECT_THAT(pq.size(), Eq(3));
  EXPECT_THAT(pq.as_string(),
              Eq("0 value: zero\n1 value: first\n2 value: second\n"));

  pq.enqueue("duplicate", 0);
  EXPECT_THAT(pq.size(), Eq(4));
  EXPECT_THAT(pq.as_string(),
              Eq("0 value: zero\n0 value: duplicate\n1 value: first\n2 value: "
                 "second\n"));

  EXPECT_THAT(pq.getRoot(), Ne(nullptr));
  EXPECT_THAT(pq.getRoot()->value, Eq("zero"));
  EXPECT_THAT(pq.getRoot()->link, Ne(nullptr));

  EXPECT_THAT(pq.dequeue(), Eq("zero"));
  EXPECT_THAT(pq.size(), Eq(3));
  EXPECT_THAT(pq.dequeue(), Eq("duplicate"));
  EXPECT_THAT(pq.size(), Eq(2));
  EXPECT_THAT(pq.dequeue(), Eq("first"));
  EXPECT_THAT(pq.size(), Eq(1));
  EXPECT_THAT(pq.dequeue(), Eq("second"));
  EXPECT_THAT(pq.size(), Eq(0));
}

TEST(PRQueueCopying, Correctly_Copied) {
  prqueue<string> pq;
  pq.enqueue("zero", 0);
  pq.enqueue("first", 1);
  pq.enqueue("first", 1);
  EXPECT_THAT(pq.size(), Eq(3));

  prqueue<string> copy;
  copy = pq;
  EXPECT_THAT(copy.size(), Eq(3));

  EXPECT_THAT(copy.peek(), Eq("zero"));
  copy.dequeue();
  EXPECT_THAT(copy.peek(), Eq("first"));
  copy.dequeue();
  EXPECT_THAT(copy.peek(), Eq("first"));

  prqueue<string> copy2(pq);
  EXPECT_THAT(copy2.size(), Eq(3));

  EXPECT_THAT(copy2.peek(), Eq("zero"));
  copy2.dequeue();
  EXPECT_THAT(copy2.peek(), Eq("first"));
  copy2.dequeue();
  EXPECT_THAT(copy2.peek(), Eq("first"));
}

TEST(PRQueueEquality, Operator_Equals) {
  prqueue<string> pq1;
  pq1.enqueue("zero", 0);
  pq1.enqueue("first", 1);

  prqueue<string> pq2;
  pq2.enqueue("zero", 0);
  pq2.enqueue("first", 1);

  EXPECT_THAT(pq1 == pq2, Eq(true));

  prqueue<string> pq3;
  pq3.enqueue("zero", 0);
  pq3.enqueue("first", 1);

  prqueue<string> pq4;
  pq4.enqueue("zero", 0);
  pq4.enqueue("sec", 1);

  EXPECT_THAT(pq3 == pq4, Eq(false));
}

TEST(PRQueueIterator, begin_and_next) {
  prqueue<string> pq;
  string value;
  int priority;

  EXPECT_THAT(pq.next(value, priority), Eq(false));

  pq.enqueue("fourth", 4);
  pq.enqueue("fourth_dup", 4);
  pq.enqueue("fourth_dup_dup", 4);
  pq.enqueue("second", 2);
  pq.enqueue("eigth", 8);
  pq.enqueue("eigth_dup", 8);
  pq.enqueue("first", 1);
  pq.enqueue("first_dup", 1);
  pq.enqueue("fifth", 5);
  pq.enqueue("ninth", 9);

  pq.begin();

  EXPECT_THAT(pq.next(value, priority), Eq(true));
  EXPECT_THAT(value, Eq("first"));
  EXPECT_THAT(priority, Eq(1));

  EXPECT_THAT(pq.next(value, priority), Eq(true));
  EXPECT_THAT(value, Eq("first_dup"));
  EXPECT_THAT(priority, Eq(1));

  EXPECT_THAT(pq.next(value, priority), Eq(true));
  EXPECT_THAT(value, Eq("second"));
  EXPECT_THAT(priority, Eq(2));

  EXPECT_THAT(pq.next(value, priority), Eq(true));
  EXPECT_THAT(value, Eq("fourth"));
  EXPECT_THAT(priority, Eq(4));

  EXPECT_THAT(pq.next(value, priority), Eq(true));
  EXPECT_THAT(value, Eq("fourth_dup"));
  EXPECT_THAT(priority, Eq(4));

  EXPECT_THAT(pq.next(value, priority), Eq(true));
  EXPECT_THAT(value, Eq("fourth_dup_dup"));
  EXPECT_THAT(priority, Eq(4));

  EXPECT_THAT(pq.next(value, priority), Eq(true));
  EXPECT_THAT(value, Eq("fifth"));
  EXPECT_THAT(priority, Eq(5));

  EXPECT_THAT(pq.next(value, priority), Eq(true));
  EXPECT_THAT(value, Eq("eigth"));
  EXPECT_THAT(priority, Eq(8));

  EXPECT_THAT(pq.next(value, priority), Eq(true));
  EXPECT_THAT(value, Eq("eigth_dup"));
  EXPECT_THAT(priority, Eq(8));

  EXPECT_THAT(pq.next(value, priority), Eq(true));
  EXPECT_THAT(value, Eq("ninth"));
  EXPECT_THAT(priority, Eq(9));

  EXPECT_THAT(pq.next(value, priority), Eq(false));
}