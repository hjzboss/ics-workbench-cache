import matplotlib.pyplot as plt
import numpy as np

x = np.array([0.984484, 0.984502, 0.984528, 0.984529, 0.984524, 0.984507, 0.984519, 0.984518, 0.984513])
cycle = np.array([24046024, 21372571, 18700224, 16029173, 13358261, 10687016, 8014828, 5343317, 2671768])
cycle = cycle / np.max(cycle) # 归一化
plt.plot(x, label="hit_rate")
plt.xlabel("batch")
plt.legend()
plt.show()

plt.plot(cycle, label="cycle")
plt.xlabel("batch")
plt.legend()
plt.show()