
#include <stdio.h>
#include <math.h>
#include "atVector.h++"

// ------------------------------------------------------------------------
// Default constructor - Clears the vector to zero and sets the size to 4
// ------------------------------------------------------------------------
atVector::atVector()
{
    data[0] = 0.0;
    data[1] = 0.0;
    data[2] = 0.0;
    data[3] = 0.0;
    vecSize = 4;
}

// ------------------------------------------------------------------------
// Size constructor - Clears the vector to zero and sets the size to the
// indicated value, if legal.
// ------------------------------------------------------------------------
atVector::atVector(int size)
{
    data[0] = 0.0;
    data[1] = 0.0;
    data[2] = 0.0;
    data[3] = 0.0;

    // Bounds checking
    if ((size < 1) || (size > 4))
    {
        printf("atVector::atVector(int): Invalid size parameter, size=%d\n",size);
        vecSize = 4;
    }
    else
        vecSize = size;
}

// ------------------------------------------------------------------------
// Data constructor - Sets the size of the vector to the size of the array,
// and sets the vector data to the array data.
// ------------------------------------------------------------------------
atVector::atVector(int size, double values[])
{
    int i;

    // Bounds checking
    if ((size < 1) || (size > 4))
    {
        printf("atVector::atVector(int, double[]): Invalid size parameter, size=%d\n",size);
        clear();
        vecSize = 4;
        return;
    }

    // Copy the data values from the given array to the vector, setting
    // the size of the vector and setting the unused entries to zero
    clear();
    vecSize = size;
    for (i = 0; i < size; i++)
        data[i] = values[i];
}

// ------------------------------------------------------------------------
// Data constructor - Sets the size of the vector to 2, and sets the
// vector data to the given data.
// ------------------------------------------------------------------------
atVector::atVector(double x, double y)
{
    data[0] = x;
    data[1] = y;
    data[2] = 0.0;
    data[3] = 0.0;
    vecSize = 2;
}

// ------------------------------------------------------------------------
// Data constructor - Sets the size of the vector to 3, and sets the
// vector data to the given data.
// ------------------------------------------------------------------------
atVector::atVector(double x, double y, double z)
{
    data[0] = x;
    data[1] = y;
    data[2] = z;
    data[3] = 0.0;
    vecSize = 3;
}

// ------------------------------------------------------------------------
// Data constructor - Sets the size of the vector to 4, and sets the
// vector data to the given data.
// ------------------------------------------------------------------------
atVector::atVector(double x, double y, double z, double w)
{
    data[0] = x;
    data[1] = y;
    data[2] = z;
    data[3] = w;
    vecSize = 4;
}

// ------------------------------------------------------------------------
// Destructor
// ------------------------------------------------------------------------
atVector::~atVector()
{
}

// ------------------------------------------------------------------------
// Sets the size of this vector to the given size (if legal), and copies
// the data from the specified array into the vector.
// ------------------------------------------------------------------------
void atVector::set(int size, double values[])
{
    int i;

    // Bounds checking
    if ((size < 1) || (size > 4))
    {
        printf("atVector::set(int, double[]): Invalid size parameter, size=%d\n",size);
        return;
    }

    // Copy the data values from the given array to the vector, setting
    // the size of the vector and setting the unused entries to zero
    clear();
    vecSize = size;
    for (i = 0; i < size; i++)
        data[i] = values[i];
}

// ------------------------------------------------------------------------
// Sets the size of the vector to 2, and sets the vector data to the given
// data.
// ------------------------------------------------------------------------
void atVector::set(double x, double y)
{
    // Copy the data values from the given array to the vector, setting the
    // size of the vector to two and setting the unused entries to zero
    data[0] = x;
    data[1] = y;
    data[2] = 0.0;
    data[3] = 0.0;
    vecSize = 2;
}

// ------------------------------------------------------------------------
// Sets the size of the vector to 3, and sets the vector data to the given
// data.
// ------------------------------------------------------------------------
void atVector::set(double x, double y, double z)
{
    // Copy the data values from the given array to the vector, setting the
    // size of the vector to three and setting the unused entry to zero
    data[0] = x;
    data[1] = y;
    data[2] = z;
    data[3] = 0.0;
    vecSize = 3;
}

