
// ATLAS: Adaptable Tool Library for Advanced Simulation
//
// Copyright 2015 University of Central Florida
//
//
// This library provides many fundamental capabilities used in creating
// virtual environment simulations.  It includes elements such as vectors,
// matrices, quaternions, containers, communication schemes (UDP, TCP, DIS,
// HLA, Bluetooth), and XML processing.  It also includes some extensions
// to allow similar code to work in Linux and in Windows.  Note that support
// for iOS and Android development is also included.
//
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// 
//     http://www.apache.org/licenses/LICENSE-2.0
// 
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.


#include <math.h>
#include <stdio.h>
#include "atGlobals.h++"
#include "atMatrix.h++"

// ------------------------------------------------------------------------
// Constructor
// Clears the matrix to zero (not identity)
// ------------------------------------------------------------------------
atMatrix::atMatrix()
{
    // I would normally call clear() to zero the matrix here, but since the
    // matrix is made up of atVectors, which zero themselves on creation
    // anyway, it's unnecessary to do it again.
}

// ------------------------------------------------------------------------
// Constructor
// Sets the matrix data to the given two-dimensional array
// ------------------------------------------------------------------------
atMatrix::atMatrix(double values[4][4])
{
    set(values);
}

// ------------------------------------------------------------------------
// Destructor
// ------------------------------------------------------------------------
atMatrix::~atMatrix()
{
}

// ------------------------------------------------------------------------
// Sets the matrix data to the given two-dimensional array
// ------------------------------------------------------------------------
void atMatrix::set(double values[4][4])
{
    int i;

    // Copy the matix values from the specified array to the matrix's
    // internal storage area
    for (i = 0; i < 4; i++)
        data[i].set(values[i][0], values[i][1], values[i][2], values[i][3]);
}

// ------------------------------------------------------------------------
// Makes this matrix an exact duplicate of the source matrix
// ------------------------------------------------------------------------
void atMatrix::copy(const atMatrix &source)
{
    (*this) = source;
}

// ------------------------------------------------------------------------
// Sets the matrix data to zero
// ------------------------------------------------------------------------
void atMatrix::clear()
{
    int i;

    for (i = 0; i < 4; i++)
        data[i].clear();
}

// ------------------------------------------------------------------------
// Sets one specific data value in matrix
// ------------------------------------------------------------------------
void atMatrix::setValue(int row, int column, double value)
{
    // Set the specified value
    data[row][column] = value;
}

// ------------------------------------------------------------------------
// Retrieves one specific data value from matrix
// ------------------------------------------------------------------------
double atMatrix::getValue(int row, int column) const
{
    // Return the desired value
    return data[row].getValue(column);
}

// ------------------------------------------------------------------------
// Checks for element-wise equality between two matricies. Two elements
// are considered equal if they are within a small default tolerance value.
// ------------------------------------------------------------------------
bool atMatrix::isEqual(const atMatrix &operand) const
{
    int i;

    // Check each pair of row vectors (this object's and the operand's)
    // for almost-equality; return false if a pair doesn't match up.
    for (i = 0; i < 4; i++)
        if (!(data[i].isEqual(operand.data[i])))
            return false;

    // If all the pairs match, return true
    return true;
}

// ------------------------------------------------------------------------
// Checks for element-wise equality between two matricies. Two elements
// are considered equal if they are within the specified tolerance value.
// ------------------------------------------------------------------------
bool atMatrix::isAlmostEqual(const atMatrix &operand, double tolerance) const
{
    int i;

    // Check each pair of row vectors (this object's and the operand's) for
    // almost-equality, 'almost' being specified by a given tolerance
    // value. Return false if a pair doesn't match up.
    for (i = 0; i < 4; i++)
        if (!(data[i].isAlmostEqual(operand.data[i], tolerance)))
            return false;

    // If all the pairs match, return true
    return true;
}

// ------------------------------------------------------------------------
// Adds the specified matrix to this matrix, keeping the result
// ------------------------------------------------------------------------
void atMatrix::add(const atMatrix &addend)
{
    int i;

    // Add each row of the addend matrix to this matrix
    for (i = 0; i < 4; i++)
        data[i] += addend.data[i];
}

// ------------------------------------------------------------------------
// Adds the specified matrix to this matrix, returning the result
// ------------------------------------------------------------------------
atMatrix atMatrix::getSum(const atMatrix &addend) const
{
    int i;
    atMatrix result;

    // Create the target matrix by adding each row of this matrix
    // to each row of the addend matrix
    for (i = 0; i < 4; i++)
        result.data[i] = data[i] + addend.data[i];

    // Return the target matrix
    return result;
}

// ------------------------------------------------------------------------
// Subtracts the specified matrix from this matrix, keeping the result
// ------------------------------------------------------------------------
void atMatrix::subtract(const atMatrix &subtrahend)
{
    int i;

    // Subtract each row of the subtrahend matrix from this matrix
    for (i = 0; i < 4; i++)
        data[i] -= subtrahend.data[i];
}

// ------------------------------------------------------------------------
// Subtracts the specified matrix from this matrix, returning the result
// ------------------------------------------------------------------------
atMatrix atMatrix::getDifference(const atMatrix &subtrahend) const
{
    int i;
    atMatrix result;

    // Create the target matrix by subtracting each row of the
    // subtrahend from each row of this matrix
    for (i = 0; i < 4; i++)
        result.data[i] = data[i] - subtrahend.data[i];

    // Return the target matrix
    return result;
}

// ------------------------------------------------------------------------
// Multiplies this matrix by the given scalar, keeping the result
// ------------------------------------------------------------------------
void atMatrix::scale(double multiplier)
{
    int i;
    
    // Multiply each row of this matrix by the given scalar
    for (i = 0; i < 4; i++)
        data[i].scale(multiplier);
}

// ------------------------------------------------------------------------
// Multiplies this matrix by the given scalar, returning the result
// ------------------------------------------------------------------------
atMatrix atMatrix::getScaled(double multiplier) const
{
    int i;
    atMatrix result;
    
    // Create the target matrix by multiplying each row of this matrix
    // by the given scalar
    for (i = 0; i < 4; i++)
        result.data[i] = data[i] * multiplier;

    // Return the target matrix
    return result;
}

// ------------------------------------------------------------------------
// Transposes this matrix, keeping the result
// ------------------------------------------------------------------------
void atMatrix::transpose()
{
    int i, j;
    double temp;
    
    // Swap the elements of this matrix across its diagonal
    for (i = 0; i < 4; i++)
        for (j = 0; j < i; j++)
        {
            temp = data[i][j];
            data[i][j] = data[j][i];
            data[j][i] = temp;
        }
}

