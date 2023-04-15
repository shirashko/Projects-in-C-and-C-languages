#include "cipher.h"

#define NUM_OF_LETTERS 26
#define LOWER_FIRST_LETTER 97  // 97 for 'a' in ASCII
#define LOWER_LAST_LETTER 122  // 122 for 'z' in ASCII
#define UPPER_FIRST_LETTER 65  // 65 for 'A' in ASCII
#define UPPER_LAST_LETTER 90  // 90 for 'Z' in ASCII
#define FOR_CYCLING 1 // for jump from last letter to first letter
                      // or the opposite

// checking if k is bigger than the number of letters in English language
int is_multi_cycling (int k)
{
  return (k >= NUM_OF_LETTERS) ? 1 : 0;
}

// checking if a current char letter is a lower case
int is_lower_case (char s)
{
  return (s >= LOWER_FIRST_LETTER && s <= LOWER_LAST_LETTER) ? 1 : 0;
}

// checking if a char letter is an upper case
int is_upper_case (char s)
{
  return (s >= UPPER_FIRST_LETTER && s <= UPPER_LAST_LETTER) ? 1 : 0;
}

// en\decode in case k is positive
void for_positive_k (char s[], const int k, const int i, const int firstLetter,
                     const int lastLetter)
{
  int new_k = k;
  if (is_multi_cycling (k))
    {
      new_k = k % NUM_OF_LETTERS;
    }
  if (lastLetter >= (int) (s[i] + new_k)) // this case is for non cycling
    {
      s[i] = s[i] + new_k; //encoding the char as requested
    }
  else // this case for cycling
    {
      const int jumps_before_cycling = lastLetter - s[i] + FOR_CYCLING;
      // add 1 for the jump from z to a
      const int jumps_after_cycling = new_k - jumps_before_cycling;
      const int encode_value = firstLetter + jumps_after_cycling;
      s[i] = (char) encode_value;
    }
}

// en\decode in case k is negative
void for_negative_k (char s[], int k, int i, int first_letter, int last_letter)
{
  if (is_multi_cycling (-k))
    {
      k = -(-k % NUM_OF_LETTERS); // - for % work as wanted
    }
  if (first_letter <= s[i] + k) // this case for non cycling letters
    {
      s[i] = s[i] + k; //encoding the char as requested
    }
  else // this case for cycling
    {
      const int jumps_before_cycling = s[i] - first_letter + FOR_CYCLING;
      //add 1 for jump from a to z
      const int jumps_after_cycling = k + jumps_before_cycling;
      const int encode_value = last_letter + jumps_after_cycling;
      s[i] = (char) encode_value;
    }
}

// Encodes the given string according to the given shift value - k.
// See full documentation in header file
void encode (char s[], int k)
{
  if (k > 0)
  {
      //checking every char in the string and encoding it if needed
      for (int i = 0; s[i] != '\0'; i++)
        {
          if (is_lower_case (s[i]))
            {
              for_positive_k (s, k, i, LOWER_FIRST_LETTER, LOWER_LAST_LETTER);
            }
          else if (is_upper_case (s[i]))
            {
              for_positive_k (s, k, i, UPPER_FIRST_LETTER, UPPER_LAST_LETTER);
            }
          //else- the char is not part of the alphabet, and we don't
          // need to change it
        }
    }
  else if (k < 0)
    {
      //checking every char in the string and encoding him
      for (int i = 0; s[i] != '\0'; i++)
        {
          if (is_lower_case (s[i]))
            {
              for_negative_k (s, k, i, LOWER_FIRST_LETTER, LOWER_LAST_LETTER);
            }
          else if (is_upper_case (s[i]))
            {
              for_negative_k (s, k, i, UPPER_FIRST_LETTER, UPPER_LAST_LETTER);
            }
          //else- the char is not part of the alphabet,
          // and we don't need to change it
        }
    } //if k=0 no change need to be done
}

// Decodes the given string according to the given shift value - k.
// See full documentation in header file
void decode (char s[], int k)
{
  if (k != 0) // if k is zero - no change need to be done
    {
      encode (s, -k);
      //the algorithm of decoding with k is equal to encoding with -k
    }
}
