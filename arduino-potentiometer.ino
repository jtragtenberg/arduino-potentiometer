// Potentiometer Reading
// by João Tragtenberg - Batebit Artesania Digital 22/01/2020
// inspired by https://www.norwegiancreations.com/2015/10/tutorial-potentiometers-with-arduino-and-filtering/
// The code uses a Exponential Moving Average Filter
// to remove noise from the potentiometer value so it
// only detects the sensor reading when there is a
// significant change in the original value.


#define TEN_BIT_READING 1 //this is the lossless option, using all the 10 bits of the Arduino ADC
#define NINE_BIT_READING 2
#define EIGHT_BIT_READING 4
#define SEVEN_BIT_READING 8 //the standard MIDI values are 7 bits long

int bitRange = TEN_BIT_READING;

//Global Variables
int sensorPin = A0;      //pin number to use the ADC
int sensorValue = 0;     //initialization of sensor variable, equivalent to EMA Y
int filteredValue = 0;   //variable to store the filtered value



void setup() {
  Serial.begin(115200);           //setup of Serial module, 115200 bits/second
}

void loop() {
  sensorValue = analogRead(sensorPin);                //read the sensor value using ADC


  filteredValue = EMA(sensorValue, filteredValue, 0.8); //This applies the Exponential Moving Average Filter
  //the amount of filtering goes from 0.0 to 1.0
  //(the greater the resolution you want, more filtering you need to put)
  //the filteredValue is iterated through this function

  if (changedValue(filteredValue, bitRange)) {   //The original bit depth of the Arduino ADC is 10bits
    //but if you need a smaller bit depth, you can change this
    //value giving a higher threshold value for the change detection
    //the smaller bit depth you use, the less filtering is needed

    //TODO: understand why the pot is jumping values when changing direction

    //Here you execute the instructions you want when the pot changes value:

    int outputValue = sensorValue / bitRange;
    
    Serial.println(outputValue); //change TEN_BIT_READING to other defined bit ranges here and above

    int potSection = sectionizePot(outputValue, 5, bitRange); //this divides the input values in #n sections.

    Serial.println(potSection);
  }

  //delay(5);
}



int EMA(int inputValue, int filteredValue, float filteringAmount) {
  return ((1 - filteringAmount) * inputValue) + (filteringAmount * filteredValue);
}

int lastValue_;
bool changedValue(int inputValue_, int threshold_) {
  if (abs(inputValue_ - lastValue_) >= threshold_) {
    lastValue_ = inputValue_;
    return true;
  }
  else return false;
}

int potSection_;
int sectionizePot(int inputValue_, int nSections_, int bitRange_) {
  int sectionSize_ = floor(1024 / bitRange_ / nSections_);
  if (inputValue_ < sectionSize_) {
    potSection_ = 0;
    return potSection_;
  }
  if (nSections_ > 2) {
    for (int i = 1 ; i < (nSections_ - 1) ; i++) {
      if ((inputValue_ >= floor(i * sectionSize_)) && (inputValue_ < floor((i + 1) * sectionSize_))) {
        potSection_ = i;
        return potSection_;
      }
    }
  }
  if (inputValue_ >= floor((nSections_ - 1) * sectionSize_)) {
    potSection_ = nSections_ - 1;
    return potSection_;
  }
}

//both of the above functions can work for integer values as well as for floating point filtered values
/*
  float EMA(float inputValue, float filteredValue, float filteringAmount) {
  return ((1 - filteringAmount) * inputValue) + (filteringAmount * filteredValue);
  }


  float lastValue_f;
  bool changedValue(float inputValue_, float threshold_) {
  if (abs(inputValue_ - lastValue_f) >= threshold_) {
    lastValue_f = inputValue_;
    return true;
  }
  else return false;
  }
*/
