#ifndef RESONANCETYPE_H
#define RESONANCETYPE_H

#include "particleType.h"

class ResonanceType : public ParticleType
{
    const double fWidth;

public:
    ResonanceType(std::string const &Name, const double Mass, const int Charge, const double Width);

    double getWidth() const;

    void Print() const;
};

#endif