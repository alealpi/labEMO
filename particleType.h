#ifndef PARTICLETYPE_H
#define PARTICLETYPE_H

#include <iostream>


class ParticleType {

    const std::string fName;
    const double fMass;
    const int fCharge;

    public:
    
    const std::string getName() const;
    double getMass() const;
    double getCharge() const;
    virtual double getWidth() const;

    virtual void Print() const;

    ParticleType(std::string const& Name, const double Mass, const int Charge);

    


};
    






#endif