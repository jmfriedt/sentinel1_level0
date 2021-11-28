It is assumed that the reference data was fetched as described in
Sentinel-1-Level-0-Data-Decoding-Package_v1_3.pdf and that the data file
s1a-iw-raw-s-vv-20200608t101309-20200608t101341-032924-03d05a.dat
in the directory
S1A_IW_RAW__0SDV_20200608T101309_20200608T101341_032924_03D05A_A50C.SAFE was 
decoded using this read_file program. This processing takes exactly 1-h on a
Panasonic CF-19 laptop fitted with a SSD hard drive. The resulting .bin files (only swath 1
with index 10 will be of interest to us) should be
```
$ ls -l ../r/S1A_IW_RAW__0SDV_20200608T101309_20200608T101341_032924_03D05A_A50C.SAFE
-rw-r--r-- 1 jmfriedt jmfriedt 240287040 Nov 28 16:42 resultSW10_T1275646407_NQ11919.bin
-rw-r--r-- 1 jmfriedt jmfriedt 268701936 Nov 28 16:47 resultSW10_T1275646409_NQ11919.bin
-rw-r--r-- 1 jmfriedt jmfriedt 268701936 Nov 28 16:52 resultSW10_T1275646412_NQ11919.bin
-rw-r--r-- 1 jmfriedt jmfriedt 268701936 Nov 28 16:57 resultSW10_T1275646415_NQ11919.bin
-rw-r--r-- 1 jmfriedt jmfriedt 268701936 Nov 28 17:02 resultSW10_T1275646418_NQ11919.bin
-rw-r--r-- 1 jmfriedt jmfriedt 268701936 Nov 28 17:08 resultSW10_T1275646420_NQ11919.bin
-rw-r--r-- 1 jmfriedt jmfriedt 268701936 Nov 28 17:13 resultSW10_T1275646423_NQ11919.bin
-rw-r--r-- 1 jmfriedt jmfriedt 268701936 Nov 28 17:18 resultSW10_T1275646426_NQ11919.bin
-rw-r--r-- 1 jmfriedt jmfriedt 268701936 Nov 28 17:23 resultSW10_T1275646429_NQ11919.bin
-rw-r--r-- 1 jmfriedt jmfriedt 268701936 Nov 28 17:28 resultSW10_T1275646431_NQ11919.bin
-rw-r--r-- 1 jmfriedt jmfriedt 268701936 Nov 28 17:33 resultSW10_T1275646434_NQ11919.bin
-rw-r--r-- 1 jmfriedt jmfriedt 268701936 Nov 28 17:38 resultSW10_T1275646437_NQ11919.bin
-rw-r----- 1 jmfriedt jmfriedt 951769796 Jun  8  2020 s1a-iw-raw-s-vv-20200608t101309-20200608t101341-032924-03d05a.dat
```
running this reference_comparison.m program with GNU/Octave will lead to the display of the filename
matching the ``Ncompare=5000`` long sequence in the binary file when compared to the provided TIFF file
```
octave:XX> reference_comparison
lines = 15499
samples = 23838
ans = 2955721296
ans = Solution found in resultSW10_T1275646420_NQ11919.bin @ 144348996
```
Matching 5000 successive values hints at correct decoding and correction of the remaining processing
bugs (the latest being storing both sign and magnitude in the hcode which is erroneous in case the
magnitude is 0, loosing the sign information which is still needed during the reconstruction
of the non-0 result).
