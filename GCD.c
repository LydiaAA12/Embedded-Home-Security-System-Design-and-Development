unsigned long gcd_Num(unsigned long a, unsigned long b,unsigned long c){
    unsigned long st, hcf;
    st=a<b?(a<c?a:c):(b<c?b:c);
     for (hcf=st;hcf>=1;hcf--)     {
       if (a%hcf==0 && b%hcf==0 && c%hcf==0)
         break;
     }
     return hcf;

}
