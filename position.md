# Analyzing the packet decoder output to extract the satellite position and velocity

See <a href="doc/Sentinel-1-SAR-Space-Packet-Protocol-Data-Unit.pdf">Sentinel-1-SAR-Space-Packet-Protocol-Data-Unit.pdf</a>
page 23: the position and velocity are updated once every second, and the information is spread over multiple
successive frames so the analysis is most easily performed as a post-processing step rather than in the
``read_file.c`` program itself.

When decoding a raw data file, the WordIndex and WordValue are displayed on the
console and can be extracted if the output of the decoder is saved in a file:
```
read_file S1B_IW_RAW__0SDV_20210216T083028_20210216T083100_025629_030DEF_1684.SAFE/s1b-iw-raw-s-vv-20210216t083028-20210216t083100-025629-030def.dat > t
cat t | grep Word |  cut -d= -f2,3 | cut -c 1-3,12-16 > pvt
```

for processing with GNU/Octave or Matlab (assuming the file ``pvt`` holds the index value and associated
word, check the file organization if Octave fails to read the file content):
```
f=fopen("pvt");
d=fscanf(f,"%x");
index=d(1:2:end);
val=d(2:2:end);

% position fields
name=['X','Y','Z'];
kindex=[1 5 9];
for m=1:3
  k=find(index==kindex(m));
  if ((k(end)+3)>length(val)) k=k(1:end-1);end
  value=uint64(((val(k)*65536+val(k+1))*65536+val(k+2))*65536+val(k+3));
  eval([name(m),"=typecast(value,'double');"]);
end

% velocity fields
name={'vX','vY','vZ'};
kindex=[13 15 17];
for m=1:3
  k=find(index==kindex(m));
  if ((k(end)+1)>length(val)) k=k(1:end-1);end
  value=uint64(val(k)*65536+val(k+1));
  eval([char(name(m)),"=typecast(value,'single')(1:2:end);"]);
end
```

The Octave program generate six variables with the satellite postion in ECEF corrdinates
in X, Y and Z and the satellite velocity components vX, vY and vZ in m/s.

As an example on the Sao Paulo dataset named S1B_IW_RAW__0SDV_20210216T083028_20210216T083100_025629_030DEF_1684,
the coordinate extraction leads to ECEF
```
X(1)  = 4864988.736389160
X(end)= 4790006.055767059
Y(1)  = -4257144.322614670
Y(end)= -4239336.969697952
Z(1)  = -2886428.462149620
Z(end)= -3034539.787984848
```
which is converted to latitude-longitude altitude (eg using https://www.oc.nps.edu/oc2902w/coord/llhxyz.htm) to
```
Latitude  : 24.18979   deg S
Longitude : 41.18779   deg W
Height    : 705180.7   m
```
which is not inconsistent with Sao Paulo location at 23.5558 S, 46.6396 W and a satellite altitude of 700 km.

Similarly, the velocity magnitude
```
v=[vX(1) vY(1) vZ(1)];
sqrt(v*v') = 7589.7
```
is not inconsistent with a coarse estimate of a satellite flying an orbit length of 40000+2*pi*700=44400 km in 100 minutes or
7400 m/s.
