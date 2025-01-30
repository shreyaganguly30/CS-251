#include "include/shape.h"

#include <sstream>
#include <string>

using namespace std;

// Shape functions
Shape::Shape() {
  this->x = 0;
  this->y = 0;
}

Shape::Shape(int x, int y) {
  this->x = x;
  this->y = y;
}

Shape::~Shape() {
  return;
}

Shape* Shape::copy() {
  return new Shape(this->x, this->y);
}

int Shape::getX() const {
  return this->x;
}

int Shape::getY() const {
  return this->y;
}

void Shape::setX(int x) {
  this->x = x;
}

void Shape::setY(int y) {
  this->y = y;
}

string Shape::as_string() const {
  ostringstream outSS;
  outSS << "It's a Shape at x: " << this->x << ", y: " << this->y;
  return outSS.str();
}

// Circle functions
Circle::Circle() {
  this->radius = 0;
}

Circle::Circle(int r) {
  this->radius = r;
}

Circle::Circle(int x, int y, int r) {
  this->x = x;
  this->y = y;
  this->radius = r;
}

Circle::~Circle() {
  return;
}

Circle* Circle::copy() {
  return new Circle(this->x, this->y, this->radius);
}

int Circle::getRadius() const {
  return this->radius;
}

void Circle::setRadius(int r) {
  this->radius = r;
}

string Circle::as_string() const {
  ostringstream outSS;
  outSS << "It's a Circle at x: " << this->x << ", y: " << this->y
        << ", radius: " << this->radius;
  return outSS.str();
}

// Rectangle functions
Rect::Rect() {
  this->x = 0;
  this->y = 0;
  this->width = 0;
  this->height = 0;
}
Rect::Rect(int w, int h) {
  this->width = w;
  this->height = h;
}
Rect::Rect(int x, int y, int w, int h) {
  this->x = x;
  this->y = y;
  this->width = w;
  this->height = h;
}

Rect::~Rect() {
  return;
}

Rect* Rect::copy() {
  return new Rect(this->x, this->y, this->width, this->height);
}

int Rect::getWidth() const {
  return this->width;
}

int Rect::getHeight() const {
  return this->height;
}

void Rect::setWidth(int w) {
  this->width = w;
}

void Rect::setHeight(int h) {
  this->height = h;
}

string Rect::as_string() const {
  ostringstream outSS;
  outSS << "It's a Rectangle at x: " << this->x << ", y: " << this->y
        << " with width: " << this->width << " and height: " << this->height;
  return outSS.str();
}

// Right Triangle Functions
RightTriangle::RightTriangle() {
  this->base = 0;
  this->height = 0;
}

RightTriangle::RightTriangle(int b, int h) {
  this->base = b;
  this->height = h;
}

RightTriangle::RightTriangle(int x, int y, int b, int h) {
  this->x = x;
  this->y = y;
  this->base = b;
  this->height = h;
}

RightTriangle::~RightTriangle() {
  return;
}

RightTriangle* RightTriangle::copy() {
  return new RightTriangle(this->x, this->y, this->base, this->height);
}

int RightTriangle::getBase() const {
  return this->base;
}

int RightTriangle::getHeight() const {
  return this->height;
}

void RightTriangle::setBase(int b) {
  this->base = b;
}

void RightTriangle::setHeight(int h) {
  this->height = h;
}

string RightTriangle::as_string() const {
  ostringstream outSS;
  outSS << "It's a Right Triangle at x: " << this->x << ", y: " << this->y
        << " with base: " << this->base << " and height: " << this->height;
  return outSS.str();
}