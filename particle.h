#ifndef PARTICLE.H
#define PARTICLE.H

#include <cmath>
#include "ResonanceType.h"


struct Momentum {
    double x;
    double y;
    double z;
};



class Particle {

    static const int fMaxNumParticleType = 10;
    static ParticleType* fParticleType[fMaxNumParticleType];
    static int fNParticleType;
    int fIndex;
    Momentum fP;

    static int FindParticle(char* ParticleName);


    public:

    Particle(char* Name, double Px, double Py, double Pz);

    int getIndex() const;

    static void AddParticleType(char* name, double mass, int charge, double width);

    /*
    void setIndex(int i) {
        if (i == ??)
        fIndex = i;
    }

    void setIndex(char* name) {}
    */
    // COSA SIGNIFICA SET INDEX?



    static void PrintParticleType();                   // è static?

    void PrintParticleInfo() const;

    Momentum getMomentum() const;

    double getMass() const;

    double getEnergy() const;

    double getInvMass(Particle const& p2) const;

    void setP(double Px, double Py, double Pz);
    
};




#endif