/*
./read_file ./saopaolo_SM/S1A_S6_RAW__0SDV_20210226T214354_20210226T214424_036766_045287_FD68.SAFE/s1a-s6-raw-s-vv-20210226t214354-20210226t214424-036766-045287.dat

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

cat result.dat  | sed 's/[0-9]//g' | sed 's/-//g' | sed 's/\.//g' | sed 's/(//g' | sed 's/)//g'  | sed 's/,/0/g' > TTT

*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <math.h>
#include <arpa/inet.h> // ntohl for proper endianness

#include "packet_decode.h"
#include "bypass.h"
//#define dump_payload

#define fref 37.53472224

int main(int argc, char **argv)
{int f,res; // ,k 
// unsigned short *suser;
 u_int16_t c,tmp16,NQ=0;
 u_int32_t tmp32,Time;
 int Secondary;
 int Count,DataLen,PID,PCAT,Sequence;
 unsigned char tablo[65536],BAQ,Swath,Typ;
 int cal_p,cposition,brcpos;
 unsigned char *user;
 float IE[52378]; // results
 float IO[52378];
 float QE[52378];
 float QO[52378];
 char brc[52378];
 // FILE *result;
 // FILE *brcfile;
 int brcfile;
 int result;
 int numline=0;
 int file_swath_number=0,file_nq=0; // for new file creation
 char filename[255];
#ifdef dump_payload
 int fo;
#endif
 if (argc<2) return(1);
 f=open(argv[1],O_RDONLY);
 // result=fopen("result.dat","w");
 // brcfile=fopen("brc.dat","w");
 // fprintf(result,"# Created by myself\n# name: x\n# type: complex matrix\n# rows: \n# columns: ");
 do
 {
  // Begin SAR Space Protocol Data Unit p.14/85
  read(f,&c,2);    // 0x0c = Pack_Ver Pack_Typ Secondary PID PCAT 
  c=ntohs(c);
  // On the i386 the host byte order is Least Significant Byte first, whereas the network byte
  // order, as used on the Internet, is Most Significant Byte first
  Secondary=(c>>11)&0x01;
  PID=(c>>4)&0x7f;
  PCAT=(c)&0xf;
  printf("%04x: %d(1)\t%d(65)\t%d(12)\t",c,Secondary,PID,PCAT);
 
  read(f,&c,2);    // 0x0c = Pack_Ver Pack_Typ Secondary PID PCAT 
  c=ntohs(c);
  Sequence=(c>>14);
  Count=(c&0x3f);
  
  read(f,&c,2);    // 0x0c = Pack_Ver Pack_Typ Secondary PID PCAT 
  DataLen=ntohs(c)+1;
  printf("%04x: %x(3)\tCount=%02d\tLen=%d(61..65533)\n",c,Sequence,Count,DataLen);
  if (((DataLen+6)%4)!=0) printf("\nERROR: Length not multiple of 4\n");
  res=read(f,tablo,DataLen);
  // End SAR Space Protocol Data Unit p.14/85 : we have the payload ... now analyze tablo
 
  // The Secondary head will be the next 62 bytes
  // https://www.andrews.edu/~tzs/timeconv/timedisplay.php
  // Time Conversion Results Jan 12, 2021 17:32:01 UTC  =  GPS Time 1294507939
  // coarse and fine time field             (6 bytes)
  tmp32=*(u_int32_t*)tablo;Time=ntohl(tmp32);
  printf("\tTime: %d",Time);               // Coarse Time = begin+6 1294507939
  tmp16=*(u_int16_t*)(tablo+4);tmp16=ntohs(tmp16);
  printf(":%d",tmp16);                     // Fine Time   = begin+10, *2^(-16) s
  // Fixed ancillary data service           (14 bytes)
  tmp32=*(u_int32_t*)(tablo+6);tmp32=ntohl(tmp32);
  printf("\t%08x(352EF853)",tmp32);        // Sync Marker = begin+12, 352EF853
  if (tmp32!=0x352EF853) printf("\nERROR: Sync marker != 352EF853");
  tmp32=*(u_int32_t*)(tablo+10);tmp32=ntohl(tmp32);
  //printf(" %08x",tmp32);                 // Data Take ID= begin+16 (TBD)
  //printf("\t%hhx",*(u_int8_t*)tablo+14); // ECC Number  = begin+20  ?! /!\ ?!
  //printf("\t%hhx",*(u_int8_t*)tablo+15); // TestMode/RXID=begin+21
  tmp32=*(u_int32_t*)(tablo+16);tmp32=ntohl(tmp32);
  //printf("\t%08x",tmp32);                  // Config ID   = begin+22 (TBD)
  // page 22

  // Sub-commutation ancillary data: PVT/attitude will be accumulated as 42 words along headers
  // p.27: Counter+value                    (3 bytes)
  printf("\tWordIndex=%hhx",*(u_int8_t*)(tablo+20)); // Word index will increment from 1 to 0x40 to fill
  tmp16=*(u_int16_t*)(tablo+21);tmp16=ntohs(tmp16);  // the array described in p.23 with ... vvv
  printf("\tWordVal=%hx",tmp16);                   // Word value

  // Counter Service                       (8 bytes)
  tmp32=*(u_int32_t*)(tablo+23);tmp32=ntohl(tmp32);
  printf("\t%08x",tmp32);                  // Space packet count (4 bytes)
  tmp32=*(u_int32_t*)(tablo+27);tmp32=ntohl(tmp32);
  printf(" %08x",tmp32);                   // PRI count (4 bytes)
  BAQ=(*(u_int8_t*)(tablo+31))&0x1f;       // BAQ mode 0x0c=FDBAQ mode0 nominal p.33: 
  printf(" BAQ=%02x(c)", (*(u_int8_t*)(tablo+31))); 
  // p.32: RADAR Configuration Support     (28 bytes) -> total=31+28=59
  printf(" BlockLen=%hhx(1F)\n",(*(u_int8_t*)(tablo+32))); 
 
  printf("\tDecim=%hhx",*(u_int8_t*)(tablo+34));     // RGDEC
  tmp16=*(u_int16_t*)(tablo+36);tmp16=ntohs(tmp16);  // Tx Pulse Ramp Rate
  if ((tmp16&0x8000)==0) printf("\tTXPRR=v%x=",tmp16);else printf("\tTXPRR=^%x=",tmp16);
  printf("%d",tmp16&0x7fff); 
  tmp16=*(u_int16_t*)(tablo+38);tmp16=ntohs(tmp16);  // Tx Pulse Start Freq
  if ((tmp16&0x8000)==0) printf("\tTXPSF=-0x%x",tmp16);else printf("\tTXPSF=+0x%x",tmp16); // 0 negative, 1 positive ?!
  printf("=%d ",tmp16&0x7fff);             // Word value
  tmp32=*(u_int32_t*)(tablo+40);tmp32=ntohl(tmp32)>>8; // keep last 24 bits
  printf("TXPL=%08x=%d ",tmp32,tmp32);     // TX Pulse length (3 bytes)
  tmp32=*(u_int32_t*)(tablo+44);tmp32=ntohl(tmp32)>>8; // keep last 24 bits
  printf("PRI=%08x=%d",tmp32,tmp32);       // PRI, needed for Doppler centroid analysis (3 bytes)
  
  cal_p=((*(u_int8_t*)(tablo+53))>>4)&0x07;
  printf(" Polar=%x", cal_p);              // SSB Data calibration (p.47) 
  Typ=(*(u_int8_t*)(tablo+57));Typ=Typ>>4; // p.52: signal type
  printf(" Typ=%hhx(0)",Typ); 
  Swath=(*(u_int8_t*)(tablo+58));          // p.54: swath number
  printf(" Swath=%hhx",Swath);
                                           // p.54 RADAR Sample Count (2 bytes)
  NQ=*(u_int16_t*)(tablo+59);NQ=ntohs(NQ); // number of quads NQ => Nsamples=2xNQ 
  if ((Swath!=file_swath_number)||(file_nq!=NQ))
   {if (file_swath_number!=0)
       {close(result);close(brcfile);printf("\nFILE written %d %d\n",2*NQ,numline);numline=0;}
    file_swath_number=Swath;
    file_nq=NQ;
    sprintf(filename,"resultSW%02d_T%d_NQ%d.bin",Swath,Time,NQ);
    result=open(filename,O_WRONLY|O_CREAT|O_TRUNC,0644); // TRUNC = remove old data
    sprintf(filename,"brcSW%02d_T%d.bin",Swath,Time);
    brcfile=open(filename,O_WRONLY|O_CREAT|O_TRUNC,0644);
   }
  printf(" NQ=%d\n",NQ);
  // if (NQ==0) fprintf(result,"%d\n",2*NQ);
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

  if ((BAQ==0x00)&&(Typ>7))    // calibration (p.52: Typ>7 for cal, and p.33: BAQMOD=0=BYPASS for cal)
     {cposition=bypass(user,NQ,IE,IO,QE,QO); 
      for (cal_p=0;cal_p<NQ;cal_p++) 
         {write(result,&IE[cal_p],sizeof(float));
          write(result,&QE[cal_p],sizeof(float));
          write(result,&IO[cal_p],sizeof(float));
          write(result,&QO[cal_p],sizeof(float));
         }
      printf(", finished processing %d calibration\n",DataLen-62); 
      if ((DataLen-62-cposition)>2) {printf("Not enough data processed: DataLen %d v.s. cposition %d\n",DataLen-62,cposition);exit(-1);}
     }
  if ((BAQ==0x0c)&&(Typ==0))   // echo data (p.52: Typ=0 for echo data and p.33 BAQ Mode=0x0C => nominal FDBAQ p.67)
     {brcpos=0;
      cposition=packet_decode(user,NQ,IE,IO,QE,QO,brc,&brcpos); 
      for (cal_p=0;cal_p<NQ;cal_p++) 
         {write(result,&IE[cal_p],sizeof(float));
          write(result,&QE[cal_p],sizeof(float));
          write(result,&IO[cal_p],sizeof(float));
          write(result,&QO[cal_p],sizeof(float));
         }
   // fprintf(result,"(%f,%f) (%f,%f) ",IE[cal_p],QE[cal_p],IO[cal_p],QO[cal_p]); // p.75: E then O
      for (cal_p=0;cal_p<brcpos;cal_p++) write(brcfile,&brc[cal_p],1);
                                      // fprintf(brcfile,"%d ",brc[cal_p]); 
      // fprintf(result,"\n");fflush(result); // manually fill # rows: entry <- grep -v ^# result.dat | wc -l
      // fprintf(brcfile,"\n");fflush(brcfile); 
      printf(", finished processing %d echo\n",DataLen-62); 
      if ((DataLen-62-cposition)>2) {printf("Not enough data processed: DataLen %d v.s. cposition %d\n",DataLen-62,cposition);exit(-1);}
     }
  numline++;
 } while ((res>0)); // until EOF
 close(f);
 close(brcfile);
 close(result);
 printf("That's all folks, the end: %dx%d samples written\n",2*NQ,numline);
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