// ------------------------------------------------------------------------
// Sets the size of the vector to 4, and sets the vector data to the given
// data.
// ------------------------------------------------------------------------
void atVector::set(double x, double y, double z, double w)
{
    // Copy the data values from the given array to the vector, setting the
    // size of the vector to four
    data[0] = x;
    data[1] = y;
    data[2] = z;
    data[3] = w;
    vecSize = 4;
}

// ------------------------------------------------------------------------
// Makes this vector an exact duplicate of the source vector.
// ------------------------------------------------------------------------
void atVector::copy(const atVector &source)
{
    (*this) = source;
}

// ------------------------------------------------------------------------
// Clears the vector to zero.
// ------------------------------------------------------------------------
void atVector::clear()
{
    data[0] = 0.0;
    data[1] = 0.0;
    data[2] = 0.0;
    data[3] = 0.0;
}

// ------------------------------------------------------------------------
// Copies the data from the source vector to this vector, setting all
// unused values to zero. This vector's size is unchanged.
// ------------------------------------------------------------------------
void atVector::clearCopy(const atVector &source)
{
    int i;
    
    // Clear the vector, and then copy its data into this vector
    clear();
    for (i = 0; i < source.vecSize; i++)
        data[i] = source.data[i];
}

// ------------------------------------------------------------------------
// Sets the size of this vector to the given size (if legal). The vector
// data is unchanged.
// ------------------------------------------------------------------------
void atVector::setSize(int size)
{
    // Bounds checking
    if ((size < 1) || (size > 4))
    {
        printf("atVector::setSize: Invalid size parameter, size=%d\n",size);
        return;
    }

    // Set the vector size
    vecSize = size;
}

// ------------------------------------------------------------------------
// Retrieves the size of this vector.
// ------------------------------------------------------------------------
int atVector::getSize() const
{
    return vecSize;
}

// ------------------------------------------------------------------------
// Sets one specific value in the vector.
// ------------------------------------------------------------------------
void atVector::setValue(int index, double value)
{
    // Bounds checking
    if ((index < 0) || (index >= vecSize))
    {
        printf("atVector::setValue: Invalid index = %d\n",index);
        return;
    }
    
    // Set the specified value
    data[index] = value;
}

// ------------------------------------------------------------------------
// Retrieves one specific value from the vector.
// ------------------------------------------------------------------------
double atVector::getValue(int index) const
{
    // Bounds checking
    if ((index < 0) || (index >= vecSize))
    {
        printf("atVector::getValue: Invalid index = %d\n",index);
        return data[0];
    }
    
    // Return the desired value
    return data[index];
}

// ------------------------------------------------------------------------
// Checks for element-wise equality between two vectors. The two vectors
// must be the same size. Two elements are considered equal if they are
// within a small default tolerance value of each other.
// ------------------------------------------------------------------------
bool atVector::isEqual(const atVector &operand) const
{
    int i;

    // Verify that the vectors are the same size
    if (vecSize != operand.vecSize)
    {
        printf("atVector::isEqual: Vector size mismatch\n");
        return false;
    }

    // Check each pair of values (this vector's and the operand vector's)
    // for almost-equality; return false if a pair doesn't match up.
    for (i = 0; i < vecSize; i++)
        if (fabs(data[i] - operand.data[i]) > AT_DEFAULT_TOLERANCE)
            return false;

    // If all the pairs match, return true
    return true;
}

// ------------------------------------------------------------------------
// Checks for element-wise equality between two vectors. The two vectors
// must be the same size. Two elements are considered equal if they are
// within the specified tolerance value of each other.
// ------------------------------------------------------------------------
bool atVector::isAlmostEqual(const atVector &operand, double tolerance) const
{
    int i;

    // Verify that the vectors are the same size
    if (vecSize != operand.vecSize)
    {
        printf("atVector::isAlmostEqual: Vector size mismatch\n");
        return false;
    }

    // Check each pair of values (this vector's and the operand vector's)
    // for almost-equality, 'almost' being specified by a given tolerance
    // value. Return false if a pair doesn't match up.
    for (i = 0; i < vecSize; i++)
        if (fabs(data[i] - operand.data[i]) > tolerance)
            return false;

    // If all the pairs match, return true
    return true;
}

