#include "TFile.h"
#include "TH1.h"
#include "TF1.h"
#include "TMath.h"
#include "TCanvas.h"
#include <iostream>



void analyse() {

    TFile* file = new TFile("results_file");


    TCanvas* c1 = new TCanvas;

    TH1* histoArray[12];
    histoArray[0] = (TH1*)file->Get("particleType_h");
    histoArray[1] = (TH1*)file->Get("azimutAngle_h");
    histoArray[2] = (TH1*)file->Get("polarAngle_h");
    histoArray[3] = (TH1*)file->Get("momentum_h");
    histoArray[4] = (TH1*)file->Get("transverseMomentum_h");
    histoArray[5] = (TH1*)file->Get("energy_h");
    histoArray[6] = (TH1*)file->Get("invMassAll_h");
    histoArray[7] = (TH1*)file->Get("invMassSameCharge_h");
    histoArray[8] = (TH1*)file->Get("invMassOppositeCharge_h");
    histoArray[9] = (TH1*)file->Get("invMassPionKaonSameCharge_h");
    histoArray[10] = (TH1*)file->Get("invMassPionKaonOppositeCharge_h");
    histoArray[11] = (TH1*)file->Get("invMassDecayed_h");


    Double_t expectedEntries[12] = {1e7, 1e7, 1e7, 1e7, 1e7, 1e7, 5e8, 2.5e8, 2.5e8, 4.45e7, 4.45e7, 1e5};
    std::cout << "CHECKING ENTRIES:" << '\n';
    for (int i = 0; i < 12; ++i) {
        auto title = histoArray[i]->GetTitle();
        auto entries = histoArray[i]->GetEntries();
        
        std::cout << title << ": expected " << expectedEntries[i] << ", got " << entries << '\n';
    }



    std::string particleNames[7] = {"pione+", "pione-", "kaone+", "kaone-", "protone+", "protone-", "K0"};
    Double_t expectedTypes[7] = {4e6, 4e6, 5e5, 5e5, 4.5e5, 4.5e5, 1e5};
    std::cout << "\nCHECKING PARTICLE TYPES OCCURRENCES:" << '\n';
    for (int i = 0; i < 7; ++i) {
        auto occurrences = histoArray[0]->GetBinContent(i + 1);
        auto error = histoArray[0]->GetBinError(i + 1);

        std::cout << "Particle " << particleNames[i] << " occurrences: expected " << expectedTypes[i] << ", got " << occurrences << " +/- " << error << '\n';
    }





    TF1* azimutalFit = new TF1("azimutalFit", "[0]", 0., 2*TMath::Pi());
    TF1* polarFit = new TF1("polarFit", "[0]", 0., TMath::Pi());
    histoArray[1]->Fit(azimutalFit, "Q");
    histoArray[2]->Fit(polarFit, "Q");
    std::cout << "\nAZIMUTAL ANGLE DISTRIBUTION FIT: " << '\n';
    std::cout << "Parameter 0: " << azimutalFit->GetParameter(0) << " +/- " << azimutalFit->GetParError(0) << '\n';
    std::cout << "Chi square / NDF: " << azimutalFit->GetChisquare() / azimutalFit->GetNDF() << '\n';
    std::cout << "Probability of the fit: " << azimutalFit->GetProb() << '\n';
    std::cout << "\nPOLAR ANGLE DISTRIBUTION FIT: " << '\n';
    std::cout << "Parameter 0: " << polarFit->GetParameter(0) << " +/- " << polarFit->GetParError(0) << '\n';
    std::cout << "Chi square / NDF: " << polarFit->GetChisquare() / polarFit->GetNDF() << '\n';
    std::cout << "Probability of the fit: " << polarFit->GetProb() << '\n';



    TF1* momentumFit = new TF1("momentumFit", "[0] * TMath::Exp(-x / [1])", 0., 9.);
    momentumFit->SetParameter(0, 9e4);
    momentumFit->SetParameter(1, 1.);
    histoArray[3]->Fit(momentumFit);
    std::cout << "\nMOMENTUM DISTRIBUTION:" << '\n';
    std::cout << "Mean: expected 1., got " << histoArray[3]->GetMean() << " +/- " << histoArray[3]->GetMeanError() << '\n'; 
    std::cout << "MOMENTUM DISTRIBUTION FIT:" << '\n';
    std::cout << "Parameter 0: " << momentumFit->GetParameter(0) << " +/- " << momentumFit->GetParError(0) << '\n';
    std::cout << "Parameter 1: " << momentumFit->GetParameter(1) << " +/- " << momentumFit->GetParError(1) << '\n';
    std::cout << "Chi square / NDF: " << momentumFit->GetChisquare() / momentumFit->GetNDF() << '\n';
    std::cout << "Probability of the fit: " << momentumFit->GetProb() << '\n';



    // ISTOGRAMMI MASSA INVARIANTE

    TF1* invMassAllFit = new TF1("invMassAllFit", "[0] * TMath::Exp(-(x - [1]) * (x - [1]) / (2 * [2] * [2]))", 0.7, 1.1);
    TF1* invMassPionKaonFit = new TF1("invMassPionKaonFit", "[0] * TMath::Exp(-(x - [1]) * (x - [1]) / (2 * [2] * [2]))", 0.7, 1.1);
    TF1* invMassDecayedFit = new TF1("invMassDecayedFit", "[0] * TMath::Exp(-(x - [1]) * (x - [1]) / (2 * [2] * [2]))", 0.7, 1.1);


    TH1* invMassSameCharge_h = histoArray[7];
    TH1* invMassOppositeCharge_h = histoArray[8];
    TH1* invMassPionKaonSameCharge_h = histoArray[9];
    TH1* invMassPionKaonOppositeCharge_h = histoArray[10];
    TH1* invMassDecayed_h = histoArray[11];

    TH1F* invMassSubtractionAll_h = new TH1F(*(TH1F*)invMassOppositeCharge_h);
    invMassSubtractionAll_h->Add(invMassSubtractionAll_h, invMassSameCharge_h, 1, -1);
    TH1F* invMassSubtractionPionKaon_h = new TH1F(*(TH1F*)invMassPionKaonOppositeCharge_h);
    invMassSubtractionPionKaon_h->Add(invMassSubtractionPionKaon_h, invMassPionKaonSameCharge_h, 1, -1);

    invMassAllFit->SetParameters(7.996, 0.8919, 0.04989);
    invMassPionKaonFit->SetParameters(7.996, 0.8919, 0.04989);
    invMassDecayedFit->SetParameters(500, 0.9, 0.05);

    invMassSubtractionAll_h->Fit(invMassAllFit);
    invMassSubtractionPionKaon_h->Fit(invMassPionKaonFit);
    invMassDecayed_h->Fit(invMassDecayedFit);

    std::cout << "\nINVARIANT MASS BETWEEN ALL PARTICLES DISTRIBUTION:" << '\n';
    std::cout << "Parameter 0: " << invMassAllFit->GetParameter(0) << " +/- " << invMassAllFit->GetParError(0) << '\n';
    std::cout << "Parameter 1 (mean): " << invMassAllFit->GetParameter(1) << " +/- " << invMassAllFit->GetParError(1) << '\n';
    std::cout << "Parameter 2 (sigma): " << invMassAllFit->GetParameter(2) << " +/- " << invMassAllFit->GetParError(2) << '\n';
    std::cout << "Chi square / NDF: " << invMassAllFit->GetChisquare() / invMassAllFit->GetNDF() << '\n';
    std::cout << "Probability of the fit: " << invMassAllFit->GetProb() << '\n';

    std::cout << "\nINVARIANT MASS BETWEEN PIONS AND KAONS DISTRIBUTION:" << '\n';
    std::cout << "Parameter 0: " << invMassPionKaonFit->GetParameter(0) << " +/- " << invMassPionKaonFit->GetParError(0) << '\n';
    std::cout << "Parameter 1 (mean): " << invMassPionKaonFit->GetParameter(1) << " +/- " << invMassPionKaonFit->GetParError(1) << '\n';
    std::cout << "Parameter 2 (sigma): " << invMassPionKaonFit->GetParameter(2) << " +/- " << invMassPionKaonFit->GetParError(2) << '\n';
    std::cout << "Chi square / NDF: " << invMassPionKaonFit->GetChisquare() / invMassPionKaonFit->GetNDF() << '\n';
    std::cout << "Probability of the fit: " << invMassPionKaonFit->GetProb() << '\n';

    std::cout << "\nINVARIANT MASS BETWEEN DECAYED PARTICLES DISTRIBUTION:" << '\n';
    std::cout << "Parameter 0: " << invMassDecayedFit->GetParameter(0) << " +/- " << invMassDecayedFit->GetParError(0) << '\n';
    std::cout << "Parameter 1 (mean): " << invMassDecayedFit->GetParameter(1) << " +/- " << invMassDecayedFit->GetParError(1) << '\n';
    std::cout << "Parameter 2 (sigma): " << invMassDecayedFit->GetParameter(2) << " +/- " << invMassDecayedFit->GetParError(2) << '\n';
    std::cout << "Chi square / NDF: " << invMassDecayedFit->GetChisquare() / invMassDecayedFit->GetNDF() << '\n';
    std::cout << "Probability of the fit: " << invMassDecayedFit->GetProb() << '\n';






    

    



}


