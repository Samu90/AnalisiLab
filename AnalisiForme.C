//analizza le forme d'onda dei 4 canali e crea un file con un tree con tutte le informazioni sull'onda
//provo con git
//prova2
//prova3
//root[0] .x AnalisiForme.C("nomeFileInput","nomeFileOutput")
Double_t Area(int t,int delta,Double_t rms, Double_t* vec){		
  Double_t A=0;		
  		
  for(int i=t;i<t+delta;i++){      		
	if(vec[i]<-3*rms){		
	  A += (-(vec[i]));		
	}//chiudo if		
      }//chiudo for i		
  return A;		
	
}		

Double_t Tau(int t,int delta,Double_t rms, Double_t* vec,Double_t area){		
   Double_t A=0;		
   if(area>0){		
   for(int i=t;i<t+delta;i++){      		
 	if(vec[i]<-3*rms){		
 	  A += (i*(-(vec[i])));		
 	}//chiudo if		
       }//chiudo for i		
   return A/area;		
   } else {return 0;}		
 		
 }


Double_t Max(Double_t* vec){
    int i;
    Double_t max=0;
    for(i=90;i<200;i++){
        if(vec[i]>max) max=vec[i];
    }
    return max;
}

void AnalisiForme(const char* fileName,const char* fileName2)
{
  gROOT->Reset();

    // apre file prende il TTree di nome "datatree" dal file
    TFile* file = new TFile(fileName);
    TTree* tree = (TTree*)file->Get("datatree");
    
    TFile* elab = new TFile(fileName2,"recreate");
    TTree* newtree = new TTree("newtree","");
    
    // setta indirizzi delle variabili di interesse
    UInt_t ch0[1000],ch2[1000],ch4[1000],ch6[1000];               //array dati
    Int_t Nevent, Nsample;                                        
    Double_t v0[1000], v2[1000], v4[1000], v6[1000], time[1000];  //array dati calibrati
    Double_t rms0=0,rms2=0,rms4=0,rms6=0;                         //rms
    Int_t t0,t2,t4,t6;                                         //tempo inizio salita
    Int_t t0sc,t2sc,t4sc;                                        //tempi scalati con t6
    Double_t A10,A12,A14,A16,A20,A22,A24,A26;                     //aree fino al tempo tH1 e tH2
    Double_t tau10,tau12,tau14,tau16;                              // tau discesa normalizzato ad A1
    Double_t tau20,tau22,tau24,tau26;
    Double_t hmax0, hmax2, hmax4, hmax6;
    // tau discesa normalizzato ad A2
    Int_t ctrl0,ctrl2,ctrl4,ctrl6;                                // controlli 
    Int_t delta1=200,delta2=400;                                    // tempo prima area e seconda area
    int i,j;
    int timeHigh1,timeHigh2;
    Double_t mean0,mean2,mean4, mean6;
    
    timeHigh1=400;
    timeHigh2=800;
    //vecchio albero
    tree->SetBranchAddress("ch0",&ch0);
    tree->SetBranchAddress("ch2",&ch2);
    tree->SetBranchAddress("ch4",&ch4);
    tree->SetBranchAddress("ch6",&ch6);	
    tree->SetBranchAddress("nevent", &Nevent);
    tree->SetBranchAddress("nsample",&Nsample);

      
    
    //nuovo albero
    newtree->Branch("Nsample",&Nsample,"Nsample/I");
    newtree->Branch("Nevent",&Nevent,"Nevent/I");
    
    newtree->Branch("v0",v0,"v0[Nsample]/D");
    newtree->Branch("v2",v2,"v2[Nsample]/D");
    newtree->Branch("v4",v4,"v4[Nsample]/D");
    newtree->Branch("v6",v6,"v6[Nsample]/D");
        
    newtree->Branch("mean0",&mean0);
    newtree->Branch("mean2",&mean2);
    newtree->Branch("mean4",&mean4);
    newtree->Branch("mean6",&mean6);
    
    newtree->Branch("rms0",&rms0);
    newtree->Branch("rms2",&rms2);
    newtree->Branch("rms4",&rms4);
    newtree->Branch("rms6",&rms6);

    newtree->Branch("t0",&t0);
    newtree->Branch("t2",&t2);
    newtree->Branch("t4",&t4);
    newtree->Branch("t6",&t6);

    newtree->Branch("t0sc",&t0sc);
    newtree->Branch("t2sc",&t2sc);
    newtree->Branch("t4sc",&t4sc);


    newtree->Branch("A10",&A10);
    newtree->Branch("A12",&A12);
    newtree->Branch("A14",&A14);
    newtree->Branch("A16",&A16);

    newtree->Branch("A20",&A20);
    newtree->Branch("A22",&A22,"A22/D");
    newtree->Branch("A24",&A24);
    newtree->Branch("A26",&A26);

    newtree->Branch("tau10",&tau10);
    newtree->Branch("tau12",&tau12);
    newtree->Branch("tau14",&tau14);
    newtree->Branch("tau16",&tau16);

    newtree->Branch("tau20",&tau20);
    newtree->Branch("tau22",&tau22);
    newtree->Branch("tau24",&tau24);
    newtree->Branch("tau26",&tau26);
    
    newtree->Branch("hmax0",&hmax0);
    newtree->Branch("hmax2",&hmax2);
    newtree->Branch("hmax4",&hmax4);
    newtree->Branch("hmax6",&hmax6);
    
    Int_t Nrms=5;
    UInt_t timeLow=90;
    Double_t m=0.2398;
    // ciclo sugli N eventi
    int nentries = tree->GetEntries();

    for(int j=0; j<nentries;j++){
      if( j%100 == 0 ) cout<< "Entry: " << j << " / " << nentries <<endl;
      tree->GetEntry(j);
      //newtree->GetEntry(j);
      
      //inizializzo array con fattore di scala
      for(i=0; i<Nsample; i++) {
        v0[i]=(Double_t)ch0[i]/m;
	v2[i]=(Double_t)ch2[i]/m;
	v4[i]=(Double_t)ch4[i]/m;
	v6[i]=(Double_t)ch6[i]/m;
        time[i]=i;
      }//chiudo for(i)
      
      mean0=0;
      mean2=0;
      mean4=0;
      mean6=0;

      //calcolo media
      for(i=0;i<timeLow;i++){
       	mean0+=v0[i];
	mean2+=v2[i];
	mean4+=v4[i];
	mean6+=v6[i];
      }//chiudo for (i)
      
      mean0/=timeLow;
      mean2/=timeLow;
      mean4/=timeLow;
      mean6/=timeLow;

     
      //riscalo valori con piedistallo a zero
      for( i=0; i<Nsample; i++) {
	v0[i]-=mean0;
	v2[i]-=mean2;
	v4[i]-=mean4;
	v6[i]-=mean6;
      }//chiudo for (i)

      //calcolo rms
      rms0=0;
      rms2=0;
      rms4=0;
      rms6=0;
      
      for(i=0;i<timeLow;i++){
	rms0+=v0[i]*v0[i];
	rms2+=v2[i]*v2[i];
	rms4+=v4[i]*v4[i];
	rms6+=v6[i]*v6[i];
      }
      
      rms0/=timeLow;
      rms2/=timeLow;
      rms4/=timeLow;
      rms6/=timeLow;
      
      rms0=sqrt(rms0);
      rms2=sqrt(rms2);
      rms4=sqrt(rms4);
      rms6=sqrt(rms6);

      if(rms0 <0.01){rms0=1/m/sqrt(3);}
      if(rms2 <0.01){rms2=1/m/sqrt(3);}
      if(rms4 <0.01){rms4=1/m/sqrt(3);}
      if(rms6 <0.01){rms6=1/m/sqrt(3);}

      //calcolo tempo inizio salita

      ctrl0=0;
      ctrl2=0;
      ctrl4=0;
      ctrl6=0;

      t0=0; // controllo se non entro negli if
      t2=0;
      t4=0;
      t6=0;
      
      for(i=0;i<Nsample;i++){

	if(ctrl0==0 && v0[i]<-Nrms*rms0 && v0[i+1]<-Nrms*rms0 && v0[i+2]<-Nrms*rms0){
	  ctrl0+=1;
	  t0=i;
	}//chiudo if
	if(ctrl2==0 && v2[i]<-Nrms*rms2 && v2[i+1]<-Nrms*rms2 && v2[i+2]<-Nrms*rms2){
	  ctrl2+=1;
	  t2=i;
	}//chiudo if 
	if(ctrl4==0 && v4[i]<-Nrms*rms4 && v4[i+1]<-Nrms*rms4 && v4[i+2]<-Nrms*rms4){
	  ctrl4+=1;
	  t4=i;
	}//chiudo if 
	if(ctrl6==0 && v6[i]>Nrms*rms6 && v6[i+1]>Nrms*rms6 && v6[i+2]>Nrms*rms6){
	  ctrl6+=1;
	  t6=i;
	}//chiudo if
	
      }//chiudo for (i)

      //calcolo i t0 scalati
      t0sc=t0-t6;
      t2sc=t2-t6;
      t4sc=t4-t6;
      
     
      //calcolo aree fino a timeHigh1
      
      
      A10=Area(t0,300,rms0,v0);
      A12=Area(t2,15,rms2,v2);
      A14=Area(t4,30,rms4,v4);
      A16=Area(t6,300,rms6,v6);
      
      //calcolo aree fino a timeHigh2
      
      
      A20=Area(t0,800,rms0,v0);
      A22=Area(t2,100,rms2,v2);
      A24=Area(t4,100,rms4,v4);
      A26=Area(t6,800,rms6,v6);
      
      //calcolo del tau1
            
      tau10=Tau(t0,300,rms0,v0,A10);
      tau12=Tau(t2,15,rms2,v2,A12);
      tau14=Tau(t4,30,rms4,v4,A14);
      tau16=Tau(t6,300,rms6,v6,A16);

      //calcolo del tau2
      tau20=Tau(t0,800,rms0,v0,A20);
      tau22=Tau(t2,20,rms2,v2,A22);
      tau24=Tau(t4,100,rms4,v4,A24);
      tau26=Tau(t6,800,rms6,v6,A26);
        
        
    
      //calcolo di hmax
        hmax0=Max(v0);
        hmax2=Max(v2);
        hmax4=Max(v4);
        hmax6=Max(v6);
        
     

      

      
      newtree->Fill();
     }//chiudo for (j) 

    
    elab->cd();

    newtree->Write();
      
    elab->Close();

    
}