// ------------------------------------------------------------------------
// Adds the addend vector to this one, keeping the result in this vector.
// The two vectors must be the same size.
// ------------------------------------------------------------------------
void atVector::add(const atVector &addend)
{
    int i;

    // Verify that the vectors are the same size
    if (vecSize != addend.vecSize)
    {
        printf("atVector::add: Vector size mismatch, (this->vecSize = %d) != (addend.vecSize = %d)\n",vecSize,addend.vecSize);
        return;
    }

    // Add each element of the addend vector to this vector
    for (i = 0; i < vecSize; i++)
        data[i] += addend.data[i];
}

// ------------------------------------------------------------------------
// Adds the addend vector to this one, returning the result. The two
// vectors must be the same size.
// ------------------------------------------------------------------------
atVector atVector::getSum(const atVector &addend) const
{
    int i;
    atVector result;

    // Verify that the vectors are the same size
    if (vecSize != addend.vecSize)
    {
        printf("atVector::getSum: Vector size mismatch, (this->vecSize = %d) != (addend.vecSize = %d)\n",vecSize,addend.vecSize);
        return result;
    }

    // Create the target vector by adding each element of this vector to
    // the corresponding element of the addend vector
    result.vecSize = vecSize;
    for (i = 0; i < vecSize; i++)
        result.data[i] = data[i] + addend.data[i];

    // Return the target vector
    return result;
}

// ------------------------------------------------------------------------
// Subtracts the subtrahend vector from this one, keeping the result in
// this vector. The two vectors must be the same size.
// ------------------------------------------------------------------------
void atVector::subtract(const atVector &subtrahend)
{
    int i;

    // Verify that the vectors are the same size
    if (vecSize != subtrahend.vecSize)
    {
        printf("atVector::subtract: Vector size mismatch, (this->vecSize = %d) != (subtrahend.vecSize = %d)\n",vecSize,subtrahend.vecSize);
        return;
    }

    // Subtract each element of the subtrahend vector from this vector
    for (i = 0; i < vecSize; i++)
        data[i] -= subtrahend.data[i];
}

// ------------------------------------------------------------------------
// Subtracts the subtrahend vector from this one, returning the result. The
// two vectors must be the same size.
// ------------------------------------------------------------------------
atVector atVector::getDifference(const atVector &subtrahend) const
{
    int i;
    atVector result;

    // Verify that the vectors are the same size
    if (vecSize != subtrahend.vecSize)
    {
        printf("atVector::getDifference: Vector size mismatch, (this->vecSize = %d) != (subtrahend.vecSize = %d)\n",vecSize,subtrahend.vecSize);
        return result;
    }

    // Create the target vector by subtracting each element of the
    // subtrahend from the corresponding element of this vectur
    result.vecSize = vecSize;
    for (i = 0; i < vecSize; i++)
        result.data[i] = data[i] - subtrahend.data[i];

    // Return the target vector
    return result;
}

// ------------------------------------------------------------------------
// Multiplies each element of this vector by the given scalar, keeping the
// result in this vector.
// ------------------------------------------------------------------------
void atVector::scale(double multiplier)
{
    int i;

    // Multiply each element of this vector by the given scalar
    for (i = 0; i < vecSize; i++)
        data[i] *= multiplier;
}

// ------------------------------------------------------------------------
// Multiplies each element of this vector by the given scalar, returning
// the result.
// ------------------------------------------------------------------------
atVector atVector::getScaled(double multiplier) const
{
    int i;
    atVector result;

    // Create the target vector by multiplying each element of this vector
    // by the given scalar
    result.vecSize = vecSize;
    for (i = 0; i < vecSize; i++)
        result.data[i] = data[i] * multiplier;

    // Return the target vector
    return result;
}
    
// ------------------------------------------------------------------------
// Returns the magnitude of this vector.
// ------------------------------------------------------------------------
double atVector::getMagnitude() const
{
    int i;
    double total = 0.0;

    // Compute the squared magnitude of this vector
    for (i = 0; i < vecSize; i++)
        total += AT_SQR(data[i]);

    // Return the square root of the square of the magnitude
    return sqrt(total);
}

// ------------------------------------------------------------------------
// Returns the square of the magnitude of this vector.
// ------------------------------------------------------------------------
double atVector::getMagnitudeSquared() const
{
    int i;
    double total = 0.0;

    // Compute the squared magnitude of this vector...
    for (i = 0; i < vecSize; i++)
        total += AT_SQR(data[i]);

    // ...and return it.
    return total;
}

