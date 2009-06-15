
#include "atQuat.h++"

#include <math.h>
#include <stdio.h>
#include "atGlobals.h++"

// ------------------------------------------------------------------------
// Default constructor - Clears the quaternion to zero.
// ------------------------------------------------------------------------
atQuat::atQuat()
{
    clear();
}

// ------------------------------------------------------------------------
// Data constructor - Sets the quaternion to the given data.
// ------------------------------------------------------------------------
atQuat::atQuat(double x, double y, double z, double w)
{
    set(x, y, z, w);
}

// ------------------------------------------------------------------------
// Data constructor - Sets the quaternion to the given data array.
// ------------------------------------------------------------------------
atQuat::atQuat(const double values[])
{
    set(values);
}

// ------------------------------------------------------------------------
// Destructor
// ------------------------------------------------------------------------
atQuat::~atQuat()
{
}

// ------------------------------------------------------------------------
// Sets the quaternion to the given data.
// ------------------------------------------------------------------------
void atQuat::set(double x, double y, double z, double w)
{
    data[0] = x;
    data[1] = y;
    data[2] = z;
    data[3] = w;
}

// -------------------------------------------------------------------------
// Sets the quaternion to the given data array
// ------------------------------------------------------------------------
void atQuat::set(const double values[])
{
    int i;

    for (i = 0; i < 4; i++)
        data[i] = values[i];
}

// -------------------------------------------------------------------------
// Makes this quaternion an exact duplicate of the source quaternion
// ------------------------------------------------------------------------
void atQuat::copy(const atQuat &source)
{
    (*this) = source;
}

// ------------------------------------------------------------------------
// Sets the quaternion to zero.
// ------------------------------------------------------------------------
void atQuat::clear()
{
    int i;

    for (i = 0; i < 4; i++)
        data[i] = 0.0;
}
    
// ------------------------------------------------------------------------
// Sets one specific data value in the quaternion.
// ------------------------------------------------------------------------
void atQuat::setValue(int index, double value)
{
    // Bounds checking
    if ((index < 0) || (index > 3))
    {
        printf("atQuat::setValue: Bad index\n");
        return;
    }
    
    // Set the specified value
    data[index] = value;
}

// ------------------------------------------------------------------------
// Retrieves one specific data value from the quaternion.
// ------------------------------------------------------------------------
double atQuat::getValue(int index) const
{
    // Bounds checking
    if ((index < 0) || (index > 3))
    {
        printf("atQuat::getValue: Bad index\n");
        return data[0];
    }
    
    // Return the desired value
    return data[index];
}

// ------------------------------------------------------------------------
// Checks for element-wise equality between two quaternions. Two elements
// are considered equal if they are within a small default tolerance value
// of each other.
// ------------------------------------------------------------------------
bool atQuat::isEqual(const atQuat &operand) const
{
    int i;

    // Check each pair of values (this quat's and the operand quat's) for
    // almost-equality; return false if a pair doesn't match up.
    for (i = 0; i < 4; i++)
        if (fabs(data[i] - operand.data[i]) > AT_DEFAULT_TOLERANCE)
            return false;

    // If all the pairs match, return true
    return true;
}

// ------------------------------------------------------------------------
// Checks for element-wise equality between two quaternions. Two elements
// are considered equal if they are within the specified tolerance value
// of each other.
// ------------------------------------------------------------------------
bool atQuat::isAlmostEqual(const atQuat &operand, double tolerance) const
{
    int i;
    
    // Check each pair of values (this quat's and the operand quat's) for
    // almost-equality, 'almost' being specified by a given tolerance
    // value. Return false if a pair doesn't match up.
    for (i = 0; i < 4; i++)
        if (fabs(data[i] - operand.data[i]) > tolerance)
            return false;

    // If all the pairs match, return true
    return true;
}

// ------------------------------------------------------------------------
// Adds the addend quaternion to this one, storing the result in this
// quaternion.
// ------------------------------------------------------------------------
void atQuat::add(const atQuat &addend)
{
    int i;

    // Add each element of the addend quat to this quat
    for (i = 0; i < 4; i++)
        data[i] += addend.data[i];
}

// ------------------------------------------------------------------------
// Adds the addend quaternion to this one, returning the result.
// ------------------------------------------------------------------------
atQuat atQuat::getSum(const atQuat &addend) const
{
    int i;
    atQuat result;

    // Create the target quat by adding each element of this quat to the
    // corresponding element of the addend quat
    for (i = 0; i < 4; i++)
        result.data[i] = data[i] + addend.data[i];

    // Return the target quat
    return result;
}

// ------------------------------------------------------------------------
// Subtracts the subtrahend quaternion from this one, storing the result
// in this quaternion.
// ------------------------------------------------------------------------
void atQuat::subtract(const atQuat &subtrahend)
{
    int i;

    // Subtract each element of the subtrahend quat from this quat
    for (i = 0; i < 4; i++)
        data[i] -= subtrahend.data[i];
}

// ------------------------------------------------------------------------
// Subtracts the subtrahend quaternion from this one, returning the result.
// ------------------------------------------------------------------------
atQuat atQuat::getDifference(const atQuat &subtrahend) const
{
    int i;
    atQuat result;

    // Create the target quat by subtracting each element of the subtrahend
    // from the corresponding element of this quat
    for (i = 0; i < 4; i++)
        result.data[i] = data[i] - subtrahend.data[i];

    // Return the target quat
    return result;
}

