#ifndef PARTICLE_H
#define PARTICLE_H

#include <cmath>
#include <cstdlib>
#include "resonanceType.h"

struct Momentum
{
    double x;
    double y;
    double z;
};

class Particle
{
    static const int fMaxNumParticleType = 10;
    static ParticleType *fParticleType[];
    static int fNParticleType;
    int fIndex;
    Momentum fP;
    static int FindParticle(std::string const &ParticleName);
    void Boost(double bx, double by, double bz);

public:
    // constructors
    Particle(std::string const &Name, double Px, double Py, double Pz);
    Particle() {}

    // getters
    int getIndex() const;
    static auto getParticleTypeArray() { return fParticleType; }
    Momentum getMomentum() const;
    double getMass() const;
    double getEnergy() const;
    double getInvMass(Particle const &p2) const;

    // setters
    void setIndex(int i);
    void setIndex(std::string const &name);
    void setP(double Px, double Py, double Pz);

    // static methods
    static void AddParticleType(std::string const &name, double mass, int charge, double width);
    static void PrintParticleType();

    void PrintParticleInfo() const;

    int Decay2body(Particle &dau1, Particle &dau2) const;
};

#endif