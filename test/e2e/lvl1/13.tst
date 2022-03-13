2

1 1 1
1 1 2
2 2 1

-1 -1 1
-1 -1 2
-2 -2 1

#RUN: %lvl1 < %s | %fc %s
#CHECK-NOT: 0
#CHECK-NOT: 1
