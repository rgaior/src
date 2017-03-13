/*
  class Styler
    by Hans Dembinski, RWTH Aachen

  Purpose: Generate a consistent style of font sizes, colors, offsets and what not
           for the daily plotting needs.
           
  Usage:   Call Styler::Row(n) with n = 1, 2, or 3 to generate a TCanvas and the
           appropriate Style for 1 or 2 or 3 plots next to each other
           The TCanvas returned by the function, is already divided in the correct
           amount of TPads. Use Styler::Row3D(n) only, if the plot is 3D view.

           To get a sophisticated choice of colors, marker and line styles, use
           Styler::GetColor(i), Styler::GetLineStyle(i), and
           Styler::GetMarkerStyle(i) for your i-th plot in the same frame.

           There are other useful functions as well:
         
           Use Styler::TopLeftText(numberOfLines) to generate a TPaveText field,
           which nicely fits into the top left corner of your plot.
           Styler::TopRightText(numberOfLines) works in the same way.

           You can also use Styler::TopRightLegend(numberOfLines,x0) to generate
           a TLegend with a nice default style. Unfortunately, you have to supply
           an x0 too, which is the percentage of the plot, where the TLegend
           frame is supposed to start. You still have to fiddle around with that
           number, it depends on the length of your TLegend entries. :-(
*/

#ifndef STYLER_H
#define STYLER_H

#include <TStyle.h>
#include <TROOT.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TLegendEntry.h>
#include <TPaveText.h>
#include <TH1.h>
#include <TGraph.h>
#include <TF1.h>
#include <TList.h>
#include <TPad.h>
#include <TLatex.h>
#include <TClass.h>
#include <TString.h>
#include <cfloat>

#include <iostream>

static double gFontSize;

class Styler {
public:
  Styler(){};
  static void GetLegendSizeInfo(TLegend* l, unsigned short& nLines, double& w) {
    nLines = l->GetListOfPrimitives()->GetEntries();
    w = 0;
    for (unsigned short i=0;i<nLines;++i)
    {
      TLegendEntry* e = (TLegendEntry*)(l->GetListOfPrimitives()->At(i));
      TLatex tmp(0.0,0.0,e->GetLabel());
      
      tmp.SetNDC();
      tmp.SetTextFont(l->GetTextFont());
      tmp.SetTextSize(l->GetTextSize());
      tmp.Draw();
      double x = tmp.GetXsize();
      if (x>w) w = x;
    }
    w/=gPad->GetX2()-gPad->GetX1();
    if (l->GetHeader()!=0)
    {
      TLatex tmp(0,0,l->GetHeader());
      tmp.SetTextFont(l->GetTextFont());
      tmp.SetTextSize(l->GetTextSize());
      if (tmp.GetYsize()>w) w = tmp.GetYsize();
      ++nLines;
    }
  }

  static void Init() {
    gROOT->SetStyle("Plain");

    gStyle->SetTitleBorderSize(0);
    gStyle->SetCanvasBorderMode(0);
    gStyle->SetFrameBorderMode(0);
    gStyle->SetPadBorderMode(0);
    gStyle->SetDrawBorder(0);
    gStyle->SetLegendBorderSize(0);

    gStyle->SetPadGridX(0);
    gStyle->SetPadGridY(0);

    gStyle->SetPaperSize(20,26);

//     gStyle->SetOptStat(111111); // Show overflow and underflow as well
    gStyle->SetOptStat(0); // Show overflow and underflow as well
    gStyle->SetOptFit(0);
//     gStyle->SetOptFit(1011);
    gStyle->SetPalette(1); // Get violet->red color palette

    gStyle->SetPadTickX(1);
    gStyle->SetPadTickY(1);

    gStyle->SetNdivisions(505,"XYZ");
  }

