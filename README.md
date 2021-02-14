# sentinel1_level0
  
Analysis of Sentinel1 level 0 data for decoding raw data, inspired by
the results shown in "Identification of C-Band Radio Frequency Interferences 
from Sentinel-1 Data" at https://www.mdpi.com/2072-4292/9/11/1183

<h1>Compilation</h1>
  
Working on a GNU/Linux computer (Debian/sid but only using basic functionalities so any distribution
should do), type ``make`` to compile (tested using gcc 10.2.1). Execute by providing to ``read_file`` 
as argument the dataset to be processed, e.g.
```
./read_file S1A_IW_RAW__0SDV_20210112T173201_20210112T173234_036108_043B95_7EA4.SAFE/s1a-iw-raw-s-vv-20210112t173201-20210112t173234-036108-043b95.dat   
```
The result will be a display of the various decoded fields and a file ``result.dat`` to be 
processed with GNU/Octave. The number of rows is missing at the moment from the header and
must be inserted manually as the result of ``grep -v ^# result.dat | wc -l``. In GNU/Octave,
``load result.dat`` and ``imagesc(abs(x))`` generated the figure displayed at the bottom
of this page.

<h1>Current status and understanding</h1>

CCSDS is a well documented protocol a bit challenging to get familiar with, as
was discussed earlier when decoding Meteor M2 weather satellite images
(https://archive.fosdem.org/2019/schedule/event/sdr_meteorm2n/). Here the error
correction seems to have already been applied after radiofrequency communication but
the data is compressed (payload must be Huffman decoded).

Example of raw file analysis output and its interpretation (primary and secondary header):

<img src="figures/res.png">

Page numbers refer to "SAR Space Packet Protocol Data Unit" found in the doc folder.

The reference dataset is descrived in section 5.3 of https://sentinels.copernicus.eu/documents/247904/685163/Sentinel-1-Level-0-Data-Decoding-Package.pdf (Sentinel-1 Level-0 Data Decoding Package). 
Be aware that the archive is huge (38260338024 bytes).

The same processing algorithm on the reference dataset leads to

<img src="figures/res_ref.png">

Once the file format was assessed, the Huffman compressed data must be uncompressed. 
Although multiple encoding mode can be used depending on operating conditions of 
Sentinel1, we focus on FDBAQ/type D [1] which seems to be the operational case. After 
processing a dataset (S1A_IW_RAW__0SDV_20210112T173201_20210112T173234_036108_043B95_7EA4.SAFE)
the following picture is obtained

<img src="figures/raw_result210214.png">

which does not seem inconsistent with a noise-like pattern prior to range compression. At
the moment the GNU/Octave header must be filled manually (row entry with the result of
``grep -v ^# result.dat | wc -l``) since the data processing is exiting at some point when
meeting an impossible BAQ value.

This result could not have been achieved without the sample code provided at
https://github.com/plops/cl-cpp-generator2/blob/master/example/08_copernicus_radar/source/
whose output and probing variable values was invaluable for debugging cases I had not 
thought of.

[1] E. Attema & al., Sentinel-1 Flexible Dynamic Block Adaptive Quantizer, Proc. 8th European Conference on Synthetic Aperture Radar (EUSAR), July 2010 at 
https://www.researchgate.net/publication/224233683_Sentinel-1_Flexible_Dynamic_Block_Adaptive_Quantizer
