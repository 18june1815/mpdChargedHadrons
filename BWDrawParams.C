#include "def.h"
#include "WriteReadFiles.h"

double constPar[MAX_PARTS][N_CENTR],
       Tpar[N_PARTS][N_CENTR], Tpar_err[N_PARTS][N_CENTR], Tpar_sys[N_PARTS][N_CENTR], 
       utPar[N_PARTS][N_CENTR], utPar_err[N_PARTS][N_CENTR], utPar_sys[N_PARTS][N_CENTR];

double avgT = 0, avgTerr = 0.;

void DrawParam(string paramName = "T", bool isSyst = true)
{
    TGraphErrors *gr[N_PARTS], *grSys[N_PARTS];
    double xerr[N_CENTR], xerrSys[N_CENTR];

    for (int i: CENTR) xerr[i] = 0., xerrSys[i] = 1;

    for (int part: PARTS)
    {
        if (paramName == "T")
            gr[part] = new TGraphErrors(N_CENTR, centrX, Tpar[part], xerr, Tpar_err[part]);
        else if (paramName == "ut")
            gr[part] = new TGraphErrors(N_CENTR, centrX, utPar[part], xerr, utPar_err[part]);
        
        gr[part]->SetMarkerStyle(8);
        gr[part]->SetMarkerSize(2);
        gr[part]->SetMarkerColor(partColors[part]);

        if (isSyst)
        {
            if (paramName == "T")
                grSys[part] = new TGraphErrors(N_CENTR, centrX, Tpar[part], xerrSys, Tpar_sys[part]);
            else if (paramName == "ut")
                grSys[part] = new TGraphErrors(N_CENTR, centrX, utPar[part], xerrSys, utPar_sys[part]);
            
            grSys[part]->SetLineColorAlpha(partColors[part], 0.6);
            grSys[part]->SetFillStyle(0);
            grSys[part]->SetFillColorAlpha(partColors[part], 0.5);
            grSys[part]->SetLineWidth(2);
            grSys[part]->SetMarkerColorAlpha(partColors[part], 0.6);
        }
    }

    TCanvas *c2 = new TCanvas("c2", "c2", 29, 30, 1200, 1000);
    c2->cd();
    c2->SetGrid();
    // c2->SetLogx();
    double ll = 10, rl = 100., pad_min = 0., pad_max = (paramName == "T") ? 0.3 : 1., 
        pad_offset_x = 1., pad_offset_y = 1., 
        pad_tsize = 0.05, pad_lsize=0.05;
    TString pad_title_y = (paramName == "T") ? "T [GeV]" : "#beta";
    TString pad_title_x = "centrality [%]";
    Format_Pad(ll, rl, pad_min, pad_max, pad_title_x, pad_title_y, pad_offset_x, pad_offset_y, pad_tsize, pad_lsize, "", 8);        
    
    TLegend *legend = new TLegend(0.2, 0.7, 0.6, 0.85);
    legend->SetBorderSize(0);
    legend->SetFillStyle(0);
    legend->SetNColumns(2);
    legend->SetTextSize(0.05);

    if (paramName == "T")
    {
        TLine *lineT = new TLine(ll, avgT, rl, avgT); 
        lineT->SetLineColor(kRed);
        lineT->SetLineWidth(2);
        lineT->SetLineStyle(kDashed);
        lineT->Draw("same");
    }

    for (int part: PARTS)
    {
       gr[part]->Draw("P SAME");
       if (isSyst) grSys[part]->Draw("P2");

       legend->AddEntry(gr[part], partTitles[part].c_str(), "P");
    }
    legend->Draw();
    c2->SaveAs(("output/BWparam_" + paramName + ".pdf").c_str());
}

void BWDrawParams ( void )
{
    ReadParam(1, Tpar, Tpar_err, Tpar_sys);
    ReadParam(2, utPar, utPar_err, utPar_sys);

    int count = 0;
    for (int centr: CENTR)
        for (int part = 0; part < 6; part++)
        {
            avgT += Tpar[part][centr];
            avgTerr += pow(Tpar_sys[part][centr], 2);
            count++;
        }
    avgT = avgT / double(count);
    avgTerr = sqrt(avgTerr) / double(count);
    cout << avgT << "  " << avgTerr << endl;


    DrawParam("T");
    DrawParam("ut");

    gROOT->ProcessLine(".q");
}