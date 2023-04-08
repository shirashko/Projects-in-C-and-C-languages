#include "markov_chain.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>

/**
 * Get random number between 0 and max_number [0,max_number).
 * @param max_number maximal number to return (not including)
 * @return random number
 */
int get_random_number(int max_number)
{
  assert(max_number != 0);
  return rand() % max_number;
}

// Get one random node from the given markov_chain's database, and return
// it's markov_node attribute (that contains the word it's point to and a
// pointer to a dynamic array of all consecutive words)
MarkovNode* get_first_random_node(MarkovChain *markov_chain)
{
  while (true)
  {
    int number = get_random_number(markov_chain->database->size);
    Node *current_node = markov_chain->database->first;
    // getting the random node that have been randomly chosen
    while (number > 0)
    {
      current_node = current_node->next;
      number--;
    }
    // now current node is the chosen node
    if ( ! markov_chain->is_last(current_node->data->data) ) // is_last return
                                                   // true if the item is last
    {
      return current_node->data;
    }
  }
}

// Choose randomly the next state, depend on it's occurrence frequency.
// See full documentation in header file
MarkovNode* get_next_random_node(MarkovNode *state_struct_ptr)
{
  assert(state_struct_ptr != NULL);
  int size = state_struct_ptr->sum_of_frequencies;
  int number = get_random_number (size); //can be zero
  int sum = 0;
  MarkovNode * chosen;
  NextNodeCounter current;
  for (int i=0; i<size; i++) // getting the chosen node
  {
    current = (state_struct_ptr->counter_list)[i];
    if(number < sum + current.frequency)
    {
      chosen = current.markov_node;
      return chosen;
    }
    sum = sum + current.frequency;
  }
  return NULL;
}

// Receive markov_chain, generate and print random sentence out of it. The
// sentence have at least 2 words in it.
// See full documentation in header file
void generate_random_sequence(MarkovChain *markov_chain, MarkovNode *
first_node, int max_length)
{
  if (first_node == NULL)
  {
    first_node = get_first_random_node (markov_chain);
  }
  MarkovNode *current_node = first_node;
  int counter = 0;
  while (!(markov_chain->is_last) (current_node->data) && counter < max_length)
  {
    markov_chain->print_func(current_node->data);
    current_node = get_next_random_node(current_node);
    counter++;
  }
  if ((markov_chain->is_last) (current_node->data))
  {
    markov_chain->print_func(current_node->data);
  }
  fprintf (stdout,"\n");
}

// Free markov_chain and all of it's content from memory
// See full documentation in header file
void free_markov_chain(MarkovChain ** ptr_chain)
{
  if ( (*ptr_chain) == NULL)
  {
    return;
  }
  if ( (*ptr_chain)->database == NULL)
  {
    free (*ptr_chain);
    *ptr_chain = NULL;
  }
  Node *current_node = (*ptr_chain)->database->first;
  Node *next_node;
  while (current_node != NULL)
  {
    if (current_node->data != NULL)
    {
      if (current_node->data->data != NULL)
      {
        (*ptr_chain)->free_data(current_node->data->data); //free the item
        // itself, the data that can point to anything (generic)
      }
      if (current_node->data->counter_list != NULL)
      {
        free(current_node->data->counter_list);
      }
      free (current_node->data);
    }
    next_node = current_node->next;
    free (current_node);
    current_node = next_node;
  }
  free((*ptr_chain)->database);
  free(*ptr_chain);
  *ptr_chain = NULL;
}

// check if word is in node->counter_list, meaning if the word already
// appeared as a consecutive word of node.
// if it did, return its place in the node->counter_list dynamic array
// if it didn't, return -1
long is_in_counter_list(MarkovChain* markov_chain, MarkovNode *node, char*
word)
{
  if (node->size != 0) //else it's definitly not in counter_list yet
  {
    assert(node->counter_list != NULL);//size != 0-not suppose to be NULL
    for (long i = 0; i <= (node->size - 1); i++)
    {
      assert((node->counter_list[i]).markov_node != NULL);
      char *cur_word = (node->counter_list[i]).markov_node->data;
      if (!markov_chain->comp_func(word, cur_word)) //if equals, comp_func
      // return 0 and then ! true, meaning we found a match -
        // consecutive item is already in counter_list
      {
        return i;
      }
    }
  }
  return -1;
}

