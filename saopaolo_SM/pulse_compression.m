% TXPRR=^8488=1160        TXPSF=-0x2932=10546 TXPL=00000917=2327
% section 4.2.1 of Sentinel-1 Level 1 Detailed Algorithm Definition, p.31 of
% https://sentinel.esa.int/documents/247904/1877131/Sentinel-1-Level-1-Detailed-Algorithm-Definition

% Sao Paolo S6
%0c1c: 1(1)      65(65)  12(12)  2562: 3(3)      Count=16        Len=25126(61..65533)
%        Time: 1298411082:45581  352ef853(352EF853)      WordIndex=2a    WordVal=0       0000c610 0000d3f6 BAQ=05(c) BlockLen=1f(1F)
%        Decim=9 TXPRR=^84cd=1229        TXPSF=-0x23fa=9210 TXPL=0000077e=1918 Polar=7 Typ=11(0) Swath=6 NQ=9993
%0c1c: 1(1)      65(65)  12(12)  2562: 3(3)      Count=17        Len=25126(61..65533)
%        Time: 1298411082:45620  352ef853(352EF853)      WordIndex=2b    WordVal=464c    0000c611 0000d3f7 BAQ=05(c) BlockLen=1f(1F)
%        Decim=9 TXPRR=^84cd=1229        TXPSF=-0x23fa=9210 TXPL=0000077e=1918 Polar=7 Typ=11(0) Swath=6 NQ=9993


pkg load signal

fref=37.53472224
fs=5/16*4*fref      % case 11 in Range Decimation, p.35 of Packet Protocol Data Unit
TXPRRcode=1229;     % upchirp
TXPSFcode=-9210;    % start
TXPLcode=1918;

TXPRR=TXPRRcode*fref^2/2^21            % MHz/us
TXPSF=TXPRR/4/fref+TXPSFcode/2^14*fref % MHz
TXPL=TXPLcode/fref                     % us

N=TXPL*fs
t=linspace(-TXPL/2,TXPL/2,N);
phi1=TXPSF+TXPRR*TXPL/2
phi2=TXPRR/2
nomchip=exp(j*2*pi*(phi1*t+phi2*t.^2));

if (1==0)  % -------------------------
if (exist('x')==0) load("all.mat");end
for k=1:size(x)(1)
  k
  tmp=xcorr(x(k,:),nomchip);
  sol(:,k)=tmp(size(x)(2):end);
end

% mychirp=sol(4999,3080:4149);
mychirp=sol(1517,800:1300); 
suplot(211)
plot(unwrap(angle(mychirp)))
suplot(212)
tmp=xcorr(sol(k,:),mychirp);
plot(abs(tmp))
end        % -------------------------

% load all1500bot.mat  % mychirp1500.mat is included in this file
load all5000top.mat
% load mychirp_water5000.mat

% ------------ 210829
%https://celestrak.com/satcat/tle.php?CATNR=39634
%SENTINEL-1A             
%1 39634U 14016A   21079.09896275 -.00000050  00000-0 -79699-6 0  9995
%2 39634  98.1819  88.1319 0001317  80.0185 280.1166 14.59198711370760

% https://sentinels.copernicus.eu/web/sentinel/technical-guides/sentinel-1-sar/products-algorithms/level-1/single-look-complex/stripmap
% range resolutio = 3.6 m
% NQ=9975*2*3.1 => ans = 61845 m @ 43.8 mean angle 

PRF=1663.5;                  % from telemetry
t=[0:length(mychirp)-1]/PRF; % time
t=t-t(floor(length(t)/2));
l=300/5405;                  % lambda

REarth=6371 % km
Hleo=693    % km
Hgeo=35786  % km
period=sqrt((Hleo+REarth)^3/(Hgeo+REarth)^3)*24*3600 % s
orbit=2*pi*(Hleo+REarth)*1000                        % m
% TLE: 24/14.5919871*3600=5921.1 s !
Vs=orbit/period              % m/s
Ve=Vs/sqrt(1+Hleo/REarth)    % m/s
% for th=[42.05 45.49 43.8]
th=45.49;
  theta=th*pi/180;             % S6 angle (rad)
  R0=Hleo/cos(theta)*1000;     % m
  Rpp=Ve^2/R0                  % m.s^-2

  ph1=exp(j*(-4*pi/l*Rpp/2*t.^2));
% end
% period = 5926.3  v.s 5921.1 s TLE
% Vs = 7489.4      v.s 7125 doc
% Rpp = 54.206 ans = -7.3775
% Rpp = 51.174 ans = -6.9649
% Rpp = 52.687 ans = -7.1708
%              v.s   -7.56 m.s^-2 experimental : 2.5% error

fDC=0; % -2*Vs/l*(x-tc*Vs)/R0;
fR=2/l*Vs^2/R0;
ph2=exp(-j*4*pi*R0/l)*exp(-j*2*pi*(fDC*t+fR*t.^2/2))

plot(unwrap(angle(mychirp))-max(unwrap(angle(mychirp))));
hold on
plot(unwrap(angle(ph1))-max(unwrap(angle(ph1))));
plot(unwrap(angle(ph2))-max(unwrap(angle(ph2))));
legend('expe','ph1','ph2')
% ------------ 210829

clear ssol
kk=1;
for k=1:10:size(sol)(1) % 19950 - first = range ; second = azimuth
  k
  tmp=xcorr(sol(k,:),mychirp);
  ssol(:,kk)=tmp(size(x)(1)-size(mychirp)(2):end);
  tmp=xcorr(sol(k,:),ph1);
  ssol1(:,kk)=tmp(size(x)(1)-size(ph1)(2):end);
  tmp=xcorr(sol(k,:),ph2);
  ssol2(:,kk)=tmp(size(x)(1)-size(ph2)(2):end);
  kk=kk+1;
end
figure
imagesc(abs(ssol(:,1:1:end)),[0 1e8])
figure
imagesc(abs(ssol1(:,1:1:end)),[0 50000])
figure
imagesc(abs(ssol2(:,1:1:end)),[0 50000])
return  % THE END

m=zeros(200,1500);
for k=1:size(sol)(1)
   k
   a(:,k)=(xcorr(sol(k,:),sol(k,:),10));
   m(floor(k/100)+1,:)=m(floor(k/100)+1,:)+abs(fftshift(fft(sol(k,:))));
end

for k=1:200
   u=polyfit(m(k,:))
end

