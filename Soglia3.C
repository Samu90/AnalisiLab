/*
 *  Macro di root per analizzare i dati.
 *  Lanciare come:
 *
 *  shell> root -l
 *  root[0] .x Soglia.C("nomefile.root",-valore soglia)
 */

void Soglia3(const char* fileName, const double soglia)
{

    // apre file prende il TTree di nome "datatree" dal file
    TFile* file = new TFile(fileName);
    TTree* tree = (TTree*)file->Get("datatree");


    // setta indirizzi delle variabili di interesse

    UInt_t ch0[4096],ch2[4096], ch4[4096],ch6[4096];
     Double_t v0[4096],v2[4096],v4[4096], v6[4096], time[4096];
     Int_t Nevent, Nsample;

     tree->SetBranchAddress("ch0",&ch0);
     tree->SetBranchAddress("ch2",&ch2);
    tree->SetBranchAddress("ch4",&ch4);
    tree->SetBranchAddress("ch6",&ch6);
	
    tree->SetBranchAddress("nevent", &Nevent);
    tree->SetBranchAddress("nsample",&Nsample);

    int j;
    Double_t mean0,mean2,mean4, mean6;
    UInt_t count=0;
    UInt_t timeThLow=90;
    UInt_t timeThHigh=300;
    UInt_t ctrl0,ctrl2,ctrl4,ctrl6;
    int i;
    Double_t m=0.2398;

    mean0=0;
    mean2=0;
    mean4=0;
    mean6=0;
    
    for(j=0;j<tree->GetEntries();j++){
      tree->GetEntry(j);
      
    for(i=0; i<Nsample; i++) {
      v0[i]=ch0[i]/m;
      v2[i]=ch2[i]/m;
      v4[i]=ch4[i]/m;
      v6[i]=ch6[i]/m;
      time[i]=i;
    }//chiudo for (i)

    for(i=0;i<timeThLow;i++){

        mean0+=v0[i];
        mean2+=v2[i];
	mean4+=v4[i];
	mean6+=v6[i];
     
    }//chiudo for (i)

      mean0/=timeThLow;
      mean2/=timeThLow;
      mean4/=timeThLow;
      mean6/=timeThLow;

      ctrl0=0;
      ctrl2=0;
      ctrl4=0;
      ctrl6=0;
	
      for(i=timeThLow;i<timeThHigh;i++){

	if((mean0-v0[i])>(-1*soglia) && ctrl0==0){
	  //  cout<< ctrl0<<endl;
	  ctrl0 +=1;
	}
	
	if((mean2-v2[i])>(-1*soglia) && ctrl2==0){
	  ctrl2 +=1;
	}
	
	if((mean4-v4[i])>(-1*soglia) && ctrl4==0){
	  ctrl4 +=1;
	  cout<< "media:" << mean4<< "  " << "tensione:"<< v4[i] << "  "<< "scarto:" << (mean4-v4[i]) << endl;
	}
	
	if((mean6-v6[i])>(-1*soglia) && ctrl6==0){
	  ctrl6 +=1;
	}
	
	
      
      }//chiudo for (i)

	if((ctrl0+ctrl2+ctrl4+ctrl6)>=4){
	cout << j << endl;
	count++;
	}
      
    }//chiudo for (j)

      cout << endl;
      cout << count << endl;
  
}