  static TCanvas* Row(const int n, bool makeCanvas=true) {
    Init();
    std::cout << "Style: " << n << " plot" << (n>1?"s":"") << " in a row" << std::endl;

    gStyle->SetTickLength(-0.02,"X");
    gStyle->SetTickLength(-0.02,"Y");
    gStyle->SetTickLength(-0.02,"Z");

    const int   font     = 42;
    gStyle->SetTextFont(font);
    gStyle->SetStatFont(font);

    gStyle->SetLabelFont(font,"XYZ");
    gStyle->SetTitleFont(font,"XYZ");

    gStyle->SetLabelOffset(0.02,"XYZ");

    gStyle->SetPadTopMargin(0.04);

    if (n == 1 || n == 2) {
      if (n == 1) {
        gStyle->SetCanvasDefW(600);
        gStyle->SetCanvasDefH(450);
      } else {
        gStyle->SetCanvasDefW(1200);
        gStyle->SetCanvasDefH(450);
      }

      const float fontSize = 0.06;

      gStyle->SetTitleSize(fontSize,"XYZ");
      gStyle->SetLabelSize(fontSize,"XYZ");

      gStyle->SetStatFontSize(fontSize);
      gStyle->SetTitleFontSize(fontSize);

      gStyle->SetTitleOffset(1.20,"X");
      gStyle->SetTitleOffset(1.40,"Y");
      gStyle->SetTitleOffset(1.20,"Z");

      gStyle->SetPadLeftMargin  (0.16);
//       gStyle->SetPadRightMargin (0.20); // w/ z-axis
      gStyle->SetPadRightMargin (0.07); // w/o z-axis
      gStyle->SetPadBottomMargin(0.17);

      gStyle->SetNdivisions(510,"X");
    }

    if (n == 3) {
      gStyle->SetCanvasDefW(1200);
      gStyle->SetCanvasDefH(450);

      const float fontSize = 0.07;

      gStyle->SetTitleSize(fontSize,"XYZ");
      gStyle->SetLabelSize(fontSize,"XYZ");

      gStyle->SetStatFontSize(fontSize);
      gStyle->SetTitleFontSize(fontSize);

      gStyle->SetTitleOffset(1.20,"X");
      gStyle->SetTitleOffset(1.88,"Y");
      gStyle->SetTitleOffset(1.20,"Z");

      gStyle->SetPadLeftMargin  (0.26);
      gStyle->SetPadRightMargin (0.07);
      gStyle->SetPadBottomMargin(0.19);
    }

    if (n == 777) {
      gStyle->SetCanvasDefW(600);
      gStyle->SetCanvasDefH(600);

      const float fontSize = 0.07;

      gStyle->SetTitleSize(fontSize,"XYZ");
      gStyle->SetLabelSize(fontSize,"XYZ");

      gStyle->SetStatFontSize(fontSize);
      gStyle->SetTitleFontSize(fontSize);

      gStyle->SetTitleOffset(1.20,"X");
      gStyle->SetTitleOffset(1.88,"Y");
      gStyle->SetTitleOffset(1.20,"Z");

      gStyle->SetPadLeftMargin  (0.07);
      gStyle->SetPadRightMargin (0.07);
      gStyle->SetPadTopMargin(0.07);
      gStyle->SetPadBottomMargin(0.07);
    }

    TCanvas* c = 0;
    if (makeCanvas) {
      c = new TCanvas;
      c->Divide(1,n);
    }
    return c;
  }

