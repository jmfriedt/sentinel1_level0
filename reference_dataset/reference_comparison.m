lines=15499
samples=23838

format long
lines*samples*4*2  % is nearly the size of the TIF file of 2955783292 bytes

l=10000000;
N=lines/2;

display_chart=0;   % 1 to display histograms

if (exist('data_tif')==0)
  f=fopen('S1A_IW_RAW__0SDV_20200608T101309_20200608T101341_032924_03D05A_A50C.TIFF_0001','rb','l');
% <ByteOrder>LITTLEENDIAN</ByteOrder>    'l'
% <CellType>FLOAT_COMPLEX</CellType>     'float32'
% <Swath>IW1</Swath>                     'resultSW10*.bin'
% <Polarization>V/V</Polarization>       processed the raw vv file only s1a-iw-raw-s-vv-20200608t101309-20200608t101341-032924-03d05a.dat
  data_tif = fread(f,samples*N*2,'float32',0,'l');
  fclose(f);
  data_tif=data_tif(1:2:end)+j*data_tif(2:2:end);
  if (display_chart)
    hist(abs(data_tif(end-100000:end)))
    title('data_tif')
  end
end

Ncompare=5230;   % vector length for comparing matching patterns between our decoding & reference dataset
d=dir('S1A_IW_RAW__0SDV_20200608T101309_20200608T101341_032924_03D05A_A50C.SAFE/resultSW*bin');
% we know SW1 is in resultSW10* but check all swaths to make sure there is only a single match
for k=1:length(d)
  f=fopen(['S1A_IW_RAW__0SDV_20200608T101309_20200608T101341_032924_03D05A_A50C.SAFE/',d(k).name],'rb');
  data_bin=fread(f,2*l,'float');
  data_bin=data_bin(1:2:end)+j*data_bin(2:2:end);
  fclose(f);
  sol=findstr(data_tif,data_bin(end-Ncompare:end));
  if (isempty(sol)==0)
     ['Solution found in ',d(k).name,' @ ',num2str(sol)]
     [data_bin(end-Ncompare-5:end-Ncompare+10)-data_tif(sol-5:sol+10)]
  end
  if (display_chart)
    figure
    hist(abs(data_bin))
    title(strrep(d(k).name,'_',' '))
  end
end

if (display_chart)
  data_tif=reshape(data_tif,samples,N/2);
  imagesc(abs(data_tif(1:10:end,:)))
end
