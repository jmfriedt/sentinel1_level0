
function sol=read_bin(filename, count, l)
% file name, number of complex data to read, line length
% x=read_bin_cal('resultSW92_T1298411052_NQ2951.bin',60,2951*2);
% plot(abs(x(:,32)));hold on; plot(abs(x(:,2)))

pkg load signal

f=fopen(filename,'rb');
if (f<0) v=0;
else
  % fref=37.53472224
  % fs=5/16*4*fref      % case 11 in Range Decimation, p.35 of Packet Protocol Data Unit
  for k=1:count % size(t)(1)   % 6592 = sweep along azimuth for range compression
    t=fread(f,2*l,'float');    % dynamically read to avoid filling memory with a huge t
    t=t(1:2:end)+t(2:2:end)*i; % see GNU Radio's read_complex_binary
    k
    sol(:,k)=t;
  end
% range compression completed
  fclose(f)
end