// ------------------------------------------------------------------------
// Transposes this matrix, returning the result
// ------------------------------------------------------------------------
atMatrix atMatrix::getTranspose() const
{
    atMatrix result;
    
    // Create the target matrix by copying it directly, and then transposing
    // it (this involves fewer accesses than copying and transposing each
    // element individually)
    result = (*this);
    result.transpose();

    // Return the target matrix
    return result;
}

// ------------------------------------------------------------------------
// Returns the value of the determinant for this matrix
// ------------------------------------------------------------------------
double atMatrix::getDeterminant() const
{
    // Cheat: Rather than coming up with a fancy algorithm for calculating
    // the determinant, since the matrix is of fixed size we can just
    // hardcode the pattern of multiplications.

    return ((data[0].getValue(0) * data[1].getValue(1) -
                data[0].getValue(1) * data[1].getValue(0)) * 
            (data[2].getValue(2) * data[3].getValue(3) -
                data[2].getValue(3) * data[3].getValue(2))) +
           ((data[0].getValue(2) * data[1].getValue(0) -
                data[0].getValue(0) * data[1].getValue(2)) *
            (data[2].getValue(1) * data[3].getValue(3) -
                data[2].getValue(3) * data[3].getValue(1))) +
           ((data[0].getValue(1) * data[1].getValue(2) -
                data[0].getValue(2) * data[1].getValue(1)) *
            (data[2].getValue(0) * data[3].getValue(3) -
                data[2].getValue(3) * data[3].getValue(0))) +
           ((data[0].getValue(0) * data[1].getValue(3) -
                data[0].getValue(3) * data[1].getValue(0)) *
            (data[2].getValue(1) * data[3].getValue(2) -
                data[2].getValue(2) * data[3].getValue(1))) +
           ((data[0].getValue(3) * data[1].getValue(1) -
                data[0].getValue(1) * data[1].getValue(3)) *
            (data[2].getValue(0) * data[3].getValue(2) -
                data[2].getValue(2) * data[3].getValue(0))) +
           ((data[0].getValue(2) * data[1].getValue(3) -
                data[0].getValue(3) * data[1].getValue(2)) *
            (data[2].getValue(0) * data[3].getValue(1) -
                data[2].getValue(1) * data[3].getValue(0)));   
}

// ------------------------------------------------------------------------
// Sets this matrix to the inverse of itself. If the matrix has a
// determinant of zero, it cannot be inverted and is left unchanged.
// ------------------------------------------------------------------------
void atMatrix::invert()
{
    // The code for this is pulled from the document "Streaming SIMD
    // Extensions - Inverse of 4x4 Matrix", Intel Technical Document
    // AP-928.

    double det;
    double sourceMat[16];
    double destMat[16];
    double temp[12];
    int i;

    // A matrix with a zero determinant can't be inverted
    det = getDeterminant();
    if (det == 0.0)
    {
        printf("atMatrix::invert: Matrix has no inverse\n");
        return;
    }

    // Extract the elements of the matrix into a local variable to improve
    // access speed. Also transpose the matrix at the same time, as this is
    // something we'd have to do anyway.
    for (i = 0; i < 4; i++)
    {
        sourceMat[i]      = data[i].getValue(0);
        sourceMat[i + 4]  = data[i].getValue(1);
        sourceMat[i + 8]  = data[i].getValue(2);
        sourceMat[i + 12] = data[i].getValue(3);
    }

    // Compute temporary values for the first round of calculations
    temp[0]  = sourceMat[10] * sourceMat[15];
    temp[1]  = sourceMat[11] * sourceMat[14];
    temp[2]  = sourceMat[9]  * sourceMat[15];
    temp[3]  = sourceMat[11] * sourceMat[13];
    temp[4]  = sourceMat[9]  * sourceMat[14];
    temp[5]  = sourceMat[10] * sourceMat[13];
    temp[6]  = sourceMat[8]  * sourceMat[15];
    temp[7]  = sourceMat[11] * sourceMat[12];
    temp[8]  = sourceMat[8]  * sourceMat[14];
    temp[9]  = sourceMat[10] * sourceMat[12];
    temp[10] = sourceMat[8]  * sourceMat[13];
    temp[11] = sourceMat[9]  * sourceMat[12];

    // Calculate the top half of the result matrix
    destMat[0]  = temp[0]*sourceMat[5] + temp[3]*sourceMat[6] +
        temp[4]*sourceMat[7];
    destMat[0] -= temp[1]*sourceMat[5] + temp[2]*sourceMat[6] +
        temp[5]*sourceMat[7];
    destMat[1]  = temp[1]*sourceMat[4] + temp[6]*sourceMat[6] +
        temp[9]*sourceMat[7];
    destMat[1] -= temp[0]*sourceMat[4] + temp[7]*sourceMat[6] +
        temp[8]*sourceMat[7];
    destMat[2]  = temp[2]*sourceMat[4] + temp[7]*sourceMat[5] +
        temp[10]*sourceMat[7];
    destMat[2] -= temp[3]*sourceMat[4] + temp[6]*sourceMat[5] +
        temp[11]*sourceMat[7];
    destMat[3]  = temp[5]*sourceMat[4] + temp[8]*sourceMat[5] +
        temp[11]*sourceMat[6];
    destMat[3] -= temp[4]*sourceMat[4] + temp[9]*sourceMat[5] +
        temp[10]*sourceMat[6];
    destMat[4]  = temp[1]*sourceMat[1] + temp[2]*sourceMat[2] +
        temp[5]*sourceMat[3];
    destMat[4] -= temp[0]*sourceMat[1] + temp[3]*sourceMat[2] +
        temp[4]*sourceMat[3];
    destMat[5]  = temp[0]*sourceMat[0] + temp[7]*sourceMat[2] +
        temp[8]*sourceMat[3];
    destMat[5] -= temp[1]*sourceMat[0] + temp[6]*sourceMat[2] +
        temp[9]*sourceMat[3];
    destMat[6]  = temp[3]*sourceMat[0] + temp[6]*sourceMat[1] +
        temp[11]*sourceMat[3];
    destMat[6] -= temp[2]*sourceMat[0] + temp[7]*sourceMat[1] +
        temp[10]*sourceMat[3];
    destMat[7]  = temp[4]*sourceMat[0] + temp[9]*sourceMat[1] +
        temp[10]*sourceMat[2];
    destMat[7] -= temp[5]*sourceMat[0] + temp[8]*sourceMat[1] +
        temp[11]*sourceMat[2];

    // Compute temporary values for the second round of calculations
    temp[0]  = sourceMat[2] * sourceMat[7];
    temp[1]  = sourceMat[3] * sourceMat[6];
    temp[2]  = sourceMat[1] * sourceMat[7];
    temp[3]  = sourceMat[3] * sourceMat[5];
    temp[4]  = sourceMat[1] * sourceMat[6];
    temp[5]  = sourceMat[2] * sourceMat[5];
    temp[6]  = sourceMat[0] * sourceMat[7];
    temp[7]  = sourceMat[3] * sourceMat[4];
    temp[8]  = sourceMat[0] * sourceMat[6];
    temp[9]  = sourceMat[2] * sourceMat[4];
    temp[10] = sourceMat[0] * sourceMat[5];
    temp[11] = sourceMat[1] * sourceMat[4];

    // Calculate the bottom half of the result matrix
    destMat[8]   = temp[0]*sourceMat[13]  + temp[3]*sourceMat[14]  +
        temp[4]*sourceMat[15];
    destMat[8]  -= temp[1]*sourceMat[13]  + temp[2]*sourceMat[14]  +
        temp[5]*sourceMat[15];
    destMat[9]   = temp[1]*sourceMat[12]  + temp[6]*sourceMat[14]  +
        temp[9]*sourceMat[15];
    destMat[9]  -= temp[0]*sourceMat[12]  + temp[7]*sourceMat[14]  +
        temp[8]*sourceMat[15];
    destMat[10]  = temp[2]*sourceMat[12]  + temp[7]*sourceMat[13]  +
        temp[10]*sourceMat[15];
    destMat[10] -= temp[3]*sourceMat[12]  + temp[6]*sourceMat[13]  +
        temp[11]*sourceMat[15];
    destMat[11]  = temp[5]*sourceMat[12]  + temp[8]*sourceMat[13]  +
        temp[11]*sourceMat[14];
    destMat[11] -= temp[4]*sourceMat[12]  + temp[9]*sourceMat[13]  +
        temp[10]*sourceMat[14];
    destMat[12]  = temp[2]*sourceMat[10]  + temp[5]*sourceMat[11]  +
        temp[1]*sourceMat[9];
    destMat[12] -= temp[4]*sourceMat[11]  + temp[0]*sourceMat[9]   +
        temp[3]*sourceMat[10];
    destMat[13]  = temp[8]*sourceMat[11]  + temp[0]*sourceMat[8]   +
        temp[7]*sourceMat[10];
    destMat[13] -= temp[6]*sourceMat[10]  + temp[9]*sourceMat[11]  +
        temp[1]*sourceMat[8];
    destMat[14]  = temp[6]*sourceMat[9]   + temp[11]*sourceMat[11] +
        temp[3]*sourceMat[8];
    destMat[14] -= temp[10]*sourceMat[11] + temp[2]*sourceMat[8]   +
        temp[7]*sourceMat[9];
    destMat[15]  = temp[10]*sourceMat[10] + temp[4]*sourceMat[8]   +
        temp[9]*sourceMat[9];
    destMat[15] -= temp[8]*sourceMat[9]   + temp[11]*sourceMat[10] +
        temp[5]*sourceMat[8];

    // Copy the resulting values back into this matrix, scaling by the inverse
    // of the determinant at the same time to form the matrix inverse
    for (i = 0; i < 4; i++)
        data[i].set(destMat[(i * 4) + 0] / det,
                    destMat[(i * 4) + 1] / det,
                    destMat[(i * 4) + 2] / det,
                    destMat[(i * 4) + 3] / det);
}

