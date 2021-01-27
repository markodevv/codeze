#include "autocomplete.h"
#include "types.h"
#include "my_string.h"
#include "container.h"

#define NUM_SUPPORTED_CHARS 95

void
trie_init(TrieNode* trie) {
	
	trie->endOfWord = false;
	trie->children = (TrieNode*)calloc(NUM_SUPPORTED_CHARS, sizeof(TrieNode));
}

void
trie_insert(TrieNode* root, String& word) {

	TrieNode* node = root;
	i32 index = 0;

	for (i32 i = 0; i < word.length; ++i) {

		index = word[i] - ' ';
		if (node->children[index].children == NULL)
			trie_init(&node->children[index]);
			
		node = &node->children[index];
	}
    node->endOfWord = true;
}


static b8
is_match(String& str, TrieNode* node, i32 index, b8 fullMatch) {

	if (!node->children)
		return false;

	if (index == str.length)
		return !fullMatch || node->endOfWord;

	return is_match(str, &node->children[str[index] - ' '], index + 1, fullMatch);
}

b8
trie_search(TrieNode* root, String& word, b8 fullMatch) {

	return is_match(word, root, 0, fullMatch);
}

static void
get_names_from_trie(TrieNode* node, Array<String>* arr, i32 level, char word[128], String& base) {
	
	if (node->endOfWord) {

		String str = str_create("");
		for (sizet i = 0; i < base.length; ++i) {
			str_push(&str, base[i]);
		}
		for (sizet i = 0; i < level; ++i) {
			str_push(&str, word[i]);
		}
		array_push(arr, str);
	}

	for (i32 i = 0; i < NUM_SUPPORTED_CHARS; ++i) {

		if (node->children[i].children) {

			word[level] = ' ' + i;
			get_names_from_trie(&node->children[i], arr, level + 1, word, base);
		}
	}
}

Array<String>
get_matching_names(TrieNode* root, String& word) {

	Array<String> names;

	if (word.length == 0) {
		
		array_init(&names, 10);
		char buffer[128];
		get_names_from_trie(root, &names, 0, buffer, word);
	}
	else if (is_match(word, root, 0, false)) {

		TrieNode* node = root;
		for (sizet i = 0; i < word.length; ++i) {

			node = &node->children[word[i] - ' '];
		}

		array_init(&names, 10);
		char buffer[128];
		get_names_from_trie(node, &names, 0, buffer, word);
	}

	return names;
}