  static TCanvas* Row3D(const int n, bool makeCanvas=true) {
    Init();

    std::cout << "Style: " << n << " 3D plot" << (n>1?"s":"") << " in a row" << std::endl;

    gStyle->SetTickLength(0.03,"X");
    gStyle->SetTickLength(0.03,"Y");
    gStyle->SetTickLength(0.03,"Z");

    if (n == 1 || n == 2) {
      if (n == 1) {
        gStyle->SetCanvasDefW(500);
        gStyle->SetCanvasDefH(450);
      } else {
        gStyle->SetCanvasDefW(1000);
        gStyle->SetCanvasDefH(450);
      }

      const int   font     = 42;
      const float fontSize = 0.06;

      gStyle->SetTextFont(font);
      gStyle->SetStatFont(font);

      gStyle->SetLabelFont(font,"XYZ");
      gStyle->SetTitleFont(font,"XYZ");

      gStyle->SetTitleSize(fontSize,"XYZ");
      gStyle->SetLabelSize(fontSize,"XYZ");

      gStyle->SetStatFontSize(fontSize);
      gStyle->SetTitleFontSize(fontSize);

      gStyle->SetTitleOffset(1.4 ,"X");
      gStyle->SetTitleOffset(1.5 ,"Y");
      gStyle->SetTitleOffset(1.6 ,"Z");

      gStyle->SetLabelOffset(0.030,"X");
      gStyle->SetLabelOffset(0.000,"Y");
      gStyle->SetLabelOffset(0.025,"Z");

      gStyle->SetPadLeftMargin  (0.18);
      gStyle->SetPadRightMargin (0.18);
      gStyle->SetPadTopMargin   (0.02);
      gStyle->SetPadBottomMargin(0.15);
    }

    if (n == 3) {
      gStyle->SetCanvasDefW(1000);
      gStyle->SetCanvasDefH(450);

      const int   font     = 42;
      const float fontSize = 0.07;

      gStyle->SetTextFont(font);
      gStyle->SetStatFont(font);

      gStyle->SetLabelFont(font,"XYZ");
      gStyle->SetTitleFont(font,"XYZ");

      gStyle->SetTitleSize(fontSize,"XYZ");
      gStyle->SetLabelSize(fontSize,"XYZ");

      gStyle->SetStatFontSize(fontSize);
      gStyle->SetTitleFontSize(fontSize);

      gStyle->SetTitleOffset(1.4,"X");
      gStyle->SetTitleOffset(1.5,"Y");
      gStyle->SetTitleOffset(1.6,"Z");

      gStyle->SetLabelOffset(0.030,"X");
      gStyle->SetLabelOffset(0.000,"Y");
      gStyle->SetLabelOffset(0.025,"Z");

      gStyle->SetPadLeftMargin  (0.25);
      gStyle->SetPadRightMargin (0.10);
      gStyle->SetPadTopMargin   (0.02);
      gStyle->SetPadBottomMargin(0.15);
    }

    TCanvas* c = 0;
    if (makeCanvas) {
      c = new TCanvas;
      c->Divide(n);
    }
    return c;
  }

  static void DrawTopLeftLegend(TLegend* l, double m = 0.07)
  {
    gFontSize=0.05;
    l->SetFillStyle(0);
    l->SetBorderSize(0);
    l->SetTextSize(gFontSize*0.9);
    l->SetTextFont(42);

    unsigned short nLines;
    double w;
    Styler::GetLegendSizeInfo(l,nLines,w);

    double dy = nLines*gFontSize*(1.0+l->GetColumnSeparation());
    double dx = w+m;
    l->SetMargin(1-w/dx);

    l->SetX1NDC(gPad->GetLeftMargin()+0.01);
    l->SetX2NDC(gPad->GetLeftMargin()+0.01+dx);
    l->SetY1NDC(1.0-gPad->GetTopMargin()-0.01-dy);
    l->SetY2NDC(1.0-gPad->GetTopMargin()-0.01);
    l->Draw();
  }

  static void DrawTopRightLegend(TLegend* l, double m = 0.07)
  {
    gFontSize=0.05;
    l->SetFillStyle(0);
    l->SetBorderSize(0);
    l->SetTextSize(gFontSize*0.9);
    l->SetTextFont(42);

    unsigned short nLines;
    double w;
    Styler::GetLegendSizeInfo(l,nLines,w);

    double dy = nLines*gFontSize*(1.0+l->GetColumnSeparation());
    double dx = w+m;
    l->SetMargin(1-w/dx);

    l->SetX1NDC(1.0-gPad->GetRightMargin()-dx-0.01);
    l->SetX2NDC(1.0-gPad->GetRightMargin()-0.01);
    l->SetY1NDC(1.0-gPad->GetTopMargin()-0.01-dy);
    l->SetY2NDC(1.0-gPad->GetTopMargin()-0.01);
    l->Draw();
  }

  static void DrawBottomLeftLegend(TLegend* l, double m = 0.07)
  {
    gFontSize=0.05;
    l->SetFillStyle(0);
    l->SetBorderSize(0);
    l->SetTextSize(gFontSize*0.9);
    l->SetTextFont(42);

    unsigned short nLines;
    double w;
    Styler::GetLegendSizeInfo(l,nLines,w);

    double dy = nLines*gFontSize*(1.0+l->GetColumnSeparation());
    double dx = w+m;
    l->SetMargin(1-w/dx);

    l->SetX1NDC(gPad->GetLeftMargin()+0.01);
    l->SetX2NDC(gPad->GetLeftMargin()+0.01+dx);
    l->SetY1NDC(gPad->GetBottomMargin()+0.01);
    l->SetY2NDC(gPad->GetBottomMargin()+0.01+dy);
    l->Draw();
  }

