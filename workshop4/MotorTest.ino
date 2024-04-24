int escRate = 50;
int freePWM = 0;
class motor{
  private:
    double thrustOut;
    double adjustment;
    int myChannel;
  public:
    void setup(double torqueX, double torqueY, double torqueZ, int pinIn){
      ledcSetup(freePWM, escRate, 10); //65536 possible values, but only some can be used
      ledcAttachPin(pinIn, freePWM);
      myChannel = freePWM;
      freePWM++;
    }

    int thrustToDuty(double thrust){
      int duty;
      double maxDuty = int(1024 * (0.002*escRate)); 
      double minDuty = int(1024 * (0.001*escRate)); 
      duty = int(minDuty + (maxDuty-minDuty)*(thrust/100));
      return duty;
    }


    void printThrust(){
        Serial.print(thrustOut);
        Serial.print("  ");
    }

    void setThrust(double thrust){
      thrustOut = thrust;
      ledcWrite(myChannel, thrustToDuty(thrust));
    }

};

  motor bl;
  motor fl;
  motor br;
  motor fr;

void setup() {
  fl.setup( 1,    1,  -1, 38);
  fr.setup(-1,    1,   1, 36);
  bl.setup( 1,   -1,   1, 39);
  br.setup(-1,   -1,  -1, 37);

  Serial.begin(115200);

  fl.setThrust(0);
  fr.setThrust(0);
  br.setThrust(0);
  bl.setThrust(0);

  delay(2500);
}

float i=0;

void loop() {
  fl.setThrust(17);
  fr.setThrust(17);
  br.setThrust(17);
  bl.setThrust(17);
  delay(500);
}