// ------------------------------------------------------------------------
// Returns the result of the vector dot product between the operand vector
// and this vector. The two vectors must be the same size.
// ------------------------------------------------------------------------
double atVector::getDotProduct(const atVector &operand) const
{
    int i;
    double total = 0.0;

    // Verify that the vectors are the same size
    if (vecSize != operand.vecSize)
    {
        printf("atVector::getDotProduct: Vector size mismatch, (this->vecSize = %d) != (operand.vecSize = %d)\n",vecSize,operand.vecSize);
        return 0.0;
    }

    // Compute the dot product
    for (i = 0; i < vecSize; i++)
        total += (data[i] * operand.data[i]);

    // Return the result
    return total;
}

// ------------------------------------------------------------------------
// Normalizes this vector, keeping the result in this vector.
// ------------------------------------------------------------------------
void atVector::normalize()
{
    double mag;

    // Get the magnitude of this vector
    mag = getMagnitude();
    
    // If the magnitude is zero, then normalization is undefined. Abort.
    if (mag == 0.0)
        return;
    
    // Divide each element of this vector by the magnitude
    for (int i = 0; i < vecSize; i++)
        data[i] /= mag;
}

// ------------------------------------------------------------------------
// Returns a normalized version of this vector.
// ------------------------------------------------------------------------
atVector atVector::getNormalized() const
{
    int i;
    atVector result;
    double mag;

    // Set the size of the result
    result.vecSize = vecSize;

    // Get the magnitude of this vector
    mag = getMagnitude();

    // If the magnitude is zero, then normalization is undefined. Return
    // a dummy zero vector result.
    if (mag == 0.0)
        return result;

    // Divide each element of the result vector by the magnitude
    for (i = 0; i < vecSize; i++)
        result.data[i] = (data[i] / mag);

    // Return the result vector
    return result;
}

// ------------------------------------------------------------------------
// Computes the vector cross product between this vector and the operand
// vector, keeping the result in this vector. Both vectors must be of
// size 3.
// ------------------------------------------------------------------------
void atVector::crossProduct(const atVector &operand)
{
    double result[3];

    // Verify that both vectors are large enough
    if ((vecSize < 3) || (operand.vecSize < 3))
    {
        printf("atVector::crossProduct: Both vectors must be at least "
            "size 3\n");
        return;
    }

    // Compute the cross product, putting the result in a temporary array
    result[0] = (data[1] * operand.data[2]) - (data[2] * operand.data[1]);
    result[1] = (data[2] * operand.data[0]) - (data[0] * operand.data[2]);
    result[2] = (data[0] * operand.data[1]) - (data[1] * operand.data[0]);

    // Copy the resulting product back into this vector
    data[0] = result[0];
    data[1] = result[1];
    data[2] = result[2];
}

// ------------------------------------------------------------------------
// Returns the result of the vector cross product between this vector and
// the operand vector. Both vectors must be of size 3.
// ------------------------------------------------------------------------
atVector atVector::getCrossProduct(const atVector &operand) const
{
    atVector result;

    // Verify that both vectors are at least size 3. Return a dummy zero
    // vector if they're not.
    if ((vecSize < 3) || (operand.vecSize < 3))
    {
        printf("atVector::getCrossProduct: Both vectors must be at least "
            "size 3\n");
        return result;
    }

    result.vecSize = 3;

    // Compute the cross product...
    result.data[0] = (data[1] * operand.data[2]) - (data[2] * operand.data[1]);
    result.data[1] = (data[2] * operand.data[0]) - (data[0] * operand.data[2]);
    result.data[2] = (data[0] * operand.data[1]) - (data[1] * operand.data[0]);
    
    // ...and return it.
    return result;
}

