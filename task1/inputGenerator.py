"""
    This program is a script to generate input for task 1C
"""
import random
import math

N_SMALL = 10
N_MEDIUM = 100
N_LARGE = 10000

X_MIN = 0
X_MAX = 1000
Y_MIN = 0
Y_MAX = 1000

CIRCLE_CENTER_X = 500
CIRCLE_CENTER_Y = 500
CIRCLE_RADIUS = 500

ERROR = 0.1

def makeRandPoints(numPoints, xMin, xMax, yMin, yMax):
    """
    Generates a set of <numPoints> random points within the range
    of given X and Y values. This function returns a list of tuples,
    each representing a random point, containing its respective (X, Y)
    coordinates.
    """

    points = []

    # Generate random points within given range
    for point in range(numPoints):
        xCoord = random.uniform(xMin, xMax)
        yCoord = random.uniform(yMin, yMax)
        points.append((xCoord, yCoord))
    
    return points

def makePointsOnCircle(numPoints, radius, centerX, centerY):
    """
    Generates a set of <numPoints> evenly spaced points on a circle's 
    circumference given the circle's radius and center point. This
    function returns a list of tuples, each representing a point on
    the circle, containing its respective (X, Y) coordinates.
    """

    points = []

    for point in range(numPoints):
        # Find the counterclockwise angle from the x-axis to the point
        angle = 2 * math.pi / numPoints * point
        # Find the point's coordinates using trigonometry
        xCoord = centerX + radius * math.cos(angle)
        yCoord = centerY + radius * math.sin(angle)
        points.append((xCoord, yCoord))
    
    return points

def makeRandPointsInSquareHull(numPoints, xMin, xMax, yMin, yMax):
    """
    Make a square convex hull with 4 points that covers the given X and Y
    range. Then generate random points within, but not on, the hull. This
    function returns a list of tuples, each representing a random point
    within the hull, containing its respective (X, Y) coordinates. The
    total number of points in the list are <numPoints> (including the 4 
    that make up the square convex hull).
    """

    # Add the 4 points that make up the square convex hull to the list
    points = [(xMin, yMin), (xMin, yMax), (xMax, yMin), (xMax, yMax)]

    # Generate random points within the hull
    # ERROR is used to make sure no generated point lies on the hull
    for point in range(numPoints - len(points)):
        xCoord = random.uniform(xMin + ERROR, xMax - ERROR)
        yCoord = random.uniform(yMin + ERROR, yMax - ERROR)
        points.append((xCoord, yCoord))

    return points

def makeTextFile(coordList, fileName):
    """
    Turn a list of coordinates into a text file which can be read
    as input by the program in task 1C
    """
    with open(fileName, 'w') as file:
        file.write(f"{len(coordList)}\n")
        for x, y in coordList:
            file.write(f"{x} {y}\n")

# Generate different input types and convert to .txt file to be used as input
directory = "my_input_sets"

input1 = makeRandPoints(N_SMALL, X_MIN, X_MAX, Y_MIN, Y_MAX)
makeTextFile(input1, f"{directory}/input1.txt")

input2 = makeRandPoints(N_MEDIUM, X_MIN, X_MAX, Y_MIN, Y_MAX)
makeTextFile(input2, f"{directory}/input2.txt")

input3 = makeRandPoints(N_LARGE, X_MIN, X_MAX, Y_MIN, Y_MAX)
makeTextFile(input3, f"{directory}/input3.txt")

input4 = makePointsOnCircle(N_SMALL, CIRCLE_RADIUS, CIRCLE_CENTER_X, CIRCLE_CENTER_Y)
makeTextFile(input4, f"{directory}/input4.txt")

input5 = makePointsOnCircle(N_MEDIUM, CIRCLE_RADIUS, CIRCLE_CENTER_X, CIRCLE_CENTER_Y)
makeTextFile(input5, f"{directory}/input5.txt")

input6 = makePointsOnCircle(N_LARGE, CIRCLE_RADIUS, CIRCLE_CENTER_X, CIRCLE_CENTER_Y)
makeTextFile(input6, f"{directory}/input6.txt")

input7 = makeRandPointsInSquareHull(N_SMALL, X_MIN, X_MAX, Y_MIN, Y_MAX)
makeTextFile(input7, f"{directory}/input7.txt")

input8 = makeRandPointsInSquareHull(N_MEDIUM, X_MIN, X_MAX, Y_MIN, Y_MAX)
makeTextFile(input8, f"{directory}/input8.txt")

input9 = makeRandPointsInSquareHull(N_LARGE, X_MIN, X_MAX, Y_MIN, Y_MAX)
makeTextFile(input9, f"{directory}/input9.txt")





