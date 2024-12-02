#include "resonanceType.h"

ResonanceType::ResonanceType(std::string const &Name, const double Mass, const int Charge, const double Width) : ParticleType(Name, Mass, Charge), fWidth{Width} {}

double ResonanceType::getWidth() const { return fWidth; }

void ResonanceType::Print() const
{
    ParticleType::Print();
    std::cout << "Width : " << fWidth << '\n';
}
