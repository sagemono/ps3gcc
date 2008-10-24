// { dg-do compile }
// We used to crash on this template when emitting debugging information
// for dwarf2.
template< typename Dummy = int > struct set0 {    
  typedef set0<> item_;  
};
set0<int> a;
