#ifndef MARKOV_CHAIN_H
#define MARKOV_CHAIN_H

#include "linked_list.h"
#include <stdio.h>  // For printf(), sscanf()
#include <stdlib.h> // For exit(), malloc()
#include <stdbool.h> // for bool

#define ALLOCATION_ERROR_MASSAGE "Allocation failure: Failed to allocate new \
memory\n"

// pointers to functions
typedef void (*print_func1) (void* data_ptr);
typedef int (*compare_func1) (void* data_ptr1, void* data_ptr2);
typedef void (*free_data1) (void* data_ptr);
typedef  void* (*copy_func1) (void* data_ptr);
typedef bool (*is_last1) (void* data_ptr);
typedef struct MarkovNode MarkovNode;

/**
 * struct represents an item in counter_list dynamic array of MarkovNode
 * that represent a data (from MarkovNode fields) consecutive item and the
 * number of appearances (frequency) the item comes after data
 */
typedef struct NextNodeCounter
{
    MarkovNode* markov_node;
    int frequency;
} NextNodeCounter;

/**
 * struct represents the data in the linked_list, when the data include a
 * pointer to an item, and a pointer (counter_list) to a dynamically memory
 * allocated array which contain the consecutive items coming after the
 * item
 */
struct MarkovNode
{
    void* data; //pointer to anything
    NextNodeCounter *counter_list; // points to a dynamic array of
    // NextNodeCounter objects, containing all the possible words coming
    // after this word (data) in the text and it's frequencies
    int size; //the number of NextNodeCounter items in the dynamically
    // allocated array counter_list
    int sum_of_frequencies; //the number of size*(the frequencies of every
    // item)
};

/**
 * struct represents a MarkovChain with a pointer to a linked_list contains
 * the database, and a pointers to the functions needed to support all
 * functions in this file using a pointer to this struct
 */
typedef struct MarkovChain {
    LinkedList *database;

    // pointer to a func that receives data from a generic type and prints it
    // returns void.
    print_func1 print_func;

    // pointer to a func that gets 2 pointers of generic data type(same one)
    // and compare between them */
    // returns: - a positive value if the first is bigger
    //          - a negative value if the second is bigger
    //          - 0 if equal
   compare_func1 comp_func;

    // a pointer to a function that gets a pointer of generic data type and
    // frees it.
    // returns void.
    free_data1 free_data;

    // a pointer to a function that gets a pointer of generic data type and
    // returns a newly allocated copy of it
    // returns a generic pointer.
    copy_func1 copy_func;

    //  a pointer to function that gets a pointer of generic data type and
    //  returns:
    //      - true if it's the last state.
    //      - false otherwise.
    is_last1 is_last;
} MarkovChain;

/**
 * Get one random state from the given markov_chain's database.
 * @param markov_chain
 * @return
 */
MarkovNode* get_first_random_node(MarkovChain *markov_chain);

/**
 * Choose randomly the next state, depend on it's occurrence frequency.
 * @param state_struct_ptr MarkovNode to choose from
 * @return MarkovNode of the chosen state
 */
MarkovNode* get_next_random_node(MarkovNode *state_struct_ptr);

/**
 * Receive markov_chain, generate and print random sentence out of it. The
 * sentence most have at least 2 words in it.
 * @param markov_chain
 * @param first_node markov_node to start with, if NULL- choose a random
 * markov_node
 * @param  max_length maximum length of chain to generate
 */
void generate_random_sequence(MarkovChain *markov_chain, MarkovNode *
first_node, int max_length);

/**
 * Free markov_chain and all of it's content from memory
 * @param markov_chain markov_chain to free
 */
void free_markov_chain(MarkovChain **markov_chain);

/**
 * Add the second markov_node to the counter list of the first markov_node.
 * If already in list, update it's counter value.
 * @param first_node
 * @param second_node
 * @param markov_chain
 * @return success/failure: true if the process was successful, false if in
 * case of allocation error.
 */
bool add_node_to_counter_list(MarkovNode *first_node, MarkovNode
*second_node, MarkovChain *markov_chain);

/**
* Check if data_ptr is in database. If so, return the markov_node wrapping it
 * in the markov_chain, otherwise return NULL.
 * @param markov_chain the chain to look in its database
 * @param data_ptr the state to look for
 * @return Pointer to the Node wrapping given state, NULL if state not in
 * database.
 */
Node* get_node_from_database(MarkovChain *markov_chain, void *data_ptr);

/**
* If data_ptr in markov_chain, return it's markov_node. Otherwise, create new
 * markov_node, add to end of markov_chain's database and return it.
 * @param markov_chain the chain to look in its database
 * @param data_ptr the state to look for
 * @return markov_node wrapping given data_ptr in given chain's database
 */
Node* add_to_database(MarkovChain *markov_chain, void *data_ptr);

#endif /* MARKOV_CHAIN_H */
