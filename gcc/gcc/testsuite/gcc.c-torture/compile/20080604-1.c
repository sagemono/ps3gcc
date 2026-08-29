/* We used to ICE on this due to gen_lowpart not handling an extra subreg. */
unsigned short f(float a)
{
  int b = *(int*)&a;
  return b;
}
