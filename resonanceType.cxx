#include "ResonanceType.h"



double ResonanceType::getWidth() const { return fWidth; }


void ResonanceType::Print() const {
    ParticleType::Print();
    std::cout << "Width : " << fWidth << '\n';
}


ResonanceType::ResonanceType(char* Name, double Mass, int Charge, double Width) : ParticleType(Name, Mass, Charge), fWidth{Width} {} 