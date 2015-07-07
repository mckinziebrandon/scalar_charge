{
/*********************************************
Name: Brandon McKinzie
Project: .rootlogon.C

Assignment:
Setup All User Customizations
*********************************************/

// BrandonStyle Defines Default Style Configuration
TStyle* BrandonStyle = new TStyle("BrandonStyle","Brandon's Root Styles");

// Default Graph/Font Options
BrandonStyle->SetTextFont(132);
BrandonStyle->SetTextSize(0.08);
BrandonStyle->SetOptStat(0);
BrandonStyle->SetOptDate(0);
BrandonStyle->SetOptTitle(0);
BrandonStyle->SetLabelSize(0.05,"xyz"); // size of axis value font
BrandonStyle->SetTitleSize(0.06,"xyz"); // size of axis title font
BrandonStyle->SetTitleFont(132,"xyz"); // font option
BrandonStyle->SetLabelFont(132,"xyz");
BrandonStyle->SetTitleOffset(1.2,"y");

// Paper & Margin Sizes
BrandonStyle->SetPaperSize(20,26);
BrandonStyle->SetPadTopMargin(0.05);
BrandonStyle->SetPadRightMargin(0.05);
BrandonStyle->SetPadBottomMargin(0.16);
BrandonStyle->SetPadLeftMargin(0.12);

// Default Canvas Options
BrandonStyle->SetCanvasColor(0); // canvas...
BrandonStyle->SetCanvasBorderMode(0);
BrandonStyle->SetFrameBorderMode(0);
BrandonStyle->SetFillColor(0);

// Default Pad Options
BrandonStyle->SetPadColor(0);
BrandonStyle->SetPadBorderMode(0);
BrandonStyle->SetPadGridX(1); // grids, tickmarks
BrandonStyle->SetPadGridY(1);
BrandonStyle->SetPadTickX(1); // grids, tickmarks
BrandonStyle->SetPadTickY(1);

// Default Object Options
//BrandonStyle->SetHistLineWidth(1.5);
//BrandonStyle->SetErrorX(0.001);

// Marker options
//BrandonStyle->SetMarkerStyle(22);
BrandonStyle->SetMarkerSize(1);
BrandonStyle->SetMarkerColor(kRed+1);

// Set Colors
const Int_t NRGBs = 5;
const Int_t NCont = 256;
Int_t MyPalette[NCont];

Double_t stops[NRGBs]   = { 0.00, 0.30, 0.61, 0.84, 1.00 };
Double_t red[NRGBs]     = { 0.00, 0.00, 0.57, 0.90, 0.51 };
Double_t green[NRGBs]   = { 0.00, 0.65, 0.95, 0.20, 0.00 };
Double_t blue[NRGBs]    = { 0.51, 0.55, 0.15, 0.00, 0.10 };
Int_t FI = TColor::CreateGradientColorTable(NRGBs, stops, red, green, blue,NCont);

for (Int_t i=0;i<NCont;i++) MyPalette[i] = FI+i;
BrandonStyle->SetPalette(256, MyPalette);
BrandonStyle->SetNumberContours(256);

// Finish Configuration
gROOT->SetStyle("BrandonStyle");
cout << "New Styles are Set!" << endl;
return;
}

