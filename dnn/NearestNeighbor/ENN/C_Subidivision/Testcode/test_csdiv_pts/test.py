from GeometricStructure import * 

f = open("test.txt","r")

line = f.readline()
n_ver = int(line)
pts = []
scale_factor =10; trans_factor = 800
for i in range(n_ver):
    line = f.readline()
    x, y = map(float, line.split())
    pts.append(Point(x*scale_factor+trans_factor,y*scale_factor+trans_factor))
    
line = f.readline()
n_edges = int(line)

for p in pts:
    p.draw()

for i in range(n_edges):
    line = f.readline()
    u, v = map(int, line.split())
    Line(pts[u],pts[v]).draw()

cv.imwrite('test.png', img)
f.close();