// ------------------------------------------------------------------------
// Returns the inverse matrix of this matrix. Returns a zero matrix if
// this matrix does not have an inverse.
// ------------------------------------------------------------------------
atMatrix atMatrix::getInverse() const
{
    atMatrix result;

    // Cheat: call the other version of the inverse function
    result = (*this);
    result.invert();

    // Return the inverted matrix
    return result;
}

// ------------------------------------------------------------------------
// Inverts this matrix, with the assumpion that the matrix only contains
// rotations and translations.  "Rigid" refers to rigid-body transforms,
// which typically only consist of rotations and translations
// ------------------------------------------------------------------------
void atMatrix::invertRigid()
{
   atMatrix invMat;

   // Get the inverse matrix and set the current matrix to match it
   invMat = this->getInverseRigid();
   (*this) = invMat;
}

// ------------------------------------------------------------------------
// Returns a matrix that is the inverse of this matrix, with the assumption
// that the matrix only contains rotations and translations.  "Rigid"
// refers to rigid-body transforms, which typically only consist of
// rotations and translations
// ------------------------------------------------------------------------
atMatrix atMatrix::getInverseRigid() const
{
   atMatrix invMat;
   double tx, ty, tz;

   // The inverse of the rotation can be done by transposing the upper 3x3
   // portion of the matrix
   invMat[0][0] = data[0][0];
   invMat[0][1] = data[1][0];
   invMat[0][2] = data[2][0];

   invMat[1][0] = data[0][1];
   invMat[1][1] = data[1][1];
   invMat[1][2] = data[2][1];

   invMat[2][0] = data[0][2];
   invMat[2][1] = data[1][2];
   invMat[2][2] = data[2][2];

   // Now, invert the translation component by "rotating" it by the inverted
   // rotation matrix and negating the result
   tx = data[0][3];
   ty = data[1][3];
   tz = data[2][3];
   invMat[0][3] = -(tx*invMat[0][0] + ty*invMat[0][1] + tz*invMat[0][2]);
   invMat[1][3] = -(tx*invMat[1][0] + ty*invMat[1][1] + tz*invMat[1][2]);
   invMat[2][3] = -(tx*invMat[2][0] + ty*invMat[2][1] + tz*invMat[2][2]);

   // Finally, just fill out the last row of the matrix, which is constant
   // in a rigid transform
   invMat[3][0] = 0.0;
   invMat[3][1] = 0.0;
   invMat[3][2] = 0.0;
   invMat[3][3] = 1.0;

   return invMat;
}

// ------------------------------------------------------------------------
// Multiplies this matrix with the given matrix; the operand matrix is
// considered to be on the left. The result is stored.
// ------------------------------------------------------------------------
void atMatrix::preMultiply(const atMatrix &operand)
{
    double result[4];
    atMatrix thisTranspose = getTranspose();
    int i;

    // Do a matrix-multiply operation between this matrix and the operand
    // matrix, with this matrix second; place the results in a temporary
    // matrix instead of this one so that we don't overwrite values that
    // may be needed for later calculations.
    for (i = 0; i < 4; i++)
    {
        // Since the second matrix is transposed, each element of the
        // result can be computed by taking the dot product of the
        // appropriate rows of each matrix. This should be a lot faster
        // than accessing each element directly, as it avoids a good deal
        // of the error checking on the atVector data accessors.
        result[0] = operand.data[i].getDotProduct(thisTranspose.data[0]);
        result[1] = operand.data[i].getDotProduct(thisTranspose.data[1]);
        result[2] = operand.data[i].getDotProduct(thisTranspose.data[2]);
        result[3] = operand.data[i].getDotProduct(thisTranspose.data[3]);

        // Copy the results to this matrix
        data[i].set(result[0], result[1], result[2], result[3]);
    }
}