// ------------------------------------------------------------------------
// Multiplies each element of this quaternion by the given scalar, storing
// the result in this quaternion.
// ------------------------------------------------------------------------
void atQuat::scale(double multiplier)
{
    int i;

    // Multiply each element of this quat by the given scalar
    for (i = 0; i < 4; i++)
        data[i] *= multiplier;
}

// ------------------------------------------------------------------------
// Multiplies each element of this quaternion by the given scalar,
// returning the result.
// ------------------------------------------------------------------------
atQuat atQuat::getScaled(double multiplier) const
{
    int i;
    atQuat result;
    
    // Create the target quat by multiplying each element of this quat
    // by the given scalar
    for (i = 0; i < 4; i++)
        result.data[i] = data[i] * multiplier;

    // Return the target quat
    return result;
}

// ------------------------------------------------------------------------
// Multiplies this quaternion by the operand quaternion, storing the result
// in this quaternion. Quaternions are multiplied using the equation:
//
//  qq' = [V, w] * [V', w']
//      = [VxV' + wV' + w'V, ww' - V.V']
//
// Where q = [V, w], q' = [V', w'], x denotes cross product, and . denotes
// dot product.
// ------------------------------------------------------------------------
void atQuat::multiplyQuat(const atQuat &operand)
{
    atQuat tempQuat, resultQuat;
    atVector myVec, operandVec, crossVec;
    double dotProd;
    
    // Find the cross product of the vector part of both quaternions (VxV')
    myVec.set(data[0], data[1], data[2]);
    operandVec.set(operand.data[0], operand.data[1], operand.data[2]);
    crossVec = myVec.getCrossProduct(operandVec);
    // Vector part total = VxV'
    resultQuat.set(crossVec[0], crossVec[1], crossVec[2], 0.0);
    
    // Add on the rest of the terms for the vector part
    // Find wV'
    tempQuat = operand;
    tempQuat.data[3] = 0.0;
    tempQuat.scale(data[3]);
    // Vector part total = VxV' + wV'
    resultQuat += tempQuat;
    
    // Find w'V
    tempQuat.set(data[0], data[1], data[2], 0.0);
    tempQuat.scale(operand.data[3]);
    // Vector part total = VxV' + wV' + w'V
    resultQuat += tempQuat;
    
    // Compute the scalar part
    // Find V.V'
    dotProd = myVec.getDotProduct(operandVec);
    // Scalar part total = ww' - V.V'
    resultQuat.data[3] = (data[3] * operand.data[3]) - dotProd;
    
    // Assign the multiplication result back to this quat
    (*this) = resultQuat;
}

// ------------------------------------------------------------------------
// Multiplies this quaternion by the operand quaternion, returning the
// result. This function 'cheats' by just calling multiplyQuat for it's
// answer.
// ------------------------------------------------------------------------
atQuat atQuat::getMultipliedQuat(const atQuat &operand) const
{
    // Create a duplicate of this quat, multiply in the operand quat, and
    // return the result
    atQuat result(data);
    result.multiplyQuat(operand);
    return result;
}

// ------------------------------------------------------------------------
// Returns the magnitude of this quaternion.
// ------------------------------------------------------------------------
double atQuat::getMagnitude() const
{
    int i;
    double result = 0.0;

    // Compute the squared magnitude of this quaternion
    for (i = 0; i < 4; i++)
        result += AT_SQR(data[i]);

    // Return the square root of the square of the magnitude
    return sqrt(result);
}

// ------------------------------------------------------------------------
// Returns the square of the magnitude of this quaternion
// ------------------------------------------------------------------------
double atQuat::getMagnitudeSquared() const
{
    // Return the square of the magnitude
    return (AT_SQR(data[0]) +
            AT_SQR(data[1]) +
            AT_SQR(data[2]) +
            AT_SQR(data[3]));
}

// ------------------------------------------------------------------------
// Returns the 4-D vector dot product of this quaternion and the operand
// quaternion.
// ------------------------------------------------------------------------
double atQuat::getDotProduct(const atQuat &operand) const
{
    int i;
    double result = 0.0;

    // Compute the dot product as the sum of the products of the
    // corresponding elements of each quaternion
    for (i = 0; i < 4; i++)
        result += (data[i] * operand.data[i]);

    // Return the product
    return result;
}

// ------------------------------------------------------------------------
// Returns a normalized version of this quaternion.
// ------------------------------------------------------------------------
atQuat atQuat::getNormalized() const
{
    atQuat result;
    double mag;
    int i;
    
    // Create the target quat by dividing the elements of this quat
    // by this quat's magnitude
    mag = getMagnitude();
    for (i = 0; i < 4; i++)
        result.data[i] = data[i] / mag;

    // Return the target quat
    return result;
}

// ------------------------------------------------------------------------
// Normalizes this quaternion, keeping the result.
// ------------------------------------------------------------------------
void atQuat::normalize()
{
    double mag;
    int i;
    
    // Divide this quat by its magnitude
    mag = getMagnitude();
    for (i = 0; i < 4; i++)
        data[i] /= mag;
}

// ------------------------------------------------------------------------
// Conjugates this quaternion, keeping the result. Quaternion conjugation
// negates the vector portion but leaves the scalar portion unchanged.
// If the quaternion represents a rotation, the conjugate is the opposite
// rotation.
// ------------------------------------------------------------------------
void atQuat::conjugate()
{
    int i;
    
    // Negate the vector portion of this quat
    for (i = 0; i < 3; i++)
        data[i] = -(data[i]);
}

