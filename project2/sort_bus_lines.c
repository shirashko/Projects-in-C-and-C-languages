#include "sort_bus_lines.h"

//swap between the attribute of destination of two BusLine items.
// see full documentation on h file
void swap(BusLine *line1, BusLine *line2)
{
  BusLine temp = *line1;
  *line1 = *line2;
  *line2 = temp;
}

//sort list of bus lines according to Ascending order by the destination
// attribute using bubble sort algorithm. see full documentation on h file
void bubble_sort(BusLine *start, BusLine *end)
{
  for (int i = 0; i < (end-start-1); i++) //-1 or without?
  {
    // Last i elements are already in place
    for (int j = 0; j < ((end-start) - i - 1); j++)
    {
      if( (*(start+j)).distance  > (*(start+j+1)).distance )
      {
        swap ((start + j), (start + j + 1));
      }
    }
  }
}

// function to find the partition position. see full documentation on h file
BusLine *partition (BusLine *start, BusLine *end)
{
  // select the rightmost element as pivot
  BusLine* pivot = end-1;

  // pointer for greater element \ address
  BusLine* i = (start - 1);

  // traverse each element of the array lines_info and compare them with the
  // pivot
  for (BusLine* j = start; j < end-1; j++) {
    if ((*j).duration <= (*pivot).duration) {

      // if element duration field smaller than pivot duration field is found
      // swap it with the greater element duration field pointed by i
      i++;

      // swap element at i with element at j
      swap(i, j);
    }
  }
  // swap the pivot element with the greater element at i
  swap((i+1), (end-1));

  // return the partition point
  return (i + 1);
}

// sort list contain lines of bus according to the duration attribute of every
// line, in Ascending order using quick sort. see full documentation on h file
void quick_sort (BusLine *start, BusLine *end)
{
  if (start < end)
  {
    // find the pivot element such that elements smaller than pivot are on
    // left of pivot, and elements greater than pivot are on right of pivot
    BusLine *pi = partition(start, end);

    // recursive call on the left of pivot
    quick_sort(start, pi);

    // recursive call on the right of pivot
    quick_sort(pi + 1, end);
  }
}

