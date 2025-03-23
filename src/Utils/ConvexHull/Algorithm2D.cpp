#include "Algorithm2D.hpp"

#include <limits>
#include <vector>
#include <algorithm>

#include "Constants.hpp"

std::vector<Vector2d *>
ConvexHull2DAlgorithm::Create(std::vector<Vector2d> &points, double tolerance)
{
    // instead of calling points.size() several times, we create this
    // variable. by the ways points is unaffected by this method
    auto numPoints = points.size();
    if (numPoints <= 2)
        return {};
    // #region Step 1 : Define Convex Octogon

    /* The first step is to quickly identify the three to eight vertices
     * based on the Akl-Toussaint heuristic. */
    auto maxX = std::numeric_limits<double>::lowest();
    auto maxXIndex = -1;
    auto maxY = std::numeric_limits<double>::lowest();
    auto maxYIndex = -1;
    auto maxSum = std::numeric_limits<double>::lowest();
    auto maxSumIndex = -1;
    auto maxDiff = std::numeric_limits<double>::lowest();
    auto maxDiffIndex = -1;
    auto minX = std::numeric_limits<double>::max();
    auto minXIndex = -1;
    auto minY = std::numeric_limits<double>::max();
    auto minYIndex = -1;
    auto minSum = std::numeric_limits<double>::max();
    auto minSumIndex = -1;
    auto minDiff = std::numeric_limits<double>::max();
    auto minDiffIndex = -1;
    // search of all points to find the extrema. What is stored here is the
    // position (or index) within points and the value
    for (auto i = 0; i < numPoints; i++) {
        auto p = points[i];
        auto x = p.x;
        auto y = p.y;
        auto sum = x + y;
        auto diff = x - y;
        if (x < minX) {
            minXIndex = i;
            minX = x;
        }

        if (y < minY) {
            minYIndex = i;
            minY = y;
        }

        if (x > maxX) {
            maxXIndex = i;
            maxX = x;
        }

        if (y > maxY) {
            maxYIndex = i;
            maxY = y;
        }

        // so that's the Akl-Toussaint (to find extrema in x and y). here,
        // we go a step further and check the sum and difference of x and y.
        // instead of a initial convex quadrilateral we have (potentially) a
        // convex octagon. Because we are adding or substracting there is a
        // slight time penalty, but that seems to be made up in the next two
        // parts where having more sortedlists (with fewer elements each) is
        // faster than fewer sortedlists (with more elements).
        if (sum < minSum) {
            minSumIndex = i;
            minSum = sum;
        }

        if (diff < minDiff) {
            minDiffIndex = i;
            minDiff = diff;
        }

        if (sum > maxSum) {
            maxSumIndex = i;
            maxSum = sum;
        }

        if (diff > maxDiff) {
            maxDiffIndex = i;
            maxDiff = diff;
        }
    }
    // what if all points are on a horizontal line? temporarily set to max
    // and min Y to min X. This'll be fixed in the function:
    // FindIntermediatePointsForLongSkinny
    if (minY == maxY)
        minYIndex = maxYIndex = minXIndex;
    // what if all points are on a vertical line? then do the opposite
    if (minX == maxX)
        minXIndex = maxXIndex = minYIndex;
    // put these on a list in counter-clockwise (CCW) direction
    std::vector<int> extremeIndices{minXIndex,    minSumIndex, minYIndex,
                                    maxDiffIndex, maxXIndex,   maxSumIndex,
                                    maxYIndex,    minDiffIndex};
    // in some cases, we need to reduce from this eight to a smaller set
    auto cvxVNum = 8;
    // The next two loops handle this reduction from 8 to as few as 3.
    // In the first loop, simply check if any indices are repeated. Thanks
    // to the CCW order, any repeat indices are adjacent on the list. Start
    // from the back of the loop and remove towards zero.
    for (int i = cvxVNum - 1; i >= 0; i--) {
        auto thisExtremeIndex = extremeIndices[i];
        auto nextExtremeIndex =
            (i == cvxVNum - 1) ? extremeIndices[0] : extremeIndices[i + 1];
        auto dx =
            std::abs(points[thisExtremeIndex].x - points[nextExtremeIndex].x);
        auto dy =
            std::abs(points[thisExtremeIndex].y - points[nextExtremeIndex].y);
        if (thisExtremeIndex == nextExtremeIndex ||
            (dx < tolerance && dy < tolerance)) {
            cvxVNum--;
            extremeIndices.erase(extremeIndices.begin() + i);
        }
    }
    // before we check if points are on top of one another or have some
    // round-off error issues, these indices are stored and sorted
    // numerically for use in the second half of part 2 where we go through
    // all the points a second time.
    std::vector<int> indicesUsed = extremeIndices;
    std::sort(indicesUsed.begin(), indicesUsed.end());

    // create the list that is eventually returned by the function.
    // Initially it will have the 3 to 8 extrema (as is produced in the
    // following loop).
    std::vector<Vector2d *> convexHullCCW;

    // on very rare occasions (long skinny diagonal set of points), there
    // may only be two extrema. in this case just add
    if (cvxVNum == 2) {
        std::vector<int> newUsedIndices;
        convexHullCCW = FindIntermediatePointsForLongSkinny(
            points, numPoints, indicesUsed[0], indicesUsed[1], newUsedIndices);
        if (newUsedIndices.empty())
            // looks like only two indices total! so all points are co-linear.
            return {};
        newUsedIndices.push_back(indicesUsed[0]);
        newUsedIndices.push_back(indicesUsed[1]);
        indicesUsed = newUsedIndices;
        std::sort(indicesUsed.begin(), indicesUsed.end());
        cvxVNum = indicesUsed.size();
    } else {
        for (auto i = cvxVNum - 1; i >= 0; i--) {
            // in other rare cases, often due to some roundoff error, the
            // extrema point will produce a concavity with its two
            // neighbors. Here, we check that case. If it does make a
            // concavity we don't use it in the initial convex hull (we have
            // captured its index and will still skip it below. it will not
            // be searched a second time). counting backwards again, we grab
            // the previous and next point and check the "cross product" to
            // see if the vertex in convex. if it is we add it to the
            // returned list.
            auto &currentPt = points[extremeIndices[i]];
            auto &prevPt = points[(i == 0) ? extremeIndices[cvxVNum - 1]
                                           : extremeIndices[i - 1]];
            auto &nextPt = points[(i == cvxVNum - 1) ? extremeIndices[0]
                                                     : extremeIndices[i + 1]];
            if ((nextPt.x - currentPt.x) * (prevPt.y - currentPt.y) +
                    (nextPt.y - currentPt.y) * (currentPt.x - prevPt.x) >
                tolerance) {
                // because we are counting backwards, we need to ensure that
                // new points are added to the front of the list
                convexHullCCW.insert(convexHullCCW.begin(), &currentPt);
            } else {
                cvxVNum--;
                // the only reason to do this is to ensure that - if the
                // loop is to continue - that the vectors are made to the
                // proper new adjacent vertices
                extremeIndices.erase(extremeIndices.begin() + i);
            }
        }
    }
    // #region Step 2 : Create the sorted zig - zag line for each extrema edge

    /* Of the 3 to 8 vertices identified in the convex hull, ... */

    // #region Set local variables for the points in the convex hull

    // This is used to limit the number of calls to convexHullCCW[] and
    // point.X and point.Y, which can take a significant amount of time.
    // Initialize the point locations and vectors:
    // At minimum, the convex hull must contain two points (e.g. consider
    // three points in a near line, the third point will be added later,
    // since it was not an extreme.)
    auto &p0 = *convexHullCCW[0];
    auto p0X = p0.x;
    auto p0Y = p0.y;
    auto &p1 = *convexHullCCW[1];
    auto p1X = p1.x;
    auto p1Y = p1.y;
    double p2X = 0, p2Y = 0, p3X = 0, p3Y = 0, p4X = 0, p4Y = 0, p5X = 0,
           p5Y = 0, p6X = 0, p6Y = 0, p7X = 0, p7Y = 0;
    auto v0X = p1X - p0X;
    auto v0Y = p1Y - p0Y;
    double v1X, v1Y, v2X = 0, v2Y = 0, v3X = 0, v3Y = 0, v4X = 0, v4Y = 0,
                     v5X = 0, v5Y = 0, v6X = 0, v6Y = 0, v7X = 0, v7Y = 0;
    // A big if statement to make sure the convex hull wraps properly, since
    // the number of initial cvxHull points changes
    if (cvxVNum > 2) {
        auto &p2 = *convexHullCCW[2];
        p2X = p2.x;
        p2Y = p2.y;
        v1X = p2X - p1X;
        v1Y = p2Y - p1Y;
        if (cvxVNum > 3) {
            auto &p3 = *convexHullCCW[3];
            p3X = p3.x;
            p3Y = p3.y;
            v2X = p3X - p2X;
            v2Y = p3Y - p2Y;
            if (cvxVNum > 4) {
                auto &p4 = *convexHullCCW[4];
                p4X = p4.x;
                p4Y = p4.y;
                v3X = p4X - p3X;
                v3Y = p4Y - p3Y;
                if (cvxVNum > 5) {
                    auto &p5 = *convexHullCCW[5];
                    p5X = p5.x;
                    p5Y = p5.y;
                    v4X = p5X - p4X;
                    v4Y = p5Y - p4Y;
                    if (cvxVNum > 6) {
                        auto &p6 = *convexHullCCW[6];
                        p6X = p6.x;
                        p6Y = p6.y;
                        v5X = p6X - p5X;
                        v5Y = p6Y - p5Y;
                        if (cvxVNum > 7) {
                            auto &p7 = *convexHullCCW[7];
                            p7X = p7.x;
                            p7Y = p7.y;
                            v6X = p7X - p6X;
                            v6Y = p7Y - p6Y;
                            // Wrap around from 7
                            v7X = p0X - p7X;
                            v7Y = p0Y - p7Y;
                        } else // Wrap around from 6
                        {
                            v6X = p0X - p6X;
                            v6Y = p0Y - p6Y;
                        }
                    } else // Wrap around from 5
                    {
                        v5X = p0X - p5X;
                        v5Y = p0Y - p5Y;
                    }
                } else {
                    // Wrap around from 4
                    v4X = p0X - p4X;
                    v4Y = p0Y - p4Y;
                }
            } else {
                // Wrap around from 3
                v3X = p0X - p3X;
                v3Y = p0Y - p3Y;
            }
        } else {
            // Wrap around from 2
            v2X = p0X - p2X;
            v2Y = p0Y - p2Y;
        }
    } else {
        // Wrap around from 1
        v1X = p0X - p1X;
        v1Y = p0Y - p1Y;
    }

    /* An array of arrays of new convex hull points along the sides of the
     * polygon created by the 3 to 8 points above. These are to be sorted
     * arrays and they are sorted by the distances (stored in
     * sortedDistances) from the started extrema vertex to the last. We are
     * going to make each array really big so that we don't have to waste
     * time extending them later. The sizes array keeps the true length. */
    std::vector<std::vector<Vector2d *>> sortedPoints(cvxVNum);
    std::vector<std::vector<double>> sortedDistances(cvxVNum);
    std::vector<int> sizes(cvxVNum);
    for (int i = 0; i < cvxVNum; i++) {
        sizes[i] = 0;
        sortedPoints[i].resize(numPoints);
        sortedDistances[i].resize(numPoints);
    }
    auto indexOfUsedIndices = 0;
    // Note: it increments after getting the current index
    auto nextUsedIndex = indicesUsed[indexOfUsedIndices++];
    /* Now a big loop. For each of the original vertices, check them with
     * the 3 to 8 edges to see if they are inside or out. If they are out,
     * add them to the proper row of the hullCands array. */
    for (auto i = 0; i < numPoints; i++) {
        if (indexOfUsedIndices < indicesUsed.size() && i == nextUsedIndex) {
            // in order to avoid a contains function call, we know to only
            // check with next usedIndex in order
            // Note: it increments after getting the current index
            nextUsedIndex = indicesUsed[indexOfUsedIndices++];
        } else {
            auto &point = points[i];
            auto newPointX = point.x;
            auto newPointY = point.y;
            if (AddToListAlong(sortedPoints[0], sortedDistances[0], sizes[0],
                               point, newPointX, newPointY, p0X, p0Y, v0X, v0Y,
                               tolerance))
                continue;
            if (AddToListAlong(sortedPoints[1], sortedDistances[1], sizes[1],
                               point, newPointX, newPointY, p1X, p1Y, v1X, v1Y,
                               tolerance))
                continue;
            if (AddToListAlong(sortedPoints[2], sortedDistances[2], sizes[2],
                               point, newPointX, newPointY, p2X, p2Y, v2X, v2Y,
                               tolerance))
                continue;
            if (cvxVNum == 3)
                continue;
            if (AddToListAlong(sortedPoints[3], sortedDistances[3], sizes[3],
                               point, newPointX, newPointY, p3X, p3Y, v3X, v3Y,
                               tolerance))
                continue;
            if (cvxVNum == 4)
                continue;
            if (AddToListAlong(sortedPoints[4], sortedDistances[4], sizes[4],
                               point, newPointX, newPointY, p4X, p4Y, v4X, v4Y,
                               tolerance))
                continue;
            if (cvxVNum == 5)
                continue;
            if (AddToListAlong(sortedPoints[5], sortedDistances[5], sizes[5],
                               point, newPointX, newPointY, p5X, p5Y, v5X, v5Y,
                               tolerance))
                continue;
            if (cvxVNum == 6)
                continue;
            if (AddToListAlong(sortedPoints[6], sortedDistances[6], sizes[6],
                               point, newPointX, newPointY, p6X, p6Y, v6X, v6Y,
                               tolerance))
                continue;
            if (cvxVNum == 7)
                continue;
            if (AddToListAlong(sortedPoints[7], sortedDistances[7], sizes[7],
                               point, newPointX, newPointY, p7X, p7Y, v7X, v7Y,
                               tolerance))
                continue;
        }
    }

    // #region Step 3 : now remove concave "zigs" from each sorted dictionary

    /* Now it's time to go through our array of sorted arrays. We search
     * backwards through the current convex hull points s.t. any additions
     * will not confuse our for-loop indexers. This approach is linear over
     * the zig-zag polyline defined by each sorted list. This linear
     * approach was defined long ago by a number of authors: McCallum and
     * Avis, Tor and Middleditch (1984), or Melkman (1985) */
    for (auto j = cvxVNum - 1; j >= 0; j--) {
        auto size = sizes[j];
        if (size == 1)
            /* If there is one and only one candidate, it must be in the
             * convex hull. Add it now. */
            convexHullCCW.insert(convexHullCCW.begin() + j + 1,
                                 sortedPoints[j][0]);
        else if (size > 1) {
            /* it seems a shame to have this list since it's nearly the same
             * as the sorted array, but it is necessary for the removal of
             * points. */
            std::vector<Vector2d *> pointsAlong;
            /* put the known starting point as the beginning of the list. */
            pointsAlong.push_back(convexHullCCW[j]);
            for (int k = 0; k < size; k++)
                pointsAlong.push_back(sortedPoints[j][k]);
            /* put the ending point on the end of the list. Need to check if
             * it wraps back around to the first in the loop (hence the
             * simple condition). */
            if (j == cvxVNum - 1)
                pointsAlong.push_back(convexHullCCW[0]);
            else
                pointsAlong.push_back(convexHullCCW[j + 1]);

            /* Now starting from second from end, work backwards looks for
             * places where the angle between the vertices is concave (which
             * would produce a negative value of z). */
            auto i = size;
            while (i > 0) {
                // auto currentPoint =
                double lX = pointsAlong[i]->x - pointsAlong[i - 1]->x,
                       lY = pointsAlong[i]->y - pointsAlong[i - 1]->y;
                double rX = pointsAlong[i + 1]->x - pointsAlong[i]->x,
                       rY = pointsAlong[i + 1]->y - pointsAlong[i]->y;
                double zValue = lX * rY - lY * rX;
                if (zValue < tolerance ||
                    (std::abs(lX) < tolerance && std::abs(lY) < tolerance)) {
                    /* remove any vertices that create concave angles. */
                    pointsAlong.erase(pointsAlong.begin() + i);
                    /* but don't reduce k since we need to check the
                     * previous angle again. Well, if you're back to the end
                     * you do need to reduce k (hence the line below). */
                    if (i == pointsAlong.size() - 1)
                        i--;
                }
                /* if the angle is convex,then continue toward the start, k-- */
                else
                    i--;
            }

            /* for each of the remaining vertices in hullCands[i-1], add them to
             * the convexHullCCW. Here we insert them backwards (k counts down)
             * to simplify the insert operation (k.e. since all are inserted @
             * i, the previous inserts are pushed up to i+1, i+2, etc. */
            for (i = pointsAlong.size() - 2; i > 0; i--)
                convexHullCCW.insert(convexHullCCW.begin() + j + 1,
                                     pointsAlong[i]);
        }
    }

    return convexHullCCW;
}

