#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#define WORD_LENGTH 17 //No word is more than 16 letters long
#define NUM_OF_WORDS 2887
#define PASSPHRASE_LENGTH 6

char (*nouns)[WORD_LENGTH];
char (*verbs)[WORD_LENGTH];
char (*adjectives)[WORD_LENGTH];
char (*conjunctions)[WORD_LENGTH];

int getNum(int limit) {
	int ret = rand() % limit;
	while(ret > RAND_MAX - (RAND_MAX % limit)) { //Get rid of bias
		ret = rand() % limit;
	}
	return ret;
}

int main(int argc, char** argv) {
	srand(1612);
	char (*strings)[WORD_LENGTH] = malloc(NUM_OF_WORDS*sizeof(char[WORD_LENGTH]));
	char (*word_classes)[6] = malloc(NUM_OF_WORDS*sizeof(char[6]));

	char word[WORD_LENGTH];
	char word_class[6];

	FILE* words = fopen("merged_list.txt", "r");

	int i = 0;
	int noun_count = 0, verb_count = 0, adj_count = 0, conj_count = 0;
	while(fscanf(words, "%s ", word) == 1) {
		strcpy(strings[i], word);
		fscanf(words, "%s\n", word_class); //Skip word class
		strcpy(word_classes[i++], word_class);
		if(strchr(word_class, 'N')) {
			noun_count++;
		}
		if(strchr(word_class, 'V')) {
			verb_count++;
		}
		if(strchr(word_class, 'A')) {
			adj_count++;
		}
		if(strchr(word_class, 'C')) {
			conj_count++;
		}
	}

	nouns = malloc(noun_count*sizeof(char[WORD_LENGTH]));
	verbs = malloc(verb_count*sizeof(char[WORD_LENGTH]));
	adjectives = malloc(adj_count*sizeof(char[WORD_LENGTH]));
	conjunctions = malloc(conj_count*sizeof(char[WORD_LENGTH]));

	i = 0;
	int noun_index = 0;
	int verb_index = 0;
	int adj_index = 0;
	int conj_index = 0;
	while(i < NUM_OF_WORDS) {
		if(strchr(word_classes[i], 'N')) {
			strcpy(nouns[noun_index], strings[i]);
			noun_index++;
		}
		if(strchr(word_classes[i], 'V')) {
			strcpy(verbs[verb_index], strings[i]);
			verb_index++;
		}
		if(strchr(word_classes[i], 'A')) {
			strcpy(adjectives[adj_index], strings[i]);
			adj_index++;
		}
		if(strchr(word_classes[i], 'C')) {
			strcpy(conjunctions[conj_index], strings[i]);
			conj_index++;
		}
		i++;
	}

	double dict_operations = 0.0;
	double brute_operations = 0.0;
	int round = 0, rounds = 1;
	double digit_place_value[6];
	digit_place_value[0] = 1;
	digit_place_value[1] = digit_place_value[0] * adj_count;
	digit_place_value[2] = digit_place_value[1] * noun_count;
	digit_place_value[3] = digit_place_value[2] * verb_count;
	digit_place_value[4] = digit_place_value[3] * conj_count;
	digit_place_value[5] = digit_place_value[4] * noun_count;
	while(round < rounds) {
		int indecies[] = {getNum(adj_count), getNum(noun_count), getNum(verb_count),
						getNum(conj_count), getNum(noun_count), getNum(verb_count)};

		char** words_in_sentence = malloc(PASSPHRASE_LENGTH*sizeof(char[WORD_LENGTH]));
		words_in_sentence[0] = adjectives[indecies[0]];
		words_in_sentence[1] = nouns[indecies[1]];
		words_in_sentence[2] = verbs[indecies[2]];
		words_in_sentence[3] = conjunctions[indecies[3]];
		words_in_sentence[4] = nouns[indecies[4]];
		words_in_sentence[5] = verbs[indecies[5]];

		int sentence_length = 0;
		i = 0;
		while(i < 6) {
			dict_operations += digit_place_value[i] * indecies[i];
			int character_index = 0;
			while(character_index < strlen(words_in_sentence[i])) {
				char chr = words_in_sentence[i][character_index];
				int chr_number = (int)chr - (int)'a' + 1;
				brute_operations += chr_number * pow(26, sentence_length);
				character_index++;
				sentence_length++;
			}
			i++;
		}
		round++;
		free(words_in_sentence);
	}
	free(nouns);
	free(verbs);
	free(adjectives);
	free(conjunctions);
	free(word_classes);
	free(strings);

	dict_operations /= rounds;
	brute_operations /= rounds;

	printf("Dictionary Attack: %.0f operations\n", dict_operations);
	printf("Brute Force Attack: %.0f operations\n", brute_operations);
}