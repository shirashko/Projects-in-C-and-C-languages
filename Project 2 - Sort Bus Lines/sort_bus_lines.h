#ifndef EX2_REPO_SORTBUSLINES_H
#define EX2_REPO_SORTBUSLINES_H
// write only between #define EX2_REPO_SORTBUSLINES_H and #endif
// EX2_REPO_SORTBUSLINES_H

/**
 * struct of bus line with attributes of line number, distance from current
 * stop to destination stop, and the duration of the ride
 */
typedef struct BusLine
{
    int line_number, distance, duration;
} BusLine;

/**
 * sort list of lines of bus according to the distance attribute of every
 * line in Ascending order using bubble sort
 * @param start is a pointer to the first BusLine in the list
 * @param end is a pointer to the end of the list+1 (one byte later)
 * return
 */
void bubble_sort (BusLine *start, BusLine *end);

/**
 * sort list of lines of bus according to the duration attribute of every
 * line in Ascending order using bubble sort
 * @param start is a pointer to the first BusLine in the list
 * @param end is a pointer to the end of the list+1 (one byte later)
 * return
 */
void quick_sort (BusLine *start, BusLine *end);

/**
 * find the final place of the last item in the sorted list
 * @param start is a pointer to the start of the list
 * @param end is a pointer to the end of the list
 * return final position last item in the sorted array
 */
BusLine *partition (BusLine *start, BusLine *end);

/**
 * replace two BusLine items
 * @param line1 is a pointer to one BusLine item
 * @param line2 is a pointer to second BusLine item
 */
void swap(BusLine *line1, BusLine *line2);

// write only between #define EX2_REPO_SORTBUSLINES_H and #endif
// EX2_REPO_SORTBUSLINES_H
#endif //EX2_REPO_SORTBUSLINES_H
