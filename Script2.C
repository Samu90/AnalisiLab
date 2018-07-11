Double_t Area(int t,int delta, Double_t* vec){
  Double_t A=0;
  
  for(int i=t;i<t+delta;i++){      
	if(vec[i]<0){
	  A += (-(vec[i]));
	}//chiudo if
      }//chiudo for i
  return A;

}

Double_t IncArea(Double_t*vec, int t0, int delta, Double_t sigma){
  Double_t Temp[2000], Aree[10000], stdev;
  int i;
  
  for(i=0;i<2000;i++){ Temp[i]=0;}
   for(i=0;i<10000;i++){ Aree[i]=0;}

  TRandom* rand= new TRandom();
  
  for(int j=0;j<10000;j++){
    for(int i=t0;i<t0+delta;i++){
      Temp[i]=rand->Gaus(vec[i],sigma);  
    }//chiudo for t0
    Aree[j]=Area(t0,delta,Temp);
    //    cout<<"Area"<<Aree[j]<<endl;
  }//chiudo for j

  stdev=TMath::RMS(10000,Aree);
  cout<< "dev=="<< stdev<< endl;
  return stdev;

  
}



Double_t MediaFormeEffScript(const char* fileName,Double_t *var)
{
    // apre file prende il TTree di nome "newtree" dal file
    TFile* file = new TFile(fileName);
    TTree* tree = (TTree*)file->Get("newtree");

    //    TFile* fileout = new TFile(fileName2, "RECREATE");
    //TTree* datatree = new TTree("datatree", "alberomedie");
    
    // setta indirizzi delle variabili di interesse
    Double_t ch0[1000],ch2[1000],ch4[1000],ch6[1000];
    Int_t Nevent, Nsample, entrate;
    Int_t t0,t2,t4,t6;
    Int_t t0sc,t2sc,t4sc,t6sc;
    Double_t taufit, AmpS, AmpC;
    Double_t SAreaC, SAreaS;
    
    tree->SetBranchAddress("v0",&ch0);
    tree->SetBranchAddress("v2",&ch2);
    tree->SetBranchAddress("v4",&ch4);
    tree->SetBranchAddress("v6",&ch6);
	
    tree->SetBranchAddress("Nevent", &Nevent);
    tree->SetBranchAddress("Nsample",&Nsample);

    tree->SetBranchAddress("t0",&t0);
    tree->SetBranchAddress("t2",&t2);
    tree->SetBranchAddress("t4",&t4);
    tree->SetBranchAddress("t6",&t6);
    

    tree->SetBranchAddress("t0sc",&t0sc);
    tree->SetBranchAddress("t2sc",&t2sc);
    tree->SetBranchAddress("t4sc",&t4sc);

    //datatree->Branch("nsample",&Nsample,"nsample/I");
    //datatree->Branch("nevent",&Nevent,"nevent/I");
    
    
    
    
    Double_t v0[1000],v2[1000],v4[1000],v6[1000];
    Double_t temp0[1000],temp2[1000],temp4[1000],temp6[1000],time[1000];   //Arrey su cui vanno fatte le somme 
    int i,j;
    int timeLow=90;


    //datatree->Branch("ch0",temp0,"ch0[nsample]/D");
    //datatree->Branch("ch2",temp2,"ch2[nsample]/D");
    //datatree->Branch("ch4",temp4,"ch4[nsample]/D");
    //datatree->Branch("ch6",temp6,"ch6[nsample]/D");

    
    //azzero i temporanei
    for(i=0;i<Nsample;i++){
	temp0[i]=0;
	temp2[i]=0;
	temp4[i]=0;
	temp6[i]=0;
	time[i]=i;
      }

      //inizilizzo tempi di sincronizzazione
   
      entrate=tree->GetEntries();
      
    for(j=0;j<entrate;j++){
      tree->GetEntry(j);

      for(i=0;i<Nsample;i++){
	v0[i]=ch0[i];
	v2[i]=ch2[i];
	v4[i]=ch4[i];
	v6[i]=ch6[i];
      }
   
    //sommo su tutti i V(i)
      for(i=t6-20; i<Nsample-1; i++) {
	temp6[100+i-t6]+=v6[i];}
      for(i=t4-20; i<Nsample-1; i++){
	temp4[100+i-t4]+=v4[i];}
      for(i=t0-20;i<Nsample-1;i++){
	temp0[100+i-t0]+=v0[i];}
      for(i=t2-20;i<Nsample-1;i++){
	temp2[100+i-t2]+=v2[i];
      }
      
      
      if( j%10 == 0 ) cout<< "Entry: " << j << " / " << entrate <<endl;
    }// chiudo for j
    
    
    // riscalo
    for(i=0;i<Nsample;i++){
      temp0[i]/=(entrate);
      temp2[i]/=(entrate);
      temp4[i]/=(entrate);
      temp6[i]/=(entrate);
    }// chiudo for i

 
    
    cout<<"vdsg"<<Nsample<<endl;
    //datatree->Fill();
    
    
    //TCanvas *cha0 = new TCanvas("cha0", "Canale 0");
    //cha0->Divide(2,2);
    //cha0->cd(1);

    TGraph *antonio = new TGraph(Nsample, time, temp0);
    antonio->SetTitle("Canale 0");
    //antonio->Draw();

    TF1 *myf = new TF1("myf","[0]*exp(-x/[1])",140,940,"Q");
    myf->SetParameter(1,300);
    myf->SetParameter(0,-100);
    antonio->Fit("myf","RV");
    gStyle->SetOptFit();
    antonio->SetTitle("Scintillazione");
    //antonio->Draw("APL");
    AmpS=myf->GetParameter(0);
    taufit=myf->GetParameter(1);
    
    //cha0->cd(2);
    TGraph *gigetto = new TGraph(Nsample, time, temp2);
    gigetto->SetTitle("Canale 2");
//  gigetto->SetMinimum(-400);
//  gigetto->SetMaximum(50);
    gigetto->SetTitle("Cherenkov con Scintillazione");
    //gigetto->Draw();

    TF1 *myf2 = new TF1("myf2","[0]*exp(-x/[1])",110,940,"Q");
    myf2->SetParameter(0,1);
    myf2->FixParameter(1,taufit);
    gigetto->Fit("myf2","RV");
    gStyle->SetOptFit();
    //gigetto->Draw("APL");
    AmpC=myf2->GetParameter(0);

    //calcolo i t0
    int ctrl0=0;
    int ctrl2=0;
    int Nrms=0;
    int nt0=0;
    int nt2=0;
    
    
    
    for(i=0;i<Nsample;i++){
      
      if(ctrl0==0 && temp0[i]<-0.3 && temp0[i+1]<-0.3){ //&& temp0[i+1]<-Nrms*rms0 && temp0[i+2]<-Nrms*rms0){
	ctrl0+=1;
	nt0=i;
      }//chiudo if
      if(ctrl2==0 && temp2[i]<-0.3 && temp2[i+1]<-0.3){ //&& temp2[i+1]<-Nrms*rms2 && temp2[i+2]<-Nrms*rms2){
	ctrl2+=1;
	nt2=i;
      }//chiudo if 
    }//chiudo for
    
    
    // riscalo e sottraggo
    for(i=0;i<Nsample;i++){
        temp2[i] -= (AmpC/AmpS * temp0[i]);
    }// chiudo for i
    
    Double_t m=0.2398;
    Double_t sigma=1/m/1.732051;//sqrt(tree->GetEntries());                    // 1/m/sqrt(3)/sqrt(N) statistico
    //ora ho v0 e v2 corrette
    //rapporto tra le aree
    //calcolo i t0
    
    cout<<nt0<< "  "<< nt2<<"  " << nt2+18<<endl; 
    //calcolo le aree

    Double_t AreaC,AreaS;
    AreaC=Area(nt2,18,temp2);  
    AreaS=Area(nt0,945-nt0,temp0);

    SAreaC=IncArea(temp2,nt2,18,sigma);
    SAreaS=IncArea(temp0,nt0,945-nt0,sigma);

    
    cout<< AreaS<< "   " <<AreaC<< endl;
    cout<< AreaC/AreaS<< endl;

    Double_t SigmaTot=sqrt((1/AreaS)*(1/AreaS)*SAreaC*SAreaC + (AreaC/AreaS/AreaS)*(AreaC/AreaS/AreaS)*SAreaS*SAreaS);
    //    Double_t SigmaTot= (SAreaC/AreaC+SAreaS/AreaS)*AreaC/AreaS;
    *var=SigmaTot;
    
    /*cha0->cd(3);
      TGraph *pablo = new TGraph(Nsample, time, temp4);
      pablo->SetTitle("Canale 4");
      //  pablo->SetMinimum(-250);
      //  pablo->SetMaximum(50);
      pablo->Draw();*/
      
      
      /*cha0->cd(4);
      TGraph *antoniocalabro = new TGraph(Nsample, time, temp2);
      antoniocalabro->SetTitle("Cherenkov senza Scintillazione");
      //  antoniocalabro->SetMinimum(-250);
      //  antoniocalabro->SetMaximum(50);
      antoniocalabro->Draw();


       Double_t scal0[1000];
      
      for(i=0;i<Nsample-2;i++){
       scal0[i] = (AmpC/AmpS*temp0[i]);
      }// chiudo for i
     
       TGraph *pippo= new TGraph(Nsample, time,scal0);
      cha0->cd(2);
      //pippo->SetLineColorAlpha(46, 0.1);
      pippo->Draw("same");
      */
      //fileout->cd();
      //datatree->Write();
      //fileout->Close();
      
      
      file->Close();

      return AreaC/AreaS;

     gPad->WaitPrimitive(); 
}


