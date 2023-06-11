import matplotlib.pyplot as plt
import sys
f = open(sys.argv[2],mode = 'r')
d = f.readlines()
x = []
for i in range(int(sys.argv[1])): 
    d[i] = float(d[i].strip('\n'))

for i in range(int(sys.argv[1])):
    x.append(i*8000/int(sys.argv[1]))

#plot
f, (ax1, ax2) = plt.subplots(ncols=1, nrows=2, sharex=True)

markers, stemlines, baselines = ax2.stem(x,d)
markers.set_visible(False)
ax2.set_ylim(0,int(sys.argv[4])*1.15)

markers, stemlines, baselines = ax1.stem(x,d)
markers.set_visible(False)
max = int(sys.argv[3])
ax1.set_ylim(int(d[max]*0.85),int(d[max]*1.15))

d = .7
kwargs = dict(marker=[(-1,-d), (1,d)], markersize=15, linestyle = 'none', color='k', clip_on=False)
ax1.plot([0,1],[0,0],transform=ax1.transAxes, **kwargs)
ax2.plot([0,1],[1,1],transform=ax2.transAxes, **kwargs)

ax1.spines['bottom'].set_visible(False)
ax2.spines['top'].set_visible(False)

f.text(0.01,0.50,"spectrum",va = 'center', rotation = 'vertical', fontsize = 10)
font1 = {'weight': 'normal', 'size': 18, 'style': 'italic'}
f.text(0.5,0.92,"DFT",va = 'center', fontdict = font1)
ax1.get_xaxis().set_visible(False)
plt.title('====================================')
ax2.set_xlabel('frequency(Hz)')
plt.show()

