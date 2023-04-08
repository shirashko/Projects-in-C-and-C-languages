#include "tests.h"
#include <string.h>

#define K_1 3 //put enum here? שימיייייי לב
#define K_2 2
#define K_3 -1
#define K_4 -3
#define K_5 29
#define K_6 3
#define K_7 2
#define K_8 -1
#define K_9 -3
#define K_10 29

// See full documentation in header file
int test_encode_non_cyclic_lower_case_positive_k ()
{
  char in[] = "abc";
  char out[] = "def";
  encode (in, K_1); //suppose to change variable from "abc" to "def"
  return strcmp (in, out) != 0;
}

// See full documentation in header file
int test_encode_cyclic_lower_case_special_char_positive_k ()
{
  char in[] = "y,z%";
  char out[] = "a,b%";
  encode (in, K_2);
  return strcmp (in, out) != 0;
}

// See full documentation in header file
int test_encode_non_cyclic_lower_case_special_char_negative_k ()
{
  char in[] = "ef*g";
  char out[] = "de*f";
  encode (in, K_3);
  return strcmp (in, out) != 0;
}

// See full documentation in header file
int test_encode_cyclic_lower_case_negative_k ()
{
  char in[] = "acb";
  char out[] = "xzy";
  encode (in, K_4);
  return strcmp (in, out) != 0;
}

// See full documentation in header file
int test_encode_cyclic_upper_case_positive_k ()
{
  char in[] = "WXY";
  char out[] = "ZAB";
  encode (in, K_5);
  return strcmp (in, out) != 0;
}

// See full documentation in header file
int test_decode_non_cyclic_lower_case_positive_k ()
{
  char in[] = "efg";
  char out[] = "bcd";
  decode (in, K_6); //3
  return strcmp (in, out) != 0;
}

// See full documentation in header file
int test_decode_cyclic_lower_case_special_char_positive_k ()
{
  char in[] = "ef^ 1g";
  char out[] = "cd^ 1e";
  decode (in, K_7); //2
  return strcmp (in, out) != 0;
}

// See full documentation in header file
int test_decode_non_cyclic_lower_case_special_char_negative_k ()
{
  char in[] = "efg 9,";
  char out[] = "fgh 9,";
  decode (in, K_8); //-1
  return strcmp (in, out) != 0;
}

// See full documentation in header file
int test_decode_cyclic_lower_case_negative_k ()
{
  char in[] = "xyz";
  char out[] = "abc";
  decode (in, K_9); //-3
  return strcmp (in, out) != 0;
}

// See full documentation in header file
int test_decode_cyclic_upper_case_positive_k ()
{
  char in[] = "ABC";
  char out[] = "XYZ";
  decode (in, K_10); // 29
  return strcmp (in, out) != 0;
}
