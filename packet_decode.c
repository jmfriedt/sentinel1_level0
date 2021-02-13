//0000: 62        54       64       c0       a6       bd       f3       f8       f9       0e       7c      c7 
//LSb:01000110 00101010 00100110 00000011 01100101 10111101 11001111 00011111 10011111 01110000 00111110 11100011
//010: BRCn=2
//00=+0 110=-1

#include <stdio.h>

struct node {struct noeud* left; struct node* right;};

int next_bit(unsigned char *p,int *cposition,int *bposition)
{int bit=((p[*cposition]>>(*bposition))&1);
 (*bposition)--;
 if ((*bposition)<0) {(*cposition)++;(*bposition)=7;}
 return(bit);
}

void create_BRC4(struct node* root)
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
   case 4: BRCn=15;printf("\nNOT SUPPORTED\n");break; // number of steps to reach the leaves
   default: printf("ERROR");return(-1);
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

void packet_decode(unsigned char *p,int NQ) // FDBAQ: section 4.4 p.67
{// IE 1st 3 bits = BRC
 // QE first 8 bits = THIDX
 unsigned char BRCn;
 int hcode[128];
 int hcode_index,packet_index;
 int cposition=0,bposition=7;
// TODO: repeat until end of packet ! depends on NQ
for (packet_index=0;packet_index<NQ;packet_index+=128)
{//printf("\npos:%d:",bposition);
 BRCn=next_bit(p,&cposition,&bposition)*4;  // MSb=4
 BRCn+=next_bit(p,&cposition,&bposition)*2; // then 2
 BRCn+=next_bit(p,&cposition,&bposition)*1; // then 1 ...
 printf("%d>",BRCn);
 for (hcode_index=0;hcode_index<128;hcode_index++) // p.68: 128 HCodes
   {hcode[hcode_index]=BRC(BRCn,p,&cposition,&bposition);
    //printf("%d",hcode[hcode_index]);
   }
}
  // QE first 8 bits = THIDX
} 
