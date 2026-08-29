/* { dg-do compile } */
// test to make sure that as below is a constant variable.
template <int>
struct a
{
   static bool f()
   {
       const int as(2);
       float anArray[as] = { 0.0f, 0.0f };
       return (anArray[0] == anArray[1]);
   }
};

