# lvl1

Program to find intersecting triangles.

## Usage

```bash
$ /path/to/Triangles/build/bin/lvl1 < input.txt
```

### Input format

```
N

x0(0) y0(0) z0(0)
x1(0) y1(0) z1(0)
x2(0) y2(0) z2(0)

x0(1) y0(1) z0(1)
x1(1) y1(1) z1(1)
x2(1) y2(1) z2(1)

...

x0(N-1) y0(N-1) z0(N-1)
x1(N-1) y1(N-1) z1(N-1)
x2(N-1) y2(N-1) z2(N-1)
```

Where `N` is number of input triangles and x0(0), y1(0) coordinates of triangles' vertices.

### Output format

Indexes of intersecting triangles
