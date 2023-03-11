void u1_3(){

  //Read experiment data from txt

  ifstream file;
  file.open("../data/analysis_data/U1(1,73)U3(1,05)T(175).txt");

  float time, v1, v2, coll_curr, u2;
  string first_5_lines, freq;

  int MAX_LEN=151; // All the files contain 151 lines of data + 5 lines of metadata

  float curr_arr[MAX_LEN];
  float u2_arr[MAX_LEN];

  int count=0;

  while(file.good()){
    if(count<5){
      getline(file,first_5_lines,'\n');
    }
    else{
      file >> time >> v1 >> v2 >> coll_curr >> u2 >> freq;
      curr_arr[count-5]=coll_curr;
      u2_arr[count-5]=u2;
    }
    count++;
  }


  //GRAPHS
  TCanvas *c1 = new TCanvas();
  c1->SetTickx();
  c1->SetTicky();

  //Error Bars of U2 and I
  float sx[MAX_LEN], sy[MAX_LEN];

  for(int i=0; i<MAX_LEN; i++){
    sx[i] = 0.15;
    sy[i] = 0.005;
  }

  TGraphErrors *gr = new TGraphErrors(MAX_LEN, u2_arr, curr_arr, sx, sy);

  gr->SetMarkerColor(kBlue);
  gr->SetLineWidth(1);
  gr->SetTitle("Collector Current (I) - U2 Voltage (V)");
  gr->GetXaxis()->SetTitle("U2 (V)");
  gr->GetYaxis()->SetTitle("I (A)");
  gr->Draw("A*");

  //Fitting Gaussians to peaks, pikced the ranges with eye.

  float adj_par = 0.3;


  TF1 *gauss_1 = new TF1("gauss_1", "gaus", 8.7-adj_par, 10.7-adj_par);
  gauss_1->SetLineWidth(3);
  gauss_1->SetLineColor(kGreen);
  gr->Fit(gauss_1, "R");

  TF1 *gauss_2 = new TF1("gauss_2", "gaus", 14.-adj_par, 15.50-adj_par);
  gauss_2->SetLineWidth(3);
  gauss_2->SetLineColor(kGreen);
  gr->Fit(gauss_2, "R+");

  TF1 *gauss_3 = new TF1("gauss_3", "gaus", 18.7-adj_par, 20.8-adj_par); 
  gauss_3->SetLineWidth(3);
  gauss_3->SetLineColor(kGreen);
  gr->Fit(gauss_3, "R+");

  TF1 *gauss_4 = new TF1("gauss_4", "gaus", 23.9-adj_par, 25.9-adj_par);
  gauss_4->SetLineWidth(3);
  gauss_4->SetLineColor(kGreen);
  gr->Fit(gauss_4, "R+");

  TF1 *gauss_5 = new TF1("gauss_5", "gaus", 29.3-adj_par, 31.3-adj_par);
  gauss_5->SetLineWidth(3);
  gauss_5->SetLineColor(kGreen);
  gr->Fit(gauss_5, "R+");

  //Get the means and sigma values of the Gaussians; and their uncertainities.

  float gauss_0_mean = gauss_0->GetParameter(1);
  float gauss_0_sigma =gauss_0->GetParameter(2);
  float gauss_0_mean_un = gauss_0->GetParError(1);
  float gauss_0_sigma_un = gauss_0->GetParError(2);

  float gauss_1_mean = gauss_1->GetParameter(1);
  float gauss_1_sigma =gauss_1->GetParameter(2);
  float gauss_1_mean_un = gauss_1->GetParError(1);
  float gauss_1_sigma_un = gauss_1->GetParError(2);

  float gauss_2_mean = gauss_2->GetParameter(1);
  float gauss_2_sigma =gauss_2->GetParameter(2);
  float gauss_2_mean_un = gauss_2->GetParError(1);
  float gauss_2_sigma_un = gauss_2->GetParError(2);

  float gauss_3_mean = gauss_3->GetParameter(1);
  float gauss_3_sigma =gauss_3->GetParameter(2);
  float gauss_3_mean_un = gauss_3->GetParError(1);
  float gauss_3_sigma_un = gauss_3->GetParError(2);

  float gauss_4_mean = gauss_4->GetParameter(1);
  float gauss_4_sigma =gauss_4->GetParameter(2);
  float gauss_4_mean_un = gauss_4->GetParError(1);
  float gauss_4_sigma_un = gauss_4->GetParError(2);

  float gauss_5_mean = gauss_5->GetParameter(1);
  float gauss_5_sigma =gauss_5->GetParameter(2);
  float gauss_5_mean_un = gauss_5->GetParError(1);
  float gauss_5_sigma_un = gauss_5->GetParError(2);

  //Find the difference 
  float delta_E0 = gauss_1_mean - gauss_0_mean;
  float delta_E1 = gauss_2_mean - gauss_1_mean;
  float delta_E2 = gauss_3_mean - gauss_2_mean;
  float delta_E3 = gauss_4_mean - gauss_3_mean;
  float delta_E4 = gauss_5_mean - gauss_4_mean;

  //Find the uncertainty

  float sigma_E0 = sqrt(pow(gauss_0_mean_un,2)+pow(gauss_1_mean_un,2));
  float sigma_E1 = sqrt(pow(gauss_1_mean_un,2)+pow(gauss_2_mean_un,2));
  float sigma_E2 = sqrt(pow(gauss_2_mean_un,2)+pow(gauss_3_mean_un,2));
  float sigma_E3 = sqrt(pow(gauss_3_mean_un,2)+pow(gauss_4_mean_un,2));
  float sigma_E4 = sqrt(pow(gauss_4_mean_un,2)+pow(gauss_5_mean_un,2));
  
  //Find the weight
  float delta_E_w0 = 1 / pow(sigma_E0,2);
  float delta_E_w1 = 1 / pow(sigma_E1,2);
  float delta_E_w2 = 1 / pow(sigma_E2,2);
  float delta_E_w3 = 1 / pow(sigma_E3,2);
  float delta_E_w4 = 1 / pow(sigma_E4,2);

  float delta_E_wsum = delta_E_w0 + delta_E_w1 + delta_E_w2 + delta_E_w3 + delta_E_w4;

  //Find the final delta E
  float delta_E = (delta_E0*delta_E_w0 + delta_E1*delta_E_w1 + delta_E2*delta_E_w2 + delta_E3*delta_E_w3 + delta_E4*delta_E_w4) / delta_E_wsum;

  //Find the final sigma E
  float sigma_E = sqrt(1/delta_E_wsum);
  
  //PRINTING PART
  
  cout << "Gauss 0 Mean: " << gauss_0_mean << " +/- " << gauss_0_mean_un << endl;
  cout << "Gauss 1 Mean: " << gauss_1_mean << " +/- " << gauss_1_mean_un << endl;
  cout << "Gauss 2 Mean: " << gauss_2_mean << " +/- " << gauss_2_mean_un << endl;
  cout << "Gauss 3 Mean: " << gauss_3_mean << " +/- " << gauss_3_mean_un << endl;
  cout << "Gauss 4 Mean: " << gauss_4_mean << " +/- " << gauss_4_mean_un << endl;
  cout << "Gauss 5 Mean: " << gauss_5_mean << " +/- " << gauss_5_mean_un << endl;

  cout << "Delta E0: " << delta_E0 << endl;
  cout << "Delta E1: " << delta_E1 << endl;
  cout << "Delta E2: " << delta_E2 << endl;
  cout << "Delta E3: " << delta_E3 << endl;
  cout << "Delta E4: " << delta_E4 << endl;

  cout << "Sigma E0: " << sigma_E0 << endl;
  cout << "Sigma E1: " << sigma_E1 << endl;
  cout << "Sigma E2: " << sigma_E2 << endl;
  cout << "Sigma E3: " << sigma_E3 << endl;
  cout << "Sigma E4: " << sigma_E4 << endl;

  cout << "Weighted mean of delta E values: " << delta_E << endl;

  cout << "Mean of sigma E values: " << sigma_E << endl;
}
