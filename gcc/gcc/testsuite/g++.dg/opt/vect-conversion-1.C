/* { dg-do compile } */
// { dg-options "-O1 -ftree-vectorize" }

 const wchar_t* appSpc( signed int NumCharacters, unsigned char Char ) {
 static wchar_t StaticString[256];
 static signed int OldNum=-1;
 for( OldNum=0; OldNum<NumCharacters; OldNum++ )
   StaticString[OldNum] = Char;
}

