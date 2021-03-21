#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include "packet_decode.h"

int bypass(unsigned char *p,int NQ,float* IE,float* IO ,float* QE,float* QO) // bypass
{int NW=(10*NQ)/16*2;
 short res;
 int pos=0,sign,index=0;
 printf("\nNQ=%d -> NW=%d\n",NQ,NW);
 while (index<(NQ))  // 8*5=40 : 4 10=bit values for 5 bytes
   {// printf("%02hhx %02hhx %02hhx %02hhx %02hhx = ",p[pos+0],p[pos+1],p[pos+2],p[pos+3],p[pos+4]);
    if (index<NQ) 
      {res=(short)(p[pos+0]&0xff)*4+(short)(p[pos+1]>>6);       // printf("%hx=",res);   
       sign=(res&0x200); res=res&(0x1ff); if (sign>0) res=-res; // printf("%03d\t",res);
       IE[index]=(float)res; 
       index++;
      }
    if (index<NQ) 
      {res=(short)(p[pos+1]&0x3f)*4*4+(short)(p[pos+2]>>4);     // printf("%hx=",res);   
       sign=(res&0x200); res=res&(0x1ff); if (sign>0) res=-res; // printf("%03d\t",res);
       IE[index]=(float)res; 
       index++;
      }
    if (index<NQ) 
      {res=(short)(p[pos+2]&0x0f)*4*4*4+(short)(p[pos+3]>>2);   // printf("%hx=",res);   
       sign=(res&0x200); res=res&(0x1ff); if (sign>0) res=-res; // printf("%03d\t",res);
       IE[index]=(float)res; 
       index++;
      }
    if (index<NQ) 
      {res=(short)(p[pos+3]&0x03)*4*4*4*4+(short)(p[pos+4]);    // printf("%hx=",res);   
       sign=(res&0x200); res=res&(0x1ff); if (sign>0) res=-res; // printf("%03d\t",res);
       IE[index]=(float)res; 
       index++;
      }
    if (index<NQ) pos+=5;
   }
 index=0;
 pos=NW+2; // next short
 while (index<(NQ))  // 8*5=40 : 4 10=bit values for 5 bytes
   {// printf("%02hhx %02hhx %02hhx %02hhx %02hhx = ",p[pos+0],p[pos+1],p[pos+2],p[pos+3],p[pos+4]);
    if (index<NQ) 
      {res=(short)(p[pos+0]&0xff)*4+(short)(p[pos+1]>>6);       // printf("%hx=",res);   
       sign=(res&0x200); res=res&(0x1ff); if (sign>0) res=-res; // printf("%03d\t",res);
       IO[index]=(float)res; 
       index++;
      }
    if (index<NQ) 
      {res=(short)(p[pos+1]&0x3f)*4*4+(short)(p[pos+2]>>4);     // printf("%hx=",res);   
       sign=(res&0x200); res=res&(0x1ff); if (sign>0) res=-res; // printf("%03d\t",res);
       IO[index]=(float)res; 
       index++;
      }
    if (index<NQ) 
      {res=(short)(p[pos+2]&0x0f)*4*4*4+(short)(p[pos+3]>>2);   // printf("%hx=",res);   
       sign=(res&0x200); res=res&(0x1ff); if (sign>0) res=-res; // printf("%03d\t",res);
       IO[index]=(float)res; 
       index++;
      }
    if (index<NQ) 
      {res=(short)(p[pos+3]&0x03)*4*4*4*4+(short)(p[pos+4]);    // printf("%hx=",res);   
       sign=(res&0x200); res=res&(0x1ff); if (sign>0) res=-res; // printf("%03d\t",res);
       IO[index]=(float)res; 
       index++;
      }
    if (index<NQ) pos+=5;
   }
 printf("\npos=%d index=%d nw=%d\n",pos,index,(2*NW+2));
 index=0;
 pos=2*(NW+2); // next short
 while (index<(NQ))  // 8*5=40 : 4 10=bit values for 5 bytes
   {// printf("%02hhx %02hhx %02hhx %02hhx %02hhx = ",p[pos+0],p[pos+1],p[pos+2],p[pos+3],p[pos+4]);
    if (index<NQ) 
      {res=(short)(p[pos+0]&0xff)*4+(short)(p[pos+1]>>6);       // printf("%hx=",res);   
       sign=(res&0x200); res=res&(0x1ff); if (sign>0) res=-res; // printf("%03d\t",res);
       QE[index]=(float)res; 
       index++;
      }
    if (index<NQ) 
      {res=(short)(p[pos+1]&0x3f)*4*4+(short)(p[pos+2]>>4);     // printf("%hx=",res);   
       sign=(res&0x200); res=res&(0x1ff); if (sign>0) res=-res; // printf("%03d\t",res);
       QE[index]=(float)res; 
       index++;
      }
    if (index<NQ) 
      {res=(short)(p[pos+2]&0x0f)*4*4*4+(short)(p[pos+3]>>2);   // printf("%hx=",res);   
       sign=(res&0x200); res=res&(0x1ff); if (sign>0) res=-res; // printf("%03d\t",res);
       QE[index]=(float)res; 
       index++;
      }
    if (index<NQ) 
      {res=(short)(p[pos+3]&0x03)*4*4*4*4+(short)(p[pos+4]);    // printf("%hx=",res);   
       sign=(res&0x200); res=res&(0x1ff); if (sign>0) res=-res; // printf("%03d\t",res);
       QE[index]=(float)res; 
       index++;
      }
    if (index<NQ) pos+=5;
   }
 index=0;
 pos=3*(NW+2); // next short
 while (index<(NQ))  // 8*5=40 : 4 10=bit values for 5 bytes
   {// printf("%02hhx %02hhx %02hhx %02hhx %02hhx = ",p[pos+0],p[pos+1],p[pos+2],p[pos+3],p[pos+4]);
    if (index<NQ) 
      {res=(short)(p[pos+0]&0xff)*4+(short)(p[pos+1]>>6);       // printf("%hx=",res);   
       sign=(res&0x200); res=res&(0x1ff); if (sign>0) res=-res; // printf("%03d\t",res);
       QO[index]=(float)res; 
       index++;
      }
    if (index<NQ) 
      {res=(short)(p[pos+1]&0x3f)*4*4+(short)(p[pos+2]>>4);     // printf("%hx=",res);   
       sign=(res&0x200); res=res&(0x1ff); if (sign>0) res=-res; // printf("%03d\t",res);
       QO[index]=(float)res; 
       index++;
      }
    if (index<NQ) 
      {res=(short)(p[pos+2]&0x0f)*4*4*4+(short)(p[pos+3]>>2);   // printf("%hx=",res);   
       sign=(res&0x200); res=res&(0x1ff); if (sign>0) res=-res; // printf("%03d\t",res);
       QO[index]=(float)res; 
       index++;
      }
    if (index<NQ) 
      {res=(short)(p[pos+3]&0x03)*4*4*4*4+(short)(p[pos+4]);    // printf("%hx=",res);   
       sign=(res&0x200); res=res&(0x1ff); if (sign>0) res=-res; // printf("%03d\t",res);
       QO[index]=(float)res; 
       index++;
      }
    if (index<NQ) pos+=5;
   }
 printf("\npos=%d index=%d\n",pos,index);
 pos=4*(NW+2); // next short
 return(pos);
}
