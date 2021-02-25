load result5.dat; 
x1=x5(1:1092,1:23988);clear x5
x1=x1(1:2:end,1:4:end)+x1(2:2:end,1:4:end)+x1(1:2:end,2:4:end)+x1(2:2:end,2:4:end)+x1(1:2:end,3:4:end)+x1(2:2:end,3:4:end)+x1(1:2:end,4:4:end)+x1(2:2:end,4:4:end);
subplot(311); 
imagesc(abs(x1),[0 150])
title('S1A\_IW\_RAW\_\_0SDV\_20210112T173201\_20210112T173234\_036108\_043B95\_7EA4')

load result6.dat; 
x1=x6(1:1546,1:23988);clear x6
x1=x1(1:2:end,1:4:end)+x1(2:2:end,1:4:end)+x1(1:2:end,2:4:end)+x1(2:2:end,2:4:end)+x1(1:2:end,3:4:end)+x1(2:2:end,3:4:end)+x1(1:2:end,4:4:end)+x1(2:2:end,4:4:end);
subplot(312); imagesc(abs(x1),[0 150])

load result7.dat; 
x1=x7(1:1546,1:23988);clear x7
x1=x1(1:2:end,1:4:end)+x1(2:2:end,1:4:end)+x1(1:2:end,2:4:end)+x1(2:2:end,2:4:end)+x1(1:2:end,3:4:end)+x1(2:2:end,3:4:end)+x1(1:2:end,4:4:end)+x1(2:2:end,4:4:end);
subplot(313); imagesc(abs(x1),[0 150])

colormap gray
print -dpng result_all.png
