8
1 1 0 3 1 0 1 3 0
0 0 0 1 0 0 0 1 0
1 0.5 0 1 0.5 1 0 0 0.5
1 0 0 0 1 0 0 0 1
0 0 0 0 3 3 0 0 3
1 1 0 1 2 3 5 4 8
9 9 9 9 9 9 9 9 9
8 8 8 8 8 8 -10 8 8

#RUN: %lvl1 < %s | %fc %s
#CHECK: 0
#CHECK: 1
#CHECK: 2
#CHECK: 3
#CHECK: 4
#CHECK: 5
