#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "include/search.h"

using namespace std;
using namespace testing;

map<string, set<string>> TINY_MAP = {
    {"eggs", {"www.shoppinglist.com"}},
    {"milk", {"www.shoppinglist.com"}},
    {"fish", {"www.shoppinglist.com", "www.dr.seuss.net"}},
    {"bread", {"www.shoppinglist.com"}},
    {"cheese", {"www.shoppinglist.com"}},
    {"red", {"www.rainbow.org", "www.dr.seuss.net"}},
    {"gre-en", {"www.rainbow.org"}},
    {"orange", {"www.rainbow.org"}},
    {"yellow", {"www.rainbow.org"}},
    {"blue", {"www.rainbow.org", "www.dr.seuss.net"}},
    {"indigo", {"www.rainbow.org"}},
    {"violet", {"www.rainbow.org"}},
    {"one", {"www.dr.seuss.net"}},
    {"two", {"www.dr.seuss.net"}},
    {"i'm", {"www.bigbadwolf.com"}},
    {"not", {"www.bigbadwolf.com"}},
    {"trying", {"www.bigbadwolf.com"}},
    {"to", {"www.bigbadwolf.com"}},
    {"eat", {"www.bigbadwolf.com"}},
    {"you", {"www.bigbadwolf.com"}}};

// CleanToken Tests
TEST(CleanToken, CleanToken_Test1) {
  string text = "hello";
  string expect = "hello";
  EXPECT_THAT(cleanToken(text), StrEq(expect));
}
TEST(CleanToken, CleanToken_Test2) {
  string text = "...hello...";
  string expect = "hello";
  EXPECT_THAT(cleanToken(text), StrEq(expect));
}
TEST(CleanToken, CleanToken_Test3) {
  string text = "hello!";
  string expect = "hello";
  EXPECT_THAT(cleanToken(text), StrEq(expect));
}
TEST(CleanToken, CleanToken_Test4) {
  string text = "?hello";
  string expect = "hello";
  EXPECT_THAT(cleanToken(text), StrEq(expect));
}
TEST(CleanToken, CleanToken_Test5) {
  string text = "FlOwEr";
  string expect = "flower";
  EXPECT_THAT(cleanToken(text), StrEq(expect));
}
TEST(CleanToken, CleanToken_Test6) {
  string text = "._.SWAN_FLEW???";
  string expect = "swan_flew";
  EXPECT_THAT(cleanToken(text), StrEq(expect));
}
TEST(CleanToken, CleanToken_Test7) {
  string text = ".p.i.n.c.h.";
  string expect = "p.i.n.c.h";
  EXPECT_THAT(cleanToken(text), StrEq(expect));
}
TEST(CleanToken, CleanToken_Test8) {
  string text = "111";
  string expect = "";
  EXPECT_THAT(cleanToken(text), StrEq(expect));
}
TEST(CleanToken, CleanToken_Test9) {
  string text = "x";
  string expect = "x";
  EXPECT_THAT(cleanToken(text), StrEq(expect));
}
TEST(CleanToken, CleanToken_Test10) {
  string text = ".....a.....";
  string expect = "a";
  EXPECT_THAT(cleanToken(text), StrEq(expect));
}
TEST(CleanToken, CleanToken_Test11) {
  string text = "\"hi\"";
  string expect = "hi";
  EXPECT_THAT(cleanToken(text), StrEq(expect));
}
TEST(CleanToken, CleanToken_Test12) {
  string text = "mo.on";
  string expect = "mo.on";
  EXPECT_THAT(cleanToken(text), StrEq(expect));
}
TEST(CleanToken, CleanToken_Test13) {
  string text = ".mo.on";
  string expect = "mo.on";
  EXPECT_THAT(cleanToken(text), StrEq(expect));
}
TEST(CleanToken, CleanToken_Test14) {
  string text = "mo.on.";
  string expect = "mo.on";
  EXPECT_THAT(cleanToken(text), StrEq(expect));
}

