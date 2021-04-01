d=dir('resultSW10_T*');
x=[];
subplot(223)
for k=1:length(d)
  if (d(k).bytes>2.2e8)
    val=str2num(d(k).name(26:30))
    y=((read_bin(d(k).name,7,val*2)));
    if (mean(abs(y))>4) plot(real(y));hold on;end
    x=[x ; mean(abs(y))];
  end
end
plot(real(y))
xlabel('fast time (a.u.)')
ylabel('I swath 1 (a.u.)')
subplot(211)
plot((x),'-')
hold on
clear x

subplot(224)
d=dir('resultSW11_T*');
x=[];
for k=1:length(d)
  if (d(k).bytes>2.2e8)
    val=str2num(d(k).name((26:30)))
    y=((read_bin(d(k).name,7,val*2)));
    if (mean(abs(y))>4) plot(real(y));hold on;end
    x=[x ; mean(abs(read_bin(d(k).name,7,val*2)))];
  end
end
plot(real(y))
xlabel('fast time (a.u.)')
ylabel('I swath 2 (a.u.)')
subplot(211)
plot((x),'-')
clear x

d=dir('resultSW12_T*');
x=[];
for k=1:length(d)
  if (d(k).bytes>2.2e8)
    val=str2num(d(k).name((26:30)))
    x=[x ; mean(abs(read_bin(d(k).name,7,val*2)))];
  end
end
subplot(211)
plot((x),'-')
ylim([2 2.8])
legend('SW1','SW2','SW3')
xlabel('measurement sequence (a.u.)')
ylabel('mean(abs(I+jQ)) (a.u.)')
