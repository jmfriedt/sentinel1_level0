void packet_decode(unsigned char *p) // FDBAQ: 
{
 char BRC,sign,hcode;
 int cposition=0,bposition=4;
 BRC=p[0]&0x7;  // IE 1st 3 bits = BRC
 sign=(p[cposition]>>bposition);bposition++;
 hcode=0;
 
  // QE first 8 bits = THIDX
} 
