#ifndef PARTICLETYPE_H
#define PARTICLETYPE_H

#include <iostream>


class ParticleType {

    const char* fName;
    const double fMass;
    const int fCharge;

    public:
    
    const char* getName() const;
    double getMass() const;
    double getCharge() const;
    virtual double getWidth() const;

    virtual void Print() const;

    ParticleType(const char* Name, const double Mass, const int Charge);

    


};
    






#endif