void Script2(){

  Double_t y[8];
  Double_t x[8];
  Double_t e[8];
  Double_t ex[8];

  for(int i=0;i<8;i++){ ex[i]=1;}
  
  y[0]=33;
  y[1]=20;
  y[2]=10;
  y[3]=0;
  y[4]=-10;
  y[5]=-20;
  y[6]=-33;
  y[7]=-40;

  //Double_t ex[8]={0};
  
  //x[0]=MediaFormeEffScript("Sel/out33sel.root",&e[0]);
  x[0]=MediaFormeEffScript("Sel/out33sel.root",&e[0]);
  x[1]=MediaFormeEffScript("Sel/out20sel.root",&e[1]);
  x[1]=MediaFormeEffScript("Sel/out20sel.root",&e[1]);
  x[2]=MediaFormeEffScript("Sel/out10sel.root",&e[2]);
  x[2]=MediaFormeEffScript("Sel/out10sel.root",&e[2]);
  x[3]=MediaFormeEffScript("Sel/out0sel.root",&e[3]);
  x[3]=MediaFormeEffScript("Sel/out0sel.root",&e[3]);
  x[4]=MediaFormeEffScript("Sel/outm10sel.root",&e[4]);
  x[4]=MediaFormeEffScript("Sel/outm10sel.root",&e[4]);
  x[5]=MediaFormeEffScript("Sel/outm20sel.root",&e[5]);
  x[5]=MediaFormeEffScript("Sel/outm20sel.root",&e[5]);
  x[6]=MediaFormeEffScript("Sel/outm33sel.root",&e[6]);
  x[6]=MediaFormeEffScript("Sel/outm33sel.root",&e[6]);
  x[7]=MediaFormeEffScript("Sel/outm40sel.root",&e[7]);
  x[7]=MediaFormeEffScript("Sel/outm40sel.root",&e[7]);

  cout<<""<<endl;
  for(int i=0;i<8;i++){cout<< x[i]<< " pm " << e[i] << endl;}
  
  TCanvas* canv= new TCanvas("mycanvas");
   TGraphErrors* grafico=new TGraphErrors(8,y,x,ex,e);
  grafico->SetMarkerStyle(8);
  grafico->GetXaxis()->SetTitle("#theta (gradi)");
  grafico->GetYaxis()->SetTitle("Ac/As");
  grafico->Draw("ap");
  


}
