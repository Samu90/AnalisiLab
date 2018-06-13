void HistoFitUniv(const char* fileName, int debug){

    TFile* file = new TFile(fileName);
    TTree* newtree2 = (TTree*)file->Get("newtree");

    /*TFile* file2 = new TFile("RisFit.root","RECREATE");
      TTree* fittree = new TTree("mytree3","");*/
    
    Double_t v0[4096], v2[4096], v4[4096], v6[4096], time[4096];  //array dati calibrati
    Double_t rms0=0,rms2=0,rms4=0,rms6=0;                         //rms
    Int_t t0,t2,t4,t6;                                         //tempo inizio salita
    Double_t A10,A12,A14,A16,A20,A22,A24,A26;                     //aree fino al tempo tH1 e tH2
    Double_t tau10,tau12,tau14,tau16;                              // tau discesa normalizzato ad A1
    Double_t tau20,tau22,tau24,tau26;                              // tau discesa normalizzato ad A2
    Double_t meanH0, rmsH0,meanH2,rmsH2;
    Int_t t0sc,t2sc,t4sc; 
    int i,j;

    //fittree->Branch("meanH0",&meanH0,"meanH0/D");
    //fittree->Branch("rmsH0",&rmsH0,"rmsH0/D");
    //fittree->Branch("meanH2",&meanH2,"meanH2/D");
    //fittree->Branch("rmsH2",&rmsH2,"rmsH2/D");
    
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
    int grafici=0;
    //TCut* taglioA= new TCut("A10>0 && A12>0");
    //TCut* taglioS=new TCut("A20<300000 && t0sc>-140 && tau20<370 && tau20>280 && A20>3000");
    //TCut* taglioC=new TCut("tau22<190 && A22>90 && t2sc>-140");

    TCut* taglioA= new TCut("");
    TCut* taglioS=new TCut("A20<300000 && t0sc>-140 && t0sc<-48 && A20>160 && (A20-A10)>100 && hmax0<=37");
    TCut* taglioC=new TCut("tau22<197 && t0-t2<7 && t0-t2>-7 && A12>10 && t2>0 && t2sc<-40 && hmax2<=26");
    

    /*TCut* taglioAf= new TCut("t0sc>-74 && t0sc<-46");
    TCut* taglioCf=new TCut("A22<12000 && tau22>=20 && t2sc<-140");
    TCut* taglioSf=new TCut("A20<300000");*/
    
    

    //t0
    
    if(grafici==1){
    TCanvas* graf = new TCanvas("mycanvas","",1200,800);
    graf->Divide(2,2);
    graf->cd(1);
    newtree2->Draw("t0",*taglioA && *taglioS);
    graf->cd(2);
    newtree2->Draw("t2",*taglioA && *taglioC);
    graf->cd(3);
    newtree2->Draw("t4",*taglioA);
    graf->cd(4);
    newtree2->Draw("t6",*taglioA);
    
    
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
    
    TCanvas* grafsc = new TCanvas("mycanvas6","",1200,800);
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
    
    //fittree->Fill();
    
    TCanvas* graftsc = new TCanvas("mycanvassc","",1200,800);
    graftsc->Divide(2,2);
    graftsc->cd(1);
    newtree2->Draw("t0sc",*taglioA && *taglioS);
    graftsc->cd(2);
    newtree2->Draw("t2sc",*taglioA && *taglioC);
    graftsc->cd(3);
    newtree2->Draw("t4sc",*taglioA);
    
    
    
    //tau1
    
    
    TCanvas* graftau = new TCanvas("mycanvas2","",1200,800);
    graftau->Divide(2,2);
    graftau->cd(1);
    newtree2->Draw("tau10",*taglioA && *taglioS);
    graftau->cd(2);
    newtree2->Draw("tau12",*taglioA && *taglioC);
    graftau->cd(3);
    newtree2->Draw("tau14",*taglioA);
    graftau->cd(4);
    newtree2->Draw("tau16",*taglioA);
    
    
    
    
    
    
    //tau2
    
    TCanvas* graftau2 = new TCanvas("mycanvas3","",1200,800);
    graftau2->Divide(2,2);
    graftau2->cd(1);
    newtree2->Draw("tau20",*taglioA && *taglioS);
    graftau2->cd(2);
    newtree2->Draw("tau22",*taglioA && *taglioC);
    graftau2->cd(3);
    newtree2->Draw("tau24",*taglioA);
    graftau2->cd(4);
    newtree2->Draw("tau26",*taglioA);
    
    
    
    
    
    
    //aree
    
    TCanvas* grafA = new TCanvas("mycanvas4","",1200,800);
    grafA->Divide(2,2);
    grafA->cd(1);
    newtree2->Draw("A10",*taglioA && *taglioS);
    grafA->cd(2);
    newtree2->Draw("A12",*taglioA && *taglioC);
    grafA->cd(3);
    newtree2->Draw("A14",*taglioA);
    grafA->cd(4);
    //newtree2->Draw("A16",*taglioA);
    
    
    
    //aree2
    
    TCanvas* grafz = new TCanvas("mycanvasn","",1200,800);                                                                                                                                                                                       
    grafz->Divide(2,2);                                                                                                                                                                                                                 
    grafz->cd(1);
    newtree2->Draw("A20",*taglioA && *taglioS);
    grafz->cd(2);
    newtree2->Draw("A22",*taglioA && *taglioC);                                                                                                                                                                                                  
    grafz->cd(3);                                                                                                                                                                                                                      
    newtree2->Draw("A24",*taglioA);
    grafz->cd(4);
    }//chudo l'if grafici

    
    //newtree2->Draw("A26",*taglioA);

    //newtree2->Scan("A10:A20:tau20:t0sc",*taglioA && *taglioS);
    // newtree2->Scan("A12:A22:A22-A12:tau22:t2sc",*taglioA && *taglioC);
    
    //file2->cd();
    //fittree->Write();
    //gPad->WaitPrimitive();
    //file2->Close();


    if(debug==0){
      TFile* newf = TFile::Open("Sel/outm33Ssel.root", "recreate");
      TTree* treeS = newtree2->CopyTree(*taglioS);
      newf->cd();
      treeS->Write();
      newf->Close();
      
      TFile* newf2 = TFile::Open("Sel/outm33Csel.root", "recreate");
      TTree* treeC = newtree2->CopyTree(*taglioC);
      newf2->cd();
      treeC->Write();
      newf2->Close();
      
      TFile* newf3 = TFile::Open("Sel/outm33sel.root", "recreate");
      TTree* treeCS = newtree2->CopyTree(*taglioC && *taglioS);
      newf3->cd();
      treeCS->Write();
      newf3->Close();
    }
    
    if(debug==1){
      TFile* newf = TFile::Open("Sel/noutm33Ssel.root", "recreate");
      TTree* treeS = newtree2->CopyTree("A10>0" && !*taglioS);
      newf->cd();
      treeS->Write();
      newf->Close();
      
      TFile* newf2 = TFile::Open("Sel/noutm33Csel.root", "recreate");
      TTree* treeC = newtree2->CopyTree("A12>0" && !*taglioC);
      newf2->cd();
      treeC->Write();
      newf2->Close();
      
      TFile* newf3 = TFile::Open("Sel/noutm33sel.root", "recreate");
      TTree* treeCS = newtree2->CopyTree("A10>0 && A12>0" && !*taglioC && !*taglioS);
      newf3->cd();
      treeCS->Write();
      newf3->Close();
    }
    
}
