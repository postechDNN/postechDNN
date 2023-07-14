import cv2 as cv
import numpy as np
import random
import copy

img = np.zeros((1200,2000,3),np.uint8) + 255

def clear_drawboard(width=1024, height=1024):
    global img
    img = np.zeros((height,width,3),np.uint8) + 255
def save_drawboard(file_name):
    global img
    cv.imwrite(file_name,img)
class Point:
    def __init__(self, x,y):
        self.x = x
        self.y = y
    def __add__(self,other):
        return Point(self.x + other.x, self.y+ other.y)
    def __sub__(self,other):
        return Point(self.x - other.x, self.y- other.y)
    def __lt__(self,other):
        if self.x == other.x :
            return self.y < other.y
        return self.x < other.x
    def __repr__(self):
        return '('+str(self.x) +', '+str(self.y)+')'
    def reverse(self):
        self.x, self.y = self.x, self.y
        return self
    def get_tuple(self):
        return (int(self.x), int(self.y))
    def __mul__(self, other):
        if type(other) == type(3.2) or type(other) == type(1):
            return Point(self.x * other , self.y * other)
        return Point(self.x * other.x ,self.y * other.y)

    def __truediv__(self, other):
        return Point(self.x / other , self.y / other)

    def norm(self):
        return np.sqrt(self.x ** 2 + self.y ** 2)
    def square_len(self):
        return self.x ** 2 + self.y ** 2
    
    def draw(self,color=(0,0,0)):
        cv.line(img, self.get_tuple(),self.get_tuple(), color,3)
        cv.imshow('image',img)

class Circle:
    def __init__(self, center, radius):
        self.center = center
        self.radius = radius
        if radius <= 0 :
            print('radius < 0 !!')

    def get_radius(self):
        return self.radius
    def get_center(self):
        return self.center
    
    def __repr__(self):
        return '[c='+self.center.__repr__()+', r= '+str(self.radius)+']'

    def include(self, other):
        r1, r2 = self.radius, other.radius
        square_d = (self.center - other.center).square_len()
        if r1 >= r2 and r1**2 > square_d and (r1 - r2)**2 >= square_d:
            return True
        return False

    def draw(self, color = (0,0,0)):
        cv.circle(img,self.center.get_tuple(),self.radius,color)
        cv.imshow('image',img)
    
class Line:
    def __init__(self, first, second):
        self.first, self.second = first, second

    def __repr__(self):
        return '['+self.first.__repr__()+', '+self.second.__repr__()+']'

    #right : negative, left : positive, onLine : zero 
    def turn_direction(self,r):
        p,q = self.first, self.second
        t = (p.x * q.y - p.y * q.x)
        t -= (p.x * r.y - p.y * r.x)
        t += (q.x * r.y - q.y * r.x)
        return t
    
    def intersection(self, other):
        t = self.turn_direction(other.first) * self.turn_direction(other.second)
        if t == 0: return True
        elif t > 0 : return False
        s = other.turn_direction(self.first) * other.turn_direction(self.second)
        if s == 0 : return True
        elif s > 0 : return False
        return True

    def vector_direction(self):
        return self.second - self.first
    
    def draw(self, color = (0,0,0)):
        cv.line(img,(int(self.first.x), int(self.first.y)), (int(self.second.x), int(self.second.y)),color)
        cv.imshow('image',img)

#Sorted by clockwise order
#Variable n(number of vertices), vertices(list of points)
class Polygon:
    def __init__(self, pts):
        self.n = len(pts)
        self.vertices = copy.deepcopy(pts)
        self.edges = []
        for i in range(len(pts)-1):
            self.edges.append(Line(pts[i],pts[i+1]))
        self.edges.append(Line(pts[len(pts)-1],pts[0]))
    
    def draw(self, color = (0,0,0)):
        for v in self.vertices:
             v.draw(color)
        for e in self.edges:
             e.draw(color)

    def rotate(self,center,t):
        ret = []
        for p in self.vertices:
            x = p.x - center.x;y = p.y - center.y
            r_x = np.cos(t) * x - np.sin(t) * y
            r_y = np.sin(t) * x + np.cos(t) * y
            ret.append(Point(r_x+center.x,r_y+center.y))
        return Polygon(ret)
        
        
        
