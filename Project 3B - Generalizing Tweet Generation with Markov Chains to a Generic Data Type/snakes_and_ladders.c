#include <string.h> // For strlen(), strcmp(), strcpy()
#include "markov_chain.h"
#include <assert.h>
#define MAX(X, Y) (((X) < (Y)) ? (Y) : (X))
#define EMPTY -1
#define BOARD_SIZE 100
#define MAX_GENERATION_LENGTH 60
#define DICE_MAX 6
#define NUM_OF_TRANSITIONS 20
#define NUM_OF_ARGS 3
#define DECIMAL_BASE 10
#define WINNING_CELL 100
#define ARGUMENTS_ERROR "Usage: invalid number of arguments"
/**
 * represents the transitions by ladders and snakes in the game
 * each tuple (x,y) represents a ladder from x to if x<y or a snake otherwise
 */
const int transitions[][2] = {{13, 4},
                              {85, 17},
                              {95, 67},
                              {97, 58},
                              {66, 89},
                              {87, 31},
                              {57, 83},
                              {91, 25},
                              {28, 50},
                              {35, 11},
                              {8,  30},
                              {41, 62},
                              {81, 43},
                              {69, 32},
                              {20, 39},
                              {33, 70},
                              {79, 99},
                              {23, 76},
                              {15, 47},
                              {61, 14}};

/**
 * struct represents a Cell in the game board
 */
typedef struct Cell {
    int number; // Cell number 1-100
    int ladder_to;  // ladder_to represents the jump of the ladder in case
    // there is one from this square
    int snake_to;  // snake_to represents the jump of the snake in case there
    // is one from this square
    //both ladder_to and snake_to should be -1 if the Cell doesn't have them
} Cell;

/** Error handler **/
static int handle_error(char *error_msg, MarkovChain **database)
{
    printf("%s", error_msg);
    if (database != NULL)
    {
        free_markov_chain(database);
    }
    return EXIT_FAILURE;
}
// create board and return 1 in case there was a memory allocation failure
// and 0 otherwise
static int create_board(Cell* cells[BOARD_SIZE])
{
  // create board without snakes and ladders
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        cells[i] = malloc(sizeof(Cell));
        if (cells[i] == NULL) //memory allocation failed
        {
            for (int j = 0; j < i; j++) {
                free(cells[j]);
            }
            handle_error(ALLOCATION_ERROR_MASSAGE,NULL);
            return EXIT_FAILURE;
        }
        *(cells[i]) = (Cell) {i + 1, EMPTY, EMPTY};
    }
    // add snaked and ladders to the board
    for (int i = 0; i < NUM_OF_TRANSITIONS; i++)
    {
        int from = transitions[i][0];
        int to = transitions[i][1];
        if (from < to) // the transition is a ladder
        {
            cells[from - 1]->ladder_to = to;
        }
        else // the transition is a snake
        {
            cells[from - 1]->snake_to = to;
        }
    }
    return EXIT_SUCCESS;
}

