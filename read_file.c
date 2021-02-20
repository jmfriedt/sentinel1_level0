/*
./read_file S1A_IW_RAW__0SDV_20210112T173201_20210112T173234_036108_043B95_7EA4.SAFE/s1a-iw-raw-s-vv-20210112t173201-20210112t173234-036108-043b95.dat

./read_file S1_L0_Decoding_Package/S1A_IW_RAW__0SDV_20200608T101309_20200608T101341_032924_03D05A_A50C.SAFE/s1a-iw-raw-s-vv-20200608t101309-20200608t101341-032924-03d05a.dat

$ xxd s1a-iw-raw-s-vv-20210112t173201-20210112t173234-036108-043b95.dat | head -5
00000000: 0c1c fb58 4159 4d28 9fa3 caa0 352e f853  ...XAYM(....5..S
00000010: 0877 2a60 0800 0000 0006 396a 6700 06bb  .w*`......9jg...
00000020: 5800 06c6 7b0c 1f00 0b08 8488 2932 0009  X...{.......)2..
00000030: 1708 0065 0100 158d 0040 f470 7188 0700  ...e.....@.pq...
00000040: 0b2f 1700 321b c65d c8c5 1663 9f51 38b5  ./..2..]...c.Q8.

SAR Space Protocol Data Unit p.14/85

/!\ page 10: bit 0 is the most significant bit !!!
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <arpa/inet.h> // htonl for proper endianness

#include "packet_decode.h"
//#define dump_payload

int main(int argc, char **argv)
{int f,res; // ,k 
// unsigned short *suser;
 u_int16_t c,tmp16,NQ=0;
 u_int32_t tmp32;
 int Secondary;
 int Count,DataLen,PID,PCAT,Sequence;
 char tablo[65536],BAQ,Typ;
 int cal_p,cposition;
 unsigned char *user;
 float IE[52378]; // results
 float IO[52378];
 float QE[52378];
 float QO[52378];
 FILE *result;
#ifdef dump_payload
 int fo;
#endif
 result=fopen("result.dat","w");
 fprintf(result,"# Created by myself\n# name: x\n# type: complex matrix\n# rows: \n# columns: ");


 //int fuser; char filename[256];

 if (argc<2) return(1);
 f=open(argv[1],O_RDONLY);
 do
 {
  // Begin SAR Space Protocol Data Unit p.14/85
  read(f,&c,2);    // 0x0c = Pack_Ver Pack_Typ Secondary PID PCAT 
  c=htons(c);
  // On the i386 the host byte order is Least Significant Byte first, whereas the network byte
  // order, as used on the Internet, is Most Significant Byte first
  Secondary=(c>>11)&0x01;
  PID=(c>>4)&0x7f;
  PCAT=(c)&0xf;
  printf("%04x: %d(1)\t%d(65)\t%d(12)\t",c,Secondary,PID,PCAT);
 
  read(f,&c,2);    // 0x0c = Pack_Ver Pack_Typ Secondary PID PCAT 
  c=htons(c);
  Sequence=(c>>14);
  Count=(c&0x3f);
  
  read(f,&c,2);    // 0x0c = Pack_Ver Pack_Typ Secondary PID PCAT 
  DataLen=htons(c)+1;
  printf("%04x: %x(3)\tCount=%d\tLen=%d(61..65533)\n",c,Sequence,Count,DataLen);
  if (((DataLen+6)%4)!=0) printf("\nERROR: Length not multiple of 4\n");
  res=read(f,tablo,DataLen);
  // End SAR Space Protocol Data Unit p.14/85 : we have the payload ... now analyze tablo
 
  // The Secondary head will be the next 62 bytes
  // https://www.andrews.edu/~tzs/timeconv/timedisplay.php
  // Time Conversion Results Jan 12, 2021 17:32:01 UTC  =  GPS Time 1294507939
  // coarse and fine time field             (6 bytes)
  tmp32=*(u_int32_t*)tablo;tmp32=htonl(tmp32);
  printf("\tTime: %d",tmp32);              // Coarse Time = begin+6 1294507939
  tmp16=*(u_int16_t*)(tablo+4);tmp16=htons(tmp16);
  printf(":%d",tmp16);                     // Fine Time   = begin+10, *2^(-16) s
  // Fixed ancillary data service           (14 bytes)
  tmp32=*(u_int32_t*)(tablo+6);tmp32=htonl(tmp32);
  printf("\t%08x(352EF853)",tmp32);        // Sync Marker = begin+12, 352EF853
  if (tmp32!=0x352EF853) printf("\nERROR: Sync marker != 352EF853");
  tmp32=*(u_int32_t*)(tablo+10);tmp32=htonl(tmp32);
  //printf(" %08x",tmp32);                 // Data Take ID= begin+16 (TBD)
  //printf("\t%hhx",*(u_int8_t*)tablo+14); // ECC Number  = begin+20  ?! /!\ ?!
  //printf("\t%hhx",*(u_int8_t*)tablo+15); // TestMode/RXID=begin+21
  tmp32=*(u_int32_t*)(tablo+16);tmp32=htonl(tmp32);
  //printf("\t%08x",tmp32);                  // Config ID   = begin+22 (TBD)
  // page 22

  // Sub-commutation ancillary data: PVT/attitude will be accumulated as 42 words along headers
  // p.27: Counter+value                    (3 bytes)
  printf("\tWordIndex=%hhx",*(u_int8_t*)(tablo+20)); // Word index will increment from 1 to 0x40 to fill
  tmp16=*(u_int16_t*)(tablo+21);tmp16=htons(tmp16);  // the array described in p.23 with ... vvv
  printf("\tWordVal=%hx",tmp16);                   // Word value

  // Counter Service                       (8 bytes)
  tmp32=*(u_int32_t*)(tablo+23);tmp32=htonl(tmp32);
  printf("\t%08x",tmp32);                  // Space packet count (4 bytes)
  tmp32=*(u_int32_t*)(tablo+27);tmp32=htonl(tmp32);
  printf(" %08x",tmp32);                   // PRI count (4 bytes)
  BAQ=(*(u_int8_t*)(tablo+31))&0x1f;       // BAQ mode 0x0c=FDBAQ mode0 nominal p.33: 
  printf(" BAQ=%02x(c)", (*(u_int8_t*)(tablo+31))); 
  // p.32: RADAR Configuration Support     (28 bytes) -> total=31+28=59
  printf(" BlockLen=%hhx(1F)",(*(u_int8_t*)(tablo+32))); 
  cal_p=(*(u_int8_t*)(tablo+53))&1; 
  // printf(" cal=%01x(0)", cal_p);        // SSB Data calibration (p.47) 
  Typ=(*(u_int8_t*)(tablo+57));            // p.52: signal type
  printf(" Typ=%hhx(0)",Typ); 

  // p.54 RADAR Sample Count               (2 bytes)
  if (NQ==0)
     {NQ=*(u_int16_t*)(tablo+59);NQ=htons(NQ); // number of quads NQ => Nsamples=2xNQ
      fprintf(result,"%d\n",2*NQ);
     }
  else
     {NQ=*(u_int16_t*)(tablo+59);NQ=htons(NQ);}// number of quads NQ => Nsamples=2xNQ
  printf(" NQ=%d\n",NQ);
  // tablo+61 = n/a (p.54: index 67)

  // p.56 User Data Field -- DataLen-62 ; 4 sections with IE, IO, QE, QO (NOT interleaved)
  user=(u_int8_t*)(tablo+62);  // User Data Field starts @ end of Secondary Header
  // p.58: format D is nominally used to output radar echo data = Decimation + FDBAQ
  // NO BYTESWAP SINCE WE WORK BIT BY BIT: Keep bytes in read() order
#ifdef dump_payload
  fo=open("/tmp/payload.bin",O_WRONLY|O_CREAT|O_TRUNC,0644); // TRUNC = remove old data
  write(fo,user,DataLen-62);
  close(fo);
#endif
  if ((BAQ==0x0c)&&(Typ==0)) cposition=packet_decode(user,NQ,IE,IO,QE,QO); 
  for (cal_p=0;cal_p<NQ;cal_p++) fprintf(result,"(%f,%f) (%f,%f) ",IE[cal_p],QE[cal_p],IO[cal_p],QO[cal_p]); // p.75: E then O
  fprintf(result,"\n");fflush(result); // manually fill # rows: entry <- grep -v ^# result.dat | wc -l
  printf(", finished processing %d\n",DataLen-62); 
  if ((DataLen-62-cposition)>2) {printf("Not enough data processed\n");exit(-1);}
 } while ((res>0)); // until EOF
 close(f);
}

// Sentinel-1-Level-1-Detailed-Algorithm-Definition.pdf page 27
/*
  •    I/Q bias removal
  •    I/Q gain imbalance correction
  •    I/Q non-orthogonality correction
For Sentinel-1 however, the instrument’s receive module performs the demodulation
in the digital domain, therefore the I/Q gain imbalance and I/Q non-orthogonality
corrections are no longer necessary.

p.33
The calibration signals for both image and PG calibration are contained in dedicated
packets. The image calibration signals are sampled at the sampling frequency of the
measurement data (which is the sampling frequency after the decimation filtering)
while the PG calibration signals are sampled at 100 MHz. The calibration signals are
not BAQ or FDBAQ encoded, so that they can be used directly (see Section 9.1.2).
*/
