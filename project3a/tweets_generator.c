#define NUM_OF_ARGS 5
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define TRUE 1
#define FALSE 0
#include "markov_chain.h"
#define MAX_LINE_LENGTH 1000
#define ALL_FILE 0
#define DECIMAL_BASE 10
#define MAX_TWEETS_WORDS 20
#define DELIMITERS " \r\n"


// if given file is invalid, print error message and return 1, 0 otherwise
int file_not_valid(FILE* text)
{
  if (text == NULL)
  {
    fprintf (stdout, "Error: can't open file");
    return TRUE;
  }
  return FALSE;
}
// in case a memory allocation fails
void memory_allocation_failed(MarkovChain **markov_chain, FILE* text)
{
    free_markov_chain (markov_chain);
    fprintf (stdout, "Allocation failure: couldn't allocate the space the "
                     "program requires");
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
int fill_database (FILE *fp, int words_to_read, MarkovChain *markov_chain)
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
                                    current_node->data) == false)
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

void output_database (LinkedList *database)
{
  FILE *output = fopen ("./output.txt", "w");
  if (output == NULL)
  {
    fprintf (stdout, "FILE_OPEN_ERROR");
    fclose (output);
    return;
  }
  Node *head = database->first;
  while (head != NULL)
  {
    fprintf (output, "%s -> \n", head->data->data);
    for (int i = 0; i < head->data->size; ++i)
    {
      fprintf (output, "\tWord %s : Freq %d\n",
               head->data->counter_list[i].markov_node->data,
               head->data->counter_list[i].frequency);
    }
    fprintf (output, "\n");
    head = head->next;
  }
  fclose (output);
}

/**
 *
 * @param argc
 * @param argv
 * @return
 */
int main(int argc, char *argv[])
{
  int num_of_words;
  if (argc < NUM_OF_ARGS)
  {
    if (argc < (NUM_OF_ARGS-1) ) //less than 3 parameters is not allowed
    {
      fprintf (stdout, "Usage: need to get %d: seed, number of tweets that "
                       "the program need to generate, file path, and number "
                       "of words to read from the file", NUM_OF_ARGS);
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
  LinkedList *linked_list = calloc (1,sizeof (LinkedList)); // no need ?
  ptr_markov_chain->database = linked_list;
  int flag = fill_database(text, num_of_words, ptr_markov_chain);
  //print_data_structure(ptr_markov_chain);
  output_database(ptr_markov_chain->database);
  if (flag) //if func success - flag = 0 , else flag = 1
  {
    memory_allocation_failed (&ptr_markov_chain,text);
    return EXIT_FAILURE;
  }
  int tweets_number = (int)strtol (argv[2],NULL, DECIMAL_BASE);
  int seed = (int)strtol (argv[1],NULL, DECIMAL_BASE);
  srand(seed); // make sure it's what I suppose to do with seed
  for (int i=1; i<=tweets_number; i++)
  {
    fprintf (stdout, "Tweet %d: ", i);
    MarkovNode *first_node = get_first_random_node (ptr_markov_chain);
    generate_random_sequence (ptr_markov_chain, first_node, MAX_TWEETS_WORDS);
  }
  fclose (text);
  free_markov_chain (&ptr_markov_chain);
  return EXIT_SUCCESS;
}
