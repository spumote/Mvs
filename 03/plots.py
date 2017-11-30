import matplotlib.pyplot as plt
import numpy as np

n = 5000000
m = n // 16
P = [1, 2, 4, 8, 16]
T_m = [1.570458, 0.854281, 0.497648, 0.439908, 0.347892]
T_q = [1.571725, 1.561989, 1.565610, 1.561010, 1.559709]
plt.figure(figsize=(14, 8))
plt.xlabel('$P$', size=20)
plt.ylabel('$T$', size=20)
plt.title('$T(P)$', size=30)
plt.scatter(P, T_m)
plt.scatter(P, T_q)
plt.plot(P, T_m, label = 'mergesort time', color = 'black')
plt.plot(P, T_q, label = 'quicksort time', color = 'red')
plt.legend()
plt.grid()
plt.savefig('T(P).png')

S = [T_m[0] / T_m[i] for i in range(5)]
plt.figure(figsize=(14, 8))
plt.xlabel('$P$', size=20)
plt.ylabel('$S$', size=20)
plt.title('$S(P)$', size=30)
plt.scatter(P, S)
plt.plot(P, S, color = 'black')
plt.legend()
plt.grid()
plt.savefig('S(P).png')

E = [S[i] / P[i] for i in range(5)]
plt.figure(figsize=(14, 8))
plt.xlabel('$P$', size=20)
plt.ylabel('$E$', size=20)
plt.title('$E(P)$', size=30)
plt.scatter(P, E)
plt.plot(P, E, color = 'black')
plt.legend()
plt.grid()
plt.savefig('E(P).png')