// ------------------------------------------------------------------------
// Returns the angle, in degrees, formed by this vector and the specified
// vector.
// ------------------------------------------------------------------------
double atVector::getAngleBetween(const atVector &endVector) const
{
    // By one of the definitions of the dot product, the cosine of the angle
    // between two vectors is equal to their dot product divided by the
    // product of their magnitudes.

    double dot, dotAbs, startMag, endMag;

    // Compute the dot product
    dot = getDotProduct(endVector);

    // Compute the magnitudes of both vectors
    startMag = getMagnitude();
    endMag = endVector.getMagnitude();

    // Special case: if one or both vectors are zero vectors, then trying to
    // find the angle between them is essentially undefined. Return a zero
    // result if this happens.
    if ((startMag < AT_DEFAULT_TOLERANCE) || (endMag < AT_DEFAULT_TOLERANCE))
        return 0.0;

    // Deal with any floating point inaccuracy here.  If the numerator and
    // denominator are nearly equal, it can cause the acos() call below to
    // return NaN's in some cases.
    dotAbs = fabs(dot);
    if (fabs(dotAbs - (startMag * endMag)) < AT_DEFAULT_TOLERANCE)
    {
        // The vectors are either equal or opposite.  We can find out using
        // the sign of the dot product
        if (dot < 0.0)
            return 180.0;
        else
            return 0.0;
    }

    // The final angle is the inverse cosine of the quotient of the dot
    // product and magnitude product
    return AT_RAD2DEG(acos(dot / (startMag * endMag)));
}

// ------------------------------------------------------------------------
// Retrieves one value from the vector as a reference to a double.
// ------------------------------------------------------------------------
double &atVector::operator[](int index)
{
    // Bounds checking
    if ((index < 0) || (index >= vecSize))
    {
        printf("atVector::operator[]: Invalid index = %d\n",index);
        return data[0];
    }
    
    // Return a reference to the desired data value
    return data[index];
}

// ------------------------------------------------------------------------
// Retrieves one value from the vector as a reference to a double.
// ------------------------------------------------------------------------
const double &atVector::operator[](int index) const
{
    // Bounds checking
    if ((index < 0) || (index >= vecSize))
    {
        printf("atVector::operator[]: Invalid index = %d\n",index);
        return data[0];
    }
    
    // Return a reference to the desired data value
    return data[index];
}

// ------------------------------------------------------------------------
// Adds the addend vector to this one, returning the result. The two
// vectors must be the same size.
// Equivalent to getSum(addend)
// ------------------------------------------------------------------------
atVector atVector::operator+(const atVector &addend) const
{
    int i;
    atVector result;

    // Verify that the vectors are the same size
    if (vecSize != addend.vecSize)
    {
        printf("atVector::operator+: Vector size mismatch, (this->vecSize = %d) != (addend.vecSize = %d)\n",vecSize,addend.vecSize);
        return result;
    }

    // Create the target vector by adding each element of this vector to
    // the corresponding element of the addend vector
    result.vecSize = vecSize;
    for (i = 0; i < vecSize; i++)
        result.data[i] = data[i] + addend.data[i];

    // Return the target vector
    return result;
}

// ------------------------------------------------------------------------
// Subtracts the subtrahend vector from this one, returning the result. The
// two vectors must be the same size.
// Equivalent to getDifference(subtrahend)
// ------------------------------------------------------------------------
atVector atVector::operator-(const atVector &subtrahend) const
{
    int i;
    atVector result;

    // Verify that the vectors are the same size
    if (vecSize != subtrahend.vecSize)
    {
        printf("atVector::operator-: Vector size mismatch, (this->vecSize = %d) != (subtrahend.vecSize = %d)\n",vecSize,subtrahend.vecSize);
        return result;
    }

    // Create the target vector by subtracting each element of the
    // subtrahend from the corresponding element of this vectur
    result.vecSize = vecSize;
    for (i = 0; i < vecSize; i++)
        result.data[i] = data[i] - subtrahend.data[i];

    // Return the target vector
    return result;
}

// ------------------------------------------------------------------------
// Multiplies each element of this vector by the given scalar, returning
// the result.
// Equivalent to getScaled(multiplier)
// ------------------------------------------------------------------------
atVector atVector::operator*(double multiplier) const
{
    int i;
    atVector result;

    // Create the target vector by multiplying each element of this vector
    // by the given scalar
    result.vecSize = vecSize;
    for (i = 0; i < vecSize; i++)
        result.data[i] = data[i] * multiplier;

    // Return the target vector
    return result;
}

// ------------------------------------------------------------------------
// Adds the addend vector to this one, keeping the result in this vector.
// The two vectors must be the same size.
// Equivalent to add(addend)
// ------------------------------------------------------------------------
void atVector::operator+=(const atVector &addend)
{
    int i;

    // Verify that the vectors are the same size
    if (vecSize != addend.vecSize)
    {
        printf("atVector::operator+=: Vector size mismatch, (this->vecSize = %d) != (addend.vecSize = %d)\n",vecSize,addend.vecSize);
        return;
    }

    // Add each element of the addend vector to this vector
    for (i = 0; i < vecSize; i++)
        data[i] += addend.data[i];
}