// ------------------------------------------------------------------------
// Conjugates this quaternion, returning the result. Quaternion conjugation
// negates the vector portion but leaves the scalar portion unchanged.
// If the quaternion represents a rotation, the conjugate is the opposite
// rotation.
// ------------------------------------------------------------------------
atQuat atQuat::getConjugate() const
{
    atQuat result;
    int i;
    
    // Create the target quat by copying the values from this quat to the
    // target quat, negating the vector elements as we go.
    for (i = 0; i < 4; i++)
    {
        if (i == 3)
            result.data[i] = data[i];
        else
            result.data[i] = -data[i];
    }

    // Return the target quat
    return result;
}

// ------------------------------------------------------------------------
// Sets this quaternion to its multiplicative inverse. The inverse of a
// quaternion is its conjugate divided by the square of its magnitude.
// ------------------------------------------------------------------------
void atQuat::invert()
{
    double mag;
    int i;
    
    // Get the magnitude of this quat
    mag = getMagnitude();

    // Invert this quat by taking the conjugate and then dividing by the
    // square of the magnitude
    conjugate();
    for (i = 0; i < 4; i++)
        data[i] /= (mag * mag);
}

// ------------------------------------------------------------------------
// Returns the multiplicative inverse of this quaternion. The inverse of a
// quaternion is its conjugate divided by the square of its magnitude.
// ------------------------------------------------------------------------
atQuat atQuat::getInverse() const
{
    // Create a duplicate of this quaternion, invert that, and return it
    atQuat result(data);
    result.invert();
    return result;
}

// ------------------------------------------------------------------------
// Sets this quaternion to a rotational quaternion representing the same
// rotation as what is stored within the matrix parameter.
// ------------------------------------------------------------------------
void atQuat::setMatrixRotation(const atMatrix &theMatrix)
{
    // Since the matrix-to-quat algorithm doesn't seem to like it when the
    // matrix contains any scaling, strip scaling out here.
    atMatrix tempMatrix;
    double xscale, yscale, zscale;

    theMatrix.getScale(&xscale, &yscale, &zscale);
    tempMatrix.setScale(1.0 / xscale, 1.0 / yscale, 1.0 / zscale);
    tempMatrix = tempMatrix * theMatrix;

    // The following algorithm is drawn from the SIGGRAPH '85 paper
    // "Animating Rotation with Quaternion Curves", by Ken Shoemake.

    double ws, xs, ys;

    ws = (1.0 + tempMatrix.getValue(0, 0) + tempMatrix.getValue(1, 1) +
        tempMatrix.getValue(2, 2)) / 4.0;
    if (ws > 1E-6)
    {
        data[3] = sqrt(ws);
        data[0] = (tempMatrix.getValue(2, 1) - tempMatrix.getValue(1, 2)) / (4.0 * data[3]);
        data[1] = (tempMatrix.getValue(0, 2) - tempMatrix.getValue(2, 0)) / (4.0 * data[3]);
        data[2] = (tempMatrix.getValue(1, 0) - tempMatrix.getValue(0, 1)) / (4.0 * data[3]);
    }
    else
    {
        data[3] = 0.0;
        xs = -(tempMatrix.getValue(1, 1) + tempMatrix.getValue(2, 2)) / 2.0;
        if (xs > 1E-6)
        {
            data[0] = sqrt(xs);
            data[1] = tempMatrix.getValue(1, 0) / (2.0 * data[0]);
            data[2] = tempMatrix.getValue(2, 0) / (2.0 * data[0]);
        }
        else
        {
            data[0] = 0.0;
            ys = (1.0 - tempMatrix.getValue(2, 2)) / 2.0;
            if (ys > 1E-6)
            {
                data[1] = sqrt(ys);
                data[2] = tempMatrix.getValue(2, 1) / (2.0 * data[1]);
            }
            else
            {
                data[1] = 0.0;
                data[2] = 1.0;
            }
        }
    }

}

