#include <stdio.h>
#include <stdlib.h>
#include "packet_decode.h"

// p.78
float BRC0[4]={3.,3.,3.16,3.53};
float BRC1[4]={4.,4.,4.08,4.37};
float BRC2[6]={6.,6.,6.,6.15, 6.5,6.88};
float BRC3[7]={9.,9.,9.,9.,9.36,9.50, 10.1};
float BRC4[9]={15.,15.,15.,15.,15.,15., 15.22, 15.50, 16.05};

// p.79
float NRL0[4]={.3637,1.0915,1.8208,2.6406};
float NRL1[5]={.3042,.9127,1.5216,2.1313,2.8426};
float NRL2[7]={.2305,.6916,1.1528,1.6140,2.0754,2.5369,3.1191};
float NRL3[10]={.1702,.5107,.8511,1.1916,1.5321,1.8726,2.2131,2.5536,2.8942,3.3744};
float NRL4[16]={.1130,.3389,.5649,.7908,1.0167,1.2428,1.4687,1.6947,1.9206,2.1466,2.3725,2.5985,2.8244,3.0504,3.2764,3.6623};

// p.80
float SF[256] = {0., 0.630, 1.250, 1.880, 2.510, 3.130, 3.760, 4.390, 5.010, 5.640, 6.270, 6.890, 7.520, 8.150, 8.770, 9.40, 10.030, 10.650, 11.280, 11.910, 12.530, 13.160, 13.790, 14.410, 15.040, 15.670, 16.290, 16.920, 17.550, 18.170, 18.80, 19.430, 20.050, 20.680, 21.310, 21.930, 22.560, 23.190, 23.810, 24.440, 25.070, 25.690, 26.320, 26.950, 27.570, 28.20, 28.830, 29.450, 30.080, 30.710, 31.330, 31.960, 32.590, 33.210, 33.840, 34.470, 35.090, 35.720, 36.350, 36.970, 37.60, 38.230, 38.850, 39.480, 40.110, 40.730, 41.360, 41.990, 42.610, 43.240, 43.870, 44.490, 45.120, 45.750, 46.370, 47., 47.630, 48.250, 48.880, 49.510, 50.130, 50.760, 51.390, 52.010, 52.640, 53.270, 53.890, 54.520, 55.150, 55.770, 56.40, 57.030, 57.650, 58.280, 58.910, 59.530, 60.160, 60.790, 61.410, 62.040, 62.980, 64.240, 65.490, 66.740, 68., 69.250, 70.50, 71.760, 73.010, 74.260, 75.520, 76.770, 78.020, 79.280, 80.530, 81.780, 83.040, 84.290, 85.540, 86.80, 88.050, 89.30, 90.560, 91.810, 93.060, 94.320, 95.570, 96.820, 98.080, 99.330, 100.580, 101.840, 103.090, 104.340, 105.60, 106.850, 108.10, 109.350, 110.610, 111.860, 113.110, 114.370, 115.620, 116.870, 118.130, 119.380, 120.630, 121.890, 123.140, 124.390, 125.650, 126.90, 128.150, 129.410, 130.660, 131.910, 133.170, 134.420, 135.670, 136.930, 138.180, 139.430, 140.690, 141.940, 143.190, 144.450, 145.70, 146.950, 148.210, 149.460, 150.710, 151.970, 153.220, 154.470, 155.730, 156.980, 158.230, 159.490, 160.740, 161.990, 163.250, 164.50, 165.750, 167.010, 168.260, 169.510, 170.770, 172.020, 173.270, 174.530, 175.780, 177.030, 178.290, 179.540, 180.790, 182.050, 183.30, 184.550, 185.810, 187.060, 188.310, 189.570, 190.820, 192.070, 193.330, 194.580, 195.830, 197.090, 198.340, 199.590, 200.850, 202.10, 203.350, 204.610, 205.860, 207.110, 208.370, 209.620, 210.870, 212.130, 213.380, 214.630, 215.890, 217.140, 218.390, 219.650, 220.90, 222.150, 223.410, 224.660, 225.910, 227.170, 228.420, 229.670, 230.930, 232.180, 233.430, 234.690, 235.940, 237.190, 238.450, 239.70, 240.950, 242.210, 243.460, 244.710, 245.970, 247.220, 248.470, 249.730, 250.980, 252.230, 253.490, 254.740, 255.990, 255.990};

void reconstruction(unsigned char *BRCn,unsigned char *THIDXn,struct sh_code *hcode,int NQ,float *result)
{int hcode_index=0,h;
 int BRCindex=0;
 int inc=128;
 do
  {
   if ((hcode_index+128)>NQ) inc=(NQ-hcode_index);                      // smaller increment to match NQ
   for (h=0;h<inc;h++) // p.68: 128 HCodes
     {switch (BRCn[BRCindex]) 
       {case 0: 
         if (THIDXn[BRCindex]<=3) 
           {if (hcode[hcode_index].mcode<3)
               result[hcode_index]=(float)(hcode[hcode_index].sign*hcode[hcode_index].mcode);
            else
               result[hcode_index]=(float)(hcode[hcode_index].sign)*BRC0[THIDXn[BRCindex]];
           }
         else  result[hcode_index]=(float)(hcode[hcode_index].sign)*NRL0[hcode[hcode_index].mcode]*SF[THIDXn[BRCindex]];
        break;
        case 1: 
         if (THIDXn[BRCindex]<=3) 
           {if (hcode[hcode_index].mcode<4)
               result[hcode_index]=(float)(hcode[hcode_index].sign*hcode[hcode_index].mcode);
            else
               result[hcode_index]=(float)(hcode[hcode_index].sign)*BRC1[THIDXn[BRCindex]];
           }
         else  result[hcode_index]=(float)(hcode[hcode_index].sign)*NRL1[hcode[hcode_index].mcode]*SF[THIDXn[BRCindex]];
         break;
        case 2: 
         if (THIDXn[BRCindex]<=5) 
           {if (hcode[hcode_index].mcode<6)
               result[hcode_index]=(float)(hcode[hcode_index].sign*hcode[hcode_index].mcode);
            else
               result[hcode_index]=(float)(hcode[hcode_index].sign)*BRC2[THIDXn[BRCindex]];
           }
         else  result[hcode_index]=(float)(hcode[hcode_index].sign)*NRL2[hcode[hcode_index].mcode]*SF[THIDXn[BRCindex]];
         break;
        case 3: 
         if (THIDXn[BRCindex]<=6) 
           {if (hcode[hcode_index].mcode<9)
               result[hcode_index]=(float)(hcode[hcode_index].sign*hcode[hcode_index].mcode);
            else
               result[hcode_index]=(float)(hcode[hcode_index].sign)*BRC3[THIDXn[BRCindex]];
           }
         else  result[hcode_index]=(float)(hcode[hcode_index].sign)*NRL3[hcode[hcode_index].mcode]*SF[THIDXn[BRCindex]];
         break;
        case 4: 
         if (THIDXn[BRCindex]<=8) 
           {if (hcode[hcode_index].mcode<15)
               result[hcode_index]=(float)(hcode[hcode_index].sign*hcode[hcode_index].mcode);
            else
               result[hcode_index]=(float)(hcode[hcode_index].sign)*BRC4[THIDXn[BRCindex]];
           }
         else  result[hcode_index]=(float)(hcode[hcode_index].sign)*NRL4[hcode[hcode_index].mcode]*SF[THIDXn[BRCindex]];
         break;
        default: printf("UNHANDLED CASE\n");exit(-1);
       }
      hcode_index++;
     }
   BRCindex++;
  }
 while (hcode_index<NQ);
}