// GatherTokens tests
TEST(GatherTokens, GatherTokens_Test1) {
  string text = "Hello there! How are you?";
  set<string> expect = {"hello", "there", "how", "are", "you"};
  EXPECT_THAT(gatherTokens(text), ContainerEq(expect));
}
TEST(GatherTokens, GatherTokens_Test2) {
  string text = "+ - * /";
  set<string> expect = {};
  EXPECT_THAT(gatherTokens(text), ContainerEq(expect));
}
TEST(GatherTokens, GatherTokens_Test3) {
  string text = "HELLO!!! FOOD is ~~YummY :)";
  set<string> expect = {"hello", "food", "is", "yummy"};
  EXPECT_THAT(gatherTokens(text), ContainerEq(expect));
}
TEST(GatherTokens, GatherTokens_Test4) {
  string text = "??? What's your name??";
  set<string> expect = {"what's", "your", "name"};
  EXPECT_THAT(gatherTokens(text), ContainerEq(expect));
}
TEST(GatherTokens, GatherTokens_Test5) {
  string text = "   hi   bye   hi   again   ";
  set<string> expect = {"hi", "bye", "hi", "again"};
  EXPECT_THAT(gatherTokens(text), ContainerEq(expect));
}
TEST(GatherTokens, GatherTokens_Test6) {
  string text = "hi HELLOOOOO     ";
  set<string> expect = {"hi", "hellooooo"};
  EXPECT_THAT(gatherTokens(text), ContainerEq(expect));
}
TEST(GatherTokens, GatherTokens_Test7) {
  string text = "   GoOdByEeE";
  set<string> expect = {"goodbyeee"};
  EXPECT_THAT(gatherTokens(text), ContainerEq(expect));
}

// BuildIndex Tests
TEST(BuildIndex, BuildIndex_Test1) {
  string filename = "data/tiny.txt";
  map<string, set<string>> expectedMap = TINY_MAP;
  map<string, set<string>> inputMap;
  int numWebsites = buildIndex(filename, inputMap);
  EXPECT_THAT(inputMap, ContainerEq(expectedMap));
  EXPECT_THAT(numWebsites, Eq(4));
}
TEST(BuildIndex, BuildIndex_Test2) {
  string filename = "data/invalidFile.txt";  // file that does not exist
  map<string, set<string>> expectedMap;      // expect blank map
  map<string, set<string>> inputMap;         // input blank map
  int numWebsites = buildIndex(filename, inputMap);
  EXPECT_THAT(inputMap, ContainerEq(expectedMap));
  EXPECT_THAT(numWebsites, Eq(0));
}

// FindQueryMatches Tests
TEST(FindQueryMatches, FindQueryMatches_Test1) {
  map<string, set<string>> mapToRead = TINY_MAP;
  string sentence = "bread";
  set<string> expectedSet = {"www.shoppinglist.com"};
  EXPECT_THAT(findQueryMatches(mapToRead, sentence), expectedSet);
}
TEST(FindQueryMatches, FindQueryMatches_Test2) {
  map<string, set<string>> mapToRead = TINY_MAP;
  string sentence = "eggs fish";
  set<string> expectedSet = {"www.shoppinglist.com", "www.dr.seuss.net"};
  EXPECT_THAT(findQueryMatches(mapToRead, sentence), expectedSet);
}
TEST(FindQueryMatches, FindQueryMatches_Test3) {
  map<string, set<string>> mapToRead = TINY_MAP;
  string sentence = "eggs +fish";
  set<string> expectedSet = {"www.shoppinglist.com"};
  EXPECT_THAT(findQueryMatches(mapToRead, sentence), expectedSet);
}
TEST(FindQueryMatches, FindQueryMatches_Test4) {
  map<string, set<string>> mapToRead = TINY_MAP;
  string sentence = "blue -indigo";
  set<string> expectedSet = {"www.dr.seuss.net"};
  EXPECT_THAT(findQueryMatches(mapToRead, sentence), expectedSet);
}
TEST(FindQueryMatches, FindQueryMatches_Test5) {
  map<string, set<string>> mapToRead = TINY_MAP;
  string sentence = "eggs orange one eat -red blue +yellow";
  set<string> expectedSet = {"www.rainbow.org"};
  EXPECT_THAT(findQueryMatches(mapToRead, sentence), expectedSet);
}
TEST(FindQueryMatches, FindQueryMatches_Test6) {
  map<string, set<string>> mapToRead = TINY_MAP;
  string sentence = "blubber";
  set<string> expectedSet = {};
  EXPECT_THAT(findQueryMatches(mapToRead, sentence), expectedSet);
}
TEST(FindQueryMatches, FindQueryMatches_Test7) {
  map<string, set<string>> mapToRead = TINY_MAP;
  string sentence = "bread blubber";
  set<string> expectedSet = {"www.shoppinglist.com"};
  EXPECT_THAT(findQueryMatches(mapToRead, sentence), expectedSet);
}
TEST(FindQueryMatches, FindQueryMatches_Test8) {
  map<string, set<string>> mapToRead = TINY_MAP;
  string sentence = "bread +random";
  set<string> expectedSet = {};
  EXPECT_THAT(findQueryMatches(mapToRead, sentence), expectedSet);
}
TEST(FindQueryMatches, FindQueryMatches_Test9) {
  map<string, set<string>> mapToRead = TINY_MAP;
  string sentence = "bread -okie";
  set<string> expectedSet = {"www.shoppinglist.com"};
  EXPECT_THAT(findQueryMatches(mapToRead, sentence), expectedSet);
}