// { dg-do assemble { target native } }
// { dg-xfail-if "eh not supported on spu" { "spu-*-*" } "*" "" }
// { dg-options "-fexceptions -fPIC -S" }

main() { throw 1; }
