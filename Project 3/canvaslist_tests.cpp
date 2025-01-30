#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "canvaslist.h"
#include "include/shape.h"

using namespace std;
using namespace testing;

/*
EXPECT_THAT - generate nonfatal failures, which don’t abort the current
function
ASSERT_THAT - generate fatal failures when they fail, and abort the
current function
*/

// CanvasListCore Tests
TEST(CanvasListCore, ctor_size) {
  CanvasList object;
  EXPECT_THAT(object.size(), Eq(0));
}

TEST(CanvasListCore, empty_false) {
  CanvasList object;
  EXPECT_THAT(object.empty(), Eq(true));
  EXPECT_THAT(object.size(), Eq(0));
  Shape* newHead = new Shape();
  object.push_front(newHead);
  EXPECT_THAT(object.front(), Ne(nullptr));
  EXPECT_THAT(object.empty(), Eq(false));
  EXPECT_THAT(object.size(), Eq(1));
  object.clear();
  EXPECT_THAT(object.size(), Eq(0));
  EXPECT_THAT(object.front(), Eq(nullptr));
}

TEST(CanvasListCore, front_nullptr) {
  CanvasList object;
  Shape* newHead = new Shape();
  object.push_front(newHead);
  EXPECT_THAT(object.front(), Ne(nullptr));
}

// Iterating Tests
TEST(CanvasListIterating, copy_constructor) {
  CanvasList object;
  Circle* newShape = new Circle(1, 1, 1);
  object.push_back(newShape);
  CanvasList newObject = object;  // deep copy
  EXPECT_THAT(dynamic_cast<Circle*>(newObject.shape_at(0)), Ne(nullptr));
}

TEST(CanvasListIterating, push_back) {
  CanvasList object;
  Shape* newShape1 = new Shape(1, 1);
  Shape* newShape2 = new Shape(2, 2);
  object.push_back(newShape1);
  object.push_back(newShape2);
  EXPECT_THAT(object.size(), Eq(2));
}

TEST(CanvasListIterating, shape_at) {
  CanvasList object;
  Circle* newShape1 = new Circle(1, 1, 3);
  Rect* newShape2 = new Rect(2, 2, 3, 3);
  RightTriangle* newShape3 = new RightTriangle(3, 3, 4, 4);
  object.push_back(newShape1);
  object.push_back(newShape2);
  object.push_back(newShape3);
  EXPECT_THAT(object.shape_at(0)->as_string(),
              StrEq("It's a Circle at x: 1, y: 1, radius: 3"));
  EXPECT_THAT(
      object.shape_at(1)->as_string(),
      StrEq("It's a Rectangle at x: 2, y: 2 with width: 3 and height: 3"));
  EXPECT_THAT(
      object.shape_at(2)->as_string(),
      StrEq("It's a Right Triangle at x: 3, y: 3 with base: 4 and height: 4"));
}

TEST(CanvasListIterating, find) {
  CanvasList object;
  Circle* newShape1 = new Circle(1, 1, 1);
  Circle* newShape2 = new Circle(1, 2, 1);
  object.push_back(newShape1);
  object.push_back(newShape2);
  EXPECT_THAT(object.find(1, 1), Eq(0));
  EXPECT_THAT(object.find(1, 2), Eq(1));
  EXPECT_THAT(object.find(5, 5), Eq(-1));
}

// Modifying Tests
TEST(CanvasListModifying, operator_equal) {
  CanvasList object1;
  CanvasList object2;
  object2 = object1;
  Circle* newShape = new Circle(1, 1, 1);
  object1.push_back(newShape);
  object2 = object1;
  EXPECT_THAT(dynamic_cast<Circle*>(object2.shape_at(0)), Ne(nullptr));
}

TEST(CanvasListModifying, pop_front) {
  CanvasList object;
  EXPECT_THAT(object.pop_front(), Eq(nullptr));
  Circle* newShape1 = new Circle(1, 1, 1);
  Circle* newShape2 = new Circle(1, 1, 2);
  object.push_back(newShape1);
  object.push_back(newShape2);
  EXPECT_THAT(object.size(), Eq(2));  // size before remove
  Shape* removedShape = object.pop_front();
  EXPECT_THAT(object.size(), Eq(1));  // size after remove
  delete removedShape;
}

TEST(CanvasListModifying, pop_back) {
  CanvasList object;
  EXPECT_THAT(object.pop_back(), Eq(nullptr));
  Circle* newShape0 = new Circle(1, 1, 1);
  object.push_back(newShape0);
  Shape* removedShape1 = object.pop_back();
  EXPECT_THAT(object.size(), Eq(0));
  Circle* newShape1 = new Circle(1, 1, 1);
  Circle* newShape2 = new Circle(1, 1, 2);
  object.push_back(newShape1);
  object.push_back(newShape2);
  EXPECT_THAT(object.size(), Eq(2));  // size before remove
  Shape* removedShape2 = object.pop_back();
  EXPECT_THAT(object.size(), Eq(1));  // size after remove
  delete removedShape1;
  delete removedShape2;
}

TEST(CanvasListModifying, remove_at) {
  CanvasList object;
  Circle* newShape1 = new Circle(1, 1, 1);
  Circle* newShape2 = new Circle(1, 1, 2);
  Circle* newShape3 = new Circle(1, 1, 3);
  Circle* newShape4 = new Circle(1, 1, 4);
  object.push_back(newShape1);
  object.push_back(newShape2);  // remove this one
  object.push_back(newShape3);
  object.push_back(newShape4);
  object.remove_at(1);
  EXPECT_THAT(object.size(), Eq(3));
  EXPECT_THAT(object.shape_at(0), Eq(newShape1));
  EXPECT_THAT(object.shape_at(1), Eq(newShape3));
  EXPECT_THAT(object.shape_at(2), Eq(newShape4));
  object.remove_at(0);
  object.remove_at(4);
}

TEST(CanvasListExtras, insert_after) {
  CanvasList object;
  Circle* newShape1 = new Circle(1, 1, 1);
  Circle* newShape2 = new Circle(1, 1, 2);
  Circle* newShape3 = new Circle(1, 1, 3);
  Circle* shapeToAdd = new Circle(1, 1, 4);
  object.push_back(newShape1);
  object.push_back(newShape2);
  object.push_back(newShape3);
  object.insert_after(0, shapeToAdd);
  EXPECT_THAT(object.size(), Eq(4));
  EXPECT_THAT(object.shape_at(0), Eq(newShape1));
  EXPECT_THAT(object.shape_at(1), Eq(shapeToAdd));
  EXPECT_THAT(object.shape_at(2), Eq(newShape2));
  EXPECT_THAT(object.shape_at(3), Eq(newShape3));
}

TEST(CanvasListExtras, remove_every_other) {
  CanvasList object;
  object.remove_every_other();
  Circle* newShape0 = new Circle(0, 0, 1);
  object.push_back(newShape0);
  object.remove_every_other();
  Circle* newShape1 = new Circle(1, 1, 1);
  Circle* newShape2 = new Circle(1, 1, 2);
  Circle* newShape3 = new Circle(1, 1, 3);
  Circle* newShape4 = new Circle(1, 1, 4);
  object.push_back(newShape1);
  object.push_back(newShape2);
  object.push_back(newShape3);
  object.push_back(newShape4);
  object.remove_every_other();
  EXPECT_THAT(object.size(), Eq(3));
}