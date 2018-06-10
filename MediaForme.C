

//Fa una media di tutte le forme d'onda contenenti eventi
//root[0] .x MediaForme("nomeFileInput","nomeFileOutput")


void MediaForme(const char* fileName)
{
gROOT->Reset();
    // apre file prende il TTree di nome "newtree" dal file
    TFile* file = new TFile(fileName);
    TTree* tree = (TTree*)file->Get("newtree");


    // setta indirizzi delle variabili di interesse
    Double_t ch0[1000],ch2[1000],ch4[1000],ch6[1000];
    Int_t Nevent, Nsample, entrate;
    Int_t t0,t2,t4,t6;
    Int_t t0sinc, t2sinc, t4sinc, t6sinc;

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
    
   
    
    
    Double_t v0[1000],v2[1000],v4[1000],v6[1000];
    Double_t temp0[1000],temp2[1000],temp4[1000],temp6[1000],time[1000];   //Arrey su cui vanno fatte le somme 
    int i,j;
    UInt_t Buoni0[23000], Buoni2[23000]; //Array dove salvo indici buoni per ch4 e ch6
    int count0=0,count2=0;
    int ctrl0,ctrl2;
    int soglia=-10; // soglia in tensione sugli eventi Cherenkov e di scintillazione 
    int timeLow=90;

    //azzero i temporanei
    for(i=0;i<Nsample;i++){
	temp0[i]=0;
	temp2[i]=0;
	temp4[i]=0;
	temp6[i]=0;
	time[i]=i;
      }

      //inizilizzo tempi di sincronizzazione
    tree->GetEntry(4);
      t0sinc=t0;
      t2sinc=t2;
      t4sinc=t4;
      t6sinc=t6;
      
      entrate=tree->GetEntries();
      
    for(j=0;j<entrate;j++){
      tree->GetEntry(j);

      for(i=0;i<Nsample;i++){
	v0[i]=ch0[i];
	v2[i]=ch2[i];
	v4[i]=ch4[i];
	//if(j%1000==0) cout<<v4[i]<<endl;
	v6[i]=ch6[i];
      }

      
    ctrl0=0;
    ctrl2=0;
    
    //controllo gli eventi cher e scint buoni
    for(i=timeLow;i<400;i++){
      
      if(ctrl0==0 && (v0[i]<soglia)){
	//cout<< "evento"<< i<< "tensione" << v0[i] << "//"<< soglia<<endl;
	ctrl0+=1;
      }//chiudo if mean...
      
      if(ctrl2==0 && (v2[i]<soglia)){
	ctrl2+=1;
      }//chiudo if
      
    }//chiudo for (i)

    //conto e salvo indice evento con 1 se buono, 0 altrimenti
    if(ctrl0>0){
      count0++;
      Buoni0[j]=1;
    } else if(ctrl0==0){
      Buoni0[j]=0;
    }
    if(ctrl2>0){
      count2++;
      Buoni2[j]=1;
    } else if(ctrl2==0){
      Buoni2[j]=0;
    }
    
    
    
   
    //sommo su tutti i V(i)
    for(i=t6-20; i<Nsample-1; i++) {
      temp6[100+i-t6]+=v6[i];}
    
    for(i=t4-20; i<Nsample-1; i++){
      temp4[100+i-t4]+=v4[i];}
    
      if(Buoni0[j]==1){
	for(i=t0-20;i<Nsample-1;i++){
	  temp0[100+i-t0]+=v0[i];}
      }
      if(Buoni2[j]==1){
	for(i=t2-20;i<Nsample-1;i++){
	  temp2[100+i-t2]+=v2[i];
	  if(i==100)cout<<"evento"<< j << "indice"<<t2-timeLow<<endl;}
      }
   
    
    if( j%100 == 0 ) cout<< "Entry: " << j << " / " << entrate <<endl;
}// chiudo for j


// riscalo
for(i=0;i<Nsample;i++){
  temp0[i]/=(count0);
  temp2[i]/=(count2);
  temp4[i]/=(entrate);
  temp6[i]/=(entrate);
 }// chiudo for i

cout<<"vdsg"<<Nsample<<endl;



    TCanvas *cha0 = new TCanvas("cha0", "Canale 0");
    cha0->Divide(2,2);
    cha0->cd(1);

    TGraph *antonio = new TGraph(Nsample, time, temp0);
    antonio->SetTitle("Canale 0");
//antonio->SetMinimum(-400);
//  antonio->SetMaximum(50);
    antonio->Draw();


    cha0->cd(2);
    TGraph *gigetto = new TGraph(Nsample, time, temp2);
    gigetto->SetTitle("Canale 2");
//  gigetto->SetMinimum(-400);
//  gigetto->SetMaximum(50);
    gigetto->Draw();



    cha0->cd(3);
    TGraph *pablo = new TGraph(Nsample, time, temp4);
    pablo->SetTitle("Canale 4");
//  pablo->SetMinimum(-250);
//  pablo->SetMaximum(50);
    pablo->Draw();

    
    cha0->cd(4);
    TGraph *antoniocalabro = new TGraph(Nsample, time, temp6);
    antoniocalabro->SetTitle("Canale 6");
//  antoniocalabro->SetMinimum(-250);
//  antoniocalabro->SetMaximum(50);
    antoniocalabro->Draw();

       file->Close();
    
}