// ------------------------------------------------------------------------
// Multiplies this matrix with the given matrix; the operand matrix is
// considered to be on the left. The result is returned.
// ------------------------------------------------------------------------
atMatrix atMatrix::getPreMultiplied(const atMatrix &operand) const
{
    atMatrix result;
    double temp[4];
    atMatrix thisTranspose = getTranspose();
    int i;

    // Do a matrix-multiply operation between this matrix and the operand
    // matrix, with this matrix second; place the results in the target
    // matrix.
    for (i = 0; i < 4; i++)
    {
        // Since the second matrix is transposed, each element of the
        // result can be computed by taking the dot product of the
        // appropriate rows of each matrix. This should be a lot faster
        // than accessing each element directly, as it avoids a good deal
        // of the error checking on the atVector data accessors.
        temp[0] = operand.data[i].getDotProduct(thisTranspose.data[0]);
        temp[1] = operand.data[i].getDotProduct(thisTranspose.data[1]);
        temp[2] = operand.data[i].getDotProduct(thisTranspose.data[2]);
        temp[3] = operand.data[i].getDotProduct(thisTranspose.data[3]);

        // Copy this row to the result matrix
        result[i].set(temp[0], temp[1], temp[2], temp[3]);
    }

    // Return the target matrix
    return result;
}

// ------------------------------------------------------------------------
// Multiplies this matrix with the given matrix; the operand matrix is
// considered to be on the right. The result is stored.
// ------------------------------------------------------------------------
void atMatrix::postMultiply(const atMatrix &operand)
{
    double result[4];
    atMatrix operandTranspose = operand.getTranspose();
    int i;

    // Do a matrix-multiply operation between this matrix and the operand
    // matrix, with this matrix first
    for (i = 0; i < 4; i++)
    {
        // Since the second matrix is transposed, each element of the
        // result can be computed by taking the dot product of the
        // appropriate rows of each matrix. This should be a lot faster
        // than accessing each element directly, as it avoids a good deal
        // of the error checking on the atVector data accessors.
        result[0] = data[i].getDotProduct(operandTranspose.data[0]);
        result[1] = data[i].getDotProduct(operandTranspose.data[1]);
        result[2] = data[i].getDotProduct(operandTranspose.data[2]);
        result[3] = data[i].getDotProduct(operandTranspose.data[3]);

        // We're done with this row of the matrix, so it's safe to just set
        // the result in place
        data[i].set(result[0], result[1], result[2], result[3]);
    }
}

// ------------------------------------------------------------------------
// Multiplies this matrix with the given matrix; the operand matrix is
// considered to be on the right. The result is returned.
// ------------------------------------------------------------------------
atMatrix atMatrix::getPostMultiplied(const atMatrix &operand) const
{
    atMatrix result;
    double temp[4];
    atMatrix operandTranspose = operand.getTranspose();
    int i;

    // Do a matrix-multiply operation between this matrix and the operand
    // matrix, with this matrix first; place the results in the result
    // matrix.
    for (i = 0; i < 4; i++)
    {
        // Since the second matrix is transposed, each element of the
        // result can be computed by taking the dot product of the
        // appropriate rows of each matrix. This should be a lot faster
        // than accessing each element directly, as it avoids a good deal
        // of the error checking on the atVector data accessors.
        temp[0] = data[i].getDotProduct(operandTranspose.data[0]);
        temp[1] = data[i].getDotProduct(operandTranspose.data[1]);
        temp[2] = data[i].getDotProduct(operandTranspose.data[2]);
        temp[3] = data[i].getDotProduct(operandTranspose.data[3]);

        // Copy the results to the result matrix
        result[i].set(temp[0], temp[1], temp[2], temp[3]);
    }

    // Return the target matrix
    return result;
}

// ------------------------------------------------------------------------
// Transforms the given point by multiplying this matrix by the point as
// a column vector on the right, returning the result. The fourth element
// of the operand is assumed to be one.
// ------------------------------------------------------------------------
atVector atMatrix::getPointXform(const atVector &operand) const
{
    atVector result;
    int i, j;

    // To be transformed in this manner, the operand vector must be
    // at least size 3.
    if (operand.getSize() < 3)
    {
        printf("atMatrix::getPointXform: Operand vector is too small\n");
        return result;
    }

    // Transform the vector by this matrix
    for (i = 0; i < 4; i++)
    {
        // Vectors start cleared by default; no need to set to zero here
        for (j = 0; j < 4; j++)
        {
            // Assume the fourth value of the vector is one
            if (j == 3)
                result[i] += data[i].getValue(j);
            else
                result[i] += (data[i].getValue(j) * operand.getValue(j));
        }
    }
    
    // Resize the result to match the size of the operand vector, and
    // return it.
    result.setSize(operand.getSize());
    return result;
}

// ------------------------------------------------------------------------
// Transforms the given vector by multiplying this matrix by the vector as
// a column vector on the right, returning the result. The fourth element
// of the operand is assumed to be zero.
// ------------------------------------------------------------------------
atVector atMatrix::getVectorXform(const atVector &operand) const
{
    atVector result;
    int i, j;

    // To be transformed in this manner, the operand vector must be
    // at least size 3.
    if (operand.getSize() < 3)
    {
        printf("atMatrix::getVectorXform: Operand vector is too small\n");
        return result;
    }

    // Transform the vector by this matrix
    for (i = 0; i < 4; i++)
    {
        // Vectors start cleared by default; no need to set to zero here.
        // Ignore the fourth value of the vector, if there is one.
        for (j = 0; j < 3; j++)
            result[i] += (data[i].getValue(j) * operand.getValue(j));
    }
    
    // Resize the result to match the size of the operand vector, and
    // return it.
    result.setSize(operand.getSize());
    return result;
}

// ------------------------------------------------------------------------
// Transforms the given homogeneous-coordinate point by multiplying this
// matrix by the point as a column vector on the right. The result is
// returned.
// ------------------------------------------------------------------------
atVector atMatrix::getFullXform(const atVector &operand) const
{
    atVector result;
    int i;

    // To be transformed in this manner, the operand vector must be
    // at least size 4.
    if (operand.getSize() < 4)
    {
        printf("atMatrix::getFullXform: Operand vector is too small\n");
        return result;
    }

    // Transform the vector by this matrix
    for (i = 0; i < 4; i++)
    {
        // This operation (summing the products of the corresponding
        // pairs of elements of the matrix-row vector and operand vector)
        // is exactly the same as taking the dot product. We already have
        // a function that does that, so just call that function.
        result[i] = data[i].getDotProduct(operand);
    }
    
    // Return the transformed vector
    return result;
}

