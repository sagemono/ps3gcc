// { dg-do run }
// { dg-options "-fpermissive" }

bool f(void)
{
  return 0;
}
void g(int i)
{
  if (i) goto bad; // { dg-warning "warning:" }
  bool a = f(); // { dg-warning "warning:" }
bad: // { dg-warning "warning:" }
  ;
}


int main(void)
{
  return 0;
}

