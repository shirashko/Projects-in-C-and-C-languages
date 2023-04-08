#include "cipher.h"
#include "tests.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define NUM_OF_ARGS_OPT1 5
#define BUFFER_LENGTH 1024
#define NUM_OF_ARGS_OPT2 2
#define DECIMAL_BASE 10

// running tests of encode and decode functions and
// return 1 if all tests was successful, 0 otherwise
int tests ()
{
  int test1 = !test_encode_non_cyclic_lower_case_positive_k ();
  int test2 = !test_encode_cyclic_lower_case_special_char_positive_k ();
  int test3 = !test_encode_non_cyclic_lower_case_special_char_negative_k ();
  int test4 = !test_encode_cyclic_lower_case_negative_k ();
  int test5 = !test_encode_cyclic_upper_case_positive_k ();
  int test6 = !test_decode_non_cyclic_lower_case_positive_k ();
  int test7 = !test_decode_cyclic_lower_case_special_char_positive_k ();
  int test8 = !test_decode_non_cyclic_lower_case_special_char_negative_k ();
  int test9 = !test_decode_cyclic_lower_case_negative_k ();
  int test10 = !test_decode_cyclic_upper_case_positive_k ();
  return (test1 && test2 && test3 && test4 & test5 && test6 && test7 &&
          test8 && test9 && test10);
}

// close files in the end of a success at the end of the program
void close_files (FILE *input_file, FILE *output_file)
{
  fclose (input_file);
  fclose (output_file);
}

int k_is_not_int (const char *enf_of_int)
{
  if (*enf_of_int != '\0') // checking arg2 is a whole number
  {
    fprintf (stderr, "The given shift value is invalid.\n");
    return 1;
  }
  else
  {
    return 0;
  }
}

// checking if the file given for en\decode does exist. return 0 if it
// does, 1 otherwise
// @param input_file is the file needed to be checked
int file_not_exist (FILE *input_file, FILE *output_file)
{
  if (input_file == NULL) //if file doesn't exist
  {
    fprintf (stderr, "The given file is invalid.\n");
    return 1;
  }
  else if (output_file == NULL)
  {
    fprintf (stderr, "The given file is invalid.\n");
    fclose (input_file);
    return 1;
  }
  else
  {
    return 0;
  }
}


// checking that the given argument from the user is "encode" or "decode".
// if it is, return 0.
// otherwise, return 1.
// @praram argv is the array of argument the main gets from the user
int command_is_not_valid (char *argv[])
{
  //checking if command is valid
  if ((strcmp (argv[1], "encode")) && strcmp (argv[1], "decode"))
    {
      fprintf (stderr, "The given command is invalid.\n");
      return EXIT_FAILURE;
    }
  return 0;
}

// copying the file accepted from the user and en\decoding the file
// according to the user request
// @param argv containing the arguments the main gets from the user
// @param array_of_content is the array we copy the file content into
// @param input_file is the file that it's content the program en\decode
// @param output_file is the output file of the program containing the
// en\decoding requested content
// @param k is the element of jumps to follow for en\decode the content
void encode_or_decode (char *argv[], char array_of_content[], FILE *input_file,
                       FILE *output_file, int k, int size_of_array_of_content)
{
  if (!strcmp (argv[1], "encode"))
    {
      while (fgets (array_of_content, size_of_array_of_content,
                    input_file) != NULL)
        //checking that there still something to encode\decode
        {
          encode (array_of_content, k); // k type need to be decided
          // transfer the de\encoded content to the output file
          fputs (array_of_content, output_file);
        }
    }
  else
  {
    //checking if there is still something to encode\decode
    while (fgets (array_of_content, size_of_array_of_content,
                  input_file) != NULL)
    {
      decode (array_of_content, k); // k type need to be decided
      fputs (array_of_content, output_file); // transfer the decoded
      // content to the output file
    }
  }
}

int main (int argc, char *argv[])
{
  if ((argc != NUM_OF_ARGS_OPT1) && (argc != NUM_OF_ARGS_OPT2))
  {
    fprintf (stderr, "The program receives 1 or 4 arguments only.\n");
    return EXIT_FAILURE;
  }
  else if (NUM_OF_ARGS_OPT1 == argc)
  {
      if (command_is_not_valid (argv)) //checking if command is not en\decode
      {
          return EXIT_FAILURE;
      }
      char *end_of_int; // to indicate if given k is an integer or not
      // strtol for getting k as integer
      int k = (int) strtol (argv[2], &end_of_int, DECIMAL_BASE);
      if (k_is_not_int (end_of_int))
      {
        return EXIT_FAILURE;
      }
      //opening the given files for the en\decoding
      FILE *input_file = fopen (argv[3], "r");
      FILE *output_file = fopen (argv[4], "w");
      if (file_not_exist (input_file, output_file))
      {
        return EXIT_FAILURE;
      }
      // creating an array that will hold the en\decoded content of the
      // input file
      char array_of_content[BUFFER_LENGTH];
      encode_or_decode (argv, array_of_content, input_file, output_file, k,
                        sizeof (array_of_content));
      close_files (input_file, output_file);
      return EXIT_SUCCESS;
  }
  else //the number of arguments is 2 - the second option
  {
      if (!strcmp (argv[1], "test")) // checking second type of input is "test"
      {
          return (tests ())? EXIT_SUCCESS : EXIT_FAILURE;
      }
      else // the second argument is not as requested
      {
        fprintf (stderr, "Usage: cipher test\n");
        return EXIT_FAILURE;
      }
  }
}
