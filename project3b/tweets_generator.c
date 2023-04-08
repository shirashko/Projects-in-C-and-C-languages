#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "markov_chain.h"
#define NUM_OF_ARGS 5
#define MAX_LINE_LENGTH 1000
#define ALL_FILE 0
#define DECIMAL_BASE 10
#define MAX_TWEETS_WORDS 20
#define DELIMITERS " \r\n"
#define ARGUMENTS_ERROR_MESSAGE "Usage: the program need to get the follow \
arguments: seed, number of tweets that the program need to generate, \
file path, and number of words to read from the given file"
#define ALLOCATION_ERROR_MASSAGE "Allocation failure: Failed to allocate new \
memory\n"
#define FILE_ERROR_MESSAGE "Error: can't open file"

// assume data_ptr points to char
// print the word argument word_ptr points at
static void print_string(void* word_ptr)
{
  fprintf (stdout, " %s", (char *) word_ptr);
}

// assume word1, word2 point to char's
// compare the strings word1, word2 points at
// return 0 if strings equals, num>0 if *word1 > *word2 , num<0 otherwise
static int compare_strings(void* word1, void* word2)
{
  return strcmp ((char*) word1, (char*) word2);
}

// assume data_ptr points to char
// free the memory the string that data_ptr points at
static void free_string (void* data_ptr)
{
  free((char*)data_ptr);
}

// assume data_ptr points to char
// allocate memory for a new string and copy the string data_ptr points at
// to the new string, and return a pointer to it, as a void pointer
static void* copy_string (void* data_ptr)
{
  char* source = data_ptr;
  assert(source != NULL);
  char* dest = calloc (strlen (source)+1, sizeof(char));
  if (dest == NULL)
  {
    return NULL;
  }
  strcpy (dest, source);
  return dest;
}
// assume data_ptr points to char
// check if the word that data_ptr points at is a word(string) ends with a dot
// return true if it points to a dot word and false otherwise
static bool is_a_dot_word (void* data_ptr)
{
  char* word = data_ptr;
  int last_char = (int) (strlen (word)-1); //word is 100 chars top so int is ok
  if (word[last_char] == '.')
  {
    return true;
  }
  return false;
}

// if given file is invalid, print error message and return 1, 0 otherwise
static bool file_not_valid(FILE* text)
{
  if (text == NULL)
  {
    fprintf (stdout, FILE_ERROR_MESSAGE);
    return true;
  }
  return false;
}
// in case of a memory allocation faiure
static void memory_allocation_failed(MarkovChain **markov_chain, FILE* text)
{
  free_markov_chain (markov_chain);
  fprintf (stdout, ALLOCATION_ERROR_MASSAGE);
  fclose (text);
}

/**
 * get a file with words and number of words to be read, read it and fill the
 * data structure with this words
 * @param fp the file to read from
 * @param words_to_read number of words to be read from fp
 * @param markov_chain the data structure to fill with the words
 * return 0 in case of success, and 1 in case of allocation memory failed
 */
static int fill_database (FILE *fp, int words_to_read, MarkovChain
*markov_chain)
{
  char line[MAX_LINE_LENGTH];
  long counter = 0;
  bool reached_max_words = false;
  while (fgets (line, MAX_LINE_LENGTH,fp) != NULL && !reached_max_words) //as
    // long as we have something to read, run on the loop
  {
    if (line[0] == '\n' || line[0] == '\r')
    {
      continue;
    }
    char *word = strtok(line, DELIMITERS); //pointer to current word
    Node* current_node = add_to_database (markov_chain,word);
    if (current_node == NULL)
    {
      return EXIT_FAILURE;
    }
    counter++; //now there is one word that we read
    Node* the_node_before = current_node;
    word = strtok(NULL, DELIMITERS);
    if (words_to_read != 0 && counter > words_to_read)
    {
      reached_max_words = true;
    }
    while (word != NULL && !reached_max_words) // while there is still
      // words to read from cur tweet
    {
      current_node = add_to_database (markov_chain, word);
      if (current_node == NULL)
      {
        return EXIT_FAILURE;
      }
      if (add_node_to_counter_list (the_node_before->data,
                                    current_node->data, markov_chain) == false)
      {
        return EXIT_FAILURE;
      }
      counter++;
      if (words_to_read != 0 && counter == words_to_read)
      {
        return  EXIT_SUCCESS;
      }
      the_node_before = current_node;
      word = strtok (NULL, DELIMITERS); //read new word
    }
  }
  return EXIT_SUCCESS;
}

// assign values in *markov_chain fields
static void fill_markov_chain_fields1(MarkovChain *markov_chain)
{
  LinkedList *linked_list = calloc (1,sizeof (LinkedList));
  markov_chain->database = linked_list;
  markov_chain->print_func = &print_string;
  markov_chain->free_data = &free_string;
  markov_chain->is_last = &is_a_dot_word;
  markov_chain->comp_func = &compare_strings;
  markov_chain->copy_func = &copy_string;
}

/**
 *
 * @param argc num of arguments
 * @param argv 1) seed
 *             2) number of tweets to generate
 *             3) a name of a file to open and read from
 *             4) number of strings to read from the file
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
int main(int argc, char *argv[])
{
  int num_of_words;
  if (argc < NUM_OF_ARGS)
  {
    if (argc < (NUM_OF_ARGS-1) ) //less than 3 parameters is not allowed
    {
      fprintf (stdout, ARGUMENTS_ERROR_MESSAGE);
      return EXIT_FAILURE;
    }
    // else the program didn't get the represents num of words to read from
    // file, so we will read everything
    num_of_words = ALL_FILE;
  }
  else
  {
    num_of_words = (int) strtol (argv[4], NULL, DECIMAL_BASE);
  }
  FILE *text = fopen(argv[3], "r"); //the file to read words from
  if (file_not_valid (text))
  {
    return EXIT_FAILURE;
  }
  MarkovChain *ptr_markov_chain = calloc(1,sizeof(MarkovChain));
  if (ptr_markov_chain == NULL)
  {
    memory_allocation_failed (&ptr_markov_chain, text);
    return EXIT_FAILURE;
  }
  fill_markov_chain_fields1(ptr_markov_chain);
  int flag = fill_database(text, num_of_words, ptr_markov_chain);
  if (flag) //if func success - flag = 0 , else flag = 1
  {
    memory_allocation_failed (&ptr_markov_chain,text);
    return EXIT_FAILURE;
  }
  int tweets_number = (int)strtol (argv[2],NULL, DECIMAL_BASE);
  int seed = (int)strtol (argv[1],NULL, DECIMAL_BASE);
  srand(seed);
  for (int i=1; i<=tweets_number; i++)
  {
    fprintf (stdout, "Tweet %d:", i);
    MarkovNode *first_node = get_first_random_node (ptr_markov_chain);
    generate_random_sequence (ptr_markov_chain, first_node, MAX_TWEETS_WORDS);
  }
  fclose (text);
  free_markov_chain (&ptr_markov_chain);
  return EXIT_SUCCESS;
}

