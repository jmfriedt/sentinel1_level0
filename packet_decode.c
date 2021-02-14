//0000: 62        54       64       c0       a6       bd       f3       f8       f9       0e       7c      c7 
//LSb:01000110 00101010 00100110 00000011 01100101 10111101 11001111 00011111 10011111 01110000 00111110 11100011
//010: BRCn=2
//00=+0 110=-1

#include <stdio.h>
#include <stdlib.h>
#include "reconstruct.h"

struct node {struct noeud* left; struct node* right;};

int next_bit(unsigned char *p,int *cposition,int *bposition)
{int bit=((p[*cposition]>>(*bposition))&1);
 (*bposition)--;
 if ((*bposition)<0) {(*cposition)++;(*bposition)=7;}
 return(bit);
}

void create_BRC4(struct node* root) // TODO
{
}

static int BRC(int BRCn,unsigned char *p,int *cposition,int *bposition)
{char hcode,sign;
 int b;
 switch (BRCn) {
   case 0: BRCn=3;break; // number of steps to reach the leaves
   case 1: BRCn=4;break; // number of steps to reach the leaves
   case 2: BRCn=6;break; // number of steps to reach the leaves
   case 3: BRCn=9;break; // number of steps to reach the leaves
   case 4: BRCn=15;printf("\nNOT SUPPORTED\n");exit(-1);break; // number of steps to reach the leaves
   default: printf("ERROR");exit(-1);
  }
 sign=next_bit(p,cposition,bposition);
 if (sign==0) sign=1; else sign=-1;
 hcode=0;
 do {
   b=next_bit(p,cposition,bposition);
   if (b==0)   // 0: end of tree
      if (BRCn==9)
        {if (hcode==0)
            return((int)(int)sign*(int)hcode);      // we reached 0 -> return
         else
            return((int)(int)sign*(int)(hcode+1));  // we reached 0 -> return
        }
      else
        return((int)(int)sign*(int)hcode);  // we reached 0 -> return
   else 
      {hcode++;                                // 1: continue
       if ((BRCn==9) && (hcode==1)) hcode++;
       if (hcode==BRCn)                        // unless last 1 was reached 
          return((int)((int)sign*(int)hcode));
      }
 } while (hcode<BRCn);
 return(99);
}

unsigned char get_THIDX(unsigned char *p,int *cposition,int *bposition)
{int res=0;
 int k;
 for (k=0;k<8;k++)
   {
    res=res<<1;
    res+=next_bit(p,cposition,bposition);
   }
 return(res);
}

int packet_decode(unsigned char *p,int NQ,float *IE, float *IO, float *QE, float *QO) // FDBAQ: section 4.4 p.67
{// IE 1st 3 bits = BRC
 // QE first 8 bits = THIDX
 int hcodeIE[52378];
 int hcodeIO[52378];
 int hcodeQE[52378];
 int hcodeQO[52378];
 unsigned char BRCn[410];   // max value p.55: 52378/128=409.2
 unsigned char THIDXn[410];
 int BRCindex;
 int h,hcode_index;
 int cposition=0,bposition=7;
 int inc=128;  // 128 samples until NQ is reached
 printf("\nstarting IE\n");
 BRCindex=0;
 hcode_index=0;
 do // repeat until end of packet: depends on NQ
  {//printf("\npos:%d:",bposition);
   BRCn[BRCindex]=next_bit(p,&cposition,&bposition)*4;  // MSb=4
   BRCn[BRCindex]+=next_bit(p,&cposition,&bposition)*2; // then 2
   BRCn[BRCindex]+=next_bit(p,&cposition,&bposition)*1; // then 1 ...
   printf("%d>",BRCn[BRCindex]);
   if ((hcode_index+128)>NQ) inc=(NQ-hcode_index);      // smaller increment to match NQ
   for (h=0;h<inc;h++) // p.68: 128 HCodes
     {hcodeIE[hcode_index]=BRC(BRCn[BRCindex],p,&cposition,&bposition); // 128 samples with same BRC
//      printf("%d",hcodeIE[hcode_index]);
      hcode_index++;
     }
   BRCindex++;
  }
 while (hcode_index<NQ);
 printf("\nIE finished, starting IO @ %d\n",cposition);
 inc=128;
 if (bposition!=7) {printf("bposition=%d->7\n",bposition);bposition=7;cposition++;} // start at new position
 if ((cposition & 1)!=0) {printf("cposition=%d++\n",cposition);cposition++;}        // odd address => +1 
 BRCindex=0;
 hcode_index=0;
 do 
  {
   if ((hcode_index+128)>NQ) inc=(NQ-hcode_index);                      // smaller increment to match NQ
   for (h=0;h<inc;h++) // p.68: 128 HCodes
     {hcodeIO[hcode_index]=BRC(BRCn[BRCindex],p,&cposition,&bposition); // 128 samples with same BRC
//      printf("%d",hcodeIO[hcode_index]);
      hcode_index++;
     }
   BRCindex++;
  }
 while (hcode_index<NQ);
 printf("IO finished, starting QE @ %d\n",cposition);
 inc=128;
 if (bposition!=7) {printf("bposition=%d\n",bposition);bposition=7;cposition++;} // start at new position
 if ((cposition & 1)!=0) {printf("cposition=%d++\n",cposition);cposition++;}     // odd address => +1 
 BRCindex=0;
 hcode_index=0;
 do 
  {THIDXn[BRCindex]=get_THIDX(p,&cposition,&bposition);
//   THIDXn[BRCindex]=p[cposition]; // 8-bit THIDX
   printf("#%d",THIDXn[BRCindex]);
   if ((hcode_index+128)>NQ) inc=(NQ-hcode_index);                      // smaller increment to match NQ
   for (h=0;h<inc;h++) // p.68: 128 HCodes
     {hcodeQE[hcode_index]=BRC(BRCn[BRCindex],p,&cposition,&bposition); // 128 samples with same BRC
//      printf("%d",hcodeQE[hcode_index]);
      hcode_index++;
     }
   BRCindex++;
  }
 while (hcode_index<NQ);
 printf("\nQE finished, starting QO @ %d\n",cposition);
 inc=128;
 if (bposition!=7) {printf("bposition=%d\n",bposition);bposition=7;cposition++;} // start at new position
 if ((cposition & 1)!=0) {printf("cposition=%d++\n",cposition);cposition++;}     // odd address => +1 
 BRCindex=0;
 hcode_index=0;
 do
  {
   if ((hcode_index+128)>NQ) inc=(NQ-hcode_index);                      // smaller increment to match NQ
   for (h=0;h<inc;h++) // p.68: 128 HCodes
     {hcodeQO[hcode_index]=BRC(BRCn[BRCindex],p,&cposition,&bposition); // 128 samples with same BRC
//      printf("%d",hcodeQO[hcode_index]);
      hcode_index++;
     }
   BRCindex++;
  }
 while (hcode_index<NQ);
 if (bposition!=7) {printf("bposition=%d->7\n",bposition);bposition=7;cposition++;} // start at new position
 if ((cposition & 1)!=0) {printf("cposition=%d++\n",cposition);cposition++;}        // odd address => +1 
 printf("finished at %d",cposition);
 reconstruction(BRCn,THIDXn,hcodeIE,NQ,IE);
 reconstruction(BRCn,THIDXn,hcodeIO,NQ,IO);
 reconstruction(BRCn,THIDXn,hcodeQE,NQ,QE);
 reconstruction(BRCn,THIDXn,hcodeQO,NQ,QO);
 return(cposition);
}