std::vector<Vector2d *>
ConvexHull2DAlgorithm::FindIntermediatePointsForLongSkinny(
    std::vector<Vector2d> &points, int numPoints, int usedIndex1,
    int usedIndex2, std::vector<int> &newUsedIndices)
{
    auto pStartX = points[usedIndex1].x;
    auto pStartY = points[usedIndex1].y;
    auto spanVectorX = points[usedIndex2].x - pStartX;
    auto spanVectorY = points[usedIndex2].y - pStartY;
    auto minCross = -DefaultPlaneDistanceTolerance;
    auto maxCross = DefaultPlaneDistanceTolerance;
    auto minCrossIndex = -1;
    auto maxCrossIndex = -1;
    for (auto i = 0; i < numPoints; i++) {
        if (i == usedIndex1 || i == usedIndex2)
            continue;
        auto p = points[i];
        auto cross =
            spanVectorX * (p.y - pStartY) + spanVectorY * (pStartX - p.x);
        if (cross < minCross) {
            minCrossIndex = i;
            minCross = cross;
        }
        if (cross > maxCross) {
            maxCrossIndex = i;
            maxCross = cross;
        }
    }

    std::vector<Vector2d *> newCvxList;
    newCvxList.push_back(&points[usedIndex1]);
    if (minCrossIndex != -1) {
        newUsedIndices.push_back(minCrossIndex);
        newCvxList.push_back(&points[minCrossIndex]);
    }
    newCvxList.push_back(&points[usedIndex2]);
    if (maxCrossIndex != -1) {
        newUsedIndices.push_back(maxCrossIndex);
        newCvxList.push_back(&points[maxCrossIndex]);
    }
    return newCvxList;
}

