def average(a, b, c, d):
    return (a + b + c + d) / 4

with open('output.txt', 'r') as file, open('average.txt', 'w') as outfile:
    for i in range(15):
        s = file.readline()
        t1 = [[], [], [], []]
        s1 = [[], [], [], []]
        for k in range(4):
            for j in range(30):
                a, b = map(int, file.readline().split())
                t1[k].append(a)
                s1[k].append(b)
            file.readline()
        res_t = []
        res_s = []
        for j in range(30):
            res_t.append(average(t1[0][j], t1[1][j], t1[2][j], t1[3][j]))
            res_s.append(average(s1[0][j], s1[1][j], s1[2][j], s1[3][j]))
        outfile.write(s.strip() + '\n')
        outfile.write(" ".join(map(str, res_t)) + '\n')
        outfile.write(" ".join(map(str, res_s)) + '\n')
        outfile.write('\n')