%https://celestrak.com/satcat/tle.php?CATNR=39634
%SENTINEL-1A             
%1 39634U 14016A   21079.09896275 -.00000050  00000-0 -79699-6 0  9995
%2 39634  98.1819  88.1319 0001317  80.0185 280.1166 14.59198711370760

% https://sentinels.copernicus.eu/web/sentinel/technical-guides/sentinel-1-sar/products-algorithms/level-1/single-look-complex/stripmap
% range resolutio = 3.6 m
% NQ=9975*2*3.1 => ans = 61845 m @ 43.8 mean angle 


close all
load mychirp_1500.mat
mychirp_gnd=mychirp;
load mychirp_water5000.mat
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
for th=[42.05 45.49 43.8]
  theta=th*pi/180;             % S6 angle (rad)
  R0=Hleo/cos(theta)*1000;     % m
  Rpp=Ve^2/R0                  % m.s^-2
  -4*pi/l*Rpp*(t(2)-t(1))

  if (th==42.05)
    subplot(211)
    plot(t,unwrap(angle(mychirp)),'b');hold on
    plot(t([350:850]-100),unwrap(angle(mychirp_gnd))+480,'g')
  else
    plot(t,-4*pi/l*Rpp/2*t.^2+max(unwrap(angle(mychirp))),'r')
  end
end
xlabel('time (s)');ylabel('phase (rad)')
a=polyfit(t(1:end-1),diff(unwrap(angle(mychirp))),1);

legend([num2str(a(1)),' rad/s expe (water)'],'expe (ground)',[num2str(-4*pi/l*Rpp*(t(2)-t(1))),' rad/s theory'],'location','northwest')
% period = 5926.3  v.s 5921.1 s TLE
% Vs = 7489.4      v.s 7125 doc
% Rpp = 54.206 ans = -7.3775
% Rpp = 51.174 ans = -6.9649
% Rpp = 52.687 ans = -7.1708
%              v.s   -7.56 m.s^-2 experimental : 2.5% error


fDC=0; % -2*Vs/l*(x-tc*Vs)/R0;
fR=2/l*Vs^2/R0;
chirp=exp(j*4*pi*R0/l)*exp(j*2*pi*(fDC*t+fR*t.^2/2))
plot(t,-unwrap(angle(chirp)),'k')