  static void DrawBottomRightLegend(TLegend* l, double m = 0.07)
  {
    gFontSize=0.05;
    l->SetFillStyle(0);
    l->SetBorderSize(0);
    l->SetTextSize(gFontSize*0.9);
    l->SetTextFont(42);

    unsigned short nLines;
    double w;
    Styler::GetLegendSizeInfo(l,nLines,w);

    double dy = nLines*gFontSize*(1.0+l->GetColumnSeparation());
    double dx = w+m;
    l->SetMargin(1-w/dx);

    l->SetX1NDC(1.0-gPad->GetRightMargin()-dx-0.01);
    l->SetX2NDC(1.0-gPad->GetRightMargin()-0.01);
    l->SetY1NDC(gPad->GetBottomMargin()+0.01);
    l->SetY2NDC(gPad->GetBottomMargin()+0.01+dy);
    l->Draw();
  }

  static TLegend* Legend(double x0, double y0, double x1, double y1) {
    TLegend* leg = new TLegend(x0,y0,x1,y1);
    leg->SetFillStyle(0);
    leg->SetBorderSize(0);
    leg->SetTextSize(gFontSize*0.9);
    leg->SetTextFont(42);
    return leg;
  }
/*
  static TLegend* Legend(double x0, double y0, double x1, double y1) {
    TLegend* leg = new TLegend(x0,y0,x1,y1);
    leg->SetFillStyle(0);
    leg->SetBorderSize(0);
    leg->SetTextSize(0.05);
    leg->SetTextFont(42);
    return leg;
  }
*/
  static TPaveText* PaveText(double x0, double y0, double x1, double y1) {
    TPaveText* tx = new TPaveText(x0,y0,x1,y1,"NDC");
    tx->SetFillStyle(0);
    tx->SetBorderSize(0);
    tx->SetTextSize(0.05);
    tx->SetTextFont(42);
    return tx;
  }

  static TLegend* TopLeftLegend(unsigned nLines, double p) {
    double dy = nLines*0.07;
    double x0 = gPad->GetLeftMargin()+0.02;
    double x1 = 1.0-gPad->GetRightMargin();
    double y0 = 1.0-gPad->GetTopMargin()-0.01-dy;
    double y1 = 1.0-gPad->GetTopMargin()-0.01;
    TLegend* l = Legend(x0,y0,x0+p*(x1-x0),y1);
    return l;
  }

  static TPaveText* TopLeftText(unsigned nLines) {
    double dy = nLines*0.07;
    double x0 = gPad->GetLeftMargin()+0.02;
    double x1 = gPad->GetLeftMargin()+0.02;
    double y0 = 1.0-gPad->GetTopMargin()-0.02-dy;
    double y1 = 1.0-gPad->GetTopMargin()-0.02;
    TPaveText* tx = Styler::PaveText(x0,y0,x1,y1);
    tx->SetTextAlign(13);
    return tx;
  }

  static TLegend* TopRightLegend(unsigned nLines, double p) {
    double dy = nLines*0.07;
    double x0 = gPad->GetLeftMargin();
    double x1 = 1.0-gPad->GetRightMargin();
    double y0 = 1.0-gPad->GetTopMargin()-0.01-dy;
    double y1 = 1.0-gPad->GetTopMargin()-0.01;
    TLegend* l = Legend(x0+p*(x1-x0),y0,x1,y1);
//     l->SetEntrySeparation(0.0);
//     l->SetTextAlign(33);
    return l;
  }

  static TPaveText* TopRightText(unsigned nLines) {
    double dy = nLines*0.07;
    double x0 = 1.0-gPad->GetRightMargin()-0.02;
    double x1 = 1.0-gPad->GetRightMargin()-0.02;
    double y0 = 1.0-gPad->GetTopMargin()-0.02-dy;
    double y1 = 1.0-gPad->GetTopMargin()-0.02;
    TPaveText* tx = Styler::PaveText(x0,y0,x1,y1);
    tx->SetTextAlign(33);
    return tx;
  }