// ------------------------------------------------------------------------
// Sets this matrix to the identity matrix.
// ------------------------------------------------------------------------
void atMatrix::setIdentity()
{
    int i;

    // Clear the matrix to all zeroes
    clear();

    // Set the matrix entries along the diagonal to one
    for (i = 0; i < 4; i++)
        data[i][i] = 1.0;
}

// ------------------------------------------------------------------------
// Returns whether or not this matrix is an identity matrix
// ------------------------------------------------------------------------
bool atMatrix::isIdentity()
{
   return ((data[0] == atVector(1.0, 0.0, 0.0, 0.0)) &&
           (data[1] == atVector(0.0, 1.0, 0.0, 0.0)) &&
           (data[2] == atVector(0.0, 0.0, 1.0, 0.0)) &&
           (data[3] == atVector(0.0, 0.0, 0.0, 1.0)));
}

// ------------------------------------------------------------------------
// Sets this matrix to a rotation matrix. The rotation is specified as a
// set of three Euler angle rotations, with the given axis constant used
// to specify the order of the axes.
// ------------------------------------------------------------------------
void atMatrix::setEulerRotation(atMathEulerAxisOrder axisOrder,
    double axis1Degrees, double axis2Degrees, double axis3Degrees)
{
    int i;
    int axis[3];
    atMatrix axisRotation[3];
    double axisDegrees[3];
    double tempVal;
    
    // Place the degree values into an array to make then easier to
    // iterate through
    axisDegrees[0] = axis1Degrees;
    axisDegrees[1] = axis2Degrees;
    axisDegrees[2] = axis3Degrees;
    
    // Decompose the axisOrder constant into three separate rotation axes
    switch (axisOrder)
    {
        case AT_EULER_ANGLES_XYZ_S:
        case AT_EULER_ANGLES_XYZ_R:
            axis[0] = 0;   axis[1] = 1;   axis[2] = 2;
            break;
        case AT_EULER_ANGLES_XZY_S:
        case AT_EULER_ANGLES_XZY_R:
            axis[0] = 0;   axis[1] = 2;   axis[2] = 1;
            break;
        case AT_EULER_ANGLES_YXZ_S:
        case AT_EULER_ANGLES_YXZ_R:
            axis[0] = 1;   axis[1] = 0;   axis[2] = 2;
            break;
        case AT_EULER_ANGLES_YZX_S:
        case AT_EULER_ANGLES_YZX_R:
            axis[0] = 1;   axis[1] = 2;   axis[2] = 0;
            break;
        case AT_EULER_ANGLES_ZXY_S:
        case AT_EULER_ANGLES_ZXY_R:
            axis[0] = 2;   axis[1] = 0;   axis[2] = 1;
            break;
        case AT_EULER_ANGLES_ZYX_S:
        case AT_EULER_ANGLES_ZYX_R:
            axis[0] = 2;   axis[1] = 1;   axis[2] = 0;
            break;
        case AT_EULER_ANGLES_XYX_S:
        case AT_EULER_ANGLES_XYX_R:
            axis[0] = 0;   axis[1] = 1;   axis[2] = 0;
            break;
        case AT_EULER_ANGLES_XZX_S:
        case AT_EULER_ANGLES_XZX_R:
            axis[0] = 0;   axis[1] = 2;   axis[2] = 0;
            break;
        case AT_EULER_ANGLES_YXY_S:
        case AT_EULER_ANGLES_YXY_R:
            axis[0] = 1;   axis[1] = 0;   axis[2] = 1;
            break;
        case AT_EULER_ANGLES_YZY_S:
        case AT_EULER_ANGLES_YZY_R:
            axis[0] = 1;   axis[1] = 2;   axis[2] = 1;
            break;
        case AT_EULER_ANGLES_ZXZ_S:
        case AT_EULER_ANGLES_ZXZ_R:
            axis[0] = 2;   axis[1] = 0;   axis[2] = 2;
            break;
        case AT_EULER_ANGLES_ZYZ_S:
        case AT_EULER_ANGLES_ZYZ_R:
            axis[0] = 2;   axis[1] = 1;   axis[2] = 2;
            break;
    }
    
    // Compute a rotation matrix for each of the three rotations
    for (i = 0; i < 3; i++)
    {
        // Initialize the matrix to zero, with a one in the
        // homogeneous scale position
        axisRotation[i].clear();
        axisRotation[i].data[3][3] = 1.0;

        // Construct a rotation matrix based on the rotation degree
        // value and the axis of rotation
        switch (axis[i])
        {
            case 0:
                // X-axis rotation matrix
                axisRotation[i].data[0][0] = 1.0;

                tempVal = cos(AT_DEG2RAD(axisDegrees[i]));
                axisRotation[i].data[1][1] = tempVal;
                axisRotation[i].data[2][2] = tempVal;

                tempVal = sin(AT_DEG2RAD(axisDegrees[i]));
                axisRotation[i].data[2][1] = tempVal;
                axisRotation[i].data[1][2] = -tempVal;

                break;
            case 1:
                // Y-axis rotation matrix
                axisRotation[i].data[1][1] = 1.0;

                tempVal = cos(AT_DEG2RAD(axisDegrees[i]));
                axisRotation[i].data[0][0] = tempVal;
                axisRotation[i].data[2][2] = tempVal;

                tempVal = sin(AT_DEG2RAD(axisDegrees[i]));
                axisRotation[i].data[0][2] = tempVal;
                axisRotation[i].data[2][0] = -tempVal;

                break;
            case 2:
                // Z-axis rotation matrix
                axisRotation[i][2][2] = 1.0;

                tempVal = cos(AT_DEG2RAD(axisDegrees[i]));
                axisRotation[i].data[0][0] = tempVal;
                axisRotation[i].data[1][1] = tempVal;

                tempVal = sin(AT_DEG2RAD(axisDegrees[i]));
                axisRotation[i].data[1][0] = tempVal;
                axisRotation[i].data[0][1] = -tempVal;

                break;
        }
    }

    // Combine the three separate rotations into a composite rotation
    // matrix. Also check for relative rotations; reverse the order of
    // the matrices if a relative axis order is specified.
    if ((axisOrder >= AT_EULER_ANGLES_XYZ_R) &&
        (axisOrder <= AT_EULER_ANGLES_ZYZ_R))
        (*this) = axisRotation[0] * axisRotation[1] * axisRotation[2];
    else
        (*this) = axisRotation[2] * axisRotation[1] * axisRotation[0];
}