// ------------------------------------------------------------------------
// Sets this quaternion to a rotational quaternion representing the same
// rotation as the given three Euler angles. The axis constant specifies
// the order of the axes for the Euler angles.
// ------------------------------------------------------------------------
void atQuat::setEulerRotation(atMathEulerAxisOrder axisOrder,
    double axis1Degrees, double axis2Degrees, double axis3Degrees)
{
    // This function simply creates three separate quaternions, one
    // for each rotation, and then combines them.

    atQuat firstQuat, secondQuat, thirdQuat, resultQuat;
    
    // Create three quaternions representing the separate rotations
    // around each axis
    switch (axisOrder)
    {
        case AT_EULER_ANGLES_XYZ_S:
        case AT_EULER_ANGLES_XYZ_R:
            firstQuat.setAxisAngleRotation(1.0, 0.0, 0.0, axis1Degrees);
            secondQuat.setAxisAngleRotation(0.0, 1.0, 0.0, axis2Degrees);
            thirdQuat.setAxisAngleRotation(0.0, 0.0, 1.0, axis3Degrees);
            break;
        case AT_EULER_ANGLES_XZY_S:
        case AT_EULER_ANGLES_XZY_R:
            firstQuat.setAxisAngleRotation(1.0, 0.0, 0.0, axis1Degrees);
            secondQuat.setAxisAngleRotation(0.0, 0.0, 1.0, axis2Degrees);
            thirdQuat.setAxisAngleRotation(0.0, 1.0, 0.0, axis3Degrees);
            break;
        case AT_EULER_ANGLES_YXZ_S:
        case AT_EULER_ANGLES_YXZ_R:
            firstQuat.setAxisAngleRotation(0.0, 1.0, 0.0, axis1Degrees);
            secondQuat.setAxisAngleRotation(1.0, 0.0, 0.0, axis2Degrees);
            thirdQuat.setAxisAngleRotation(0.0, 0.0, 1.0, axis3Degrees);
            break;
        case AT_EULER_ANGLES_YZX_S:
        case AT_EULER_ANGLES_YZX_R:
            firstQuat.setAxisAngleRotation(0.0, 1.0, 0.0, axis1Degrees);
            secondQuat.setAxisAngleRotation(0.0, 0.0, 1.0, axis2Degrees);
            thirdQuat.setAxisAngleRotation(1.0, 0.0, 0.0, axis3Degrees);
            break;
        case AT_EULER_ANGLES_ZXY_S:
        case AT_EULER_ANGLES_ZXY_R:
            firstQuat.setAxisAngleRotation(0.0, 0.0, 1.0, axis1Degrees);
            secondQuat.setAxisAngleRotation(1.0, 0.0, 0.0, axis2Degrees);
            thirdQuat.setAxisAngleRotation(0.0, 1.0, 0.0, axis3Degrees);
            break;
        case AT_EULER_ANGLES_ZYX_S:
        case AT_EULER_ANGLES_ZYX_R:
            firstQuat.setAxisAngleRotation(0.0, 0.0, 1.0, axis1Degrees);
            secondQuat.setAxisAngleRotation(0.0, 1.0, 0.0, axis2Degrees);
            thirdQuat.setAxisAngleRotation(1.0, 0.0, 0.0, axis3Degrees);
            break;

        case AT_EULER_ANGLES_XYX_S:
        case AT_EULER_ANGLES_XYX_R:
            firstQuat.setAxisAngleRotation(1.0, 0.0, 0.0, axis1Degrees);
            secondQuat.setAxisAngleRotation(0.0, 1.0, 0.0, axis2Degrees);
            thirdQuat.setAxisAngleRotation(1.0, 0.0, 0.0, axis3Degrees);
            break;
        case AT_EULER_ANGLES_XZX_S:
        case AT_EULER_ANGLES_XZX_R:
            firstQuat.setAxisAngleRotation(1.0, 0.0, 0.0, axis1Degrees);
            secondQuat.setAxisAngleRotation(0.0, 0.0, 1.0, axis2Degrees);
            thirdQuat.setAxisAngleRotation(1.0, 0.0, 0.0, axis3Degrees);
            break;
        case AT_EULER_ANGLES_YXY_S:
        case AT_EULER_ANGLES_YXY_R:
            firstQuat.setAxisAngleRotation(0.0, 1.0, 0.0, axis1Degrees);
            secondQuat.setAxisAngleRotation(1.0, 0.0, 0.0, axis2Degrees);
            thirdQuat.setAxisAngleRotation(0.0, 1.0, 0.0, axis3Degrees);
            break;
        case AT_EULER_ANGLES_YZY_S:
        case AT_EULER_ANGLES_YZY_R:
            firstQuat.setAxisAngleRotation(0.0, 1.0, 0.0, axis1Degrees);
            secondQuat.setAxisAngleRotation(0.0, 0.0, 1.0, axis2Degrees);
            thirdQuat.setAxisAngleRotation(0.0, 1.0, 0.0, axis3Degrees);
            break;
        case AT_EULER_ANGLES_ZXZ_S:
        case AT_EULER_ANGLES_ZXZ_R:
            firstQuat.setAxisAngleRotation(0.0, 0.0, 1.0, axis1Degrees);
            secondQuat.setAxisAngleRotation(1.0, 0.0, 0.0, axis2Degrees);
            thirdQuat.setAxisAngleRotation(0.0, 0.0, 1.0, axis3Degrees);
            break;
        case AT_EULER_ANGLES_ZYZ_S:
        case AT_EULER_ANGLES_ZYZ_R:
            firstQuat.setAxisAngleRotation(0.0, 0.0, 1.0, axis1Degrees);
            secondQuat.setAxisAngleRotation(0.0, 1.0, 0.0, axis2Degrees);
            thirdQuat.setAxisAngleRotation(0.0, 0.0, 1.0, axis3Degrees);
            break;

        default:
            printf("atQuat::setEulerRotation: Invalid axis order value\n");
            return;
    }
    
    // I _think_ quaternions are multiplied in the same way as
    // rotation matricies; since the point to be rotated gets multiplied
    // as a column vector on the right, the first quaternion that
    // would get multiplied in should be on the right as well.
    // If this is a relative axis rotation, then that order is reversed.
    if ((axisOrder >= AT_EULER_ANGLES_XYZ_R) &&
        (axisOrder <= AT_EULER_ANGLES_ZYZ_R))
        // reversed
        (*this) = firstQuat * secondQuat * thirdQuat;
    else
        // standard
        (*this) = thirdQuat * secondQuat * firstQuat;
}

