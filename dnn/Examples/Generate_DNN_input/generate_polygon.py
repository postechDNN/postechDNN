import math, random
from PIL import Image
from PIL import ImageDraw

def generatePolygon( ctrX, ctrY, aveRadius, irregularity, spikeyness, numVerts ) :
    irregularity = clip( irregularity, 0,1 ) * 2*math.pi / numVerts
    spikeyness = clip( spikeyness, 0,1 ) * aveRadius

    # generate n angle steps
    angleSteps = []
    lower = (2*math.pi / numVerts) - irregularity
    upper = (2*math.pi / numVerts) + irregularity
    sum = 0
    for i in range(numVerts) :
        tmp = random.uniform(lower, upper)
        angleSteps.append( tmp )
        sum = sum + tmp

    # normalize the steps so that point 0 and point n+1 are the same
    k = sum / (2*math.pi)
    for i in range(numVerts) :
        angleSteps[i] = angleSteps[i] / k

    # now generate the points
    points = []
    angle = random.uniform(0, 2*math.pi)
    for i in range(numVerts) :
        r_i = clip( random.gauss(aveRadius, spikeyness), 0, 2*aveRadius )
        x = ctrX + r_i*math.cos(-angle)
        y = ctrY + r_i*math.sin(-angle)
        points.append((x,y))
        angle = angle - angleSteps[i]
    return points

def clip(x, min, max) :
    if( min > max ) :  return x    
    elif( x < min ) :  return min
    elif( x > max ) :  return max
    else :             return x


verts = generatePolygon( ctrX=250, ctrY=250, aveRadius=100, irregularity=0.5, spikeyness=0.6, numVerts=16 )

black = (0,0,0)
white=(255,255,255)
im = Image.new('RGB', (500, 500), white)
imPxAccess = im.load()
draw = ImageDraw.Draw(im)
tupVerts = list(map(tuple,verts))
# either use .polygon(), if you want to fill the area with a solid colour
draw.polygon( tupVerts, outline=black,fill=white )
# or .line() if you want to control the line thickness, or use both methods together!
draw.line( tupVerts+[tupVerts[0]], width=2, fill=black )

im.show()
