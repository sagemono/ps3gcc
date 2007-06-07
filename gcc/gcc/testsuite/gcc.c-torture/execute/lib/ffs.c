int
ffs(int word)
{
  int i;

  if (!word)
    return 0;

  i = 0;
  for (;;)
    {
      if (((1 << i++) & word) != 0)
        return i;
    }
}
