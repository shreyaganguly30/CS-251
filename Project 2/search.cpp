#include "include/search.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

string cleanToken(const string& token) {
  string cleanedToken = "";
  bool thereAreLetters = false;
  bool noLeadPunct = false;
  bool noEndPunct = false;
  int start = 0;
  int end = token.length() - 1;
  // check if theres letters in the string
  for (size_t i = 0; i < token.length(); i++) {
    if (isalpha(token.at(i))) {
      thereAreLetters = true;
    }
  }
  // if there's no letters, return empty string
  if (!thereAreLetters) {
    return cleanedToken;
  }
  // find first index with letters
  while (!noLeadPunct) {
    if (ispunct(token.at(start))) {
      start++;
    } else {
      noLeadPunct = true;
    }
  }
  // find last index with letters
  while (!noEndPunct) {
    if (ispunct(token.at(end))) {
      end--;
    } else {
      noEndPunct = true;
    }
  }
  // between the first and last index with letters,
  // concatenate the chars turning them lowercase
  for (int i = start; i <= end; i++) {
    cleanedToken += tolower(token.at(i));
  }
  return cleanedToken;
}

set<string> gatherTokens(const string& text) {
  set<string> tokenSet;
  // use istringstream to read string
  istringstream iss(text);
  string word, cleanedWord;
  // while iss can extract another word
  while (iss >> word) {
    cleanedWord = cleanToken(word);
    if (cleanedWord != "") {  // if it's not an empty string, add it to the set
      tokenSet.insert(cleanedWord);
    }
  }
  return tokenSet;
}

int buildIndex(const string& filename, map<string, set<string>>& index) {
  int numURL = 0;
  ifstream inFS;
  inFS.open(filename);
  string url;
  string line;
  set<string> wordsFromString;
  set<string> URLS;
  // while you can get another url and line of text
  if (inFS.is_open()) {
    while (getline(inFS, url) && getline(inFS, line)) {
      numURL++;  // increase the URL count
      wordsFromString = gatherTokens(line);
      for (string current : wordsFromString) {
        index[current];            // creates mapping
        URLS = index.at(current);  // accesses URL set at current word
        URLS.insert(url);          // inserts new URL into set
        index[current] = URLS;     // creates mapping between word and set
      }
    }
    inFS.close();
    return numURL;
  } else {
    return 0;
  }
}

set<string> findQueryMatches(const map<string, set<string>>& index,
                             const string& sentence) {
  set<string> websitesWithWords;
  istringstream iss(sentence);
  string word;
  // check first word
  if (iss >> word) {
    word = cleanToken(word);
    if (index.count(word)) {  // if word is a key
      websitesWithWords = index.at(word);
    }
  }
  // while there are more words, check for modifier
  while (iss >> word) {
    char modifier = word[0];
    word = cleanToken(word);
    // if prepended with a +, only keep shared wesbites
    if (index.count(word) == 1 && modifier == '+') {
      set<string> inBoth;
      for (string current : index.at(word)) {
        if (websitesWithWords.find(current) != websitesWithWords.end()) {
          inBoth.insert(current);
        }
      }
      websitesWithWords = inBoth;
    }
    // if prepended with + and is not a key, reset set to empty
    else if (index.count(word) == 0 && modifier == '+') {
      websitesWithWords = {};
    }
    // if prepended with a -, remove from set
    else if (index.count(word) == 1 && modifier == '-') {
      for (string current : index.at(word)) {
        if (websitesWithWords.find(current) != websitesWithWords.end()) {
          websitesWithWords.erase(current);
        }
      }
    }
    // if no prepend, then insert into set
    else if (index.count(word) == 1) {
      for (string current : index.at(word)) {
        websitesWithWords.insert(current);
      }
    }
  }
  return websitesWithWords;
}

void searchEngine(const string& filename) {
  map<string, set<string>> search;
  set<string> matchingPages;
  int numPages = buildIndex(filename, search);
  string query;
  ifstream inFS;
  inFS.open(filename);
  // if file is not open, print invalid file
  if (!inFS.is_open()) {
    cout << "Invalid filename." << endl;
  }
  // print num websites and num words in file
  cout << "Stand by while building index..." << endl;
  cout << "Indexed " << numPages << " pages containing " << search.size()
       << " unique terms" << endl;
  cout << endl;
  // prompt user for queries
  while (true) {
    cout << "Enter query sentence (press enter to quit): " << endl;
    getline(cin, query);
    // if query is empty, break
    if (query == "") {
      break;
    }
    // print query info
    matchingPages = findQueryMatches(search, query);
    cout << "Found " << matchingPages.size() << " matching pages" << endl;
    for (string website : matchingPages) {
      cout << website << endl;
    }
    cout << endl;
  }
  inFS.close();
  cout << "Thank you for searching!";
}
