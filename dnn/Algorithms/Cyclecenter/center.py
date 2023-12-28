import networkx as nx
import matplotlib.pyplot as plt
from pygraphviz import *

class CycleGraph:
    def __init__(self, w, e):
        assert len(w) >= 3
        assert len(w) == len(e)

        self.vertex_num = len(w) - 1
        self.weight = w.copy() # index starts from 1.
        self.edge = e.copy() # edge[i] means the length of v_i to v_((i + 1) % vertex_num).
        self.__edgeSum = e.copy() # edgeSum[i] = countertclockwise length of v_1 to v_((i + 1) % vertex_num).
        for i in range (2, self.vertex_num + 1):
            self.__edgeSum[i] = self.__edgeSum[i] + self.__edgeSum[i - 1]
        self.__perimeter = self.__edgeSum[-1]
        self.__radius = float("inf")
        self.__i_star = 0
        self.__center_left = -1
        self.__center_right = -1
        self.__center_loc = 0
        self.__active_vertex = []
        self.__active_interval = []

    def printCenter(self):
        self.__getCenter()
        print("Center is between v%d and v%d, %.2f away from v%d." \
              %(self.__center_left, self.__center_right, self.__center_loc, self.__center_left))

    def showCenter(self):
        if self.__center_left == -1:
            self.__getCenter()
        G = AGraph()
        G.add_nodes_from(range(1, self.vertex_num + 1))
        edge_length = {}
        for i in range(1, self.vertex_num):
            G.add_edge(i, i + 1, len = self.edge[i])
        G.add_edge(self.vertex_num, 1, len = self.edge[self.vertex_num])

        G = nx.nx_agraph.from_agraph(G)
        pos = nx.nx_agraph.graphviz_layout(G)

        loc1 = pos[str(self.__center_left)]; loc2 = pos[str(self.__center_right)]
        m = self.__center_loc; n = self.edge[self.__center_left] - self.__center_loc
        x = (m * loc2[0] + n * loc1[0]) / (m + n); y = (m * loc2[1] + n * loc1[1]) / (m + n)
        G.add_node('C')
        pos['C'] = (x, y)

        nx.draw(G, pos, with_labels=True)
        plt.show()

    def __length(self, loc1, loc2): # length between loc1 and loc2 (loc: counterclockwise location measured from v1.)
        if loc1 > loc2:
            loc1, loc2 = loc2, loc1
        return min(loc2 - loc1, self.__perimeter - loc2 + loc1)

    def __lengthV(self, v_i, v_j): # length between vertex v_i and v_j.
        return self.__length(self.__edgeSum[v_i - 1], self.__edgeSum[v_j - 1])

    def __isDominated(self, v1, v2, v3):
        I1 = self.__dominatingInterval(v2, v1)
        I2 = self.__dominatingInterval(v2, v3)
        intersection = self.__intervalIntersection(I1, I2)
        return intersection[0] < 0

    def __relabelVertices(self):
        max_len = 0
        for i in range(2, self.vertex_num + 1):
            length_to_vi = self.weight[i] * self.__lengthV(1, i);
            if max_len < length_to_vi:
                self.__i_star = i
                max_len = length_to_vi

    def __getActiveVertices(self):
        L_a = [self.__i_star]
        if self.__i_star == self.vertex_num:
            L_a.append(1)
        else:
            L_a.append(self.__i_star + 1)

        for idx in range(3, self.vertex_num + 1):
            i = self.__i_star + idx - 1
            if i > self.vertex_num:
                i %= self.vertex_num
            if self.__isDominated(L_a[-1], i, self.__i_star):
                continue
            while len(L_a) >= 3:
                first, second = L_a[-1], L_a[-2]
                L_a.pop(-1)
                if not self.__isDominated(second, first, i):
                    L_a.append(first)
                    break
            L_a.append(i)
        self.__active_vertex = L_a

    def __dominatingInterval(self, v1, v2):
        loc1 = self.__edgeSum[v1 - 1]; loc2 = self.__edgeSum[v2 - 1]
        w1 = self.weight[v1]; w2 = self.weight[v2]
        if v1 < v2:
            len1 = abs(loc2 - loc1); len2 = self.__perimeter - len1
            mid_between = loc1 + (w2 / (w1 + w2)) * len1
            mid_antipodal = loc2 + (w1 / (w1 + w2)) * len2
            if mid_between > mid_antipodal:
                mid_antipodal += self.__perimeter
            return mid_between, mid_antipodal
        else:
            len1 = abs(loc1 - loc2); len2 = self.__perimeter - len1
            mid_between = loc2 + (w1 / (w1 + w2)) * len1
            mid_antipodal = loc1 + (w2 / (w1 + w2)) * len2
            if mid_antipodal > mid_between:
                mid_between += self.__perimeter
            return mid_antipodal, mid_between

    def __intervalIntersection(self, I1, I2):
        if I1[0] > I2[0]:
            I1, I2 = I2, I1
        l1 = I1[0]; r1 = I1[1]
        l2 = I2[0]; r2 = I2[1]
        if r1 > self.__perimeter and r2 > self.__perimeter:
            return max(l1, l2), min(r1, r2) - self.__perimeter
        elif r1 > self.__perimeter:
            if l1 < r2:
                 return l1, r2
            elif l2 < r1 - self.__perimeter:
                 return l2, r1 - self.__perimeter
        elif r2 > self.__perimeter:
            if l2 < r1:
                return l2, r1
            elif l1 < r2 - self.__perimeter:
                return l1, r2 - self.perimeter
        else:
            if l2 < r1:
                return l2, r1
        return -1.0, -1.0

    def __getDominatingIntervals(self):
        sz = len(self.__active_vertex)
        self.__active_interval = []
        for idx in range(sz):
            i = self.__active_vertex[idx]
            l = self.__active_vertex[idx - 1] if idx > 0 else self.__active_vertex[-1]
            r = self.__active_vertex[(idx + 1) % sz]
            I1 = self.__dominatingInterval(i, l)
            I2 = self.__dominatingInterval(i, r)
            intersection = self.__intervalIntersection(I1, I2)
            self.__active_interval.append((i, intersection))

    def __getCenter(self):
        self.__relabelVertices()
        self.__getActiveVertices()
        self.__getDominatingIntervals()
        for vertex, (I_l, I_r) in self.__active_interval:
            l1 = self.weight[vertex] * self.__length(self.__edgeSum[vertex - 1], I_l)
            l2 = self.weight[vertex] * self.__length(self.__edgeSum[vertex - 1], I_r)
            loc = I_l if l1 < l2 else I_r
            l = min(l1, l2)
            if l < self.__radius:
                self.__center_loc = loc
                self.__radius = l
        for i in range(self.vertex_num):
            loc1 = self.__edgeSum[i - 1]
            loc2 = self.__edgeSum[i]
            if loc1 <= self.__center_loc and self.__center_loc <= loc2:
                self.__center_left = i
                self.__center_right = i + 1
                self.__center_loc -= loc1
                break
        if self.__center_left == -1:
            self.__center_left = self.vertex_num
            self.__center_right = 1
            self.__center_loc -= self.__edgeSum[self.vertex_num - 1]
