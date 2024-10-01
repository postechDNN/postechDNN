# This is a sample Python script.

# Press Shift+F10 to execute it or replace it with your code.
# Press Double Shift to search everywhere for classes, files, tool windows, actions, and settings.
from pyhull.convex_hull import ConvexHull
from pyhull.delaunay import DelaunayTri
from pyhull.voronoi import VoronoiTess
import random
import os
import numpy as np

eps = pow(0.1, 5)

class PointSet:
    # def __init__(self, _dimension):
    #    self.dimension = _dimension
    #    self.points = []

    # PS = PointSet(dimension, cells[j], dimension+2, None, midPointsList, num2ind(dimension, numIntervalsPerAxis, cells[j]))

    def __init__(self, _dimension, _num_points, _bound=None, _valList=[], _L=[], pointsReady = None):
        self.dimension = _dimension
        self.points = []

        if pointsReady != None:
            self.points = pointsReady

        elif _bound == None:
            for i in range(_num_points):
                p = Point(_dimension)
                p.setRandom(None, _valList, _L)
                self.addPoint(p)
        else:
            for i in range(_num_points):
                p = Point(_dimension)
                p.setRandom(_bound)
                self.addPoint(p)

    def print(self, _storeDir):
        f = open(_storeDir, 'w')
        # print dimension
        f.write(str(self.dimension))
        f.write("\n")
        # print number of points
        f.write(str(len(self.points)))
        f.write("\n")
        # print points
        # p is a Point instance

        for i in range(len(self.points)):
            p = self.points[i]
        # for p in self.points:
            # 'value' is a float variable

            for j in range(len(p.values)):
                value = p.values[j]
                f.write(str(value))
                if j != len(p.values)-1:
                    f.write(" ")

            '''
            for value in p.values:
                f.write(str(value))
                f.write(' ')
                # f.write(*(p.values))
            '''

            if i != len(self.points)-1:
                f.write("\n")
        f.close()

    def addPoint(self, _p):
        self.points.append(_p)

# tri -
# dir - string to represent the directory
def myPrint(tri, dir):
    f = open(dir, 'w')
    f.write(str(len(tri.vertices)))
    f.write("\n")
    for i in range(len(tri.vertices)):
        face = tri.vertices[i]

    # for face in tri.vertices:
        f.write(str(tri.dim + 1)) # 맞나?
        f.write(" ")

        for j in range(len(face)):
            value = face[j]
        # for value in face:
            f.write(str(value)) # , sep = ' ', end = ' ')
            if j != len(face)-1:
                f.write(" ")

        if i != len(tri.vertices)-1:
            f.write("\n")
    f.close()

# PS - point set
def PS2List(PS):
    L = []
    for p in PS.points:
        pv = p.values
        L.append(pv)
    return L

class Point:
    def __init__(self, _dimension, _values = None):
        self.dimension = _dimension

        if _values == None:
            self.values = []
        else:
            self.values = _values

    # bound as cube
    def setRandom(self, bound, _valList=[], _L=[]):

        if bound == None:
            self.values = []
            for i in range(len(_L)):
                val = random.uniform(_valList[_L[i]], _valList[_L[i] + 1] - eps)
                self.values.append(val)

        else:
            # 초기화한 뒤
            self.values = []
            for i in range(self.dimension):
                val = random.uniform(-bound, bound)
                self.values.append(val)

    # def setRandom(self, _valList, _L):


# 000에서 999까지 string으로 리턴하는 함수
# three digit number
def threeDigitNum(n):
    if 0 <= n and n <= 9:
        return '00' + str(n)
    elif 10 <= n and n <= 99:
        return '0' + str(n)
    elif 100 <= n and n <= 999:
        return str(n)

# 00에서 99까지 string으로 리턴하는 함수
def twoDigitNum(n):
    if 0 <= n and n <= 9:
        return '00' + str(n)
    elif 10 <= n and n <= 99:
        return '0' + str(n)

