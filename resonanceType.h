#ifndef RESONANCETYPE.H
#define RESONANCETYPE.H

#include "ParticleType.h"


class ResonanceType : public ParticleType {

    const double fWidth;

    public:

    double getWidth() const;

    void Print() const {}

    ResonanceType(char* Name, double Mass, int Charge, double Width);
    
};




#endif