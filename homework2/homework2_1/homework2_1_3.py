# 宿題1
# Pythonで計測 numpyのdotで比べてみる
import numpy, sys, time

if (len(sys.argv) != 2):
    print("usage: python %s N" % sys.argv[0])
    quit()

n = int(sys.argv[1])
a = numpy.zeros((n, n)) # Matrix A
b = numpy.zeros((n, n)) # Matrix B
c = numpy.zeros((n, n)) # Matrix C

# Initialize the matrices to some values.
for i in range(n):
    for j in range(n):
        a[i, j] = i * n + j
        b[i, j] = j * n + i
        c[i, j] = 0

# test_kaisuu回繰り返して計測
test_kaisuu = 10
# 計測開始
begin = time.time()
for kaisuu in range(test_kaisuu):
    c = numpy.dot(a,b)
end = time.time()
# 計測終了
print(n,"x",n,"行列の掛け算 (",test_kaisuu,"回)",sep="")
print("time: %.6f sec" % (end - begin))

# Print C for debugging. Comment out the print before measuring the execution time.
total = 0
for i in range(n):
    for j in range(n):
        # print c[i, j]
        total += c[i, j]
# Print out the sum of all values in C.
# This should be 450 for N=3, 3680 for N=4, and 18250 for N=5.
print("sum: %.6f" % total)
