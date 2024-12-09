#ifndef __DEF_H_
#define __DEF_H_

#include "input/FormatOfEverything.h"

#include "TCanvas.h"
#include "TStyle.h"
#include "TGraphErrors.h"
#include "TF1.h"
#include <TH1.h>
#include <TFile.h>
#include "TLegend.h"
#include "BlastWave.h"

const int MAX_CENTR = 10;
const int MAX_PARTS = 6;
const int N_CENTR = 6;
const int N_PARTS = 6;
const int PARTS[] = {0, 1, 2, 3, 4, 5};
const int PARTS_GLOBAL[] = {0, 2, 4};
const int CENTR[] = {0, 1, 2, 3, 4, 5};

TH1D *hSpectra[6][12];
TGraphErrors *grSpectra[6][12];

string particles[6] = {"pip", "pim", "kp", "km", "p", "ap"};
string partTitles[6] = { "#pi^{+}","#pi^{#minus}","K^{+}","K^{#minus}","p", "#bar{p}"};
double masses[6] = {0.13957061, 0.13957061, 0.493667, 0.493667, 0.938272, 0.938272};
Color_t centrColors[11] = {kRed, kBlue, kGreen + 2, kBlack, kBlack, kBlack, kBlack, kBlack, kBlack, kBlack, kBlack};
Color_t partColors[6] = {kRed, kRed, kBlue, kBlue, kGreen + 2, kGreen + 2};
string centrTitles[10] = {"0-10%", "10-20%", "20-30%", "30-40%", "40-60%", "60-80%"};
// string centrTitles[10] = {"0-20%", "20-40%", "40-60%", "60-80%", "40-50%", "50-60%", "60-70%", "70-80%"};


// =============== Для  BlastWave ======================
TF1 *ifuncxGlobal[MAX_PARTS][N_CENTR];
double paramsGlobal[N_CENTR][5]; // 5 - количество параметров 1) T 2) ut 3) const pi 4) const K 5) const p
// по частицам
double con[]    = {10, 10, 1, 1, 0.1, 0.1};  
double conmin[] = {0, 0, 0, 0, 0, 0};
double conmax[] = {500, 500, 50, 50, 5000, 5000};

//For GlobalFit
double conGlobal[]    = {100, 100, 120, 60, 0.01, 0.01};  
double conminGlobal[] = {0, 0, 0, 0, 0, 0};
double conmaxGlobal[] = {5000, 5000, 5000, 5000, 5000, 5000};
//=======================================================

void SetSpectra(string inputFileName = "postprocess_mpdpid10", string type = "pt")
{
    TFile *f = new TFile(("input/" + inputFileName + ".root").c_str());
    TDirectory *fd;

    for (int i = 0; i < 6; i++)
    {
        fd = (TDirectory*)f->Get(particles[i].c_str());
        fd->cd();
        for (int centr = 0; centr < N_CENTR; centr++)
        {
            string name = "h__pt_" + particles[i] +"_centrality" + to_string(centr) + "_mc_y-0.5_0.5";
            cout << name << endl;
            hSpectra[i][centr] = (TH1D *)fd->Get(name.c_str());    
            

            const int N_BINS = hSpectra[i][centr]->GetNbinsX();
            double mT[N_BINS], pT[N_BINS], sp[N_BINS], sp_err[N_BINS], xerr[N_BINS];
            for (int bin = 1; bin < N_BINS; bin++)
            {
                sp[bin - 1] = hSpectra[i][centr]->GetBinContent(bin);
                sp_err[bin - 1] = hSpectra[i][centr]->GetBinError(bin);
                xerr[bin - 1] = 0.;
                pT[bin - 1] = hSpectra[i][centr]->GetBinCenter(bin);
                mT[bin - 1] = sqrt(pT[bin - 1] * pT[bin - 1] + masses[i] * masses[i]) - masses[i];
            }
    
            if (type == "mt") 
                grSpectra[i][centr] = new TGraphErrors(N_BINS - 1, mT, sp, xerr, sp_err);
            else if (type == "pt")
                grSpectra[i][centr] = new TGraphErrors(hSpectra[i][centr]);

            grSpectra[i][centr]->SetLineColor(centrColors[centr]);
        }
    }
}

#endif /* __DEF_H_ */