// ------------------------------------------------------------------------
// Retreives the rotation indicated by this matrix as a set of three Euler
// angle rotations. The specified axis constant is used to determine the
// order of the reconstructed rotations.
//
// Note: NULL pointers may be passed in to denote unwanted return values
// ------------------------------------------------------------------------
void atMatrix::getEulerRotation(atMathEulerAxisOrder axisOrder,
    double *axis1Degrees, double *axis2Degrees, double *axis3Degrees) const
{
    // I hope this works...  I got the actual engine for this code
    // from some other site (Princeton, I think). It effectively
    // compresses all of the different axis combinations into
    // two different cases.
    
    bool isRepeat, isOdd;
    int i, j, k;
    double tempDouble;
    double result1, result2, result3;
    
    // First, determine all of the vital data for each axis combination
    switch (axisOrder)
    {
        case AT_EULER_ANGLES_XYZ_S:
        case AT_EULER_ANGLES_ZYX_R:
            i = 0;   j = 1;   k = 2;   isRepeat = 0;   isOdd = 0;
            break;
        case AT_EULER_ANGLES_XZY_S:
        case AT_EULER_ANGLES_YZX_R:
            i = 0;   j = 2;   k = 1;   isRepeat = 0;   isOdd = 1;
            break;
        case AT_EULER_ANGLES_YXZ_S:
        case AT_EULER_ANGLES_ZXY_R:
            i = 1;   j = 0;   k = 2;   isRepeat = 0;   isOdd = 1;
            break;
        case AT_EULER_ANGLES_YZX_S:
        case AT_EULER_ANGLES_XZY_R:
            i = 1;   j = 2;   k = 0;   isRepeat = 0;   isOdd = 0;
            break;
        case AT_EULER_ANGLES_ZXY_S:
        case AT_EULER_ANGLES_YXZ_R:
            i = 2;   j = 0;   k = 1;   isRepeat = 0;   isOdd = 0;
            break;
        case AT_EULER_ANGLES_ZYX_S:
        case AT_EULER_ANGLES_XYZ_R:
            i = 2;   j = 1;   k = 0;   isRepeat = 0;   isOdd = 1;
            break;
        case AT_EULER_ANGLES_XYX_S:
        case AT_EULER_ANGLES_XYX_R:
            i = 0;   j = 1;   k = 2;   isRepeat = 1;   isOdd = 0;
            break;
        case AT_EULER_ANGLES_XZX_S:
        case AT_EULER_ANGLES_XZX_R:
            i = 0;   j = 2;   k = 1;   isRepeat = 1;   isOdd = 1;
            break;
        case AT_EULER_ANGLES_YXY_S:
        case AT_EULER_ANGLES_YXY_R:
            i = 1;   j = 0;   k = 2;   isRepeat = 1;   isOdd = 1;
            break;
        case AT_EULER_ANGLES_YZY_S:
        case AT_EULER_ANGLES_YZY_R:
            i = 1;   j = 2;   k = 0;   isRepeat = 1;   isOdd = 0;
            break;
        case AT_EULER_ANGLES_ZXZ_S:
        case AT_EULER_ANGLES_ZXZ_R:
            i = 2;   j = 0;   k = 1;   isRepeat = 1;   isOdd = 0;
            break;
        case AT_EULER_ANGLES_ZYZ_S:
        case AT_EULER_ANGLES_ZYZ_R:
            i = 2;   j = 1;   k = 0;   isRepeat = 1;   isOdd = 1;
            break;
    }

    // Run the angle-finder algorithm
    double yVal;
    if (isRepeat)
    {
        // One axis was repeated
        yVal = sqrt(AT_SQR(data[i].getValue(j)) + AT_SQR(data[i].getValue(k)));
        if (yVal > 1E-6)
        {
            result1 = AT_RAD2DEG(atan2(data[i].getValue(j), data[i].getValue(k)));
            result2 = AT_RAD2DEG(atan2(yVal, data[i].getValue(i)));
            result3 = AT_RAD2DEG(atan2(data[j].getValue(i), -data[k].getValue(i)));
        }
        else
        {
            result1 = AT_RAD2DEG(atan2(-data[j].getValue(k), data[j].getValue(j)));
            result2 = AT_RAD2DEG(atan2(yVal, data[i].getValue(i)));
            result3 = AT_RAD2DEG(0.0);
        }
    }
    else
    {
        // Each axis used only once
        yVal = sqrt(AT_SQR(data[i].getValue(i)) + AT_SQR(data[j].getValue(i)));
        if (yVal > 1E-6)
        {
            result1 = AT_RAD2DEG(atan2(data[k].getValue(j), data[k].getValue(k)));
            result2 = AT_RAD2DEG(atan2(-data[k].getValue(i), yVal));
            result3 = AT_RAD2DEG(atan2(data[j].getValue(i), data[i].getValue(i)));
        }
        else
        {
            result1 = AT_RAD2DEG(atan2(-data[j].getValue(k), data[j].getValue(j)));
            result2 = AT_RAD2DEG(atan2(-data[k].getValue(i), yVal));
            result3 = AT_RAD2DEG(0.0);
        }
    }
    
    // Check for 'odd' axis ordering and negate the result if so
    if (isOdd)
    {
        result1 = -result1;
        result2 = -result2;
        result3 = -result3;
    }

    // Check for relative rotations; swap the first and third result if so
    if ((axisOrder >= AT_EULER_ANGLES_XYZ_R) &&
        (axisOrder <= AT_EULER_ANGLES_ZYZ_R))
    {
        tempDouble = result1;
        result1 = result3;
        result3 = tempDouble;
    }
    
    // Copy the desired results to the designated locations
    if (axis1Degrees)
        *axis1Degrees = result1;
    if (axis2Degrees)
        *axis2Degrees = result2;
    if (axis3Degrees)
        *axis3Degrees = result3;
}

