//0000: 62        54       64       c0       a6       bd       f3       f8       f9       0e       7c      c7 
//LSb:01000110 00101010 00100110 00000011 01100101 10111101 11001111 00011111 10011111 01110000 00111110 11100011
//010: BRCn=2
//00=+0 110=-1

#include <stdio.h>
#include <stdlib.h>
#include "reconstruct.h"

//#define msg printf
#define msg(...)

//#define dump_payload

int next_bit(unsigned char *p,int *cposition,int *bposition)
{int bit=((p[*cposition]>>(*bposition))&1);
 (*bposition)--;
 if ((*bposition)<0) {(*cposition)++;(*bposition)=7;}
 return(bit);
}

static struct sh_code BRC4(unsigned char *p,int *cposition,int *bposition) // TODO: never tested !
{int hcode,sign;
 struct sh_code sol;
 int b;
 sign=next_bit(p,cposition,bposition);
 if (sign==0) sol.sign=1; else sol.sign=-1;
 hcode=0;
 do {
   b=next_bit(p,cposition,bposition);
   switch (hcode) 
    {case 5:
       if (b==0) // must be BEFORE hcode=5 at bottom
          {b=next_bit(p,cposition,bposition);
           if (b==0) {sol.mcode=5;return(sol);}  // BRC4,1100
           else      {sol.mcode=6;return(sol);}  // BRC4,1101
          }
       else hcode=6;
     break;
     case 6:
     case 7:
     case 8:
       if (b==0) {sol.mcode=hcode+1;return(sol);}
       else hcode++;
     break;
     case 9:
       if (b==0) 
          {b=next_bit(p,cposition,bposition);
           if (b==0) {sol.mcode=10;return(sol);} // BRC4,11111100
           else      {sol.mcode=11;return(sol);} // BRC4,11111101
          }
       else hcode++;
     break;
     case 10:
       if (b==0) 
          {b=next_bit(p,cposition,bposition);
           if (b==0) {sol.mcode=12;return(sol);} // BRC4,111111100
           else      {sol.mcode=13;return(sol);} // BRC4,111111101
          }
       else 
          {b=next_bit(p,cposition,bposition);
           if (b==0) {sol.mcode=14;return(sol);} // BRC4,111111100
           else      {sol.mcode=15;return(sol);} // BRC4,111111101
          }
     break;
     case 0:
       if (b==0)   // first 0
          {b=next_bit(p,cposition,bposition);
           if (b==0) {sol.mcode=0;return(sol);}  // BRC4,00 
           else
              {b=next_bit(p,cposition,bposition);
               if (b==0) {sol.mcode=1;return(sol);} // BRC4,010
               else      {sol.mcode=2;return(sol);} // BRC4,011
              }
          }
       else
          {b=next_bit(p,cposition,bposition);
           if (b==0)  // BRC4,00 
              {b=next_bit(p,cposition,bposition);
               if (b==0) {sol.mcode=3;return(sol);} // BRC4,100
               else      {sol.mcode=4;return(sol);} // BRC4,101
              }
           else hcode=5;
          }
     break;
    }
 } while (hcode<=15);
 sol.mcode=999;
 exit(-1);    // should never get here
 return(sol);
}

