function sol=read_bin(filename, count, l)
% file name, number of complex data to read, line length
% x=read_bin('result06_1298411052.bin',floor(6592*1.8), 19950);

% filename='result06_1298411052.bin'   % filename
% count=floor(6592*1.8)                % max number of lines (azimuth) my computer can handle
% l=19950                              % range length (NQ*2)

pkg load signal

f=fopen(filename,'rb');
if (f<0) v=0;
else
  fref=37.53472224
  fs=5/16*4*fref      % case 11 in Range Decimation, p.35 of Packet Protocol Data Unit
  TXPRRcode=1229;     % upchirp
  TXPSFcode=-9210;    % start
  TXPLcode=1918;
  
% range compression 
  TXPRR=TXPRRcode*fref^2/2^21            % MHz/us
  TXPSF=TXPRR/4/fref+TXPSFcode/2^14*fref % MHz
  TXPL=TXPLcode/fref                     % us
  
  N=TXPL*fs
  tim=linspace(-TXPL/2,TXPL/2,N);
  phi1=TXPSF+TXPRR*TXPL/2
  phi2=TXPRR/2
  nomchip=exp(j*2*pi*(phi1*tim+phi2*tim.^2));
  
  sol=zeros(l,count); % size(t)(2),size(t)(1));      % avoid dynamic memory allocation
  for k=1:count % size(t)(1)   % 6592 = sweep along azimuth for range compression
    t=fread(f,2*l,'float');    % dynamically read to avoid filling memory with a huge t
    t=t(1:2:end)+t(2:2:end)*i; % see GNU Radio's read_complex_binary
    k
    tmp=xcorr(nomchip,t);
    tmp=tmp(floor(length(nomchip)/2)+1:length(t)+floor(length(nomchip)/2));
    sol(:,k)=tmp;
  end
% range compression completed
  clear t
  fclose(f)

  [m,ind]=max(abs(sol(:)))
  [r,c]=ind2sub(size(sol),ind);
  abs(sol(r,c))
% search for chirp along azimuth
  mychirp=sol(r,c-970:c+300);
  figure(1)
  subplot(211)
  plot(unwrap(angle(mychirp)))
  subplot(212)
  tmp=xcorr(mychirp,sol(r,:)); tmp=tmp(floor(length(mychirp)/2)+1:size(sol)(2)+floor(length(mychirp)/2));
  plot(abs(tmp)/max(abs(tmp)))
  hold on
  plot(abs(sol(r,:))/max(abs(sol(r,:))));

% azimuth compression
  ssol=sol';  % avoid dynamic memory allocation
  for k=1:l % size(sol)(1)   % 19950 = sweep range for azimuth compression
    k
    tmp=xcorr(mychirp,sol(k,:)); 
    tmp=tmp(floor(length(mychirp)/2)+1:size(sol)(2)+floor(length(mychirp)/2));
    ssol(:,k)=tmp;
  end
end
clear sol
imagesc(fliplr(flipud(abs(ssol(1:3:end,1:3:end)))),[1 1e8])
