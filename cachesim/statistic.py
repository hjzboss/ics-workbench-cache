import matplotlib.pyplot as plt

hit_rate = []
cycle = []

with open('./log/hit_rate.txt', encoding='utf-8') as f1:
  t = f1.readline()
  while t:
    hit_rate.append(t.strip('\n'))
    t = f1.readline()
print(hit_rate)

with open('./log/cycle.txt', encoding='utf-8') as f2:
  t = f2.readline()
  while t:
    cycle.append(t.strip('\n'))
    t = f2.readline()
print(cycle)

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