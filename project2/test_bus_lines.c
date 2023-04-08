#include "test_bus_lines.h"
#define FAILURE 0
#define SUCCESS 1

// check if the given list of lines of bus is sorted according to the
// destination attribute of every. see full documentation in h file
int is_sorted_by_distance (BusLine *start, BusLine *end)
{
  for(int i=0; i < (end-start-1); i++)
  {
    if ( start[i].distance > start[i+1].distance)
    {
      return FAILURE;
    }
  }
  return SUCCESS;
}

// check if the given list of lines of bus is sorted according to the
// duration attribute of every. see full documentation in h file
int is_sorted_by_duration (BusLine *start, BusLine *end)
{
  for(int i=0; i < (end-start-1); i++)
  {
    if ( start[i].duration > start[i+1].duration)
    {
      return FAILURE;
    }
  }
  return SUCCESS;
}

// check if the original array contain the same number of lines as the sorted
// array, and that there isn't any new number lines mistakenly added to the
// sorted array. see full documentation in h file
int is_equal (BusLine *start_sorted, BusLine *end_sorted, BusLine
*start_original, BusLine *end_original)
{
  long j;
  int found;
  for (int i = 0; i < (end_sorted - start_sorted); i++)
  {
    j = 0;
    found = 0;
    while ((j < (end_original - start_original)) && (found == 0))
    {
      if ((start_original[i].line_number) == (start_sorted[j].line_number))
      {
        if ((start_original[i].distance) == (start_sorted[j].distance) &&
            (start_original[i].duration) == (start_sorted[j].duration))
        {
          found = 1; //ending this loop because we found line number is present
        }
      }
      j++;
    }
    if (found == 0) // then line number from original is not in the sorted list
    {
      return FAILURE;
    }
  }
  return SUCCESS;
}