def makedirs(path):
    if not os.path.exists(path):
        os.makedirs(path)

#
def makeMidPointsList(bound, numIntervalsPerAxis):
    L = []
    L.append(-bound)
    curValue = -bound
    for i in range(numIntervalsPerAxis - 1):
        curValue += 2 * bound / numIntervalsPerAxis
        L.append(curValue)
    # L.append(2 * bound )
    L.append(bound)
    return L

# point를 받아서 어느 cube에 있는지 확인
# 랜덤 생성한 point가, polytope이 들어있는 cell에 없을 때까지 계속 랜덤을 돌릴 때
# 이 함수를 서브루틴으로 사용
def inWhichCubeList(midPointsList, p):
    # midPointListCopy = []
    # for val in midPointList:
    #     midPointListCopy

    L = []
    for val in p:
        for i in range(len(midPointsList)):
            if midPointsList[i] <= val and val < midPointsList[i+1]:
                L.append(i)
                break
    return L

def inWhichCubeNum(midPointsList, p):
    num = 0
    curDigit = 1

    for val in p:
        for i in range(len(midPointsList)-1, -1, -1):
            if midPointsList[i] <= val and val < midPointsList[i+1]:
                num += i * curDigit
                curDigit *= len(midPointsList) - 1
                break

    return num

    # for numIntervalsPerAxis

# bound - double (실수 값 하나)
# Box는 각 차원에 대해서 [-bound, bound]
# p - list of doubles (size d: dimension)
def outOfBoxRange(bound, p):
    for i in range(len(p)):
        if p[i] < -bound or bound <= p[i]:
            return True
    return False


#
def computeCellsRandom(dim, numIntervalsPerAxis, numCells):
    L = []

    for i in range(numCells):
        # val2 = random.randint(0, pow(3, 7) - 1)
        # val3 = random.randint(0, pow(1, 8) - 1)
        val = random.randint(0, pow(numIntervalsPerAxis, dim) - 1)
        L.append(val)

    return L

# result = []
# while len(result) < 6:
#     num = random.randint(1, 45)  # 1~45 사이의 숫자중 임의의 숫자 생성
#     if num not in result:  # 중복 숫자 뽑기 방지
#         result.append(num)

#
def selectEmptyCubeRandom(_boolList, dim, numIntervalsPerAxis):
    #
    if (_boolList.count):
        pass

    #

    L = []
    for i in dim:
        L.append(random.randint(0, numIntervalsPerAxis - 1))
    return L