// Add the second markov_node to the counter_list of the first markov_node.
// If already in counter_list, update it's counter value.
// return true for success, false if a memory allocation failed
// See full documentation in header file
bool add_node_to_counter_list(MarkovNode *first_node, MarkovNode
*second_node, MarkovChain *markov_chain)
{
  if (markov_chain->is_last(first_node->data)) //if first_node is_last there
    // shouldn't be a counter_list for it, and second_node doesn't need to
    // be added to anywhere
  {
    return true;
  }
  //check if consecutive word is already in counter_list
  long i = is_in_counter_list(markov_chain, first_node, second_node->data);
  if (i != -1) // second_node->data is in counter_list
  {
    (first_node->counter_list[i]).frequency++;
    first_node->sum_of_frequencies++;
    return true;
  }
  // if got here, then second_node->data is not in counter_list, meaning we
  // need to add it
  if (first_node->size != 0) //need to allocate new space
  {
    first_node->counter_list = realloc (first_node->counter_list,
                                        sizeof (NextNodeCounter)
                                        * (first_node->size+1));
    if (first_node->counter_list == NULL) // memory allocation failed
    {
      return false;
    }
  }
  // indexes in range [0,new_size-1] so first use previous size then enlarge
  // it by one
  first_node->counter_list[first_node->size].markov_node = second_node;
  first_node->counter_list[first_node->size].frequency = 1;
  first_node->sum_of_frequencies++;
  first_node->size++; //added new word to counter_list
  return true;
}


// Check if data_ptr is in database. If so, return the Node wrapping it in
// the markov_chain, otherwise return NULL.
// See full documentation in header file
Node* get_node_from_database(MarkovChain *markov_chain, void *data_ptr)
{
  assert(markov_chain->database != NULL);
  Node *current = markov_chain->database->first;
  while (current != NULL)
  {
    assert(current->data != NULL);
    void *cur_item = current->data->data;
    if (!markov_chain->comp_func(data_ptr, cur_item)) //if items are equals
    {
      return current;
    }
    current = current->next;
  }
  // if we got here, data_ptr is not in data structure
  return NULL;
}

// create markov_node that points to data_ptr char, and returns its node. if
// memory allocation failed- return NULL.
MarkovNode* create_markov_node(void *data_ptr)
{
  MarkovNode *markov_node = calloc (1,sizeof (MarkovNode));
  if (markov_node == NULL)
  {
    return NULL;
  }
  markov_node->counter_list = calloc (1,sizeof (NextNodeCounter));
  if (markov_node->counter_list == NULL)
  {
    return NULL;
  }
  markov_node->size = 0; //no consecutive words in counter_list yet
  markov_node->sum_of_frequencies = 0; //no consecutive words in
  // counter_list yet
  markov_node->data = data_ptr;
  return markov_node;
}

// check if data_ptr is in markov_chain
// if data_ptr is in markov_chain return its node. Otherwise, create new
// markov_node, add to end of markov_chain's database and return its node.
Node* add_to_database(MarkovChain *markov_chain, void *data_ptr)
{
  //check if data_ptr is already in the database
  Node *node = get_node_from_database (markov_chain, data_ptr);
  if (node == NULL) // node isn't in the database, and we need to add it
  {
    void* item = markov_chain->copy_func(data_ptr);
    if (item == NULL) // memory allocation failed
    {
      return NULL;
    }
    MarkovNode *markov_node = create_markov_node (item);//create
    // MarkovNode to put as the attribute of the new Node
    if (add(markov_chain->database, markov_node)) //malloc in add failed
    {
      return NULL;
    }
    // now database.last is new node with data->data == data_ptr
    return markov_chain->database->last;
  }
  return node; //in case data_ptr was in the database already
}




