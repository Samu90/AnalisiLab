void Histodati(const char* fileName){

    TFile* file = new TFile(fileName);
    TTree* newtree2 = (TTree*)file->Get("newtree");

    //TFile* file2 = new TFile("RisFit.root","RECREATE");
    //TTree* fittree = new TTree("mytree3","");
    
    Double_t v0[4096], v2[4096], v4[4096], v6[4096], time[4096];  //array dati calibrati
    Double_t rms0=0,rms2=0,rms4=0,rms6=0;                         //rms
    Int_t t0,t2,t4,t6;                                         //tempo inizio salita
    Double_t A10,A12,A14,A16,A20,A22,A24,A26;                     //aree fino al tempo tH1 e tH2
    Double_t tau10,tau12,tau14,tau16;                              // tau discesa normalizzato ad A1
    Double_t tau20,tau22,tau24,tau26;                              // tau discesa normalizzato ad A2
    Int_t t0sc,t2sc,t4sc; 
    int i,j;

    
    
    newtree2->SetBranchAddress("v0",v0);
    newtree2->SetBranchAddress("v2",v2);
    newtree2->SetBranchAddress("v4",v4);
    newtree2->SetBranchAddress("v6",v6);	
    
    newtree2->SetBranchAddress("rms0",&rms0);
    newtree2->SetBranchAddress("rms2",&rms2);
    newtree2->SetBranchAddress("rms4",&rms4);
    newtree2->SetBranchAddress("rms6",&rms6);

    newtree2->SetBranchAddress("t0",&t0);
    newtree2->SetBranchAddress("t2",&t2);
    newtree2->SetBranchAddress("t4",&t4);
    newtree2->SetBranchAddress("t6",&t6);

    newtree2->SetBranchAddress("t0sc",&t0sc);
    newtree2->SetBranchAddress("t2sc",&t2sc);
    newtree2->SetBranchAddress("t4sc",&t4sc);
    
    newtree2->SetBranchAddress("A10",&A10);
    newtree2->SetBranchAddress("A12",&A12);
    newtree2->SetBranchAddress("A14",&A14);
    newtree2->SetBranchAddress("A16",&A16);
    newtree2->SetBranchAddress("A20",&A20);
    newtree2->SetBranchAddress("A22",&A22);
    newtree2->SetBranchAddress("A24",&A24);
    newtree2->SetBranchAddress("A26",&A26);

    newtree2->SetBranchAddress("tau10",&tau10);
    newtree2->SetBranchAddress("tau12",&tau12);
    newtree2->SetBranchAddress("tau14",&tau14);
    newtree2->SetBranchAddress("tau16",&tau16);

    newtree2->SetBranchAddress("tau20",&tau20);
    newtree2->SetBranchAddress("tau22",&tau22);
    newtree2->SetBranchAddress("tau24",&tau24);
    newtree2->SetBranchAddress("tau26",&tau26);

    int nbin=40;
    //t0
    TH1D* h0= new TH1D("histo t0","",nbin,0.,200);
    TH1D* h2= new TH1D("histo t2","",nbin,0.,200);
    TH1D* h4= new TH1D("histo t4","",nbin,0.,200);
    TH1D* h6= new TH1D("histo t6","",nbin,0.,200);
      
      for(i=0;i<newtree2->GetEntries();i++){
	newtree2->GetEntry(i);
	h0->Fill(t0);
	h2->Fill(t2);
	h4->Fill(t4);
	h6->Fill(t6);
      }

      TCanvas* graf = new TCanvas("mycanvas","");
      graf->Divide(2,2);
      graf->cd(1);
      h0->Draw();
      graf->cd(2);
      h2->Draw();
      graf->cd(3);
      h4->Draw();
      graf->cd(4);
      h6->Draw();
      //t0 scalati
      TH1D* s0= new TH1D("histo t0sc","",nbin*4,-200,30);
      TH1D* s2= new TH1D("histo t2sc","",nbin*4,-200,30);
    TH1D* s4= new TH1D("histo t4sc","",nbin*4,-200,30);

      
      for(i=0;i<newtree2->GetEntries();i++){
	newtree2->GetEntry(i);
	s0->Fill(t0sc);
	s2->Fill(t2sc);
	s4->Fill(t4sc);

      }

      //s0->Fit("gaus");
      //s2->Fit("gaus");

      
      TCanvas* grafsc = new TCanvas("mycanvas6","");
      grafsc->Divide(2,2);
      grafsc->cd(1);
      s0->Draw();
      grafsc->cd(2);
      s2->Draw();
      grafsc->cd(3);
      s4->Draw();

      //tau1
    TH1D* g0= new TH1D("histo tau0","",2*nbin,0.,200);
    TH1D* g2= new TH1D("histo tau2","",2*nbin,0.,200);
    TH1D* g4= new TH1D("histo tau4","",2*nbin,0.,200);
    TH1D* g6= new TH1D("histo tau6","",2*nbin,0.,200);
      
      for(i=0;i<newtree2->GetEntries();i++){
	newtree2->GetEntry(i);
	g0->Fill(tau10);
	g2->Fill(tau12);
	g4->Fill(tau14);
	g6->Fill(tau16);
      }

      TCanvas* graftau = new TCanvas("mycanvas2","");
      graftau->Divide(2,2);
      graftau->cd(1);
      g0->Draw();
      graftau->cd(2);
      g2->Draw();
      graftau->cd(3);
      g4->Draw();
      graftau->cd(4);
      g6->Draw();


      //tau2
    TH1D* p0= new TH1D("histo2 tau0","",2*nbin,0.,200);
    TH1D* p2= new TH1D("histo2 tau2","",2*nbin,0.,200);
    TH1D* p4= new TH1D("histo2 tau4","",2*nbin,0.,200);
    TH1D* p6= new TH1D("histo2 tau6","",2*nbin,0.,200);
      
      for(i=0;i<newtree2->GetEntries();i++){
	newtree2->GetEntry(i);
	p0->Fill(tau20);
	p2->Fill(tau22);
	p4->Fill(tau24);
	p6->Fill(tau26);
      }

      TCanvas* graftau2 = new TCanvas("mycanvas3","");
      graftau2->Divide(2,2);
      graftau2->cd(1);
      p0->Draw();
      graftau2->cd(2);
      p2->Draw();
      graftau2->cd(3);
      p4->Draw();
      graftau2->cd(4);
      p6->Draw();

      //aree
      TH1D* q0= new TH1D("histo0 A1","",nbin*6,0.,400);
      TH1D* q2= new TH1D("histo2 A1","",nbin*6,0.,2200);
      TH1D* q4= new TH1D("histo4 A1","",nbin*6,0.,10000);
      TH1D* q6= new TH1D("histo6 A1","",nbin*6,0.,400);
      
      for(i=0;i<newtree2->GetEntries();i++){
	newtree2->GetEntry(i);
	q0->Fill(A10);
	q2->Fill(A12);
	q4->Fill(A14);
	q6->Fill(A16);
      }

      TCanvas* grafA = new TCanvas("mycanvas4","");
      grafA->Divide(2,2);
      grafA->cd(1);
      q0->Draw();
      grafA->cd(2);
      q2->Draw();
      grafA->cd(3);
      q4->Draw();
      grafA->cd(4);
      q6->Draw();

      TH1D* z0= new TH1D("histo0 A2","",nbin*6,0.,400);                                                                                                                                                                               
      TH1D* z2= new TH1D("histo2 A2","",nbin*6,0.,2200);                                                                                                                                                                                 
      TH1D* z4= new TH1D("histo4 A2","",nbin*6,0.,10000);                                                                                                                                                                                 
      TH1D* z6= new TH1D("histo6 A2","",nbin*6,0.,400);
      
      for(i=0;i<newtree2->GetEntries();i++){
	
        newtree2->GetEntry(i);                                                                                                                                                                                                            
        z0->Fill(A10);                                                                                                                                                                                                                    
        z2->Fill(A12);                                                                                                                                                                                                                    
        z4->Fill(A14);                                                                                                                                                                                                                    
        z6->Fill(A16);                                                                                                                                                                                                                    
      }
      
      TCanvas* grafz = new TCanvas("mycanvasn","");                                                                                                                                                                                       
      grafz->Divide(2,2);                                                                                                                                                                                                                 
      grafz->cd(1);                                                                                                                                                                                                                       
      z0->Draw();                                                                                                                                                                                                                         
      grafz->cd(2);                                                                                                                                                                                                                       
      z2->Draw();                                                                                                                                                                                                                         
      grafz->cd(3);                                                                                                                                                                                                                       
      z4->Draw();                                                                                                                                                                                                                         
      grafz->cd(4);                                                                                                                                                                                                                       
      z6->Draw(); 
}