// ------------------------------------------------------------------------
// Retrieves the rotation indicated by this quaternion as a set of three
// Euler angles. The axis constant specifies the axis order of the
// resulting reconstructed rotations.
//
// Note: NULL pointers may be passed in to denote unwanted return values
// ------------------------------------------------------------------------
void atQuat::getEulerRotation(atMathEulerAxisOrder axisOrder,
    double *axis1Degrees, double *axis2Degrees, double *axis3Degrees) const
{
    // Cheat: call the matrix version of this routine and return
    // whatever that returns.
    atMatrix theMatrix;
    
    // Set the temp matrix's rotation to the same rotation that this
    // quat contains, and get the Euler rotations from that
    theMatrix.setQuatRotation(*this);
    theMatrix.getEulerRotation(axisOrder, axis1Degrees, axis2Degrees,
        axis3Degrees);
}

// ------------------------------------------------------------------------
// Sets this quaternion to a rotational quaternion representing a rotation
// around the axis specified by the vector (x, y, z), and rotating by
// the specified number of degrees.
// If the axis passed in is all zero, the final quaternion will have no
// rotation, regardless of the degrees specified.
// ------------------------------------------------------------------------
void atQuat::setAxisAngleRotation(double x, double y, double z,
    double rotDegrees)
{
    atVector axis(x, y, z);
    double tempVal;

    // Check for a zero axis of rotation
    if ((x == 0.0) && (y == 0.0) && (z == 0.0))
    {
        data[0] = 0.0;
        data[1] = 0.0;
        data[2] = 0.0;
        data[3] = 1.0;
        return;
    }

    // Axes of rotation must always be normalized
    axis.normalize();

    // Compte the final quaternion, which consists of a vector part of
    // the rotation axis scaled by the sine of the rotation degree
    // measure, and a scalar part of the cosine of the degree measure.
    tempVal = sin(AT_DEG2RAD(rotDegrees / 2.0));
    data[0] = axis[0] * tempVal;
    data[1] = axis[1] * tempVal;
    data[2] = axis[2] * tempVal;
    data[3] = cos(AT_DEG2RAD(rotDegrees / 2.0));
}

// ------------------------------------------------------------------------
// Retrieves the axis and amount of rotation represented by this
// quaternion.
//
// Note: NULL pointers may be passed in to denote unwanted return values
// ------------------------------------------------------------------------
void atQuat::getAxisAngleRotation(double *x, double *y, double *z,
    double *rotDegrees) const
{
    atVector axis;
    double mag, temp, degrees;

    // If the first three values of the quaternion are virtually zero,
    // then this quaternion represents no rotation.
    axis.set(data[0], data[1], data[2]);
    mag = axis.getMagnitude();
    if (mag < AT_DEFAULT_TOLERANCE)
    {
        if (x)
            *x = 0.0;
        if (y)
            *y = 0.0;
        if (z)
            *z = 0.0;
        if (rotDegrees)
            *rotDegrees = 0.0;
        return;
    }

    // Obtain the axis of rotation by normalizing the first three values
    axis.normalize();

    // Make sure that the angle of rotation hasn't drifted outside the
    // -1.0 - 1.0 range due to floating point error, otherwise the acos()
    // call below will fail.
    temp = data[3];
    if (temp > 1.0)
        temp = 1.0;
    else if (temp < -1.0)
        temp = -1.0;

    // Obtain the angle of rotation using the fourth component of the
    // quaternion
    degrees = AT_RAD2DEG(acos(temp) * 2.0);

    // Check which return values are desired and copy the function results
    // into the specified locations
    if (x)
        *x = axis[0];
    if (y)
        *y = axis[1];
    if (z)
        *z = axis[2];
    if (rotDegrees)
        *rotDegrees = degrees;
}

