import matplotlib.pyplot as plt

with open('average.txt', 'r') as file:
    for i in range(5):
        sts = []
        ts = []
        ss = []
        for j in range(3):
            st = file.readline().strip()
            t = list(map(float, file.readline().split()))
            s = list(map(float, file.readline().split()))
            file.readline()
            sts.append(st)
            ts.append(t)
            ss.append(s)

        b = range(100, 3100, 100)
        plt.figure()
        plt.title(sts[0].split()[0])
        for i in range(3):
            plt.plot(b, ts[i])
            plt.xlabel("N")
            plt.ylabel("microseconds")
        plt.legend(sts)

        plt.figure()
        plt.title(sts[0].split()[0])
        for i in range(3):
            plt.plot(b, ss[i])
            plt.xlabel("N")
            plt.ylabel("number of comparisons")
        plt.legend(sts)

plt.show()
