/*
 *  Macro di root per analizzare i dati.
 *  Lanciare come:
 *
 *  shell> root -l
 *  root[0] .L Analyze.C
 *  root[1] Analyze("nomeDelFile.root")
 */

void PlotMedia(const char* fileName, int j)
{

    // apre file prende il TTree di nome "datatree" dal file
    TFile* file = new TFile(fileName);
    TTree* tree = (TTree*)file->Get("datatree");


    // setta indirizzi delle variabili di interesse
    Double_t ch0[1000],ch2[1000],ch4[1000],ch6[1000];
    Int_t Nevent, Nsample;
    
    tree->SetBranchAddress("ch0",&ch0);
    tree->SetBranchAddress("ch2",&ch2);
    tree->SetBranchAddress("ch4",&ch4);
    tree->SetBranchAddress("ch6",&ch6);
	
    tree->SetBranchAddress("nevent", &Nevent);
    tree->SetBranchAddress("nsample",&Nsample);
    
    

    
    tree->GetEntry(j);
    
    Double_t v0[1000], v2[1000], v4[1000], v6[1000], time[1000];

    for(int i=0;i<1000;i++){
      v0[i]=0;
      v2[i]=0;
      v4[i]=0;
      v6[i]=0;
    }

    
    for(int i=0;i<Nsample;i++){
      v0[i]=ch0[i];
      v2[i]=ch2[i];
      v4[i]=ch4[i];
      v6[i]=ch6[i];
      time[i]=i;

    }
    
    TCanvas *cha0 = new TCanvas("cha0", "Canale 0");
    cha0->Divide(2,2);

    cha0->cd(1);
    TGraph *antonio = new TGraph(Nsample, time, v0);
    antonio->SetTitle("Canale 0");
    antonio->Draw();

    cha0->cd(2);
    TGraph *gigetto = new TGraph(Nsample, time, v2);
    gigetto->SetTitle("Canale 2");
    gigetto->Draw();



    cha0->cd(3);
    TGraph *pablo = new TGraph(Nsample, time, v4);
    pablo->SetTitle("Canale 4");
    pablo->Draw();

    
    cha0->cd(4);
    TGraph *antoniocalabro = new TGraph(Nsample, time, v6);
    antoniocalabro->SetTitle("Canale 6");
    antoniocalabro->Draw();

   
}
