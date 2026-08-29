/* Test to check that the following program aborts as expected
 * on the insertion of embedded zeroes in the initialization of a
 * two dimensional array */
int main()
{
  char str[2][34] = {"a\0c","b"};

  if (str[0][2] != 'c')
    __builtin_abort ();

  return 0;
}

