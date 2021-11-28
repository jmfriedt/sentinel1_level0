lines=15499
samples=23838
format long
lines*samples*4*2  % is nearly the size of the TIF file

l=10000000;
N=lines/2;

if (exist('data_tif')==0)
  f=fopen('S1A_IW_RAW__0SDV_20200608T101309_20200608T101341_032924_03D05A_A50C.TIFF_0001','rb','l');
% <Swath>IW1</Swath>
% <Polarization>V/V</Polarization>
  data_tif = fread(f,samples*N*2,'float32',0,'l');
  fclose(f);
  data_tif=data_tif(1:2:end)+j*data_tif(2:2:end);
  hist(abs(data_tif(end-100000:end)))
  title('data_tif')
end

d=dir('S1A_IW_RAW__0SDV_20200608T101309_20200608T101341_032924_03D05A_A50C.SAFE/resultSW10*bin');
for k=1:length(d)
  f=fopen(['S1A_IW_RAW__0SDV_20200608T101309_20200608T101341_032924_03D05A_A50C.SAFE/',d(k).name],'rb');
  data_bin=fread(f,2*l,'float');
  data_bin=data_bin(1:2:end)+j*data_bin(2:2:end);
  fclose(f);
  sol=findstr(data_tif,data_bin(end-10:end));
  if (isempty(sol)==0)
     ['Solution found in ',d(k).name,' @ ',num2str(sol)]
     [data_bin(end-20:end) data_tif(sol-10:sol+10)]
  end
%  figure
%  hist(abs(data_bin))
  title(strrep(d(k).name,'_',' '))
end

%data_tif=reshape(data_tif,samples,N/2);
%imagesc(abs(data_tif(1:10:end,:)))
