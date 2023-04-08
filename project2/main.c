#include "sort_bus_lines.h"
#include "test_bus_lines.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_CHARS 60
#define DECIMAL_BASE 10
#define MAX_LINE_NUMBER 999
#define MIN_LINE_NUMBER 1
#define MAX_DISTANCE 1000
#define MIN_DISTANCE 0
#define MAX_DURATION 100
#define MIN_DURATION 10

// check the arguments of main is corresponded with the program requires
int arguments_invalid(int argc, char *argv[])
{
  // in case arguments are invalid
  if ((argc != 2) || (strcmp (argv[1], "test") && strcmp (argv[1],"bubble")
  && strcmp (argv[1], "quick") ) )
  {
    fprintf (stdout, "USAGE: need to enter quick, bubble or test");
    return EXIT_FAILURE;
  }
  else
  {
    return EXIT_SUCCESS;
  }
}
// check that the given attributes of every line is valid and in the range
// the program demands. if info is not valid return 1, 0 otherwise
int info_not_valid(int line_number,int distance, int duration)
{
  //שימי לב שכרגע אם נותנים לך רק שני ארגומנטים אין טעות!!!
  if (line_number > MAX_LINE_NUMBER || line_number < MIN_LINE_NUMBER)
  {
    fprintf (stdout, "ERROR: line number should be an integer between 1 and "
                     "999 (includes).\n");
    return EXIT_FAILURE;
  }
  if (distance < MIN_DISTANCE || distance > MAX_DISTANCE)
  {
    fprintf (stdout, "ERROR: distance should be an integer between 0 and "
                     "1000 (includes).\n");
    return EXIT_FAILURE;
  }
  if (duration < MIN_DURATION || duration > MAX_DURATION)
  {
    fprintf (stdout, "ERROR: duration should be an integer between 10 and "
                     "100 (includes).\n");
    return EXIT_FAILURE;
  }
  else
  {
    return EXIT_SUCCESS;
  }
}

// make the number of lines valid and according to the program's demands
long make_num_of_lines_valid(long lines_number)
{
  while (lines_number <= 0)
  {
    char input_lines_number[MAX_CHARS];
    fprintf (stdout, "ERROR: Please notice lines number is a whole number "
                     "bigger than 0.\n");
    fprintf (stdout, "Enter number of lines. Then enter\n");
    fgets (input_lines_number, MAX_CHARS, stdin);
    lines_number = strtol (input_lines_number, NULL, DECIMAL_BASE);
  }
  return lines_number;
}

// getting a valid input of the attributes of a line from the user
int getting_info_on_lines(long lines_number, BusLine* lines_info)
{
  char current_line[MAX_CHARS];
  long current_lines_number = 0;
  int line_number;
  int distance;
  int duration;
  while (current_lines_number < lines_number)
  {
    fprintf (stdout, "Enter line info. Then enter\n");
    //put the input on the current_line array
    fgets (current_line, MAX_CHARS, stdin);
    if (sscanf (current_line, "%d,%d,%d", &line_number,&distance,&duration)
    == EOF)     //check if sscanf worked appropriately
    {
      return EXIT_FAILURE;
    }
    if (!info_not_valid(line_number, distance, duration))
    {
      lines_info[current_lines_number].line_number = line_number;
      lines_info[current_lines_number].distance = distance;
      lines_info[current_lines_number].duration = duration;
      current_lines_number++;
    } //else repeat loop with this bus line until valid
  }
  return EXIT_SUCCESS;
}

// check bubble sort works well
void tests_bubble_sort(BusLine* start_original1, BusLine* start_sorted,int
number_lines)
{
  if (is_sorted_by_distance (start_sorted, start_sorted + number_lines))
  {
    fprintf (stdout, "TEST 1 PASSED: bubble sort works\n");
  }
  else
  {
    fprintf (stdout, "TEST 1 FAILED: bubble sort doesn't work\n");
  }
  if (is_equal (start_sorted, start_sorted + number_lines, start_original1,
                start_original1 + number_lines))
  {
    fprintf (stdout, "TEST 2 PASSED: the same values is on the sorted "
                     "list\n");
  }
  else
  {
    fprintf (stdout, "TEST 2 FAILED: there isn't same values is on the sorted"
                     "list\n");
  }
}

