/*
 *  Macro di root per analizzare i dati.
 *  Lanciare come:
 *
 *  shell> root -l
 *  root[0] .L Analyze.C
 *  root[1] Analyze("nomeDelFile.root")
 */

void Analyze1CanvElab(const char* fileName, const int j)
{

    // apre file prende il TTree di nome "datatree" dal file
    TFile* file = new TFile(fileName);
    TTree* tree = (TTree*)file->Get("newtree");


    // setta indirizzi delle variabili di interesse
    Double_t ch0[4096],ch2[4096],ch4[4096],ch6[4096];
    Int_t Nevent, Nsample;
    tree->SetBranchAddress("v0",&ch0);
    tree->SetBranchAddress("v2",&ch2);
    tree->SetBranchAddress("v4",&ch4);
    tree->SetBranchAddress("v6",&ch6);
	
    tree->SetBranchAddress("Nevent", &Nevent);
    tree->SetBranchAddress("Nsample",&Nsample);
    
    int p;
    p=j;

    for(int k=0;k<tree->GetEntries();k++){
      if(j==0) p=k;
    
      cout << p << endl;
      tree->GetEntry(p);
      
      
      Double_t v0[4096], v2[4096], v4[4096], v6[4096], time[4096];
      Double_t m=0.2398;
      for(int i=0; i<Nsample; i++) {
        v0[i]=ch0[i];
	v2[i]=ch2[i];
	v4[i]=ch4[i];
	v6[i]=ch6[i];
        time[i]=i;
      }

      
    /*Double_t mean0,mean2,mean4, mean6;
    UInt_t timeThLow=90;
    mean0=0;
    mean2=0;
    mean4=0;
    mean6=0;
    for(int i=0;i<timeThLow;i++){
      
      mean0+=v0[i];
      mean2+=v2[i];
      mean4+=v4[i];
      mean6+=v6[i];
      
    }//chiudo for (i)
    
    mean0/=timeThLow;
    mean2/=timeThLow;
    mean4/=timeThLow;
    mean6/=timeThLow;
    
    
    for(int i=0; i<Nsample; i++) {
      v0[i]-=mean0;
      v2[i]-=mean2;
      v4[i]-=mean4;
      v6[i]-=mean6;
    }
    */
    
    
      TCanvas *cha0 = new TCanvas("cha0", "Canale 0",1000,650);
      cha0->Divide(2,2);
      cha0->cd(1);
      TGraph *antonio = new TGraph(Nsample, time, v0);
      antonio->SetTitle("Canale 0");
      antonio->Draw();
      
      
      
      cha0->cd(2);
      TGraph *gigetto = new TGraph(Nsample, time, v2);
      gigetto->SetTitle("Canale 2");
      gigetto->Draw();
      gigetto->SetMinimum(-200);
      
      
      cha0->cd(3);
      TGraph *pablo = new TGraph(Nsample, time, v4);
      pablo->SetTitle("Canale 4");
      pablo->Draw();
      
    
      cha0->cd(4);
      TGraph *antoniocalabro = new TGraph(Nsample, time, v6);
      antoniocalabro->SetTitle("Canale 6");
      antoniocalabro->Draw();
      
      
      cha0->Update();
      if(j!=0) break;
      gPad->WaitPrimitive();
    }//chiudo for
    
    
}
