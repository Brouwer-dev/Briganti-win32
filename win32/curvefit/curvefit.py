import numpy as np
import matplotlib.pyplot as plt
from scipy.optimize import curve_fit

def func(x, a, b, c):
    return a * np.square(x) + b*x + c

xdata = np.array([
42,
84,
101,
119,
140,
155,
180,
200,
219,
237,
258,
281])

ydata = np.array([
0.02,
0.05,
0.07,
0.1,
0.15,
0.2,
0.3,
0.4,
0.5,
0.6,
0.7,
0.8])

plt.plot(xdata, ydata, 'b-', label='data')

popt, pcov = curve_fit(func, xdata, ydata)
popt
plt.plot(xdata, func(xdata, *popt), 'r-',
         label='fit: a=%1.8f, b=%1.8f, c=%1.8f' % tuple(popt))

popt, pcov = curve_fit(func, xdata, ydata, bounds=(0, [.1, .1, .1]))
popt
plt.plot(xdata, func(xdata, *popt), 'g--',
         label='fit: a=%1.8f, b=%1.8f, c=%1.8f' % tuple(popt))

plt.xlabel('x')
plt.ylabel('y')
plt.legend()
plt.show()