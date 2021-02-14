# sentinel1_level0

Analysis of Sentinel1 level 0 data for decoding raw data, inspired by
the results shown in "Identification of C-Band Radio Frequency Interferences 
from Sentinel-1 Data" at https://www.mdpi.com/2072-4292/9/11/1183

At the moment the packets of data files are properly decoded and the actual
payload is under investigation (Huffman decoding). Example of raw file analysis
output and its interpretation:

<img src="figures/res.png">

Page numbers refer to "SAR Space Packet Protocol Data Unit" found in the doc folder.

The reference dataset is descrived in section 5.3 of https://sentinels.copernicus.eu/documents/247904/685163/Sentinel-1-Level-0-Data-Decoding-Package.pdf (Sentinel-1 Level-0 Data Decoding Package). 
Be aware that the archive is huge (38260338024 bytes).

The same processing algorithm on the reference dataset leads to

<img src="figures/res_ref.png">

Once the file format was assessed, the Huffman compressed data must be uncompressed. 
Although multiple encoding mode can be used depending on operating conditions of 
Sentinel1, we focus on FDBAQ/type D which seems to be the operational case. After 
processing a dataset (S1A_IW_RAW__0SDV_20210112T173201_20210112T173234_036108_043B95_7EA4.SAFE)
the following picture is obtained

<img src="figures/raw_result210214.png">

which does not seem inconsistent with a noise-like pattern prior to range compression. At
the moment the GNU/Octave header must be filled manually (row entry with the result of
``grep -v ^# result.dat | wc -l``) since the data processing is exiting at some point when
meeting an impossible BAQ value.