bool ConvexHull2DAlgorithm::AddToListAlong(
    std::vector<Vector2d *> &sortedPoints, std::vector<double> &sortedKeys,
    int &size, Vector2d &newPoint, double newPointX, double newPointY,
    double basePointX, double basePointY, double edgeVectorX,
    double edgeVectorY, double tolerance)
{
    auto vectorToNewPointX = newPointX - basePointX;
    auto vectorToNewPointY = newPointY - basePointY;
    auto newDxOut =
        vectorToNewPointX * edgeVectorY - vectorToNewPointY * edgeVectorX;
    if (newDxOut <= tolerance)
        return false;
    auto newDxAlong =
        edgeVectorX * vectorToNewPointX + edgeVectorY * vectorToNewPointY;
    int index = BinarySearch(sortedKeys.data(), size, newDxAlong);
    if (index >= 0) {
        // non-negative values occur when the same key is found. In this case,
        // we only want to keep the one vertex that sticks out the farthest.
        auto &ptOnList = *sortedPoints[index];
        auto onListDxOut = (ptOnList.x - basePointX) * edgeVectorY -
                           (ptOnList.y - basePointY) * edgeVectorX;
        if (newDxOut > onListDxOut)
            sortedPoints[index] = &newPoint;
    } else {
        // here a new value is found. first, invert the index to find where
        // to put it
        index = ~index;
        // as a slight time saver, we can check the two points that will
        // surround this new point. If it makes a concave corner then don't
        // add it. this part is actually in the middle condition ("else if
        // (index < size)"). We don't need to perform this check if the
        // insertion is at either at. At the beginning ("index == 0"), we
        // still need to increment the rest of the list
        if (index == 0) {
            for (int i = size; i > index; i--) {
                sortedKeys[i] = sortedKeys[i - 1];
                sortedPoints[i] = sortedPoints[i - 1];
            }
            sortedKeys[index] = newDxAlong;
            sortedPoints[index] = &newPoint;
            size++;
        } else if (index < size) {
            auto &prevPt = *sortedPoints[index - 1];
            auto &nextPt = *sortedPoints[index];
            double lX = newPointX - prevPt.x, lY = newPointY - prevPt.y;
            double rX = nextPt.x - newPointX, rY = nextPt.y - newPointY;
            double zValue = lX * rY - lY * rX;
            // if cross produce is negative (well, is less than some small
            // positive number, then new point is concave) then don't add
            // it. also, don't add it if the point is nearly identical
            // (again, within the tolerance) of the previous point.
            if (zValue < tolerance ||
                (std::abs(lX) < tolerance && std::abs(lY) < tolerance))
                return false;
            // Else
            for (int i = size; i > index; i--) {
                sortedKeys[i] = sortedKeys[i - 1];
                sortedPoints[i] = sortedPoints[i - 1];
            }
            sortedKeys[index] = newDxAlong;
            sortedPoints[index] = &newPoint;
            size++;
        } else { // if at the end, then no need to increment any other members.
            sortedKeys[index] = newDxAlong;
            sortedPoints[index] = &newPoint;
            size++;
        }
    }
    return true;
}

int ConvexHull2DAlgorithm::BinarySearch(double *data, int length, double value)
{
    auto lo = 0;
    auto hi = length - 1;
    while (lo <= hi) {
        int i = lo + ((hi - lo) >> 1);
        auto c = data[i];
        if (c == value)
            return i;
        if (c < value)
            lo = i + 1;
        else
            hi = i - 1;
    }
    return ~lo;
}
