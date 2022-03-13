3

1 0 0
0 1 0
0 0 0

1 0 0
0 1 0
0 0 0

2 0 0
3 0 0
0 2 0

#RUN: %lvl1 < %s | %fc %s
#CHECK:     0
#CHECK:     1
#CHECK-NOT: 2
