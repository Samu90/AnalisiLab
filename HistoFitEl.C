void HistoFitEl(const char* fileName){

    TFile* file = new TFile(fileName);
    TTree* newtree2 = (TTree*)file->Get("newtree");

    TFile* file2 = new TFile("RisFit.root","RECREATE");
    TTree* fittree = new TTree("mytree3","");
    
    Double_t v0[4096], v2[4096], v4[4096], v6[4096], time[4096];  //array dati calibrati
    Double_t rms0=0,rms2=0,rms4=0,rms6=0;                         //rms
    Int_t t0,t2,t4,t6;                                         //tempo inizio salita
    Double_t A10,A12,A14,A16,A20,A22,A24,A26;                     //aree fino al tempo tH1 e tH2
    Double_t tau10,tau12,tau14,tau16;                              // tau discesa normalizzato ad A1
    Double_t tau20,tau22,tau24,tau26;                              // tau discesa normalizzato ad A2
    Double_t meanH0, rmsH0,meanH2,rmsH2;
    Int_t t0sc,t2sc,t4sc; 
    int i,j;

    fittree->Branch("meanH0",&meanH0,"meanH0/D");
    fittree->Branch("rmsH0",&rmsH0,"rmsH0/D");
    fittree->Branch("meanH2",&meanH2,"meanH2/D");
    fittree->Branch("rmsH2",&rmsH2,"rmsH2/D");

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
    // TCut* tagliot2sc= new TCut("t2sc>-80 && t2sc<-35");
    TCut* taglioBoth= new TCut("A10>0 && A12>0 && A14>0 && A16>0 && A10<200000 && t0sc>-140 && t0sc<-47 && t0!=0 && t2!=0 && t4!=0");
    TCut* taglioScint= new TCut("A10/A20 != 1");
    TCut* taglioCher= new TCut("t2sc < -40 && t2sc > -150 && t0-t2>-4");
    

    //t0

    TCanvas* graf = new TCanvas("mycanvas","");
    graf->Divide(2,2);
    graf->cd(1);
    newtree2->Draw("t0",*taglioBoth && *taglioScint);
    graf->cd(2);
    newtree2->Draw("t2",*taglioBoth && *taglioCher);
    graf->cd(3);
    newtree2->Draw("t4",*taglioBoth);
    graf->cd(4);
    newtree2->Draw("t6",*taglioBoth);
    
    
    //t0 scalati
    
    TH1D* s0= new TH1D("histo t0sc","",nbin,-80,-35);
    TH1D* s2= new TH1D("histo t2sc","",nbin,-80,-40);
    TH1D* s4= new TH1D("histo t4sc","",nbin*4,-200,30);
    
    for(i=0;i<newtree2->GetEntries();i++){
      newtree2->GetEntry(i);
      s0->Fill(t0sc);
      s2->Fill(t2sc);
      s4->Fill(t4sc);
    }
    
    TCanvas* grafsc = new TCanvas("mycanvas6","");
    //media e deviazione standard degli istogrammi e riempio il tree
    TF1 *fitFcn0 = new TF1("fitFcn0","gaus");
    TF1 *fitFcn2 = new TF1("fitFcn2","gaus");
    
    grafsc->Divide(2,2);
    
    grafsc->cd(1);
    s0->Fit("fitFcn0");
    cout<<"media0:  "<<fitFcn0->GetParameter(1)<<endl;
    cout<<"rms0:  "<<fitFcn0->GetParameter(2)<<endl;
    meanH0=fitFcn0->GetParameter(1);
    rmsH0=fitFcn0->GetParameter(2);
    
    grafsc->cd(2);
    s2->Fit("fitFcn2");
    cout<<"media2:  "<<fitFcn2->GetParameter(1)<<endl;
    cout<<"rms2:  "<<fitFcn2->GetParameter(2)<<endl;
    meanH2=fitFcn2->GetParameter(1);
    rmsH2=fitFcn2->GetParameter(2);
    
    grafsc->cd(3);
    s4->Draw();
    grafsc->cd(4);
    
    fittree->Fill();
    
    TCanvas* graftsc = new TCanvas("mycanvassc","");
    graftsc->Divide(2,2);
    graftsc->cd(1);
    newtree2->Draw("t0sc",*taglioBoth && *taglioScint);
    graftsc->cd(2);
    newtree2->Draw("t2sc",*taglioBoth && *taglioCher);
    graftsc->cd(3);
    newtree2->Draw("t4sc",*taglioBoth);
     
    
    //tau1
    
    TCanvas* graftau = new TCanvas("mycanvas2","");
    graftau->Divide(2,2);
    graftau->cd(1);
    newtree2->Draw("tau10",*taglioBoth && *taglioScint);
    graftau->cd(2);
    newtree2->Draw("tau12",*taglioBoth && *taglioCher);
    graftau->cd(3);
    newtree2->Draw("tau14",*taglioBoth);
    graftau->cd(4);
    newtree2->Draw("tau16",*taglioBoth);
   
    
    //tau2
    
    TCanvas* graftau2 = new TCanvas("mycanvas3","");
    graftau2->Divide(2,2);
    graftau2->cd(1);
    newtree2->Draw("tau20",*taglioBoth && *taglioScint);
    graftau2->cd(2);
    newtree2->Draw("tau22",*taglioBoth && *taglioCher);
    graftau2->cd(3);
    newtree2->Draw("tau24",*taglioBoth);
    graftau2->cd(4);
    newtree2->Draw("tau26",*taglioBoth);
    
    
    // t0-t2
    TCanvas* graft2vst0 = new TCanvas("mycanvas","");
    newtree2->Draw("t0-t2",*taglioBoth && *taglioScint && *taglioCher);

    
    //aree
    
    TCanvas* grafA = new TCanvas("mycanvas4","");
    grafA->Divide(2,2);
    grafA->cd(1);
    newtree2->Draw("A10",*taglioBoth && *taglioScint);
    grafA->cd(2);
    newtree2->Draw("A12",*taglioBoth && *taglioCher);
    grafA->cd(3);
    newtree2->Draw("A14",*taglioBoth);
    grafA->cd(4);
    newtree2->Draw("A16",*taglioBoth);
    
    
    //aree2
    
    TCanvas* grafz = new TCanvas("mycanvasn","");
    grafz->Divide(2,2);
    grafz->cd(1);
    newtree2->Draw("A20",*taglioBoth && *taglioScint);
    grafz->cd(2);
    newtree2->Draw("A22",*taglioBoth && *taglioCher);
    grafz->cd(3);
    newtree2->Draw("A24",*taglioBoth);
    grafz->cd(4);
    newtree2->Draw("A26",*taglioBoth);


    TCanvas* grafy = new TCanvas("mycanvasxx","");
    newtree2->Draw("A10:A12",*taglioBoth && *taglioScint && *taglioCher);

    
    //file2->cd();
    //fittree->Write();
    //gPad->WaitPrimitive();
    //file2->Close();

    //Creo nuovi alberi con eventi che superano tagli
    //Creo albero con eventi che superano il taglio di scintillazione
    
    TFile* f3 = new TFile("ScintBuoni.root","RECREATE");
    TTree* nuovoalbero1 = newtree->CopyTree(*taglioBoth && *taglioScint);
    f3->cd();
    nuovoalbero1->Write();
    f3->Close();

    //Creo albero con eventi che superano il taglio di Cherenkov
    
    TFile* f4 = new TFile("CherBuoni.root","RECREATE");
    TTree* nuovoalbero2 = newtree->CopyTree(*taglioBoth && *taglioCher);
    f4->cd();
    nuovoalbero2->Write();
    f4->Close();

    //Creo albero con eventi che superano tutti i tagli
    
    TFile* f5 = new TFile("Buoni.root","RECREATE");
    TTree* nuovoalbero3 = newtree->CopyTree(*taglioBoth && *taglioScint && *taglioCher);
    f5->cd();
    nuovoalbero3->Write();
    f5->Close();
    
     
  

}

