#include "ParticleType.h"


const char* ParticleType::getName() const { return fName; }
    
double ParticleType::getMass() const { return fMass; }

double ParticleType::getCharge() const { return fCharge; }

void ParticleType::Print() const {
    std::cout << "Name : " << fName << '\n';
    std::cout << "Mass : " << fMass << '\n';
    std::cout << "Charge : " << fCharge << '\n'; 
}


ParticleType::ParticleType(const char* Name, const double Mass, const int Charge) : fName{Name}, fMass{Mass}, fCharge{Charge} {}