import cv2 as cv
import numpy as np
import sys, os

class Point:
    def __init__(self, x,y):
        self.x = x
        self.y = y
    def get_tuple(self):
        return (int(self.x), int(self.y))
    def draw(self, img, color = (0,0,0)):
        cv.line(img, self.get_tuple(),self.get_tuple(), color,3)
        cv.imshow('image',img)

class Edge:
    def __init__(self, first, second):
        self.s, self.t = first, second    
    def draw(self, img,color = (0,0,0)):
        cv.line(img,(int(self.s.x), int(self.s.y)), (int(self.t.x), int(self.t.y)),color)
        cv.imshow('image',img)

class Polygon:
    #vertices : list of points
    def __init__(self,vertices):
        self.vertices = vertices
        self.edges = []
        self.size = len(vertices)
        for i in range(self.size):
            s = vertices[i]
            t = vertices[(i+1)%self.size]
            self.edges.append(Edge(s,t))
    def draw(self, img,color = (0,0,0)):
        for e in self.edges:
            e.draw(img)
            cv.imshow('image',img)

testfiles = open("testfiles.txt","r")
while True:
    line = testfiles.readline()
    if not line : break
    input_path, query_path = line.split()

    out_path = (input_path.split('\\'))[-1]
    out_path = (out_path.split('.'))[0] +'.png'
    out_path = '\\'.join((input_path.split('\\'))[:-1]) +"\\"+out_path

    img = np.zeros((1200,1200,3),np.uint8) + 255
    input_f = open(input_path)
    query_f = open(query_path)
    n_pts, n_polys = input_f.readline().split()
    n_pts = int(n_pts)
    n_polys = int(n_polys)
    pts = []
    polys = []
    for i in range(n_pts):
        x,y = input_f.readline().split()
        pts.append(Point(float(x),float(y)))
    for i in range(n_polys):
        n_vertices = int(input_f.readline())
        vertices=[]
        for j in range(n_vertices):
            x,y = input_f.readline().split()
            vertices.append(Point(float(x),float(y)))
        polys.append(Polygon(vertices))

    for p in pts:
        p.draw(img)
    for poly in polys:
        poly.draw(img)
    
    n_query = int(query_f.readline())
    queries=[]
    for i in range(n_query):
        x,y = query_f.readline().split()
        queries.append(Point(float(x),float(y)))
    for p in queries:
        p.draw(img,(0,0,255))
    cv.imwrite(out_path,img)