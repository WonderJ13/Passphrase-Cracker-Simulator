#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <math.h>

#define WORD_LENGTH 17 //No word is more than 16 letters long
#define NUM_OF_WORDS 2887
#define PASSPHRASE_LENGTH 6

int main(int argc, char** argv) {
	srand(1612);
	char (*strings)[WORD_LENGTH] = malloc(NUM_OF_WORDS*sizeof(char[WORD_LENGTH]));
	char word[WORD_LENGTH];
	FILE* words = fopen("merged_list.txt", "r");

	int i = 0;
	while(fscanf(words, "%s ", word) == 1) {
		strcpy(strings[i++], word);
		fscanf(words, "%s\n", word); //Skip word class
	}

	double dict_operations = 0.0;
	double brute_operations = 0.0;
	int round = 0, rounds = 100000;
	while(round < rounds) {
		int word_length = 0;
		int passphrase_length = 0;
		while(word_length < PASSPHRASE_LENGTH) {
			int index = rand() % NUM_OF_WORDS;
			while(index > RAND_MAX - (RAND_MAX % NUM_OF_WORDS)) { //Needed for less bias
				index = rand() % NUM_OF_WORDS;
			}
			dict_operations += index * pow(NUM_OF_WORDS, word_length);
			i = 0;
			while(i < strlen(strings[index])) {
				char chr = strings[index][i];
				int chr_number = (int)chr - (int)'a' + 1;
				brute_operations += chr_number * pow(26, passphrase_length);
				i++;
				passphrase_length++;
			}
			//printf("%s ", strings[index]);
			word_length++;
		}
		round++;
	}
	dict_operations /= rounds;
	brute_operations /= rounds;
	printf("Dictionary Attack: %.0f operations\n", dict_operations);
	printf("Brute Force Attack: %.0f operations\n", brute_operations);
	fclose(words);
	free(strings);
	return 1;
}