// ------------------------------------------------------------------------
// Sets this quaternion to represent the coordinate space rotation that
// will rotate the directions specified by originForward and originUp to
// match those specified by targetForward and targetUp, respectively.
// ------------------------------------------------------------------------
void atQuat::setVecsRotation(const atVector &originForward, const atVector &originUp,
    const atVector &targetForward, const atVector &targetUp)
{
    atVector startDir, startUp, endDir, endUp;
    atVector newUp;
    atVector rotAxis;
    double rotAngle, dotProd;
    atVector componentVec;
    atQuat roll;
    atVector zeroVector, yVector, zVector;

    // Clean up the input vectors by forcing them to be size 3 and
    // normalizing them
    startDir.clearCopy(originForward);
    startDir.setSize(3);
    startDir.normalize();
    startUp.clearCopy(originUp);
    startUp.setSize(3);
    startUp.normalize();
    endDir.clearCopy(targetForward);
    endDir.setSize(3);
    endDir.normalize();
    endUp.clearCopy(targetUp);
    endUp.setSize(3);
    endUp.normalize();

    // Initialize this quat and some utility vectors
    set(0.0, 0.0, 0.0, 1.0);
    zeroVector.set(0.0, 0.0, 0.0);
    yVector.set(0.0, 1.0, 0.0);
    zVector.set(0.0, 0.0, 1.0);

    // * First, compute the rotation that rotates the forward vectors
    // to match
    if (!(startDir == endDir))
    {
        // The first axis of rotation is equal to the cross product of
        // the starting forward direction vector and the ending forward
        // direction vector
        rotAxis = startDir.getCrossProduct(endDir);

        // Special case: check to see if the originForward and targetForward
        // vectors are pointing in exactly opposite directions
        if (rotAxis == zeroVector)
        {
            // Pick an arbitrary axis of rotation that is not parallel
            // to the forward vectors
            if (startDir.getCrossProduct(zVector) == zeroVector)
                rotAxis = yVector;
            else
                rotAxis = zVector;

            // Force this new rotation axis to be perpendicular to the
            // forward vectors
            dotProd = startDir.getDotProduct(rotAxis);
            componentVec = startDir * dotProd;
            rotAxis -= componentVec;
            rotAxis.normalize();

            rotAngle = 180.0;
        }
        else
        {
            // Amount to rotate is equal to the angle between the starting
            // and ending forward vectors
            rotAngle = startDir.getAngleBetween(endDir);
        }

        // Call one of the quat's functions to set this quat as the
        // first rotation
        setAxisAngleRotation(rotAxis[0], rotAxis[1], rotAxis[2], rotAngle);
    }

    // * Second, with both forward directions aligned, 'roll' the rotation
    // around the forward vector so that the up vectors match

    // Apply the rotation that rotates originForward to targetForward to
    // the originUp vector
    newUp = rotatePoint(startUp);

    // Don't compute the roll if either of the up direction vectors are
    // parallel to their corresponding forward vectors
    if (!(startDir == startUp) && !(endDir == endUp))
    {
        // Force originUp to be perpendicular to originForward
        dotProd = endDir.getDotProduct(newUp);
        componentVec = endDir * dotProd;
        newUp -= componentVec;
        newUp.normalize();

        // Force targetUp to be perpendicular to targetForward
        dotProd = endDir.getDotProduct(endUp);
        componentVec = endDir * dotProd;
        endUp -= componentVec;
        endUp.normalize();

        // Abort at this point if the two 'up' vectors are already aligned
        if (!(newUp == endUp))
        {
            // Compute the axis and angle of rotation. Although the forward
            // vector should be suitable as an axis of rotation, I'm computing
            // it again here for simplicity; the getAngleBetween function can
            // sometimes return a negative angle, but the cross product will
            // be the opposite direction in those cases, so the two negatives
            // cancel out.
            rotAxis = newUp.getCrossProduct(endUp);

            // Check for the special case where the two up vectors are
            // opposite directions
            if (rotAxis == zeroVector)
            {
                rotAxis = endDir;
                rotAngle = 180.0;
            }
            else
                rotAngle = newUp.getAngleBetween(endUp);

            // Create the second rotation quat as the roll rotation
            roll.setAxisAngleRotation(rotAxis[0], rotAxis[1], rotAxis[2],
                rotAngle);

            // Combine the two rotations
            (*this) = roll * (*this);
        }
    }
}

// ------------------------------------------------------------------------
// Returns a rotation quaternion that represents the portion of the
// rotation contained in this quaterion that rotates around the axis
// specified as the parameter.
// ------------------------------------------------------------------------
atQuat atQuat::getDecomposition(const atVector &targetAxis) const
{
    atQuat result;
    atVector axis;

    // The axis of rotation MUST be of length 3
    if (targetAxis.getSize() != 3)
    {
        printf("getDecomposition: Bad axis vector size\n");
        return result;
    }

    // The formula for this computation is as follows:
    //
    //                           Wr
    //  W  =  -----------------------------------------
    //        [ (A*Xr + B*Yr + C*Zr)^2 + (Wr)^2 ] ^ 1/2
    //
    // Where W is the fourth value of the result quaternion, [Xr Yr Zr Wr] is
    // the quaternion to be decomposed, and [A B C] is the (unit) vector
    // representing the desired axis of rotation. Additionally, the resulting
    // quaternion must have its axis scaled by S (the sine of the rotation
    // angle); this angle is calculated as the inverse cosine of W.

    double wSqr, wrSqr, N, NSqr, wSign, s, w;

    // Force the axis to be unit length
    axis = targetAxis.getNormalized();

    // Calculate the (A*Xr + B*Yr + C*Zr)^2 term
    N = (axis[0] * data[0]) + (axis[1] * data[1]) + (axis[2] * data[2]);
    NSqr = AT_SQR(N);

    // Calculate Wr^2
    wrSqr = AT_SQR(data[3]);

    // Calculate W
    wSqr = wrSqr / (NSqr + wrSqr);
    w = sqrt(wSqr);

    // * Fix the sign of W by determining if the rotation is positive or
    // negative with respect to the axis of rotation
    wSign = 1.0;

    // Compare the input axis with the quaternion axis
    if (N < 0.0)
        wSign *= -1.0;

    // Examine the quaternion rotation amount to see if it is positive or
    // negative
    if (data[3] < 0.0)
        wSign *= -1.0;

    // Apply the calculated sign to W
    w *= wSign;

    // Calculate S
    s = sin(acos(w));

    // Compute the final quaternion
    result[0] = axis[0] * s;
    result[1] = axis[1] * s;
    result[2] = axis[2] * s;
    result[3] = w;
    return result;
}

// ------------------------------------------------------------------------
// Returns a scalar value that is equivalent to the angle part of the
// quaternion that would rotate this quaternion to targetRotation.  This
// assumes that both this quaternion and the targetRotation quaternion are
// unit quaternions that each represent rotations.
// ------------------------------------------------------------------------
double atQuat::getRotationDifference(const atQuat &targetRotation) const
{
    atQuat difference;
    double angle;

    // Get the difference between the two quaternions by multiplying this
    // quaternion by the conjugate of the target rotation
    difference = (*this) * targetRotation.getConjugate();

    // Extract the angle measurement from the w component of the difference
    // quaternion.
    angle = AT_RAD2DEG(acos(difference[AT_W]) * 2.0);

    return angle;
}

