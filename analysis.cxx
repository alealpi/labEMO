#include "TFile.h"
#include "TH1.h"
#include "TF1.h"
#include "TMath.h"
#include "TCanvas.h"
#include "TStyle.h"
#include <iostream>

void analyse()
{
    TFile *file = new TFile("results.root");

    TH1F *histoArray[12];
    histoArray[0] = (TH1F *)file->Get("particleType_h");
    histoArray[1] = (TH1F *)file->Get("azimutAngle_h");
    histoArray[2] = (TH1F *)file->Get("polarAngle_h");
    histoArray[3] = (TH1F *)file->Get("momentum_h");
    histoArray[4] = (TH1F *)file->Get("transverseMomentum_h");
    histoArray[5] = (TH1F *)file->Get("energy_h");
    histoArray[6] = (TH1F *)file->Get("invMassAll_h");
    histoArray[7] = (TH1F *)file->Get("invMassSameCharge_h");
    histoArray[8] = (TH1F *)file->Get("invMassOppositeCharge_h");
    histoArray[9] = (TH1F *)file->Get("invMassPionKaonSameCharge_h");
    histoArray[10] = (TH1F *)file->Get("invMassPionKaonOppositeCharge_h");
    histoArray[11] = (TH1F *)file->Get("invMassDecayed_h");

    // DISPLAYING HISTOGRAMS ENTRIES
    Double_t expectedEntries[12] = {1e7, 1e7, 1e7, 1e7, 1e7, 1e7, 5e8, 2.5e8, 2.5e8, 4.45e7, 4.45e7, 1e5};
    std::cout << "\nCHECKING HISTOGRAMS ENTRIES:" << '\n';
    for (int i = 0; i < 12; ++i)
    {
        auto title = histoArray[i]->GetTitle();
        auto entries = histoArray[i]->GetEntries();

        std::cout << title << ": expected " << expectedEntries[i] << ", got " << entries << '\n';
    }

    // DISPLAYING PARTICLE TYPES OCCURRENCES
    TH1F *particleType_h = (TH1F *)file->Get("particleType_h");
    std::string particleNames[7] = {"pion+", "pion-", "kaon+", "kaon-", "proton+", "proton-", "K0"};
    Double_t expectedTypes[7] = {4e6, 4e6, 5e5, 5e5, 4.5e5, 4.5e5, 1e5};
    std::cout << "\nCHECKING PARTICLE TYPES OCCURRENCES:" << '\n';
    for (int i = 0; i < 7; ++i)
    {
        auto occurrences = particleType_h->GetBinContent(i + 1);
        auto error = particleType_h->GetBinError(i + 1);

        std::cout << "Particle " << particleNames[i] << " occurrences: expected " << expectedTypes[i] << ", got " << occurrences << " +/- " << error << '\n';
    }

    // creating first canva with particle types, angles and momentum distributions
    TCanvas *particles_canva = new TCanvas("particles_canva", "Distributions");
    particles_canva->Divide(2, 2);

    // PARTICLE TYPES DISTRIBUTION
    // DRAWING
    particles_canva->cd(1);
    gStyle->SetOptStat(10);
    particleType_h->SetXTitle("Particle type");
    particleType_h->SetYTitle("Occurrences");
    particleType_h->Draw();

    // AZIMUT ANGLE DISTRIBUTION
    particles_canva->cd(2);
    TH1F *azimutAngle_h = (TH1F *)file->Get("azimutAngle_h");
    TF1 *azimutalFit = new TF1("azimutalFit", "[0]", 0., 2 * TMath::Pi());
    azimutalFit->SetParName(0, "Parameter 0");
    // FITTING
    azimutalFit->SetParameter(0, 33000);
    azimutAngle_h->Fit(azimutalFit, "QR");
    // DRAWING
    azimutAngle_h->SetMinimum(0.);
    gStyle->SetOptStat(10);
    gStyle->SetOptFit(1111);
    azimutAngle_h->SetXTitle("Azimutal angle #phi (rad)");
    azimutAngle_h->SetYTitle("Occurrences");
    azimutAngle_h->Draw();
    // DISPLAYING RESULTS
    std::cout << "\nAZIMUTAL ANGLE DISTRIBUTION FIT: " << '\n';
    std::cout << "Parameter 0: " << azimutalFit->GetParameter(0) << " +/- " << azimutalFit->GetParError(0) << '\n';
    std::cout << "Chi square / NDF: " << azimutalFit->GetChisquare() / azimutalFit->GetNDF() << '\n';
    std::cout << "Probability of the fit: " << azimutalFit->GetProb() << '\n';

    // POLAR ANGLE DISTRIBUTION
    particles_canva->cd(3);
    TH1F *polarAngle_h = (TH1F *)file->Get("polarAngle_h");
    TF1 *polarFit = new TF1("polarFit", "[0]", 0., TMath::Pi());
    polarFit->SetParName(0, "Parameter 0");
    // FITTING
    polarFit->SetParameter(0, 66000);
    polarAngle_h->Fit(polarFit, "QR");
    // DRAWING
    polarAngle_h->SetMinimum(0.);
    gStyle->SetOptStat(10);
    gStyle->SetOptFit(1111);
    polarAngle_h->SetXTitle("Polar angle #theta (rad)");
    polarAngle_h->SetYTitle("Occurrences");
    polarAngle_h->Draw();
    // DISPLAYING RESULTS
    std::cout << "\nPOLAR ANGLE DISTRIBUTION FIT: " << '\n';
    std::cout << "Parameter 0: " << polarFit->GetParameter(0) << " +/- " << polarFit->GetParError(0) << '\n';
    std::cout << "Chi square / NDF: " << polarFit->GetChisquare() / polarFit->GetNDF() << '\n';
    std::cout << "Probability of the fit: " << polarFit->GetProb() << '\n';

    // MOMENTUM DISTRIBUTION
    particles_canva->cd(4);
    TH1F *momentum_h = (TH1F *)file->Get("momentum_h");
    TF1 *momentumFit = new TF1("momentumFit", "[0] * TMath::Exp(-x / [1])", 0., 9.);
    momentumFit->SetParName(0, "Parameter 0");
    momentumFit->SetParName(1, "Mean");
    // FITTING
    momentumFit->SetParameter(0, 5e5);
    momentumFit->SetParameter(1, 1.);
    momentum_h->Fit(momentumFit, "QR");
    // DRAWING
    gStyle->SetOptStat(10);
    gStyle->SetOptFit(1111);
    momentum_h->SetXTitle("Momentum P (GeV)");
    momentum_h->SetYTitle("Occurrences");
    momentum_h->Draw();
    // DISPLAYING RESULTS
    std::cout << "\nMOMENTUM DISTRIBUTION:" << '\n';
    std::cout << "Mean: expected 1., got " << histoArray[3]->GetMean() << " +/- " << histoArray[3]->GetMeanError() << '\n';
    std::cout << "MOMENTUM DISTRIBUTION FIT:" << '\n';
    std::cout << "Parameter 0: " << momentumFit->GetParameter(0) << " +/- " << momentumFit->GetParError(0) << '\n';
    std::cout << "Parameter 1: " << momentumFit->GetParameter(1) << " +/- " << momentumFit->GetParError(1) << '\n';
    std::cout << "Chi square / NDF: " << momentumFit->GetChisquare() / momentumFit->GetNDF() << '\n';
    std::cout << "Probability of the fit: " << momentumFit->GetProb() << '\n';

    /////////////////////////////////////////////////////////////
    ////////////// INVARIANT MASS HISTOGRAMS ////////////////////
    /////////////////////////////////////////////////////////////

    // creating second canva, with invariant mass distributions
    TCanvas *invMass_canva = new TCanvas("invMass_canva", "Invariant mass distributions");
    invMass_canva->Divide(2, 2);

    // INVARIANT MASS BETWEEN ALL PARTICLES DISTRIBUTION
    invMass_canva->cd(1);
    TH1F *invMassSameCharge_h = (TH1F *)file->Get("invMassSameCharge_h");
    TH1F *invMassOppositeCharge_h = (TH1F *)file->Get("invMassOppositeCharge_h");
    TH1F *invMassSubtractionAll_h = new TH1F("invMassSubtractionAll_h", "Inv. mass from subtraction (opposite charge - same charge)", 900, 0., 5.);
    invMassSubtractionAll_h->Add(invMassOppositeCharge_h, invMassSameCharge_h, 1, -1);
    invMassSubtractionAll_h->SetEntries(invMassOppositeCharge_h->GetEntries());
    TF1 *invMassAllFit = new TF1("invMassAllFit", "[0] * TMath::Exp(-(x - [1]) * (x - [1]) / (2 * [2] * [2]))", 0.7, 1.1);
    invMassAllFit->SetParName(0, "Parameter 0");
    invMassAllFit->SetParName(1, "Mean");
    invMassAllFit->SetParName(2, "Sigma");
    // FITTING
    invMassAllFit->SetParameters(4000, 0.89166, 0.05);
    invMassSubtractionAll_h->Fit(invMassAllFit, "QR");
    // DRAWING
    gStyle->SetOptStat(10);
    gStyle->SetOptFit(1111);
    invMassSubtractionAll_h->SetXTitle("Invariant mass (GeV)");
    invMassSubtractionAll_h->SetYTitle("Occurrences");
    invMassSubtractionAll_h->SetAxisRange(0.65, 1.15);
    invMassSubtractionAll_h->Draw();
    // DISPLAYING RESULTS
    std::cout << "\nINVARIANT MASS BETWEEN ALL PARTICLES DISTRIBUTION:" << '\n';
    std::cout << "Entries: " << invMassSubtractionAll_h->GetEntries() << '\n';
    std::cout << "Parameter 0: " << invMassAllFit->GetParameter(0) << " +/- " << invMassAllFit->GetParError(0) << '\n';
    std::cout << "Parameter 1 (mean): " << invMassAllFit->GetParameter(1) << " +/- " << invMassAllFit->GetParError(1) << '\n';
    std::cout << "Parameter 2 (sigma): " << invMassAllFit->GetParameter(2) << " +/- " << invMassAllFit->GetParError(2) << '\n';
    std::cout << "Chi square / NDF: " << invMassAllFit->GetChisquare() / invMassAllFit->GetNDF() << '\n';
    std::cout << "Probability of the fit: " << invMassAllFit->GetProb() << '\n';

    // INVARIANT MASS BETWEEN PIONS AND KAONS DISTRIBUTION
    invMass_canva->cd(2);
    TH1F *invMassPionKaonSameCharge_h = (TH1F *)file->Get("invMassPionKaonSameCharge_h");
    TH1F *invMassPionKaonOppositeCharge_h = (TH1F *)file->Get("invMassPionKaonOppositeCharge_h");
    TH1F *invMassSubtractionPionKaon_h = new TH1F("invMassSubtractionPionKaon_h", "Inv. mass from subtraction (#piK opposite charge - #piK same charge)", 900, 0., 5.);
    invMassSubtractionPionKaon_h->Add(invMassPionKaonOppositeCharge_h, invMassPionKaonSameCharge_h, 1, -1);
    invMassSubtractionPionKaon_h->SetEntries(invMassPionKaonOppositeCharge_h->GetEntries());
    TF1 *invMassPionKaonFit = new TF1("invMassPionKaonFit", "[0] * TMath::Exp(-(x - [1]) * (x - [1]) / (2 * [2] * [2]))", 0.7, 1.1);
    invMassPionKaonFit->SetParName(0, "Parameter 0");
    invMassPionKaonFit->SetParName(1, "Mean");
    invMassPionKaonFit->SetParName(2, "Sigma");
    // FITTING
    invMassPionKaonFit->SetParameters(4000, 0.89166, 0.05);
    invMassSubtractionPionKaon_h->Fit(invMassPionKaonFit, "QR");
    // DRAWING
    gStyle->SetOptStat(10);
    gStyle->SetOptFit(1111);
    invMassSubtractionPionKaon_h->SetXTitle("Invariant mass (GeV)");
    invMassSubtractionPionKaon_h->SetYTitle("Occurrences");
    invMassSubtractionPionKaon_h->SetAxisRange(0.65, 1.15);
    invMassSubtractionPionKaon_h->Draw();
    // DISPLAYING RESULTS
    std::cout << "\nINVARIANT MASS BETWEEN PIONS AND KAONS DISTRIBUTION:" << '\n';
    std::cout << "Entries: " << invMassSubtractionPionKaon_h->GetEntries() << '\n';
    std::cout << "Parameter 0: " << invMassPionKaonFit->GetParameter(0) << " +/- " << invMassPionKaonFit->GetParError(0) << '\n';
    std::cout << "Parameter 1 (mean): " << invMassPionKaonFit->GetParameter(1) << " +/- " << invMassPionKaonFit->GetParError(1) << '\n';
    std::cout << "Parameter 2 (sigma): " << invMassPionKaonFit->GetParameter(2) << " +/- " << invMassPionKaonFit->GetParError(2) << '\n';
    std::cout << "Chi square / NDF: " << invMassPionKaonFit->GetChisquare() / invMassPionKaonFit->GetNDF() << '\n';
    std::cout << "Probability of the fit: " << invMassPionKaonFit->GetProb() << '\n';

    // INVARIANT MASS BETWEEN DECAYED PARTICLES DISTRIBUTION
    invMass_canva->cd(3);
    TH1F *invMassDecayed_h = (TH1F *)file->Get("invMassDecayed_h");
    TF1 *invMassDecayedFit = new TF1("invMassDecayedFit", "[0] * TMath::Exp(-(x - [1]) * (x - [1]) / (2 * [2] * [2]))", 0.7, 1.1);
    invMassDecayedFit->SetParName(0, "Parameter 0");
    invMassDecayedFit->SetParName(1, "Mean");
    invMassDecayedFit->SetParName(2, "Sigma");
    // FITTING
    invMassDecayedFit->SetParameters(2000, 0.89166, 0.05);
    invMassDecayed_h->Fit(invMassDecayedFit, "QR");
    // DRAWING
    gStyle->SetOptStat(10);
    gStyle->SetOptFit(1111);
    invMassDecayed_h->SetXTitle("Invariant mass (GeV)");
    invMassDecayed_h->SetYTitle("Occurrences");
    invMassDecayed_h->SetAxisRange(0.65, 1.15);
    invMassDecayed_h->Draw();
    // DISPLAYING RESULTS
    std::cout << "\nINVARIANT MASS BETWEEN DECAYED PARTICLES DISTRIBUTION:" << '\n';
    std::cout << "Entries: " << invMassDecayed_h->GetEntries() << '\n';
    std::cout << "Parameter 0: " << invMassDecayedFit->GetParameter(0) << " +/- " << invMassDecayedFit->GetParError(0) << '\n';
    std::cout << "Parameter 1 (mean): " << invMassDecayedFit->GetParameter(1) << " +/- " << invMassDecayedFit->GetParError(1) << '\n';
    std::cout << "Parameter 2 (sigma): " << invMassDecayedFit->GetParameter(2) << " +/- " << invMassDecayedFit->GetParError(2) << '\n';
    std::cout << "Chi square / NDF: " << invMassDecayedFit->GetChisquare() / invMassDecayedFit->GetNDF() << '\n';
    std::cout << "Probability of the fit: " << invMassDecayedFit->GetProb() << '\n';
}
