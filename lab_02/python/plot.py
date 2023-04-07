import matplotlib.pyplot as plt
n = [10 ** 6 * (k + 1) for k in range(10)]

py_data = [0.8480511999999987, 
           1.6670838000000003, 
           2.496741, 
           3.3451416000000016, 
           4.1487373000000005, 
           4.9716594, 
           5.8844740999999985, 
           6.644416199999995, 
           7.474989099999988, 
           8.322890799999996]

cpp_data = [1.14e-01, 
            2.27e-01, 
            3.41e-01, 
            4.55e-01, 
            5.69e-01, 
            6.98e-01, 
            8.01e-01, 
            9.15e-01, 
            1.03e+00,
             1.15e+00]

plt.scatter(n, py_data, label = "Python")
plt.scatter(n, cpp_data, label = "C++")
plt.xlabel("Number of loops")
plt.ylabel("Elapsed time")
plt.xscale('log')
plt.yscale('log')
plt.legend()
plt.show()
