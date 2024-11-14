#include "particleType.h"
#include "resonanceType.h"
#include "particle.h"

#include "TRandom.h"
#include "TFile.h"
#include "TMath.h"
#include "TH1.h"
#include "TBenchmark.h"




void Main() {

    R__LOAD_LIBRARY(particleType_cxx.so);
    R__LOAD_LIBRARY(resonanceType_cxx.so);
    R__LOAD_LIBRARY(particle_cxx.so);

    gBenchmark->Start("Benchmark");


    Particle::AddParticleType("pione+", 0.13957, +1, 0.);
    Particle::AddParticleType("pione-", 0.13957, -1, 0.);
    Particle::AddParticleType("kaone+", 0.49367, +1, 0.);
    Particle::AddParticleType("kaone-", 0.49367, -1, 0.);
    Particle::AddParticleType("protone+", 0.93827, +1, 0.);
    Particle::AddParticleType("protone-", 0.93827, -1, 0.);
    Particle::AddParticleType("K0", 0.89166, 0, 0.050);

    gRandom->SetSeed();


    const int N = 120;
    Particle EventParticles[N];


    TH1I* particleType_h = new TH1I("particleType_h", "Particle types distribution", 7, 0, 7);
    TH1F* azimutAngle_h = new TH1F("azimutAngle_h", "Azimut angle distribution", 300, 0., 2* TMath::Pi());
    TH1F* polarAngle_h = new TH1F("polarAngle_h", "Polar angle distribution", 150, 0., TMath::Pi());
    TH1F* momentum_h = new TH1F("momentum_h", "Momentum distribution", 200, 0., 10.);
    TH1F* transverseMomentum_h = new TH1F("transverseMomentum_h", "Transverse momentum distribution", 200, 0., 10.);
    TH1F* energy_h = new TH1F("energy_h", "Energy distribution", 200, 0., 5.);
    TH1F* invMassAll_h = new TH1F("invMassAll_h", "Inv. mass for all particles", 600, 0., 5.);
    invMassAll_h->Sumw2();
    TH1F* invMassSameCharge_h = new TH1F("invMassSameCharge_h", "Inv. mass for particles with same charge", 7000, 0., 5.);
    invMassSameCharge_h->Sumw2();
    TH1F* invMassOppositeCharge_h = new TH1F("invMassOppositeCharge_h", "Inv. mass for particles with opposite charge", 7000, 0., 5.);
    invMassOppositeCharge_h->Sumw2();
    TH1F* invMassPionKaonSameCharge_h = new TH1F("invMassPionKaonSameCharge_h", "Inv. mass for pions and kaons with same charge", 7000, 0., 5.);
    invMassPionKaonSameCharge_h->Sumw2();
    TH1F* invMassPionKaonOppositeCharge_h = new TH1F("invMassPionKaonOppositeCharge_h", "Inv. mass for pions and kaons with opposite charge", 7000, 0., 5.);
    invMassPionKaonOppositeCharge_h->Sumw2();
    TH1F* invMassDecayed_h = new TH1F("invMassDecayed_h", "Inv. mass for particles decayed from K0", 7000, 0., 2.);
    invMassDecayed_h->Sumw2();

    
    

    for (int i = 1; i <= 1E5; ++i) {

        int NK0 = 0;  // numero di particelle K0 presenti nell'array EventParticles

        
        for (int j = 0; j < 100; ++j) {
          
            double phi = gRandom->Uniform(0., 2 * TMath::Pi());
            double theta = gRandom->Uniform(0., TMath::Pi());
            double P = gRandom->Exp(1.);

            EventParticles[j].setP(P * TMath::Sin(theta) * TMath::Cos(phi), P * TMath::Sin(theta) * TMath::Sin(phi), P * TMath::Cos(theta));

            double x = gRandom->Rndm();
            if (x <= 0.4) {
                EventParticles[j].setIndex("pione+");
            } else if (x <= 0.8) {
                EventParticles[j].setIndex("pione-");
            } else if (x <= 0.85) {
                EventParticles[j].setIndex("kaone+");
            } else if (x <= 0.9) {
                EventParticles[j].setIndex("kaone-");
            } else if (x <= 0.945) {
                EventParticles[j].setIndex("protone+");
            } else if (x <= 0.99) {
                EventParticles[j].setIndex("protone-");
            } else {
                EventParticles[j].setIndex("K0");


                double decayInto = gRandom->Rndm();
                Particle decayProduct1{};
                Particle decayProduct2{};
                if (decayInto <= 0.5) {
                    decayProduct1.setIndex("pione+");
                    decayProduct2.setIndex("kaone-");
                } else {
                    decayProduct1.setIndex("pione-");
                    decayProduct2.setIndex("kaone+");
                }

                EventParticles[j].Decay2body(decayProduct1, decayProduct2);
                EventParticles[100 + NK0 * 2] = decayProduct1;
                EventParticles[100 + NK0 * 2 + 1] = decayProduct2;

                double invMassDecayed = (EventParticles[100 + NK0 * 2]).getInvMass(EventParticles[100 + NK0 * 2 + 1]);
                invMassDecayed_h->Fill(invMassDecayed);    

                ++NK0;
            }


            auto p = EventParticles[j];
            particleType_h->Fill(p.getIndex());
            azimutAngle_h->Fill(phi);
            polarAngle_h->Fill(theta);
            momentum_h->Fill(P);
            transverseMomentum_h->Fill(std::sqrt(p.getMomentum().x * p.getMomentum().x + p.getMomentum().y * p.getMomentum().y));
            energy_h->Fill(p.getEnergy());
        

        }


        for (int k = 0; k < 100 + 2 * NK0 - 1; ++k) {

            for (int l = k + 1; l < 100 + 2 * NK0; ++l) {
                
                Particle p1 = EventParticles[k];
                ParticleType* p1_properties = p1.getParticleTypeArray()[p1.getIndex()];
                double q1 = p1_properties->getCharge();

                Particle p2 = EventParticles[l];
                ParticleType* p2_properties = p2.getParticleTypeArray()[p2.getIndex()];
                double q2 = p2_properties->getCharge();


                if (p1_properties->getName() != "K0" && p2_properties->getName() != "K0") {

                    double invMass = p1.getInvMass(p2);
                    invMassAll_h->Fill(invMass);

                    if (q1 == q2) {
                        invMassSameCharge_h->Fill(invMass);
                    }
                        /*if ((p1_properties->getName() == "pione+" && p2_properties->getName() == "kaone+") || 
                            (p1_properties->getName() == "pione-" && p2_properties->getName() == "kaone-") || 
                            (p1_properties->getName() == "kaone+" && p2_properties->getName() == "pione+") || 
                            (p1_properties->getName() == "kaone-" && p2_properties->getName() == "pione-")) 
                        {
                            invMassPionKaonSameCharge_h->Fill(invMass);
                        }*/


                    if (q1 != q2) {
                        invMassOppositeCharge_h->Fill(invMass);

                        /*if ((p1_properties->getName() == "pione+" && p2_properties->getName() == "kaone-") || 
                            (p1_properties->getName() == "pione-" && p2_properties->getName() == "kaone+") || 
                            (p1_properties->getName() == "kaone+" && p2_properties->getName() == "pione-") || 
                            (p1_properties->getName() == "kaone-" && p2_properties->getName() == "pione+")) 
                        {
                            invMassPionKaonOppositeCharge_h->Fill(invMass);
                        }*/
                    }

                    if ((p1_properties->getName() == "pione+" && p2_properties->getName() == "kaone-") || 
                        (p1_properties->getName() == "pione-" && p2_properties->getName() == "kaone+")) 
                    {
                        invMassPionKaonOppositeCharge_h->Fill(invMass);
                    }


                    if ((p1_properties->getName() == "pione+" && p2_properties->getName() == "kaone+") || 
                        (p1_properties->getName() == "pione-" && p2_properties->getName() == "kaone-")) 
                    {
                        invMassPionKaonSameCharge_h->Fill(invMass);
                    }



                }
            }

            

            
        }

        /*if (NK0 != 0) {
            for (int k = 100; k < 100 + 2 * NK0; k = k + 2) {
                Particle p1 = EventParticles[k];
                Particle p2 = EventParticles[k + 1];

                double invMass = p1.getInvMass(p2);
                invMassDecayed_h->Fill(invMass);
            }


        }*/




    }


    TFile* file = new TFile("results.root", "RECREATE");

    particleType_h->Write();
    azimutAngle_h->Write();
    polarAngle_h->Write();
    momentum_h->Write();
    transverseMomentum_h->Write();
    energy_h->Write();
    invMassAll_h->Write();
    invMassSameCharge_h->Write();
    invMassOppositeCharge_h->Write();
    invMassPionKaonSameCharge_h->Write();
    invMassPionKaonOppositeCharge_h->Write();
    invMassDecayed_h->Write();

    file->Close();

    gBenchmark->Show("Benchmark");

}