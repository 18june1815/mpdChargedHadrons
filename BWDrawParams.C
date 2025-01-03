#include "def.h"
#include "ReadFiles.h"

void DrawParam(string paramName = "T")
{
    TGraphErrors *gr[N_PARTS];
    double xerr[N_CENTR];

    for (int i: CENTR) xerr[i] = 0.;

    for (int part: PARTS)
    {
        if (paramName == "T")
            gr[part] = new TGraphErrors(N_CENTR, centrX, Tpar[part], xerr, xerr); // Tpar_err[part]);
        else if (paramName == "ut")
            gr[part] = new TGraphErrors(N_CENTR, centrX, utPar[part], xerr, xerr); // utPar_err[part]);
        
        gr[part]->SetMarkerStyle(8);
        gr[part]->SetMarkerSize(2);
        gr[part]->SetMarkerColor(partColors[part]);
    }

    TCanvas *c2 = new TCanvas("c2", "c2", 29, 30, 1200, 1000);
    c2->cd();
    c2->SetGrid();
    // c2->SetLogx();
    double ll = 10, rl = 100., pad_min = 0., pad_max = (paramName == "T") ? 0.3 : 1., 
        pad_offset_x = 1., pad_offset_y = 1., 
        pad_tsize = 0.05, pad_lsize=0.05;
    TString pad_title_y = (paramName == "T") ? "T [GeV]" : "#LTu_{t}#GT";
    TString pad_title_x = "centrality [%]";
    Format_Pad(ll, rl, pad_min, pad_max, pad_title_x, pad_title_y, pad_offset_x, pad_offset_y, pad_tsize, pad_lsize, "", 8);        
    
    TLegend *legend = new TLegend(0.2, 0.7, 0.6, 0.85);
    legend->SetBorderSize(0);
    legend->SetFillStyle(0);
    legend->SetNColumns(2);
    legend->SetTextSize(0.05);

    for (int part: PARTS)
    {
       gr[part]->Draw("P SAME");
       legend->AddEntry(gr[part], partTitles[part].c_str(), "P");
    }
    legend->Draw();
    c2->SaveAs(("output/BWparam_" + paramName + ".pdf").c_str());
}

void BWDrawParams ( void )
{
    ReadParams();
    DrawParam("T");
    DrawParam("ut");
    gROOT->ProcessLine(".q");
}