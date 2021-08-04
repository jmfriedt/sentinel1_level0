1. decode the raw level0 stripmap datasets acquired over Sao Paulo:

```bash
../read_file ./S1A_S6_RAW__0SDV_20210226T214354_20210226T214424_036766_045287_FD68.SAFE/s1a-s6-raw-s-vv-20210226t214354-20210226t214424-036766-045287.dat > output.txt
```

No need to leave the whole stripmap dataset decode, the first 5 GB is more than
enough
```bash
5933337648 Aug  4 14:47 resultSW06_T1298411052_NQ9975.bin
```

2. Process the dataset, first with range compression using the theoretical chirp
along range from SM6 (StripMap 6 parameters), identify the strongest target 
and azimuth compress using this chirp
```Octave
[x,xx]=read_bin_sm('resultSW06_T1298411052_NQ9975.bin',floor(1500*11), 19950);
```

<img src="210803_sm_chirps.png">

<img src="210803_sm_map.png">

3. As shown in ``cmp_pulse.m``, the theoretical azimuth chirp seems to be 
properly predicted but the correlation with this phase distribution does not
seem to allow for azimuth compression: to be understood !