// ------------------------------------------------------------------------
// Transforms the given point by this quaternion as a rotation. Equivalent
// to changing the quaternion into a rotation matrix and multiplying the
// point by the resulting matrix. The homogeneous coordinate value w of
// the point, if it exists, is ignored.
// ------------------------------------------------------------------------
atVector atQuat::rotatePoint(const atVector &targetPoint) const
{
    atVector resultPt;
    atQuat targetAsQuat, conjQuat, resultQuat;
    int i;
    int targetPointSize;
    
    // Construct the inverse of this quaternion. Since rotation
    // quaternions are unit length, the inverse of the quaternion is
    // equivalent to its conjugate.
    conjQuat.set(data);
    conjQuat.conjugate();

    // Determine how much of the targetPoint parameter to use
    targetPointSize = targetPoint.getSize();
    if (targetPointSize > 3)
        targetPointSize = 3;

    // Create a quaternion out of the point to be rotated
    targetAsQuat.clear();
    for (i = 0; i < targetPointSize; i++)
        targetAsQuat.data[i] = targetPoint.getValue(i);

    // The point rotation is accomplished by computing the composite
    // quaternion [Q * P * (Q^-1)]
    resultQuat = (*this) * targetAsQuat * conjQuat;
    
    // Convert the resulting quaternion back into a point
    resultPt = targetPoint;
    for (i = 0; i < targetPointSize; i++)
        resultPt[i] = resultQuat.data[i];

    // Return the resulting point
    return resultPt;
}

// ------------------------------------------------------------------------
// Spherical Linear intERPolation
// Returns a quaternion that is an interpolation between this quaternion
// as the source point and the destination quaternion. The parameter value
// should range from 0.0 to 1.0, inclusive, and denotes the interpolation
// value. The path taken by the interpolated quaternion is linear, but
// the velocity is non-linear (due to the trig functions involved).
// ------------------------------------------------------------------------
atQuat atQuat::slerp(const atQuat &destination, double parameter) const
{
    atQuat startQuat, destQuat, resultQuat;
    double dotProd, theta, sinTheta, q1val, q2val;
    
    // Bounds checking
    if ((parameter < 0.0) || (parameter > 1.0))
    {
        printf("atQuat::slerp: 'parameter' must be in range 0.0 - 1.0\n");
        return resultQuat;
    }

    // Force both quats to be unit length; our calculations won't work
    // otherwise.
    if (AT_EQUAL(getMagnitudeSquared(), 1.0))
        startQuat = (*this);
    else
        startQuat = getNormalized();
    if (AT_EQUAL(destination.getMagnitudeSquared(), 1.0))
        destQuat = destination;
    else
        destQuat = destination.getNormalized();

    // * Calculate the angle between the two quaternions as 4-D vectors
    // by computing the inverse cosine of their dot product
    dotProd = startQuat.getDotProduct(destQuat);

    // If the dot product of the two quaternions is negative, then the
    // angle between the two rotations is greater than 180 degrees. If we
    // were to do an interpolation with the two quats, the interpolated
    // path would be the 'long way' around the sphere. Instead, since the
    // negative of a rotation quaternion specifies the same rotation, we
    // can negate the destination quat, which will force the interpolation
    // to take the shortest path without changing the actual destination. 
    if (dotProd < 0.0)
    {
        // Negate the destination quat
        destQuat.scale(-1.0);

        // Negating one quat also negates their dot product
        dotProd *= -1.0;
    }

    // If the dot product is very close to one, then the two quats contain
    // the same rotation. Just return this quaternion.
    if (dotProd > (1.0 - AT_DEFAULT_TOLERANCE))
        return startQuat;

    // Finish calculating the angle
    theta = acos(dotProd);

    // * Interpolate between the two quaternions by scaling each one based
    // on the parameter value, and summing the resulting quaternions. The
    // 'spherical' effect is accomplished with the sine functions.

    // Compute the sin of the angle between the two quaternions
    sinTheta = sin(theta);

    // Scale the starting quaternion
    q1val = sin((1.0 - parameter) * theta) / sinTheta;
    startQuat.scale(q1val);

    // Scale the destination quaternion
    q2val = sin(parameter * theta) / sinTheta;
    destQuat.scale(q2val);

    // Combine the two and finish
    resultQuat = startQuat + destQuat;
    return resultQuat;
}

// ------------------------------------------------------------------------
// Retrieves one value from the quaternion as a reference to a double.
// ------------------------------------------------------------------------
double &atQuat::operator[](int index)
{
    // Bounds checking
    if ((index < 0) || (index > 3))
    {
        printf("atQuat::operator[]: Illegal subscript\n");
        return data[0];
    }
    
    // Return a reference to the desired data value
    return data[index];
}

// ------------------------------------------------------------------------
// Adds the addend quaternion to this one, returning the result.
// Equivalent to getSum(addend)
// ------------------------------------------------------------------------
atQuat atQuat::operator+(const atQuat &addend) const
{
    int i;
    atQuat result;

    // Create the target quat by adding each element of this quat to the
    // corresponding element of the addend quat
    for (i = 0; i < 4; i++)
        result.data[i] = data[i] + addend.data[i];

    // Return the target quat
    return result;
}