// ------------------------------------------------------------------------
// Sets this matrix to a rotation matrix. The rotation is specified by a
// rotational quaternion.
// ------------------------------------------------------------------------
void atMatrix::setQuatRotation(const atQuat &quat)
{
    atQuat theQuat;
    double x, y, z, w;

    // Normalize the given quaternion and extract the values from it
    theQuat = quat.getNormalized();
    x = theQuat[0];
    y = theQuat[1];
    z = theQuat[2];
    w = theQuat[3];
    
    // Initialize the matrix with zeroes and set the homogeneous
    // coordinate scale to one
    clear();
    data[3][3] = 1.0;
    
    // Compute the rotation matrix; the formula for doing this should
    // be available from any decent source of information about
    // quaternions.
    data[0][0] = 1.0 - (2.0 * AT_SQR(y)) - (2.0 * AT_SQR(z));
    data[0][1] = (2.0 * x * y) - (2.0 * w * z);
    data[0][2] = (2.0 * x * z) + (2.0 * w * y);
    data[1][0] = (2.0 * x * y) + (2.0 * w * z);
    data[1][1] = 1.0 - (2.0 * AT_SQR(x)) - (2.0 * AT_SQR(z));
    data[1][2] = (2.0 * y * z) - (2.0 * w * x);
    data[2][0] = (2.0 * x * z) - (2.0 * w * y);
    data[2][1] = (2.0 * y * z) + (2.0 * w * x);
    data[2][2] = 1.0 - (2.0 * AT_SQR(x)) - (2.0 * AT_SQR(y));
}

// ------------------------------------------------------------------------
// Sets this matrix to a translation matrix
// ------------------------------------------------------------------------
void atMatrix::setTranslation(double dx, double dy, double dz)
{
    int i;
    
    // Initialize the matrix to an identity matrix
    clear();
    for (i = 0; i < 4; i++)
        data[i][i] = 1.0;

    // Copy the translation values to the translation entries in the
    // matrix
    data[0][3] = dx;
    data[1][3] = dy;
    data[2][3] = dz;
}

// ------------------------------------------------------------------------
// Gets the translation represented in this matrix. May include the effects
// of any scale matrices that have been multiplied into this matrix.
// ------------------------------------------------------------------------
void atMatrix::getTranslation(double *dx, double *dy, double *dz) const
{
    // The matrix's translation is stored in the top three elements of the
    // last column of the matrix. Simply return those three values without
    // any extra processing.
    if (dx)
        (*dx) = data[0][3];
    if (dy)
        (*dy) = data[1][3];
    if (dz)
        (*dz) = data[2][3];
}

// ------------------------------------------------------------------------
// Gets the translation represented in this matrix as a 3-component
// atVector. May include the effects of any scale matrices that have been
// multiplied into this matrix.
// ------------------------------------------------------------------------
atVector atMatrix::getTranslation() const
{
    // The matrix's translation is stored in the top three elements of the
    // last column of the matrix. Simply return those three values without
    // any extra processing.
    return atVector(data[0][3], data[1][3], data[2][3]);
}

// ------------------------------------------------------------------------
// Sets this matrix to a (not necessarily uniform) scaling matrix
// ------------------------------------------------------------------------
void atMatrix::setScale(double sx, double sy, double sz)
{
    // Clear the matrix, and set the values along the diagonal to the
    // desired scale values
    clear();
    data[0][0] = sx;
    data[1][1] = sy;
    data[2][2] = sz;
    data[3][3] = 1.0;
}

// ------------------------------------------------------------------------
// Attempts to get the scale represented in this matrix for each direction.
// May not return the correct answer if the matrix has been modified in any
// way except for multiplying together translations, rotations, and scales.
// ------------------------------------------------------------------------
void atMatrix::getScale(double *sx, double *sy, double *sz) const
{
    // Compute the scale for each direction by taking the three row vectors
    // in the matrix's upper-left 3x3 submatrix and calculating their
    // magnitudes.
    atVector tempVec;

    // For each desired value, copy the row from the matrix, strip off the
    // last value, and compute the magnitute of what's left
    if (sx)
    {
        tempVec = data[0];
        tempVec.setSize(3);
        (*sx) = tempVec.getMagnitude();
    }
    if (sy)
    {
        tempVec = data[1];
        tempVec.setSize(3);
        (*sy) = tempVec.getMagnitude();
    }
    if (sz)
    {
        tempVec = data[2];
        tempVec.setSize(3);
        (*sz) = tempVec.getMagnitude();
    }
}

// ------------------------------------------------------------------------
// Retrieves one row of the matrix as a atVector reference. Useful in
// conjunction with the atVector's operator[] to access one specific
// element of the matrix.
// ------------------------------------------------------------------------
atVector &atMatrix::operator[](int index)
{
    // Bounds checking
    if ((index < 0) || (index >= 4))
    {
        printf("atMatrix::operator[]: Invalid index\n");
        return data[0];
    }

    // Return the desired row
    return data[index];
}

// ------------------------------------------------------------------------
// Retrieves one row of the matrix as a atVector reference. Useful in
// conjunction with the atVector's operator[] to access one specific
// element of the matrix.
// ------------------------------------------------------------------------
const atVector &atMatrix::operator[](int index) const
{
    // Bounds checking
    if ((index < 0) || (index >= 4))
    {
        printf("atMatrix::operator[]: Invalid index\n");
        return data[0];
    }

    // Return the desired row
    return data[index];
}

// ------------------------------------------------------------------------
// Adds the specified matrix to this matrix, returning the result
// Equivalent to getSum(addend)
// ------------------------------------------------------------------------
atMatrix atMatrix::operator+(const atMatrix &addend) const
{
    int i;
    atMatrix result;

    // Create the target matrix by adding each row of this matrix
    // to each row of the addend matrix
    for (i = 0; i < 4; i++)
        result.data[i] = data[i] + addend.data[i];

    // Return the target matrix
    return result;
}

// ------------------------------------------------------------------------
// Subtracts the specified matrix from this matrix, returning the result
// Equivalent to getDifference(subtrahend)
// ------------------------------------------------------------------------
atMatrix atMatrix::operator-(const atMatrix &subtrahend) const
{
    int i;
    atMatrix result;

    // Create the target matrix by subtracting each row of the
    // subtrahend from each row of this matrix
    for (i = 0; i < 4; i++)
        result.data[i] = data[i] - subtrahend.data[i];

    // Return the target matrix
    return result;
}

// ------------------------------------------------------------------------
// Multiplies this matrix with the given matrix; the operand matrix is
// considered to be on the right. The result is returned.
// Equivalent to getPostMultiplied(operand)
// ------------------------------------------------------------------------
atMatrix atMatrix::operator*(const atMatrix &operand) const
{
    double temp[4];
    atMatrix result;
    atMatrix operandTranspose = operand.getTranspose();
    int i;

    // Do a matrix-multiply operation between this matrix and the operand
    // matrix, with this matrix first; place the results in the target
    // matrix.
    for (i = 0; i < 4; i++)
    {
        // Since the second matrix is transposed, each element of the
        // result can be computed by taking the dot product of the
        // appropriate rows of each matrix. This should be a lot faster
        // than accessing each element directly, as it avoids a good deal
        // of the error checking on the atVector data accessors.
        temp[0] = data[i].getDotProduct(operandTranspose.data[0]);
        temp[1] = data[i].getDotProduct(operandTranspose.data[1]);
        temp[2] = data[i].getDotProduct(operandTranspose.data[2]);
        temp[3] = data[i].getDotProduct(operandTranspose.data[3]);

        // Copy the results to the result matrix
        result[i].set(temp[0], temp[1], temp[2], temp[3]);
    }

    // Return the target matrix
    return result;
}

