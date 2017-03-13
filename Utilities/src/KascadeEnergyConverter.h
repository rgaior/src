class KascadeEnergyConverter {

public:
  KascadeEnergyConverter();
  double CalculateEnergy(double showerSize, double muonSize, 
                         double coreX, double coreY, double zenith,
                         double azimuth) const;


private:
  enum EDimensions {
    eTableX = 51,
    eTableY = 29,
    eTableZ = 16 
  }; 
  float fCorrectionArrayMario[eTableX][eTableY][eTableZ];

};