// ------------------------------------------------------------------------
// Subtracts the subtrahend quaternion from this one, returning the result.
// Equivalent to getDifference(subtrahend)
// ------------------------------------------------------------------------
atQuat atQuat::operator-(const atQuat &subtrahend) const
{
    int i;
    atQuat result;

    // Create the target quat by subtracting each element of the subtrahend
    // from the corresponding element of this quat
    for (i = 0; i < 4; i++)
        result.data[i] = data[i] - subtrahend.data[i];

    // Return the target quat
    return result;
}

// ------------------------------------------------------------------------
// Multiplies this quaternion by the operand quaternion, returning the
// result. This function 'cheats' by just calling multiplyQuat for it's
// answer.
// Equivalent to getMultipliedQuat(operand)
// ------------------------------------------------------------------------
atQuat atQuat::operator*(const atQuat &operand) const
{
    // Create a duplicate of this quat, multiply in the operand quat, and
    // return the result
    atQuat result(data);
    result.multiplyQuat(operand);
    return result;
}

// ------------------------------------------------------------------------
// Adds the addend quaternion to this one, storing the result in this
// quaternion.
// Equivalent to add(addend)
// ------------------------------------------------------------------------
void atQuat::operator+=(const atQuat &addend)
{
    int i;

    // Add each element of the addend quat to this quat
    for (i = 0; i < 4; i++)
        data[i] += addend.data[i];
}

// ------------------------------------------------------------------------
// Subtracts the subtrahend quaternion from this one, storing the result
// in this quaternion.
// Equivalent to subtract(subtrahend)
// ------------------------------------------------------------------------
void atQuat::operator-=(const atQuat &subtrahend)
{
    int i;

    // Subtract each element of the subtrahend quat from this quat
    for (i = 0; i < 4; i++)
        data[i] -= subtrahend.data[i];
}

// ------------------------------------------------------------------------
// Multiplies this quaternion by the operand quaternion, storing the result
// in this quaternion.
// Equivalent to multiplyQuat(operand)
// ------------------------------------------------------------------------
void atQuat::operator*=(const atQuat &operand)
{
    // The multiplyQuat function is too complex to duplicate here; just
    // call that instead.
    multiplyQuat(operand);
}

// ------------------------------------------------------------------------
// Prints a text representation of this quaternion to stdout
// ------------------------------------------------------------------------
void atQuat::print() const
{
    // Enclose the quaternion's components in parentheses.
    // Print the vector portion inside angle brackets and the scalar
    // portion outside
    printf("( <%0.4lf, %0.4lf, %0.4lf>, %0.4lf )", data[0], data[1], data[2],
        data[3]);
}

// ------------------------------------------------------------------------
// Prints a text representation of this vector to the specified file
// ------------------------------------------------------------------------
void atQuat::print(FILE *fp) const
{
    // Enclose the quaternion's components in parentheses.
    // Print the vector portion inside angle brackets and the scalar
    // portion outside
    fprintf(fp, "( <%0.4lf, %0.4lf, %0.4lf>, %0.4lf )", data[0], data[1], 
        data[2], data[3]);
}

// ------------------------------------------------------------------------
// Checks for element-wise equality between two quaternions. Two elements
// are considered equal if they are within a small default tolerance value
// of each other.
// Equivalent to isEqual(operand)
// ------------------------------------------------------------------------
bool atQuat::operator==(const atQuat &operand) const
{
    int i;

    // Check each pair of values (this quat's and the operand quat's) for
    // almost-equality; return false if a pair doesn't match up.
    for (i = 0; i < 4; i++)
        if (fabs(data[i] - operand.data[i]) > AT_DEFAULT_TOLERANCE)
            return false;

    // If all the pairs match, return true
    return true;
}

// ------------------------------------------------------------------------
// atItem-derived method.  Return true if the given item is an equivalent
// quaternion
// ------------------------------------------------------------------------
bool atQuat::equals(atItem * otherItem)
{
   atQuat * otherQuat;

   // Try to cast the other item to an atQuat
   otherQuat = dynamic_cast<atQuat *>(otherItem);

   // Return true iff the other item is a quaternion and is equivalent to
   // this quaternion (within default tolerance)
   if ((otherQuat != NULL) && (isEqual(*otherQuat)))
      return true;
   else
      return false;
}


// ------------------------------------------------------------------------
// atItem derived method.  Return an integer indicating whether this
// quaternion is less than (negative) equal (zero) or greater than
// (positive) the other quaternion.
// ------------------------------------------------------------------------
int atQuat::compare(atItem * otherItem)
{
   atQuat * otherQuat;
   double   thisAngle, otherAngle;
   double   angleDiff;

   // Try to cast the item to a vector
   otherQuat = dynamic_cast<atQuat *>(otherItem);

   // See if the other item is a vector
   if (otherQuat != NULL)
   {
      // Since quaternions are almost always used for rotations, we'll
      // compare using the angle of rotation, which is encoded in the
      // quaternion's W component

      // Get the angle rotation for each quaternion
      getAxisAngleRotation(NULL, NULL, NULL, &thisAngle);
      otherQuat->getAxisAngleRotation(NULL, NULL, NULL, &otherAngle);

      // Use the angle difference to determine the "greater" quaternion
      angleDiff = otherAngle - thisAngle;
      if (angleDiff > 0.0)
         return 1;
      else if (angleDiff < 0.0)
         return -1;
      else
         return 0;
   }
   else
   {
      // Return the default atItem comparison
      return atItem::compare(otherItem);
   }
}

