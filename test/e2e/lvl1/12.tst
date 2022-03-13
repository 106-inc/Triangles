8

1 1 1
1 1 2
2 2 1

-1 -1 1
-1 -1 2
-2 -2 1

1 1 -1
1 1 -2
2 2 -1

-1 -1 -1
-1 -1 -2
-2 -2 -1

1 -1 1
2 -2 1
1 -1 2

-1 1 1
-2 2 1
-1 1 2

1 -1 -1
2 -2 -1
1 -1 -2

-1 1 -1
-2 2 -1
-1 1 -2

#RUN: %lvl1 < %s | %fc %s
#CHECK-NOT: 0
#CHECK-NOT: 1
#CHECK-NOT: 2
#CHECK-NOT: 3
#CHECK-NOT: 4
#CHECK-NOT: 5
#CHECK-NOT: 6
#CHECK-NOT: 7
