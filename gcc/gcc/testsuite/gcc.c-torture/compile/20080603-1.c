int vgGeneratePerVertex (  long *_normal,   long *pVG )
{
 long *pu32_col;
 long *pu32_colpaint0;
 long *pu32_colpaint1;
 long cpt1;
 long i, cpt;
 long *vertex, *vertexlast,*normal = _normal;

 for(cpt = cpt1 = 0; vertex != vertexlast; vertex++, normal++, cpt++)
   {
     *pVG += (unsigned char ) ((*(unsigned *) &vertex) & 0xFF);
     *vertex = 1;
     vertex[1] = 1;
   }
}



int vgGeneratePerVertex1 (  long *_normal,   long *pVG )
{
 long *pu32_col;
 long *pu32_colpaint0;
 long *pu32_colpaint1;
 long cpt1;
 long i, cpt;
 long *vertex, *vertexlast,*normal = _normal;

 for(cpt = cpt1 = 0; vertex != vertexlast; vertex++, normal++, cpt++)
   {
     *pVG += (unsigned char ) ((*(unsigned *) &vertex) & 0xFF);
   }
}
