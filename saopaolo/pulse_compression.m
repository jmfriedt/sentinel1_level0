% TXPRR=^8488=1160        TXPSF=-0x2932=10546 TXPL=00000917=2327
% section 4.2.1 of Sentinel-1 Level 1 Detailed Algorithm Definition, p.31 of
% https://sentinel.esa.int/documents/247904/1877131/Sentinel-1-Level-1-Detailed-Algorithm-Definition

pkg load signal

fref=37.53472224
fs=4/11*4*fref      % case 11 in Range Decimation, p.35 of Packet Protocol Data Unit
TXPRRcode=1160;     % upchirp
TXPSFcode=-10546;   % negative start
TXPLcode=2327;

if (1==0)
TXPRR=TXPRRcode*fref^2/2^21            % MHz/us
TXPSF=TXPRR/4/fref+TXPSFcode/2^14*fref % MHz
TXPL=TXPLcode/fref                     % us

N=TXPL*fs
t=linspace(-TXPL/2,TXPL/2,N);
phi1=TXPSF+TXPRR*TXPL/2
phi2=TXPRR/2
nomchip=exp(j*2*pi*(phi1*t+phi2*t.^2));

if (exist('x1')==0) load("result7.dat");x1=x7;clear x7;end
for k=1:size(x1)(1)
  sol(:,k)=xcorr(x1(k,:),nomchip);
end
sol=sol(size(x1)(2):end,:);
end

load all.mat
sol=sol(1:23984,:);

solc=(fft(sol));
solc=solc(1:4:end,:)+solc(2:4:end,:)+solc(3:4:end,:)+solc(4:4:end,:);
solc=solc(1:4:end,:)+solc(2:4:end,:)+solc(3:4:end,:)+solc(4:4:end,:);
sold=(fft(sol'));
sold=sold(:,1:4:end,:)+sold(:,2:4:end)+sold(:,3:4:end)+sold(:,4:4:end);
sold=sold(:,1:4:end,:)+sold(:,2:4:end)+sold(:,3:4:end)+sold(:,4:4:end);
sold=sold';
% averaging by 16
sol=sol(1:4:end,:)+sol(2:4:end,:)+sol(3:4:end,:)+sol(4:4:end,:);
sol=sol(1:4:end,:)+sol(2:4:end,:)+sol(3:4:end,:)+sol(4:4:end,:);
%sol=sol(:,1:2:end-1,:)+sol(:,2:2:end);

subplot(121)
imagesc(abs(sol'))
axis([600 900 1 800])

figure
imagesc(abs(solc'))
figure
imagesc(abs(sold))
