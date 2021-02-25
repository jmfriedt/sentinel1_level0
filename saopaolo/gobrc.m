d=dir('./brc?.dat');
brc=[];
for n=1:length(d)
  x=load(d(n).name);
  subplot(8,2,2*n-1)
  imagesc(x,[0 3]) 
  brc=[brc ; x];
end
subplot(1,2,2)
imagesc(brc(1:10:end,:)) 
