
#ifndef AT_VECTOR_HPP
#define AT_VECTOR_HPP

class atVector;

#include <stdio.h>

#include "atGlobals.h++"
#include "atItem.h++"
#include "atOSDefs.h"


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

    void            print() const;
    void            print(FILE *fp) const;

    virtual bool    equals(atItem * otherItem);
    virtual int     compare(atItem * otherItem);
};

ATLAS_SYM atVector operator*(double multiplier, atVector operand);

#endif
