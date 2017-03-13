#include "KascadeEnergyConverter.h"
#include "Configuration.h"
#include <cmath>
#include <fstream>
#include <string>
#include <iostream>
#include <stdexcept>
using namespace std;

KascadeEnergyConverter::KascadeEnergyConverter() {

  int verbosity = Configuration::GetInstance().GetVerbosity();

  const string& fileName = 
    Configuration::GetInstance().GetConfigurationFileName("KascadeEnergyTable");

  if ( verbosity > 0 ) {
    cout << "\n  ==[KascadeEnergyConverter()]==>" << endl;
    cout << "    conversion table from " << fileName << endl;
  }


  FILE *fp;
  // Apply Muon Correction Functions based on the event selection
  if( (fp=fopen(fileName.c_str(),"r"))==NULL){
    throw std::runtime_error(string("KascadeEnergyConverter() ")+
                             string("error reading energy conversion"
                                    " table") + fileName);
  }

  for(int i=0;i<eTableX;i++){
    for(int j=0;j<eTableY;j++){
      float xbin, ybin;
      fscanf(fp,"%f %f ",&xbin,&ybin);
      for(int k=0;k<eTableZ;k++){
        fscanf(fp,"%f",&fCorrectionArrayMario[i][j][k]);
      }
    }
  }
  fclose(fp);

}

double KascadeEnergyConverter::CalculateEnergy(double showerSize, 
                                               double muonSize, 
                                               double coreX, double coreY, 
                                               double zenith,
                                               double azimuth) const {

 


  float elg, emg, eng, ak, xd, yd, zd, diska;
  float Sizmgc=0.;  //corrected muon number
  float Sizeg = showerSize;
  float Sizmg = muonSize;

  if ( log10(Sizeg) < 5.5 ) {
    /*
    cerr << " KascadeEnergyConverter - warning: size too small "
         << Sizeg;
    Sizeg = pow(10.,5.501);
    cerr << " (setting to " << Sizeg << endl;
    */
    return 0.;
  }


  float Xcg = coreX;
  float Ycg = coreY;
  float Zeg = zenith;
  float Azg = azimuth;

  //  float Nch,Nmu;
  float ang,kappa,energy;

  /*
  printf("Nmu size (Sizmg lev.3)? \n");
  scanf("%f",&Sizmg);
  printf("Xcg Core location (m)? \n");
  scanf("%f",&Xcg);
  printf("Ycg Core location (m)? \n");
  scanf("%f",&Ycg);
  printf("Zeg Zenith Angle (rad) should be < 40 deg? \n");
  scanf("%f",&Zeg);
  printf("Azg Azimuth Angle (rad)? \n");
  scanf("%f",&Azg);
  */


  // Azg is the KRETA ntuple entry AZG (azimut angle), Zeg is ZEG (zenith angle)
  elg=-sin(Zeg)*sin(Azg);
  emg=-sin(Zeg)*cos(Azg);
  eng=-cos(Zeg);
  // Transformation to shower coord:
  ak=elg*0.+emg*0.+eng*1.5;
  xd=elg*ak+(1.-elg*elg)*Xcg-elg*emg*Ycg;
  yd=emg*ak-emg*elg*Xcg+(1.-emg*emg)*Ycg;
  zd=eng*ak-eng*elg*Xcg-eng*emg*Ycg;
  diska=sqrt(pow((0.-xd),2) + pow((0.-yd),2) + pow((1.5-zd),2));
  // Divide the event into a bin:
  int idist, ith, imu;
  idist= int(floor(diska/25.));
  ith=int(floor((1./cos(Zeg)-1.)*50));
  imu=int(floor((log10(Sizmg)-4.)*10));
  if(imu<0) imu=0;
  if(imu>50) imu=50;
  if(idist>28) idist=28;
  if(ith>15) ith=15;
  //  printf("%d %d %d \n",imu,idist,ith);
  Sizmgc=(log10(Sizmg) + fCorrectionArrayMario[imu][idist][ith]);  
  Sizeg=log10(Sizeg);

  ang=Zeg*180./M_PI;

  if(ang < 16.71) {
    kappa=(-(Sizeg-Sizmgc)+(0.96029+0.08176*Sizeg))/((0.96029+0.08176*Sizeg)-(0.01115+0.15504*Sizeg));
    energy=(0.93960-0.05839*kappa)*Sizeg+1.1017+0.6625*kappa;
    energy=pow(10,energy);
    return energy*1e9;
  }
  if(ang >= 16.71 && ang < 23.99) {
    kappa=(-(Sizeg-Sizmgc)+(1.11510+0.05376*Sizeg))/((1.11510+0.05376*Sizeg)-(-0.13512+0.16555*Sizeg));
    energy=(0.94488-0.07806*kappa)*Sizeg+1.1319+0.8264*kappa;
    energy=pow(10,energy);
    return energy*1e9;
  }
  if(ang >= 23.99 && ang < 29.86) {
    kappa=(-(Sizeg-Sizmgc)+(1.03040+0.05980*Sizeg))/((1.03040+0.05980*Sizeg)-(-0.14095+0.15450*Sizeg));
    energy=(0.94332-0.08154*kappa)*Sizeg+1.1961+0.9089*kappa;
    energy=pow(10,energy);
    return energy*1e9;
  }
  if(ang >= 29.86 && ang < 35.09) {
    kappa=(-(Sizeg-Sizmgc)+(0.81923+0.06755*Sizeg))/((0.81923+0.06755*Sizeg)-(-0.31778+0.16913*Sizeg));
    energy=(0.93774-0.05491*kappa)*Sizeg+1.4258+0.6609*kappa;
    energy=pow(10,energy);
    return energy*1e9;
  }
  if(ang >= 35.09 && ang <= 40.00) {
    kappa=(-(Sizeg-Sizmgc)+(0.69480+0.07953*Sizeg))/((0.69480+0.07953*Sizeg)-(-0.24169+0.14337*Sizeg));
    energy=(0.9506-0.0706*kappa)*Sizeg+1.4779+0.7656*kappa;
    energy=pow(10,energy);
    return energy*1e9;
  }

  return 0.;

}