# 낮은 value는 closed, 높은 value는 open
# newDigit 진법
def num2ind(dimension, numIntervalsPerAxis, num): # newDigit):
    index = []

    temp = 1
    repeatL = [1]
    for i in range(dimension - 1):
        temp *= numIntervalsPerAxis
        repeatL.insert(0, temp)
        # repeatL.append(numIntervalsPerAxis)

    for i in range(len(repeatL)):
        if i == len(repeatL) - 1:
            index.append(num // repeatL[i])
            # index.append(num % repeatL[i])
        else :
            index.append(num // repeatL[i])
            num %= repeatL[i]

    # num

    return index

"""
vector<int> num2index(vector<int> num_subspaces_over_axes, vector<int> denominators, int num) {
	vector<int> index;

	// copy i to compute index
	for (int j = 0; j < denominators.size(); j++) {
		if (j == denominators.size() - 1) {
			index.push_back(num / denominators[j]);
			index.push_back(num % denominators[j]);
		}
		else {
			index.push_back(num / denominators[j]);
			num %= denominators[j];
		}
	}

	return index;
}
"""

# return type - list of d-dim points (size = numPoints)
def generateRandomPoints(bound, dimension, numPoints):
    points = []

    for i in range(numPoints):
        point = []
        for j in range(dimension):
            point.append(random.uniform(-bound, bound))
        points.append(point)

    return points

# Press the green button in the gutter to run the script.
if __name__ == '__main__':

    # pts = [[-0.5, -0.5], [-0.5, 0.5], [0.5, -0.5], [0.5, 0.5], [0, 0]]
    # tri = DelaunayTri(pts)
    # print(tri.vertices)

    createDirsFlag = True
    deleteFilesFlag = True
    speedFlag = True

    bound = 100.0
    dimension = 4

    # 데이터셋 총 100개
    # polytope 10개, point 10000개
    numIterations = 100
    numPolytopes = 10
    numPoints = 10000
    # 4^5 = 1024개 cell
    numIntervalsPerAxis = 5
    numQueries = 1000

    midPointsList = makeMidPointsList(bound, numIntervalsPerAxis)

    numClusters = 10

    if createDirsFlag:
        # 폴더 만들기
        if speedFlag:
            storeDir = "C:/Users/HWI/Documents/epsGraphTestDataSpeed"
            numIterations = 80
        else:
            storeDir = "C:/Users/HWI/Documents/epsGraphTestData"

        # 데이터셋 100개니까
        # 00 - 99까지의 리스트
        for i in range(numIterations):

            if speedFlag:
                if 0 <= i < 20:
                    numPolytopes = 10
                elif 20 <= i < 40:
                    numPolytopes = 25
                elif 40 <= i < 60:
                    numPolytopes = 50
                elif 60 <= i < 80:
                    numPolytopes = 75


            tempDir = storeDir + "/" + twoDigitNum(i)
            makedirs(tempDir)

            tempPolytopesDir = tempDir + "/" + "polytopes"
            for j in range(numPolytopes):
                makedirs(tempPolytopesDir + "/" + str(j))

            tempPointsDir = tempDir + "/" + "points"
            makedirs(tempPointsDir)

        print('created directories')


    if deleteFilesFlag:
        if speedFlag:
            storeDir = "C:/Users/HWI/Documents/epsGraphTestDataSpeed"
            numIterations = 80
        else:
            storeDir = "C:/Users/HWI/Documents/epsGraphTestData"

        # 데이터셋 100개니까
        # 00 - 99까지의 리스트
        for i in range(numIterations):

            if speedFlag:
                if 0 <= i < 20:
                    numPolytopes = 10
                elif 20 <= i < 40:
                    numPolytopes = 25
                elif 40 <= i < 60:
                    numPolytopes = 50
                elif 60 <= i < 80:
                    numPolytopes = 75

            tempDir = storeDir + "/" + twoDigitNum(i)
            # makedirs(tempDir)

            tempPolytopesDir = tempDir + "/" + "polytopes"
            for j in range(numPolytopes):
                # makedirs(tempPolytopesDir + "/" + str(j))
                # 파일 있는지 확인하고 있으면 지우기
                for file in os.scandir(tempPolytopesDir + "/" + str(j)):
                    os.remove(file.path)

            tempPointsDir = tempDir + "/" + "points"
            for file in os.scandir(tempPointsDir):
                os.remove(file.path)
            # makedirs(tempPointsDir)

        print('deleted existing files')



    # print('bound: ', bound)
    # print('dimension: ', dimension)

    for i in range(numIterations):

        if speedFlag:
            if 0 <= i < 20:
                numPolytopes = 10
                numPoints = 20000
            elif 20 <= i < 40:
                numPolytopes = 25
                numPoints = 50000
            elif 40 <= i < 60:
                numPolytopes = 50
                numPoints = 100000
            elif 60 <= i < 80:
                numPolytopes = 75
                numPoints = 200000

        # "Users\HWI\Documents"
        if speedFlag:
            storeDir = "C:/Users/HWI/Documents/epsGraphTestDataSpeed"
        else:
            storeDir = "C:/Users/HWI/Documents/epsGraphTestData"

        storeDir += "/" + twoDigitNum(i)

        # print('iteration #', i)

        # first
        # prevent overlapping

        # generate 10 polytopes
        # each of minimum size (d+1 points)

        # ex) C:/Users/HWI/Documents/epsGraphTestData/00/polytopes
        storePolytopesDir = storeDir + "/" + "polytopes"

        cells = computeCellsRandom(dimension, numIntervalsPerAxis, numPolytopes)

        for j in range(numPolytopes):

            # ex) C:/Users/HWI/Documents/epsGraphTestData/00/polytopes/0/
            temp = storePolytopesDir + "/" + str(j) + "/"

            # ex) C:/Users/HWI/Documents/epsGraphTestData/00/polytopes/0/points.txt
            tempPoints = temp + "points.txt"
            # ex) C:/Users/HWI/Documents/epsGraphTestData/00/polytopes/0/tets.txt
            tempTets = temp + "tets.txt"

            # while()
            # selectEmptyCubeRandom
            # thisCubeEmpty

            # generate point set
            # PS = PointSet(dimension, dimension+2, bound)
            # def __init__(self, _dimension, _num_points, _valList, _L, ):
            # PS = PointSet(dimension, cells[j], dimension+2, None, midPointsList, num2ind(dimension, numIntervalsPerAxis, cells[j]))
            PS = PointSet(dimension, dimension+2, None, midPointsList, num2ind(dimension, numIntervalsPerAxis, cells[j]))
            PSList = PS2List(PS)

            PS.print(tempPoints)
            # PS.print(storePointsDir)

            # tetrahedralize (after computing convex hull)
            # data type of tri - temp
            tri = DelaunayTri(PSList)
            myPrint(tri, tempTets)

        storePointsDir = storeDir + "/" + "points"

        # generate point set of size 10000

        if (not speedFlag and i >= 50) or (speedFlag and i // 10 % 2 == 1): # 49:
            # uniform
            PS = PointSet(dimension, numPoints, bound)
        else :
            # clustered
            # clusters = []

            numPointsPerCluster = numPoints / numClusters
            totalPoints = []

            for _ in range(numClusters):
                pt = generateRandomPoints(bound, dimension, 1)
                center = pt[0]
                # variance는 차원별로 동일
                revVariance = random.uniform(2, 5)
                variance = bound * (1 / revVariance)


                # 각 cluster마다 numPoints / numClusters 만큼 분배 (같은 수만큼)

                curPoints = []
                # curNumPoints = 0

                while len(curPoints) < numPointsPerCluster:
                    # 축별로 1d normal distribution으로
                    # point를 만들고

                    p = []
                    for jj in range(len(center)):
                        val = np.random.normal(center[jj], variance, 1)
                        p.append(val.item())

                    # 만약 그게 옳으면 append
                    if not outOfBoxRange(bound, p):
                        num = inWhichCubeNum(midPointsList, p)
                        if num not in cells:
                            curPoints.append(Point(dimension, p))

                # revVariance

                # random.uniform()

                totalPoints += curPoints

            PS = PointSet(_dimension = dimension, _num_points = len(totalPoints), pointsReady = totalPoints)

        PS.print(storePointsDir + "/" + "points.txt")

        # generate query points

        # numFinalPoints = 0
        # while numFinalPoints != 100:

        points = []

        for j in range(numQueries):

            while True:
                randNum = random.randint(0, pow(numIntervalsPerAxis, dimension) - 1)
                if randNum not in cells:
                    print('randNum selected, ' + str(randNum))
                    break

            PS = PointSet(dimension, 1, None, midPointsList, num2ind(dimension, numIntervalsPerAxis, randNum))
            points.append(PS.points[0])

        PSfinal = PointSet(_dimension = dimension, _num_points = numQueries, pointsReady = points)
        PSfinal.print(storePointsDir + "/" + "queries.txt")
        # queryPoint

        # generate points
        # points

        print('finished generating data set ' + str(i))

# See PyCharm help at https://www.jetbrains.com/help/pycharm/