// ------------------------------------------------------------------------
// Subtracts the subtrahend vector from this one, keeping the result in
// this vector. The two vectors must be the same size.
// Equivalent to subtract(subtrahend)
// ------------------------------------------------------------------------
void atVector::operator-=(const atVector &subtrahend)
{
    int i;

    // Verify that the vectors are the same size
    if (vecSize != subtrahend.vecSize)
    {
        printf("atVector::operator-=: Vector size mismatch, (this->vecSize = %d) != (subtrahend.vecSize = %d)\n",vecSize,subtrahend.vecSize);
        return;
    }

    // Subtract each element of the subtrahend vector from this vector
    for (i = 0; i < vecSize; i++)
        data[i] -= subtrahend.data[i];
}

// ------------------------------------------------------------------------
// Multiplies each element of this vector by the given scalar, keeping the
// result in this vector.
// Equivalent to scale(multiplier)
// ------------------------------------------------------------------------
void atVector::operator*=(double multiplier)
{
    int i;

    // Multiply each element of this vector by the given scalar
    for (i = 0; i < vecSize; i++)
        data[i] *= multiplier;
}

// ------------------------------------------------------------------------
// Checks for element-wise equality between two vectors. The two vectors
// must be the same size. Two elements are considered equal if they are
// within a small default tolerance value of each other.
// Equivalent to isEqual(operand)
// ------------------------------------------------------------------------
bool atVector::operator==(const atVector &operand) const
{
    int i;

    // Verify that the vectors are the same size
    if (vecSize != operand.vecSize)
    {
        printf("atVector::operator==: Vector size mismatch\n");
        return false;
    }

    // Check each pair of values (this vector's and the operand vector's)
    // for almost-equality; return false if a pair doesn't match up.
    for (i = 0; i < vecSize; i++)
        if (fabs(data[i] - operand.data[i]) > AT_DEFAULT_TOLERANCE)
            return false;

    // If all the pairs match, return true
    return true;
}

// ------------------------------------------------------------------------
// Prints a text representation of this vector to stdout
// ------------------------------------------------------------------------
void atVector::print() const
{
    int i;

    // Enclose the components of the vector in angle brackets
    printf("<");

    // Print all but the last component with a trailing comma and space
    for (i = 0; i < vecSize-1; i++)
        printf("%0.4lf, ", data[i]);

    // Print the last component and close with an angle bracket
    printf("%0.4lf>", data[vecSize-1]);
}

// ------------------------------------------------------------------------
// Prints a text representation of this vector to the specified file
// ------------------------------------------------------------------------
void atVector::print(FILE *fp) const
{
    int i;

    // Enclose the components of the vector in angle brackets
    fprintf(fp, "<");

    // Print all but the last component with a trailing comma and space
    for (i = 0; i < vecSize-1; i++)
        fprintf(fp, "%0.4lf, ", data[i]);

    // Print the last component and close with an angle bracket
    fprintf(fp, "%0.4lf>", data[vecSize-1]);
}

// ------------------------------------------------------------------------
// Related function
// Multiplies each element of this vector by the given scalar, returning
// the result.
// ------------------------------------------------------------------------
atVector operator*(double multiplier, atVector operand)
{
    atVector result;
    
    // Create the target vector by multiplying each element of this vector
    // by the given scalar
    result.setSize(operand.getSize());
    for (int i = 0; i < operand.getSize(); i++)
        result[i] = multiplier * operand[i];

    // Return the target vector
    return result;
}

// ------------------------------------------------------------------------
// atItem derived method.  Return whether the given atItem is an equivalent
// atVector
// ------------------------------------------------------------------------
bool atVector::equals(atItem * otherItem)
{
   atVector * otherVector;

   // Try to cast the item to a vector
   otherVector = dynamic_cast<atVector *>(otherItem);

   // Return true iff the item is a vector and they are equivalent (within
   // default tolerance)
   if ((otherVector != NULL) && (isEqual(*otherVector)))
      return true;
   else
      return false;
}
