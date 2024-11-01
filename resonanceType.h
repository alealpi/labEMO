#ifndef RESONANCETYPE_H
#define RESONANCETYPE_H

#include "particleType.h"


class ResonanceType : public ParticleType {

    const double fWidth;

    public:

    double getWidth() const;

    void Print() const;

    ResonanceType(const char* Name, const double Mass, const int Charge, const double Width);
    
};




#endif