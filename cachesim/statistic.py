#!/home/hjz/anaconda3/envs/pytorch64/bin/python3
import matplotlib.pyplot as plt
import numpy as np

hit_rate = []
cycle = []

with open('./log/hit_rate.txt', encoding='utf-8') as f1:
  t = f1.readline()
  while t:
    hit_rate.append(float(t.strip('\n')))
    t = f1.readline()
print(hit_rate)

with open('./log/cycle.txt', encoding='utf-8') as f2:
  t = f2.readline()
  while t:
    cycle.append(int(t.strip('\n')))
    t = f2.readline()
print(cycle)

hit_rate = np.array(hit_rate)
cycle = np.array(cycle)
hit_rate = hit_rate / np.sum(hit_rate)
cycle = cycle / np.sum(cycle)
ratio = hit_rate / cycle

print(hit_rate)
print(cycle)
print(ratio)

plt.plot(hit_rate, label="hit_rate")
plt.xlabel("iter")
plt.ylabel("hit_rate")
plt.legend()
plt.show()

plt.plot(cycle, label="cycle")
plt.xlabel("iter")
plt.ylabel("cycle")
plt.legend()
plt.show()

plt.plot(ratio, label="hit_rate/cycle")
plt.xlabel("iter")
plt.ylabel("ratio")
plt.legend()
plt.show()