#ifndef EX2_REPO_TESTBUSLINES_H
#define EX2_REPO_TESTBUSLINES_H


// write only between #define EX2_REPO_TESTBUSLINES_H and #endif
// EX2_REPO_TESTBUSLINES_H
#include "sort_bus_lines.h"

/**
 * check if the given list of lines of bus is sorted according to the
 * destination attribute of every
 * line in Ascending order using bubble sort
 * @param start is a pointer to the first BusLine in the list
 * @param end is a pointer to the end of the list+1 (one byte later)
 * return 1 if list is sorted, 0 otherwise
 */
int is_sorted_by_distance (BusLine *start, BusLine *end);

/**
 * check if the given list of lines of bus is sorted according to the
 * duration attribute of every
 * line in Ascending order using bubble sort
 * @param start is a pointer to the first BusLine in the list
 * @param end is a pointer to the end of the list+1 (one byte later)
 * return 1 if list is sorted, 0 otherwise
 */
int is_sorted_by_duration (BusLine *start, BusLine *end);


/**
 * check if the original array contain the same number lines  as the sorted
 * array, and that there isn't new number lines mistakenly added to the
 * sorted array
 * @param start_sorted points to the start of the sorted list
 * @param end_sorted points to the end of the sorted list (+1????)
 * @param start_original points to the start of the original list
 * @param end_sorted points to the end of the original list (+1????)
 * return 1 if the lists are equal, 0 otherwise
 */
int is_equal (BusLine *start_sorted,
              BusLine *end_sorted, BusLine *start_original,
              BusLine *end_original);


// write only between #define EX2_REPO_TESTBUSLINES_H and #endif
// EX2_REPO_TESTBUSLINES_H
#endif //EX2_REPO_TESTBUSLINES_H
