void my_itoa(int value, char *res)
{
  int pos = 0;
  if (value < 0)
  {
    value = value * -1;
    res[pos] = '-';
    pos++;
  }
  while (value >= 10)
  {
    res[pos] = value % 10 + '0';
    pos++;
    value /= 10;
  }
  res[pos++] = value;
  res[pos] = '\0';
}

