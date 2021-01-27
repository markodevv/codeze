#pragma once
#include "types.h"
#include "container.h"

struct TrieNode {
	
	TrieNode* children;
	b8 endOfWord;
};

struct String;
void trie_init(TrieNode* trie);
void trie_insert(TrieNode* root, String& word);
b8 trie_search(TrieNode* root, String& word, b8 fullMatch);
Array<String> get_matching_names(TrieNode* root, String& word);