// ------------------------------------------------------------------------
// Adds the specified matrix to this matrix, keeping the result
// Equivalent to add(addend)
// ------------------------------------------------------------------------
void atMatrix::operator+=(const atMatrix &addend)
{
    int i;

    // Add each row of the addend matrix to this matrix
    for (i = 0; i < 4; i++)
        data[i] += addend.data[i];
}

// ------------------------------------------------------------------------
// Subtracts the specified matrix from this matrix, keeping the result
// Equivalent to subtract(subtrahend)
// ------------------------------------------------------------------------
void atMatrix::operator-=(const atMatrix &subtrahend)
{
    int i;

    // Subtract each row of the subtrahend matrix from this matrix
    for (i = 0; i < 4; i++)
        data[i] -= subtrahend.data[i];
}

// ------------------------------------------------------------------------
// Prints the specified row of the matrix to stdout
// ------------------------------------------------------------------------
void atMatrix::printRow(int rowNum) const
{
    printRow(rowNum, stdout);
}

// ------------------------------------------------------------------------
// Reads the specified row of the matrix from stdin
// ------------------------------------------------------------------------
void atMatrix::readRow(int rowNum)
{
    readRow(rowNum, stdin);
}

// ------------------------------------------------------------------------
// Prints the specified row of the matrix to the specfied file
// ------------------------------------------------------------------------
void atMatrix::printRow(int rowNum, FILE *fp) const
{
    // Make sure the row number is valid
    if ((rowNum < 0) || (rowNum >= 4))
    {
        printf("atMatrix::printRow:  Invalid row specified\n");
        return;
    }

    // Assume that the matrix is an affine transform matrix, which
    // generally doesn't have large numbers in it.  Doing so allows
    // us to have an idea how wide the matrix's columns will be.
    fprintf(fp, "%8.4lf %8.4lf %8.4lf %8.4lf", data[rowNum].getValue(0), 
        data[rowNum].getValue(1), data[rowNum].getValue(2),
        data[rowNum].getValue(3));
}

// ------------------------------------------------------------------------
// Reads the file into the specified row of the matrix
// ------------------------------------------------------------------------
void atMatrix::readRow(int rowNum, FILE *fp)
{
    double value0, value1, value2, value3;

    // Make sure the row number is valid
    if ((rowNum < 0) || (rowNum >= 4))
    {
        printf("atMatrix::readRow:  Invalid row specified\n");
        return;
    }

    // Read in the row from the file
    fscanf(fp, "%lf %lf %lf %lf", &value0, &value1, &value2, &value3);

    // Put the information into the matrix
    data[rowNum].setValue(0, value0);
    data[rowNum].setValue(1, value1);
    data[rowNum].setValue(2, value2);
    data[rowNum].setValue(3, value3);
}

// ------------------------------------------------------------------------
// Prints a representation of the matrix to stdout
// ------------------------------------------------------------------------
void atMatrix::print() const
{
    print(stdout);
}

// ------------------------------------------------------------------------
// Prints a representation of the matrix to the specfied file
// ------------------------------------------------------------------------
void atMatrix::print(FILE *fp) const
{
    int i;

    // Call printRow() for each of the four rows of the matrix.  Add a
    // newline after each row.
    for (i = 0; i < 4; i++)
    {
        printRow(i, fp);
        fprintf(fp, "\n");
    }
}

// ------------------------------------------------------------------------
// Reads in a representation of the matrix from stdin
// ------------------------------------------------------------------------
void atMatrix::read()
{
    read(stdin);
}

// ------------------------------------------------------------------------
// Reads in a representation of the matrix from the specfied file
// ------------------------------------------------------------------------
void atMatrix::read(FILE *fp)
{
    int i;

    // Call readRow() for each of the four rows of the matrix.
    for (i = 0; i < 4; i++)
        readRow(i, fp);
}

// ------------------------------------------------------------------------
// Checks for element-wise equality between two matricies. Two elements
// are considered equal if they are within a small default tolerance value.
// Equivalent to isEqual(operand)
// ------------------------------------------------------------------------
bool atMatrix::operator==(const atMatrix &operand) const
{
    int i;

    // Check each pair of row vectors (this object's and the operand's)
    // for almost-equality; return false if a pair doesn't match up.
    for (i = 0; i < 4; i++)
        if (!(data[i].isEqual(operand.data[i])))
            return false;

    // If all the pairs match, return true
    return true;
}

// ------------------------------------------------------------------------
// atItem-derived method.  Return true if the given item is an equivalent
// matrix
// ------------------------------------------------------------------------
bool atMatrix::equals(atItem * otherItem)
{
   atMatrix * otherMatrix;

   // Try to cast the other item to an atMatrix
   otherMatrix = dynamic_cast<atMatrix *>(otherItem);

   // Return true iff the other item is a matrix and is equivalent to
   // this matrix (within default tolerance)
   if ((otherMatrix != NULL) && (isEqual(*otherMatrix)))
      return true;
   else
      return false;
}

// ------------------------------------------------------------------------
// atItem-derived method.  Return an integer value indicating whether this
// matrix is less than (negative), equal to (zero), or greater than
// (positive) the other matrix.  Mainly used for sorted containers (like
// atMap).
// ------------------------------------------------------------------------
int atMatrix::compare(atItem * otherItem)
{
   atMatrix * otherMatrix;
   atMatrix   diffMat;
   atVector   diffVec;
   double     diffSum;

   // Try to cast the item to a vector
   otherMatrix = dynamic_cast<atMatrix *>(otherItem);

   // See if the other item is a vector
   if (otherMatrix != NULL)
   {
      // We'll use the element-wise difference of the two matrices to
      // determine "greater" or "less".  See the atVector::compare()
      // method for justification.
      
      // Subtract the two matrices element-wise
      diffMat = (*this) - (*otherMatrix);

      // Sum the rows of the difference matrix into a difference vector
      diffVec = diffMat[0] + diffMat[1] + diffMat[2] + diffMat[3];

      // Sum the elements of the difference vector
      diffSum = diffVec[0] + diffVec[1] + diffVec[2] + diffVec[3];

      // Use the difference sum to assign a comparison result
      if (diffSum > 0)
         return 1;
      else if (diffSum < 0)
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

