2

1 0 0
0 1 0
0 0 1

2 0 0
0 2 0
0 0 2

#RUN: %lvl1 < %s | %fc %s
#CHECK-NOT: 0
#CHECK-NOT: 1
