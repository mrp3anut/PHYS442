void histo(){

  TCanvas *canvas = new TCanvas();
  canvas->SetTickx();
  canvas->SetTicky();

  float x[10] = {5.070, 5.090, 5.118, 5.086, 5.149, 5.110, 5.176, 5.188, 5.117, 5.119};
  
  TH1F *histo = new TH1F("histo", "Distribution of Weighted Means of Delta E Values", 3, 5.05, 5.20);

  for(int i=0; i<10; i++){
    histo->Fill(x[i]);
  }

  histo->SetStats(0); 
  histo->SetFillColor(kBlue); 
  histo->GetXaxis()->SetTitle("Weighted Means of Delta E Values");
  histo->GetYaxis()->SetTitle("# of Delta E Values in range");
  histo->GetYaxis()->SetRangeUser(0,5.5);
  histo->Draw();


  TF1 *gauss = new TF1("gaus", "gaus", 5.05, 5.20);
  gauss->SetLineColor(kRed);
  gauss->SetLineWidth(2);

  histo->Fit(gauss, "R");

  float gauss_mean = gauss->GetParameter(1);
  float gauss_un = gauss->GetParameter(2);

  TLegend *leg1 = new TLegend();
  leg1->AddEntry(histo, "Measured Data", "f");
  leg1->AddEntry(gauss, "Gaussian Fit", "l");
  leg1->Draw();

  cout << "Mean value of the Gaussian Fit: " << gauss_mean << " unc. " << gauss_un << endl;
}
