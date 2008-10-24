/* { dg-do compile } */
/* { dg-options "-dA" } */
/* Test that debugging information for SayHelloAndCalculate is
 * emitted */
namespace CleverMonkey
{
   static
   void SayHelloAndCalculate(void) __attribute__((noinline));
   static
   void SayHelloAndCalculate(void)
    {
__builtin_printf("Hello from Bristol Zoo!\n");

      int one = 1;
      int nine = 9;
      __builtin_printf("One plus nine is equal to  %d \n", one + nine );
    }
}
static void SayGoodbye(void)
{
  __builtin_printf("Goodbye!\n");
}
int main(void)
{
    CleverMonkey::SayHelloAndCalculate();
    SayGoodbye();
} /* { dg-final { scan-assembler "\\\"SayHelloAndCalculate|\\\"_ZN12CleverMonkey20SayHelloAndCalculateEv" } } */

