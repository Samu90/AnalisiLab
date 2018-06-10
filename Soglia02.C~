/*
 *  Macro di root per analizzare i dati.
 *  Lanciare come:
 *
 *  shell> root -l
 *  root[0] .x Soglia.C("nomefile.root",-valore soglia)
 */

void Soglia46(const char* fileName, const double soglia)
{

    // apre file prende il TTree di nome "datatree" dal file
    TFile* file = new TFile(fileName);
    TTree* tree = (TTree*)file->Get("datatree");


    // setta indirizzi delle variabili di interesse

     UInt_t ch4[4096],ch6[4096];
     Double_t v4[4096], v6[4096], time[4096];
     Int_t Nevent, Nsample;

    tree->SetBranchAddress("ch4",&ch4);
    tree->SetBranchAddress("ch6",&ch6);
	
    tree->SetBranchAddress("nevent", &Nevent);
    tree->SetBranchAddress("nsample",&Nsample);

    int j;
    Double_t mean4=0, mean6=0;
    UInt_t count=0;
    UInt_t timeThLow=90;
    UInt_t timeThHigh=300;
    int i;
    Double_t m=0.2398;
    Double_t val4=0,val6=0;
    int ctrl4;
    int ctrl6;
    
     for(j=0;j<tree->GetEntry();j++){
      tree->GetEntry(j);
      mean4=0;
      mean6=0;
      
    for(i=0; i<Nsample; i++) {
      v4[i]=ch4[i]/m;
      v6[i]=ch6[i]/m;
      time[i]=i;
    }//chiudo for (i)

    for(i=0;i<timeThLow;i++){
     
	mean4+=v4[i];
	mean6+=v6[i];
     
    }//chiudo for (i)
    
      mean4/=timeThLow;
      mean6/=timeThLow;
      
      for(i=0; i<Nsample; i++) {
	v4[i]-=mean4;
	v6[i]-=mean6;
	
	//cout<< time[i]<< "  "<<v4[i]<<endl;
      }//chiudo for (i)
      
      ctrl4=0;
      ctrl6=0;
      
      for(i=timeThLow;i<timeThHigh;i++){
	
	if(ctrl4==0 && (v4[i]<soglia)){
	  ctrl4+=1;
	  val4=v4[i];
	}//chiudo if mean...
	
	if(ctrl6==0 && (v6[i]<soglia)){
	  ctrl6+=1;
	  val6=v6[i];
	}//chiudo if
	
      }//chiudo for (i)
      
      if(ctrl4>0 && ctrl6>0){
	count++;
	cout<< "valore4= "<< val4 << endl;
	cout<< "valore6= "<< val6 << endl;
	cout << j << endl;
      }
      
     }//chiudo for (j)
     
     cout << endl;
     cout << count << endl;
     
}
