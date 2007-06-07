/* This used to ICE due to tablejump_p following deleted JUMP_LABELs.  */

/* { dg-do compile } */
/* { dg-options "-O2 -g" } */

int main (void)
{
  int len;

  for (len = 0; len > 0; len--)
    {
      unsigned int chk_cmd = 0x20;

      switch (chk_cmd)
	{
	case 0x20: break;
	case 0x30: break;
	case 0x22: break;
	case 0x21: break;
	case 0x32: break;
	case 0x31: break;
	case 0x24: break;
	default:   return -1;
	}
     }

  return 0;
}

