#!/home/hjz/anaconda3/envs/pytorch64/bin/python3
import matplotlib.pyplot as plt

hit_rate = {}

total_num = 0

with open('./log/hit_rate.txt', encoding='utf-8') as f1:
  total_num = int(f1.readline().strip('\n'))
  batch_num = int(f1.readline().strip('\n'))
  for i in range(total_num):
    tmp = []
    for j in range(batch_num):
      t = f1.readline()
      tmp.append(float(t.strip('\n')))
    hit_rate[i] = tmp
print(hit_rate)

for i in range(total_num):
  plt.plot(hit_rate[i], label=f"hit_rate{i}")
plt.xlabel("iter")
plt.ylabel("hit_rate")
plt.legend()
plt.show()
