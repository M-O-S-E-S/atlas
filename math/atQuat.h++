
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


#ifndef AT_QUAT_HPP
#define AT_QUAT_HPP

class atQuat;

#include <stdio.h>

#include "atGlobals.h++"
#include "atItem.h++"
#include "atMatrix.h++"
#include "atOSDefs.h++"
#include "atVector.h++"


class ATLAS_SYM atQuat : public atItem
{
private:

    // Quaternion data is represented with vector portion first
    // [x, y, z, w] => [V, w]
    double      data[4];

public:

                   atQuat();
                   atQuat(double x, double y, double z, double w);
                   atQuat(const double values[]);
                   ~atQuat();

    void           set(double x, double y, double z, double w);
    void           set(const double values[]);
    void           copy(const atQuat &source);
    void           clear();
    
    void           setValue(int index, double value);
    double         getValue(int index) const;
    bool           isEqual(const atQuat &operand) const;
    bool           isAlmostEqual(const atQuat &operand,
                                 double tolerance) const;

    void           add(const atQuat &addend);
    atQuat         getSum(const atQuat &addend) const;
    void           subtract(const atQuat &subtrahend);
    atQuat         getDifference(const atQuat &subtrahend) const;
    void           scale(double multiplier);
    atQuat         getScaled(double multiplier) const;
    void           multiplyQuat(const atQuat &operand);
    atQuat         getMultipliedQuat(const atQuat &operand) const;
    
    double         getMagnitude() const;
    double         getMagnitudeSquared() const;
    double         getDotProduct(const atQuat &operand) const;
    atQuat         getNormalized() const;
    void           normalize();
    void           conjugate();
    atQuat         getConjugate() const;
    void           invert();
    atQuat         getInverse() const;
    
    void           setMatrixRotation(const atMatrix &theMatrix);
    void           setEulerRotation(atMathEulerAxisOrder axisOrder,
                       double axis1Degrees, double axis2Degrees,
                       double axis3Degrees);
    void           getEulerRotation(atMathEulerAxisOrder axisOrder,
                       double *axis1Degrees, double *axis2Degrees,
                       double *axis3Degrees) const;
    void           setAxisAngleRotation(double x, double y, double z,
                       double rotDegrees);
    void           getAxisAngleRotation(double *x, double *y, double *z,
                       double *rotDegrees) const;
    void           setVecsRotation(const atVector &originForward,
                       const atVector &originUp,
                       const atVector &targetForward,
                       const atVector &targetUp);

    atQuat         getDecomposition(const atVector &targetAxis) const;

    double         getRotationDifference(const atQuat &targetRotation) const;

    atVector       rotatePoint(const atVector &targetPoint) const;

    atQuat         slerp(const atQuat &destination, double parameter) const;
    atQuat         nlerp(const atQuat &destination, double parameter) const;
    
    double         &operator[](int index);
    const double   &operator[](int index) const;
    atQuat         operator+(const atQuat &addend) const;
    atQuat         operator-(const atQuat &subtrahend) const;
    atQuat         operator*(const atQuat &operand) const;
    void           operator+=(const atQuat &addend);
    void           operator-=(const atQuat &subtrahend);
    void           operator*=(const atQuat &operand);
    bool           operator==(const atQuat &operand) const;

    void           print() const;
    void           print(FILE *fp) const;

    virtual bool   equals(atItem * otherItem);
    virtual int    compare(atItem * otherItem);
};

ATLAS_SYM atQuat operator*(double multiplier, atQuat operand);

#endif
