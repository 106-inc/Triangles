11

1 0 0
0 1 0
0 0 0

0 0 0
0 0 1
0 0 -1

1 0 0
1 0 1
1 0 -1

0 1 0
0 1 1
0 1 -1

0.5 0.5 0
0.5 0.5 1
0.5 0.5 -1

0.25 0.25 0
0.25 0.25 1
0.25 0.25 -1

5 5 0
5 5 1
5 5 -1

2 0 0
2 0 1
2 0 -1

0 2 0
0 2 1
0 2 -1

-1 0 0
-1 0 1
-1 0 -1

0 -1 0
0 -1 1
0 -1 -1


#RUN: %lvl1 < %s | %fc %s
#CHECK: 0
#CHECK: 1
#CHECK: 2
#CHECK: 3
#CHECK: 4
#CHECK: 5
#CHECK-NOT: {{[0-9]+}}