  static void SetGreyPalette(unsigned int nSteps) {
    nSteps += 1;
    double* steps = new double[nSteps];
    double* col   = new double[nSteps];
    for (unsigned int i=0;i<nSteps;++i) {
      steps[i] = 1.0/nSteps*(i+1);
      col[i]   = 1.0-1.0/nSteps*(i+1);
    }
    //gStyle->CreateGradientColorTable(nSteps,steps,col,col,col,nSteps-1);
    delete [] steps;
    delete [] col;
  }

  static int GetColor(int i) {
    const UShort_t color[11] = { 1,2,4, 8,6,7, 41,45,40, 37,33 };
    return color[i];
  }
  
  static int GetLineStyle(int i) {
    const UShort_t lineStyle[11] = { 1 ,2 ,3 ,  4, 5, 6,  7, 8, 1,  2, 3 };
    return lineStyle[i];
  }
  
  static int GetMarkerStyle(int i) {
    const UShort_t markerStyle[11] = { 20,21,22, 23,24,25, 26,27,28,  2, 5 };
    return markerStyle[i];
  }

  static void CommonAxisRange(TAxis* a1, TAxis* a2, TAxis* a3=0) {
    gPad->GetCanvas()->Update();
    double mymin = a1->GetXmin();
    double mymax = a1->GetXmax();
    if (a2->GetXmin()<mymin) mymin = a2->GetXmin();
    if (a3 && a3->GetXmin()<mymin) mymin = a3->GetXmin();
    if (a2->GetXmax()>mymax) mymax = a2->GetXmax();
    if (a3 && a3->GetXmax()>mymax) mymax = a3->GetXmax();
//     TAxis* axisList[3];
//     TList* l=c->GetListOfPrimitives();
//     int n = l->GetEntries();
//     for(int ipad=0;ipad<n;++ipad) {
//       TPad* pad = (TPad*)l->At(ipad);
//       double y0 = pad->GetUymin();
//       double y1 = pad->GetUymax();
//       std::cout << y0 << " " << y1 << std::endl;
//       if (mymin > y0) mymin = y0;
//       if (mymax < y1) mymax = y1;
// //       std::cout << pad << std::endl;
// //       TList* l2 = pad->GetListOfPrimitives();
// //       std::cout << l2 << std::endl;
// //       TObject* item = l2->At(0);
// //       std::cout << item << std::endl;
// //       TAxis* axis = 0;
// //       TClass* baseTH1 = item->IsA()->GetBaseClass("TH1");
// //       if (baseTH1) axis = ((TH1*)item)->GetYaxis();
// //       TClass* baseTGraph = item->IsA()->GetBaseClass("TGraph");
// //       if (baseTGraph) axis = ((TGraph*)item)->GetYaxis();
// //       TClass* baseTF1 = item->IsA()->GetBaseClass("TF1");
// //       if (baseTF1) axis = ((TF1*)item)->GetYaxis();
// //       if (mymin > axis->GetXmin()) mymin = axis->GetXmin();
// //       if (mymax < axis->GetXmax()) mymax = axis->GetXmax();
// //       axisList[ipad] = axis;
//     }
//     for (int ipad=0;ipad<n;++ipad) {
//       TPad* pad = (TPad*)l->At(ipad);
//       std::cout << ipad << endl;
//       std::cout << mymin << " " << mymax << std::endl;
//       double dummy,x0,x1;
//       pad->GetRangeAxis(x0,dummy,x1,dummy);
//       cout << x0 << " " << x1 << " " << mymin << " " << mymax << endl;
// //       pad->RangeAxis(x0,mymin,x1,mymax);
// //       pad->RedrawAxis();
// //       axisList[ipad]->SetRangeUser(mymin,mymax);
//     }
    a1->SetRangeUser(mymin,mymax);
    a2->SetRangeUser(mymin,mymax);
    if (a3) a3->SetRangeUser(mymin,mymax);
//     c->Update();
    gPad->GetCanvas()->Update();
  }
};

#endif
