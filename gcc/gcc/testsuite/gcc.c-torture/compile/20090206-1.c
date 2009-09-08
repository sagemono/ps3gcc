 void f(unsigned);
 void foo (unsigned var) 
 {
   unsigned int key_inline_FindKey_74334;
   void* _Val_inline__Lower_bound_77251;
   do {
     key_inline_FindKey_74334 = var;
     _Val_inline__Lower_bound_77251 = (void*)&key_inline_FindKey_74334;
     unsigned int DAG_temp99632;
     union __block_indir0_u {  struct {  unsigned int val;  } __indir_struct;  }
       * __indir_union = (union __block_indir0_u*)_Val_inline__Lower_bound_77251;
     DAG_temp99632 = __indir_union->__indir_struct.val;
     f(DAG_temp99632);
     var = var + 1;
  } while (1);
}
