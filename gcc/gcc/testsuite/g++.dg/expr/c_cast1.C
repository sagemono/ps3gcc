// { dg-do compile }
// { dg-options "-Wcast-qual" }
/* Test to check warning mentioning qualifiers is emitted when volatile
   is cast away using a C style cast. */
int* foo (volatile int *p)
{
    return (int*)p; /* { dg-warning "casts away qualifiers" } */
}