/**
 * fills database
 * @param markov_chain it points to the database we fill
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
static int fill_database(MarkovChain *markov_chain)
{
    Cell* cells[BOARD_SIZE];
    if(create_board(cells) == EXIT_FAILURE)
    {
        return EXIT_FAILURE;
    }
    MarkovNode *from_node = NULL, *to_node = NULL;
    size_t index_to;
    for (size_t i = 0; i < BOARD_SIZE; i++)
    {
        add_to_database(markov_chain, cells[i]); //add to markov_chain
        // pointer cell struct item
    }

    for (size_t i = 0; i < BOARD_SIZE; i++) // it's a transition
    {
        from_node = get_node_from_database(markov_chain,cells[i])->data;

        if (cells[i]->snake_to != EMPTY || cells[i]->ladder_to != EMPTY)
        {
            index_to = MAX(cells[i]->snake_to,cells[i]->ladder_to) - 1;
            to_node = get_node_from_database(markov_chain, cells[index_to])
                    ->data;
            add_node_to_counter_list(from_node, to_node, markov_chain);
        }
        else // throwing a cube
        {
            for (int j = 1; j <= DICE_MAX; j++)
            {
                index_to = ((Cell*) (from_node->data))->number + j - 1;
                if (index_to >= BOARD_SIZE)
                {
                    break;
                }
                to_node = get_node_from_database(markov_chain, cells[index_to])
                        ->data;
                add_node_to_counter_list(from_node, to_node, markov_chain);
            }
        }
    }
    // free temp arr
    for (size_t i = 0; i < BOARD_SIZE; i++)
    {
        free(cells[i]);
    }
    return EXIT_SUCCESS;
}
// assume data_ptr points to a Cell. print the cell in the wanted format
static void print_cell(void* data_ptr)
{
  Cell* cell_ptr =  data_ptr;
  assert(cell_ptr != NULL);
  if (cell_ptr->number == WINNING_CELL)
  {
    fprintf (stdout, "[%d]", cell_ptr->number);
  }
  else if (cell_ptr->ladder_to != EMPTY)
  {
    fprintf (stdout, "[%d]-ladder to %d -> ", cell_ptr->number, cell_ptr->ladder_to);
  }
  else if (cell_ptr->snake_to != EMPTY)
  {
    fprintf (stdout, "[%d]-snake to %d -> ", cell_ptr->number,
             cell_ptr->snake_to);
  }
  else
  {
    fprintf (stdout, "[%d] -> ", cell_ptr->number);
  }
}
// assume data_ptr points to a Cell. compare between two cells according to
// the number field
static int compare_cells(void* data_ptr1, void* data_ptr2)
{
  Cell* cell1= data_ptr1;
  Cell* cell2 = data_ptr2;
  if (cell1->number > cell2->number)
  {
    return 1;
  }
  if (cell1->number == cell2->number)
  {
    return 0;
  }
  return -1;
}

// assume data_ptr points to a Cell
static void free_cell(void* data_ptr)
{
  free((Cell*)data_ptr);
}

// assume data_ptr points to a Cell
// copy the cell to a new memory block and return a pointer to this copy
static void* copy_cell (void* data_ptr)
{
  Cell *source_cell = data_ptr;
  Cell *dest_cell = malloc (sizeof (Cell));
  return memcpy (dest_cell, source_cell, sizeof (Cell)); //return dest
}

// assume data_ptr points to a Cell
// check if the cell is the last cell - the winning cell (100)
// return true if it is the last cell, false otherwise
static bool is_last(void* data_ptr)
{
  Cell* cell = data_ptr;
  if (cell->number == WINNING_CELL)
  {
    return true;
  }
  return false;
}

// assign values in *markov_chain fields
static void fill_markov_chain_fields2(MarkovChain *markov_chain)
{
  LinkedList *linked_list = calloc (1,sizeof (LinkedList));
  markov_chain->database = linked_list;
  markov_chain->print_func = &print_cell;
  markov_chain->comp_func = &compare_cells;
  markov_chain->free_data = &free_cell;
  markov_chain->copy_func = &copy_cell;
  markov_chain->is_last = &is_last;
}

/**
 * @param argc num of arguments
 * @param argv 1) Seed
 *             2) Number of paths to generate
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
int main(int argc, char *argv[])
{
  if (NUM_OF_ARGS != argc)
  {
    fprintf (stdout, ARGUMENTS_ERROR);
    return EXIT_FAILURE;
  }
  MarkovChain *ptr_markov_chain = calloc(1,sizeof(MarkovChain));
  if (ptr_markov_chain == NULL)
  {
    return handle_error (ALLOCATION_ERROR_MASSAGE, NULL);
  }
  fill_markov_chain_fields2(ptr_markov_chain);
  int flag = fill_database( ptr_markov_chain);
  if (flag == EXIT_FAILURE)
  {
    return handle_error (ALLOCATION_ERROR_MASSAGE, &ptr_markov_chain);
  }
  int paths_number = (int)strtol (argv[2],NULL, DECIMAL_BASE);
  int seed = (int)strtol (argv[1],NULL, DECIMAL_BASE);
  srand(seed);
  for (int i=1; i<=paths_number; i++)
  {
    fprintf (stdout, "Random Walk %d: ", i);
    MarkovNode *first_node = ptr_markov_chain->database->first->data;
    generate_random_sequence (ptr_markov_chain, first_node, MAX_GENERATION_LENGTH);
  }
  free_markov_chain (&ptr_markov_chain);
  return EXIT_SUCCESS;
}
