#ifndef PARTICLE_H
#define PARTICLE_H

#include <cmath>
#include <cstdlib>
#include "resonanceType.h"


struct Momentum {
    double x;
    double y;
    double z;
};



class Particle {

    static const int fMaxNumParticleType = 10;
    static ParticleType* fParticleType[];
    static int fNParticleType;
    int fIndex;
    Momentum fP;

    static int FindParticle(std::string const& ParticleName);

    void Boost(double bx, double by, double bz);


    public:

    Particle(std::string const& Name, double Px, double Py, double Pz);

    Particle() {}

    int getIndex() const;

    static auto getParticleTypeArray() { return fParticleType; }

    static void AddParticleType(std::string const& name, double mass, int charge, double width);

    

    //////////////////// CONTROLLARE ///////////////////////

    void setIndex(int i) {
        if (i >= 0 && i <= fNParticleType - 1) {
            fIndex = i;
        } else {
            std::cout << "Impossibile settare l'indice: tipo di particella non trovata" << '\n';
        }
    }

    void setIndex(std::string const& name) {
        int i = FindParticle(name);
        if (i == -1) {
            std::cout << "Impossibile settare l'indice: tipo di particella non trovata" << '\n';
        } else {
            fIndex = i;
        }
    }
    
    ////////////////////////////////////////////////////////



    static void PrintParticleType();                   // è static?

    void PrintParticleInfo() const;

    Momentum getMomentum() const;

    double getMass() const;

    double getEnergy() const;

    double getInvMass(Particle const& p2) const;

    void setP(double Px, double Py, double Pz);

    int Decay2body(Particle &dau1,Particle &dau2) const;    






    
};




#endif