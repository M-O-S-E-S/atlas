
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


#ifndef AT_VECTOR_HPP
#define AT_VECTOR_HPP

class atVector;

#include <stdio.h>

#include "atGlobals.h++"
#include "atItem.h++"
#include "atOSDefs.h++"


class ATLAS_SYM atVector : public atItem
{
private:

    double      data[4];
    int         vecSize;

public:

                    // Default vector size is 4
                    atVector();
                    atVector(int size);
                    atVector(int size, double values[]);
                    atVector(double x, double y);
                    atVector(double x, double y, double z);
                    atVector(double x, double y, double z, double w);
                    ~atVector();

    void            set(int size, double values[]);
    void            set(double x, double y);
    void            set(double x, double y, double z);
    void            set(double x, double y, double z, double w);
    void            copy(const atVector &source);
    
    void            clear();
    void            clearCopy(const atVector &source);

    void            setSize(int size);
    int             getSize() const;
    void            setValue(int index, double value);
    double          getValue(int index) const;
    bool            isEqual(const atVector &operand) const;
    bool            isAlmostEqual(const atVector &operand, 
                                  double tolerance) const;

    void            add(const atVector &addend);
    atVector        getSum(const atVector &addend) const;
    void            subtract(const atVector &subtrahend);
    atVector        getDifference(const atVector &subtrahend) const;
    void            scale(double multiplier);
    atVector        getScaled(double multiplier) const;
    
    double          getMagnitude() const;
    double          getMagnitudeSquared() const;
    double          getDotProduct(const atVector &operand) const;
    void            normalize();
    atVector        getNormalized() const;
    void            crossProduct(const atVector &operand);
    atVector        getCrossProduct(const atVector &operand) const;
    double          getAngleBetween(const atVector &endVector) const;

    double          &operator[](int index);
    const double    &operator[](int index) const;
    atVector        operator+(const atVector &addend) const;
    atVector        operator-(const atVector &subtrahend) const;
    atVector        operator*(double multiplier) const;
    void            operator+=(const atVector &addend);
    void            operator-=(const atVector &subtrahend);
    void            operator*=(double multiplier);
    bool            operator==(const atVector &operand) const;
    bool            operator!=(const atVector &operand) const;

    void            print() const;
    void            print(FILE *fp) const;

    virtual bool    equals(atItem * otherItem);
    virtual int     compare(atItem * otherItem);
};

ATLAS_SYM atVector operator*(double multiplier, atVector operand);

#endif
