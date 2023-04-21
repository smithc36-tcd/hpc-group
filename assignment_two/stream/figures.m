% x1=[ 11200.5  12608.3   12235.2    11996.6   12730.7        ]
% x2= [  33298.0  33375.9   33311.3   34379.5    33999.8 ]
% x3=[42701.0   43847.7   42172.4  42172.4   42116.8  ]
% x4=[48619.0  45990.2   45977.6   47662.5   45977.6  ]
% y=[1 2 3 4]
% av = [mean(x1) mean(x2) mean(x3) mean(x4) ]
% va = [std(x1) std(x2) std(x3) std(x4) ]
% yneg=[min(x1)-mean(x1) min(x2)-mean(x2) min(x3)-mean(x3) min(x4)-mean(x4)]
% ypos=[max(x1)-mean(x1) max(x2)-mean(x2) max(x3)-mean(x3) max(x4)-mean(x4)]
% errorbar(y,av,yneg,ypos,'.-','LineWidth',1.5)
% xlabel("thread number")
% ylabel("bandwidth (MB/s)")
x4=[ 44894.9  45883.3    46949.0   47689.6 46808.2  ]
x5=[21019.5 20332.9 20418.9 20610.8 21450.8]
x6=[32894.9 31904.9 30922.9 31110.7 29514.0]
y=[1 2 3 ]
av = [mean(x4) mean(x5) mean(x6) ]
va = [std(x4) std(x5) std(x6) ]
yneg=[min(x4)-mean(x4) min(x5)-mean(x5) min(x6)-mean(x6) ]
ypos=[max(x4)-mean(x4) max(x5)-mean(x5) max(x6)-mean(x6) ]
errorbar(y,av,yneg,ypos,'.-','LineWidth',1.5)
ylabel("bandwidth (MB/s)")