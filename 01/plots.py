import matplotlib.pyplot as plt
import numpy as np

T = [2.981986, 1.520924, 0.948300, 0.831190, 0.818137]
P = [1,2,4,8,16]

plt.figure(figsize=(7, 7))
plt.xlabel('P')
plt.ylabel('T')
plt.scatter(P, T)
plt.plot(P, T, 'r', label='T(P)')
plt.grid()
plt.legend(bbox_to_anchor=(0.6, 1), loc=4)
plt.savefig('t(p).png')

S = [T[0] / T[i] for i in range(len(T))]
plt.figure(figsize=(7, 7))
plt.xlabel('P')
plt.ylabel('S - ускорение работы программы на p')
plt.scatter(P, S)
plt.plot(P, S, 'b', label='S(P)')
plt.grid()
plt.legend(bbox_to_anchor=(0.6, 1), loc=4)
plt.savefig('s(p).png')

E = [S[i] / P[i] for i in range(len(T))]
plt.figure(figsize=(7, 7))
plt.xlabel('P')
plt.ylabel('E - эффективность на p')
plt.scatter(P, E)
plt.plot(P, E, 'black', label='E(P)')
plt.grid()
plt.legend(bbox_to_anchor=(0.6, 1), loc=4)
plt.savefig('e(p).png')

# P = 4
T = [0.006282, 0.008325, 0.018825, 0.094500, 0.784633, 3.102137]
N = [10, 100, 1000, 10000, 100000, 400000]
plt.figure(figsize=(20, 7))
plt.xlabel('N - количество экспериментов')
plt.ylabel('T - время работы программы')
plt.scatter(N, T)
plt.plot(N, T, 'r', label='T(N)')
plt.legend(bbox_to_anchor=(0.6, 1), loc=4)
plt.grid()
plt.savefig('t(n).png')

S = [T[0] / T[i] for i in range(len(T))]
plt.figure(figsize=(20, 7))
plt.xlabel('N - количество экспериментов')
plt.ylabel('S')
plt.scatter(N, S)
plt.plot(N, S, 'b', label='S(N)')
plt.legend(bbox_to_anchor=(0.6, 1), loc=4)
plt.grid()
plt.savefig('s(n).png')

E = [S[i] / N[i] for i in range(len(N))]
plt.figure(figsize=(20, 7))
plt.xlabel('N - количество экспериментов')
plt.ylabel('E')
plt.scatter(N, E)
plt.plot(N, E, 'black', label='E(N)')
plt.legend(bbox_to_anchor=(0.6, 1), loc=4)
plt.grid()
plt.savefig('e(n).png')