static struct sh_code BRC(int BRCn,unsigned char *p,int *cposition,int *bposition)
{int hcode;
 int sign;
 int b;
 struct sh_code sol;
 switch (BRCn) {
   case 0: BRCn=3;break; // number of steps to reach the leaves BRC0
   case 1: BRCn=4;break; // number of steps to reach the leaves BRC1
   case 2: BRCn=6;break; // number of steps to reach the leaves BRC2
   case 3: BRCn=9;break; // number of steps to reach the leaves BRC3
   case 4: return(BRC4(p,cposition,bposition));printf("\nCheck if BRC4 output is correct\n");exit(0);break;
   default: printf("ERROR");exit(-1);
  }
 sign=next_bit(p,cposition,bposition);
 if (sign==0) sol.sign=1; else sol.sign=-1;
 hcode=0;
 do {
   b=next_bit(p,cposition,bposition);
   if (b==0)                                   // 0: end of tree
      if ((BRCn==9) && (hcode==0))             // first 0 (hcode==0) of BRC3
        {b=next_bit(p,cposition,bposition);
         if (b==0)
            {sol.mcode=hcode;return(sol);}     // we reached 00 of BRC3
         else
            {sol.mcode=hcode+1;return(sol);}   // we reached 01 of BRC3
        }
      else
        {sol.mcode=hcode;return(sol);}         // we reached 0 -> return
   else 
      {hcode++;                                // 1: continue
       if ((BRCn==9) && (hcode==1)) hcode++;
       if (hcode==BRCn)                        // unless last 1 was reached 
          {sol.mcode=hcode;return(sol);}       // end of tree reached
      }
 } while (hcode<BRCn);
 exit(-1);                                     // ERROR in decoding Huffman
 sol.mcode=99;
 return(sol);                                  // should never be reached
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

int packet_decode(unsigned char *p,int NQ,float *IE, float *IO, float *QE, float *QO,char *brc,int *brcpos) // FDBAQ: section 4.4 p.67
{// IE 1st 3 bits = BRC
 // QE first 8 bits = THIDX
 struct sh_code hcodeIE[52378];
 struct sh_code hcodeIO[52378];
 struct sh_code hcodeQE[52378];
 struct sh_code hcodeQO[52378];
 unsigned char BRCn[410];   // max value p.55: 52378/128=409.2
 unsigned char THIDXn[410];
 int BRCindex;
 int h,hcode_index;
 int cposition=0,bposition=7;
 int inc=128;  // 128 samples until NQ is reached
 msg("\nstarting IE\n");
 BRCindex=0;
 hcode_index=0;
 do // repeat until end of packet: depends on NQ
  {//msg("\npos:%d:",bposition);
   BRCn[BRCindex]=next_bit(p,&cposition,&bposition)*4;  // MSb=4
   BRCn[BRCindex]+=next_bit(p,&cposition,&bposition)*2; // then 2
   BRCn[BRCindex]+=next_bit(p,&cposition,&bposition)*1; // then 1 ...
#ifdef dump_payload
   msg("\n");
#endif
   msg("%d>",BRCn[BRCindex]);
   brc[*brcpos]=BRCn[BRCindex];
   (*brcpos)++;
   if ((hcode_index+128)>NQ) inc=(NQ-hcode_index);      // smaller increment to match NQ
   for (h=0;h<inc;h++) // p.68: 128 HCodes
     {hcodeIE[hcode_index]=BRC(BRCn[BRCindex],p,&cposition,&bposition); // 128 samples with same BRC
#ifdef dump_payload
      msg("%d",hcodeIE[hcode_index]);
#endif
      hcode_index++;
     }
   BRCindex++;
  }
 while (hcode_index<NQ);
 msg("\nIE finished, starting IO @ %d\n",cposition);
 inc=128;
 if (bposition!=7) {msg("bposition=%d->7\n",bposition);bposition=7;cposition++;} // start at new position
 if ((cposition & 1)!=0) {msg("cposition=%d++\n",cposition);cposition++;}        // odd address => +1 
 BRCindex=0;
 hcode_index=0;
 do 
  {
   if ((hcode_index+128)>NQ) inc=(NQ-hcode_index);                      // smaller increment to match NQ
   for (h=0;h<inc;h++) // p.68: 128 HCodes
     {hcodeIO[hcode_index]=BRC(BRCn[BRCindex],p,&cposition,&bposition); // 128 samples with same BRC
//      msg("%d",hcodeIO[hcode_index]);
      hcode_index++;
     }
   BRCindex++;
  }
 while (hcode_index<NQ);
 msg("IO finished, starting QE @ %d\n",cposition);
 inc=128;
 if (bposition!=7) {msg("bposition=%d\n",bposition);bposition=7;cposition++;} // start at new position
 if ((cposition & 1)!=0) {msg("cposition=%d++\n",cposition);cposition++;}     // odd address => +1 
 BRCindex=0;
 hcode_index=0;
 do 
  {THIDXn[BRCindex]=get_THIDX(p,&cposition,&bposition);
//   THIDXn[BRCindex]=p[cposition]; // 8-bit THIDX
   msg("#%d",THIDXn[BRCindex]);
   if ((hcode_index+128)>NQ) inc=(NQ-hcode_index);                      // smaller increment to match NQ
   for (h=0;h<inc;h++) // p.68: 128 HCodes
     {hcodeQE[hcode_index]=BRC(BRCn[BRCindex],p,&cposition,&bposition); // 128 samples with same BRC
//      msg("%d",hcodeQE[hcode_index]);
      hcode_index++;
     }
   BRCindex++;
  }
 while (hcode_index<NQ);
 msg("\nQE finished, starting QO @ %d\n",cposition);
 inc=128;
 if (bposition!=7) {msg("bposition=%d\n",bposition);bposition=7;cposition++;} // start at new position
 if ((cposition & 1)!=0) {msg("cposition=%d++\n",cposition);cposition++;}     // odd address => +1 
 BRCindex=0;
 hcode_index=0;
 do
  {
   if ((hcode_index+128)>NQ) inc=(NQ-hcode_index);                      // smaller increment to match NQ
   for (h=0;h<inc;h++) // p.68: 128 HCodes
     {hcodeQO[hcode_index]=BRC(BRCn[BRCindex],p,&cposition,&bposition); // 128 samples with same BRC
//      msg("%d",hcodeQO[hcode_index]);
      hcode_index++;
     }
   BRCindex++;
  }
 while (hcode_index<NQ);
 if (bposition!=7) {msg("bposition=%d->7\n",bposition);bposition=7;cposition++;} // start at new position
 if ((cposition & 1)!=0) {msg("cposition=%d++\n",cposition);cposition++;}        // odd address => +1 
 msg("finished at %d",cposition);
 reconstruction(BRCn,THIDXn,hcodeIE,NQ,IE);
 reconstruction(BRCn,THIDXn,hcodeIO,NQ,IO);
 reconstruction(BRCn,THIDXn,hcodeQE,NQ,QE);
 reconstruction(BRCn,THIDXn,hcodeQO,NQ,QO);
 return(cposition);
}
