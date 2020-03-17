#include "TROOT.h"
#include "TH1.h"
#include "TCanvas.h"
#include "TASImage.h"
#include "TApplication.h"
#include "TSystem.h"


#include "assert.h"

#include <iostream>
#include <stdio.h>


#include <cstdio>
#include <cstdlib>
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <unistd.h>
#include <sys/types.h>
#include <random>
#include <fstream>
#include <sstream>  // for std::ostringstream
#include <string>
using namespace std;




void init_srand(void) {
    
    //Declare variable to hold seconds on clock.
    time_t seconds;
    //Get value from system clock and place in seconds variable.
    time(&seconds);
    //Convert seconds to a unsigned
    
    //srand((unsigned int) seconds);
    srand((unsigned)time(NULL));
    srand48(seconds);
}
double dist(int r, int g, int b, int r2, int g2, int b2){
    
    double D = pow(r-r2,2) + pow(g-g2,2)+ pow(b-b2,2) ;
    return sqrt(D);
}


class Pixel_prop{
public:
    int R;
    int G;
    int B;
    double dist_bl;
    double dist_bl2;
};






int main(int argc, char** argv){
    
    
    if (argc<3) {
        cout << "Usage: simapix_start image1 image2 <output=out.png>" << endl;
        return 0;
    }
    TString fsrc=argv[1];
    TString ftgt=argv[2];
    TString fout;
    argc>3 ? fout = argv[3] : fout="out.png";
    cout << "Reading images: source= " << fsrc << " target= " << ftgt << endl;
    cout << "Output= " << fout << endl;
    
    TApplication theApp("App", &argc, argv);
    
    // create image objects
    TASImage *src = new TASImage(fsrc.Data());
    TASImage *tgt = new TASImage(ftgt.Data());
    TASImage *out = new TASImage(*src); // start with copy of source
    TASImage *out2 = new TASImage(*tgt);
    
    // Test image geometry, exit if they are not the same dimensions
    assert ( src->GetWidth() == tgt->GetWidth() && src->GetHeight() == tgt->GetHeight() );
    cout << "Pixel Geometry: " << src->GetWidth() << " x " << src->GetHeight() << endl;
    Long_t numPix=src->GetWidth()*src->GetHeight();
    Long_t numPix2 = tgt->GetWidth()*tgt->GetHeight();
    // *** The work happens here
    // access the pixels for the output image
    // each pixel is a 32-bit word, 1 byte each for (alpha,red,green,blue)
    // don't touch alpha (bits 31:28)
    
    // examples of pixel manipulations
    UInt_t *outPix = out->GetArgbArray();
    UInt_t *outPix2 = out2->GetArgbArray();
    
    static int N = numPix;
    
    Pixel_prop src_pix[N];
    Pixel_prop tgt_pix[N];
    cout<<"A"<<endl;
    std::random_device rd;     // only used once to initialise (seed) engine
    std::mt19937 rng(rd());    // random-number engine used (Mersenne-Twister in this case)
    std::uniform_int_distribution<int> uni(0,N-1); // guaranteed unbiased

    for (int i=0;i< numPix; i++){
        
        uint32_t r_src =  outPix[i] & 0x00ff0000;
        r_src = r_src>>16;
        uint32_t r_tgt =  outPix2[i] & 0x00ff0000;
        r_tgt = r_tgt>>16;
        src_pix[i].R = r_src;
        tgt_pix[i].R = r_tgt;
        
        
        uint32_t g_src =  outPix[i] & 0x0000ff00;
        g_src = g_src>>8;
        uint32_t g_tgt =  outPix2[i] & 0x0000ff00;
        g_tgt = g_tgt>>8;
        src_pix[i].G = g_src;
        tgt_pix[i].G = g_tgt;
        
        
        uint32_t b_src =  outPix[i] & 0x000000ff;
        
        uint32_t b_tgt =  outPix2[i] & 0x000000ff;
        
        src_pix[i].B = b_src;
        tgt_pix[i].B = b_tgt;
        
        
        
    }
    
    
    
        for(double T= 5;T>1;T-=1){
            
            //cout<<"T"<<T<<endl;
            
            
            
            
            int MC_Times=20;
                
                for(int i=0;i<MC_Times;i++){
                    
                    for(int n=0;n<N;n++){
                        
                        
                        
                        
                            
                            int r2 = uni(rng);
                            
                            double E1 = dist(tgt_pix[n].R,tgt_pix[n].G,tgt_pix[n].B,src_pix[n].R,src_pix[n].G,src_pix[n].B) + dist(tgt_pix[r2].R,tgt_pix[r2].G,tgt_pix[r2].B,src_pix[r2].R,src_pix[r2].G,src_pix[r2].B);
                        
                        
                        
                        
                        double tempr=src_pix[n].R;
                        double tempg=src_pix[n].G;
                        double tempb=src_pix[n].B;
                        
                        src_pix[n].R= src_pix[r2].R;
                        src_pix[n].G= src_pix[r2].G;
                        src_pix[n].B= src_pix[r2].B;
                        
                        src_pix[r2].R = tempr;
                        src_pix[r2].G = tempg;
                        src_pix[r2].B = tempb;
                        
                        
                        
                        
                            double E2 = dist(tgt_pix[n].R,tgt_pix[n].G,tgt_pix[n].B,src_pix[n].R,src_pix[n].G,src_pix[n].B) + dist(tgt_pix[r2].R,tgt_pix[r2].G,tgt_pix[r2].B,src_pix[r2].R,src_pix[r2].G,src_pix[r2].B);
                            
                            double dEnergy = E2-E1;
                            //cout<<dEnergy<<endl;
                            double r_prob = drand48();
                            if(r_prob >  exp(-dEnergy/T)){
                                
                                double tempr=src_pix[n].R;
                                double tempg=src_pix[n].G;
                                double tempb=src_pix[n].B;
                                
                                src_pix[n].R= src_pix[r2].R;
                                src_pix[n].G= src_pix[r2].G;
                                src_pix[n].B= src_pix[r2].B;
                                
                                src_pix[r2].R = tempr;
                                src_pix[r2].G = tempg;
                                src_pix[r2].B = tempb;
                            }
                        }
                        
                        
                        
                    }
            
            

        
        
        
        
        
        }
    //SA bracket remove later
    
    for(int i=0;i<N;i++){
        outPix2[i]= src_pix[i].B + src_pix[i].G*pow(2,8) +src_pix[i].R*pow(2,16) + 4278190080;
    }
    
    
    TCanvas *c1 = new TCanvas("c1", "images", 640, 480);
    c1->Divide(2,2);
    
    c1->cd(1);
    c1->Draw();
    src->Draw("X");
    c1->cd(2);
    tgt->Draw("X");
    c1->cd(3);
    out2->Draw("X");
    c1->Print("collage.png");
    // save the new image
    out2->WriteImage(fout.Data());
    
    
    
    
    
    
    
    cout << "Press ^c to exit" << endl;
    theApp.SetIdleTimer(30,".q");  // set up a failsafe timer to end the program
    theApp.Run();
    
}