// check quick sort works well
void tests_quick_sort(BusLine* start_original2, BusLine* start_sorted,int
number_lines)
{
  if (is_sorted_by_duration (start_sorted, start_sorted + number_lines))
  {
    fprintf (stdout, "TEST 3 PASSED: quick sort works\n");
  }
  else
  {
    fprintf (stdout, "TEST 3 FAILED:  quick sort doesn't work\n");
  }
  if (is_equal (start_sorted, start_sorted + number_lines, start_original2,
                start_original2 + number_lines))
  {
    fprintf (stdout, "TEST 4 PASSED: the same values is on the sorted "
                     "list\n");
  }
  else
  {
    fprintf (stdout, "TEST 4 FAILED: there isn't same values is on the sorted"
                     "list\n");
  }
}

//printing the sorted list in the same format as we got it, only now
// sorted by the requested attribute - arg
void printing_sorted_lines (long lines_number, BusLine* lines_info)
{
  for (int i=0; i < lines_number; i++)
  {
    fprintf (stdout, "%d,%d,%d\n", (*(lines_info+i)).line_number,(*
        (lines_info+i)).distance, (*(lines_info+i)).duration );
  }
}

// free memory
void free_memory(BusLine* pointer)
{
  free (pointer);
  pointer = NULL;
}

//testing the sorts functions
int tests(long lines_number, BusLine* lines_info)
{
  //creating copies of the bus lines information for comparison
  BusLine *copy1 = (BusLine*) malloc (sizeof(BusLine) * lines_number);
  BusLine *copy2 =  (BusLine*) malloc (sizeof(BusLine) * lines_number);
  if (copy1 == NULL)  //malloc couldn't allocate memory
  {
    free_memory(lines_info);
    return EXIT_FAILURE;
  }
  if (copy2 == NULL)
  {
    free_memory(lines_info);
    free_memory(copy1);
    return EXIT_FAILURE;
  }
  memcpy(copy1, lines_info, sizeof(BusLine) * lines_number);
  memcpy(copy2, lines_info, sizeof(BusLine) * lines_number);

  //sorting to see if works appropriately
  bubble_sort(lines_info, lines_info + lines_number);
  quick_sort (lines_info, lines_info + lines_number);
  tests_bubble_sort(copy1, lines_info, lines_number);
  tests_quick_sort(copy2, lines_info, lines_number);
  free_memory(copy1);
  free_memory(copy2);
  return EXIT_SUCCESS;
}

/**
 * the program gets lines of busses from the user and sorts them
 * according to the given argument
 * if the argument is bubble then sorts the lines according to
 * if the argument is quick then sorts the lines according to
 * if the argument is test - testing if the function of sorting working
 * appropriately
 */
int main (int argc, char *argv[])
{
  if (arguments_invalid(argc, argv))
  {
    return EXIT_FAILURE;
  }
  // getting number of lines from the user
  char input_lines_number[MAX_CHARS]; //will contain user input
  fprintf (stdout, "Enter number of lines. Then enter\n");
  fgets (input_lines_number,MAX_CHARS, stdin);// enter input to the array

  //converting the string gotten from the user to a whole number
  long lines_number = strtol (input_lines_number, NULL,DECIMAL_BASE);

  //checking if number of lines gotten from the user is valid. if not- repeat
  lines_number = make_num_of_lines_valid(lines_number);

  // now we have a valid number of bus lines. now allocate memory for saving
  // the info of every line
  BusLine* lines_info =  (BusLine*) malloc (sizeof(BusLine) * lines_number);
  if (lines_info == NULL)// malloc couldn't allocate memory
  {
    return EXIT_FAILURE;
  }
  if (getting_info_on_lines(lines_number, lines_info)) //getting info from user
  {
    free(lines_info);
    return EXIT_FAILURE;
  }
  // now we have all the information we need to sort according to the
  // requested argument (quick or bubble) or for testing for test arg
  if (!strcmp(argv[1], "test")) //strcmp return 0 if equals
  {
    if (tests(lines_number, lines_info))
    {
      return EXIT_FAILURE;
    }
  }
  else if (!strcmp (argv[1], "bubble")) //strcmp return 0 if equals
  {
    bubble_sort(lines_info, lines_info + lines_number);
    printing_sorted_lines(lines_number, lines_info);
  }
  else //argv[1] is equal to "quick"
  {
    quick_sort (lines_info, lines_info + lines_number);
    printing_sorted_lines(lines_number, lines_info);
  }
  free_memory(lines_info);
}





