function sol=read_bin(filename, count, l)
% file name, number of complex data to read, line length
% x=read_bin('result06_1298411052.bin',floor(6592*1.8), 19950);

% filename='result06_1298411052.bin'   % filename
% count=floor(6592*1.8)                % max number of lines (azimuth) my computer can handle
% l=19950                              % range length (NQ*2)

pkg load signal

f=fopen(filename,'rb');
t=fread(f,2*l,'float');    % dynamically read to avoid filling memory with a huge t
sol=t(1:2:end)+t(2:2:end)*i; % see GNU Radio's read_complex_binary
