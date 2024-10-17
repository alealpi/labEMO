#include "Particle.h"




int Particle::FindParticle(char *ParticleName) {
    int i = 0;
    for (; i < fMaxNumParticleType; ++i)
    {
        if (fParticleType[i]->getName() == ParticleName)
        {
            return i;
        }
    }
    return -1;
}

Particle::Particle(char *Name, double Px = 0., double Py = 0., double Pz = 0.) {
    fP.x = Px;
    fP.y = Py;
    fP.z = Pz;

    fIndex = FindParticle(Name);
    if (fIndex == -1)
    {
        std::cout << "Particella non trovata" << '\n';
    }
}


int Particle::getIndex() const { return fIndex; }


void Particle::AddParticleType(char* name, double mass, int charge, double width = 0.) {

    if (fNParticleType >= fMaxNumParticleType) {
        std::cout << "Massimo numero di tipi di particelle raggiunto" << '\n';
    }

    if (FindParticle(name) != -1) {
        std::cout << "Tipo di particella già esistente" << '\n';  // vedere se fa danni o no
    } else {
        if (width == 0.) {
            fParticleType[fNParticleType] = new ParticleType(name, mass, charge);   // controllare posizione [...]
        } else {
            fParticleType[fNParticleType] = new ResonanceType(name, mass, charge, width);   // controllare posizione [...]
        }
    }
}



void Particle::PrintParticleType() {                   // è static?
    for (int i = 0; i < fNParticleType; ++i) {
        fParticleType[i]->Print();
    }
}



void Particle::PrintParticleInfo() const {
    std::cout << "Indice : " << fIndex << '\n';
    std::cout << "Nome : " << fParticleType[fIndex]->getName() << '\n';
    std::cout << "Impulso : (" << fP.x << ", " << fP.y << ", " << fP.z << ")" << '\n';
}


Momentum Particle::getMomentum() const { return fP; }

double Particle::getMass() const {
    return fParticleType[fIndex]->getMass();
}

double Particle::getEnergy() const {
    return std::sqrt(getMass() * getMass() + fP.x * fP.x + fP.y * fP.y + fP.z * fP.z);
}

double Particle::getInvMass(Particle const& p2) const {
    auto P2 = p2.getMomentum();
    return std::sqrt(std::pow(getEnergy() + p2.getEnergy(), 2) - std::pow(fP.x + P2.x, 2) - std::pow(fP.y + P2.y, 2) - std::pow(fP.z + P2.z, 2));
}


void Particle::setP(double Px, double Py, double Pz) {
    fP.x = Px;
    fP.y = Py;
    fP.z = Pz;
}