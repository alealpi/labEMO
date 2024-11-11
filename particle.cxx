#include "particle.h"


ParticleType* Particle::fParticleType[Particle::fMaxNumParticleType];
int Particle::fNParticleType = 0;


int Particle::FindParticle(std::string const& ParticleName) {
    int i = 0;
    for (; i < fMaxNumParticleType; ++i)
    {
        if (fParticleType[i] != nullptr && fParticleType[i]->getName() == ParticleName)
        {
            return i;
        }
    }
    return -1;
}

Particle::Particle(std::string const& Name, double Px = 0., double Py = 0., double Pz = 0.) {
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


void Particle::AddParticleType(std::string const& name, double mass, int charge, double width) {

    if (fNParticleType >= fMaxNumParticleType) {
        std::cout << "Massimo numero di tipi di particelle raggiunto" << '\n';
    }

    if (FindParticle(name) != -1) {
        std::cout << "Tipo di particella già esistente" << '\n';  // vedere se fa danni o no
    } else {
        if (width == 0.) {
            fParticleType[fNParticleType] = new ParticleType(name, mass, charge);   // controllare posizione [...]
            // e.g. ho una particella nell'array che è in posizione 0: la seconda particella la vorrò mettere in posizione 1, e 1=fNParticleType
        } else {
            fParticleType[fNParticleType] = new ResonanceType(name, mass, charge, width);   // controllare posizione [...]
            // stessa cosa di sopra
        }
        ++fNParticleType;
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


int Particle::Decay2body(Particle &dau1,Particle &dau2) const {
  if(getMass() == 0.0){
    printf("Decayment cannot be preformed if mass is zero\n");
    return 1;
  }
  
  double massMot = getMass();
  double massDau1 = dau1.getMass();
  double massDau2 = dau2.getMass();

  if(fIndex > -1){ // add width effect

    // gaussian random numbers

    float x1, x2, w, y1;
    
    double invnum = 1./RAND_MAX;
    do {
      x1 = 2.0 * rand()*invnum - 1.0;
      x2 = 2.0 * rand()*invnum - 1.0;
      w = x1 * x1 + x2 * x2;
    } while ( w >= 1.0 );
    
    w = sqrt( (-2.0 * log( w ) ) / w );
    y1 = x1 * w;

    massMot += fParticleType[fIndex]->getWidth() * y1;

  }

  if(massMot < massDau1 + massDau2){
    printf("Decayment cannot be preformed because mass is too low in this channel\n");
    return 2;
  }
  
  double pout = sqrt((massMot*massMot - (massDau1+massDau2)*(massDau1+massDau2))*(massMot*massMot - (massDau1-massDau2)*(massDau1-massDau2)))/massMot*0.5;

  double norm = 2*M_PI/RAND_MAX;

  double phi = rand()*norm;
  double theta = rand()*norm*0.5 - M_PI/2.;
  dau1.setP(pout*sin(theta)*cos(phi),pout*sin(theta)*sin(phi),pout*cos(theta));
  dau2.setP(-pout*sin(theta)*cos(phi),-pout*sin(theta)*sin(phi),-pout*cos(theta));

  double energy = sqrt(fP.x*fP.x + fP.y*fP.y + fP.z*fP.z + massMot*massMot);

  double bx = fP.x/energy;
  double by = fP.y/energy;
  double bz = fP.z/energy;

  dau1.Boost(bx,by,bz);
  dau2.Boost(bx,by,bz);

  return 0;
}


void Particle::Boost(double bx, double by, double bz)
{

  double energy = getEnergy();

  //Boost this Lorentz vector
  double b2 = bx*bx + by*by + bz*bz;
  double gamma = 1.0 / sqrt(1.0 - b2);
  double bp = bx*fP.x + by*fP.y + bz*fP.z;
  double gamma2 = b2 > 0 ? (gamma - 1.0)/b2 : 0.0;

  fP.x += gamma2*bp*bx + gamma*bx*energy;
  fP.y += gamma2*bp*by + gamma*by*energy;
  fP.z += gamma2*bp*bz + gamma*bz*energy;
}
