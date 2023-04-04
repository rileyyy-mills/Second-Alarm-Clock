#include <Adafruit_CircuitPlayground.h>
#include <math.h>

int light_sensor_value;  
int default_brightness = 4;
int RGB[3]={0,0,0};

// minimum values needed to turn on LEDs 1-10 (in order)
int LED_val_required[10]={0,10,20,30,40,50,60,70,80,90};

// recorded_values in order from most recent [0] to oldest [5]
int recorded_values[6]={0, 0, 0, 0, 0, 0};


void reorderArray(int s){
  // returns updated array with newest value in & oldest value out

  int temp_val_1, temp_val_2 = s;
  
  // Reorders array
  for (int i=0; i < 6; i++){

    temp_val_1 = recorded_values[i];

    recorded_values[i] = temp_val_2;

    temp_val_2 = temp_val_1;
  }

  // Prints the new array for debugging purposes
  //Serial.print("NEW LIST: "); 
  for(int j=0; j<6; j++) {
    Serial.print("["); Serial.print(recorded_values[j]); Serial.print("] ");
  }  
}


void calculateValues(){
  // Calculates min, max, and average values within array
  int val_avg=0, val_min=0, val_max=0;

  // Iterates over list; Assigns max and min values
  for(int a=0, b=1; a < 5; a++, b++) {

    int temp_max = max(recorded_values[a], recorded_values[b]);
    val_max = max(val_max, val_max);

    int temp_min = min(recorded_values[a], recorded_values[b]);
    val_min = min(val_min, temp_min);
  }
}


void displayLightLevels(int sensor_value){
  // Turns LEDs 1-10 on/off based on inputted value

  for(int LED=0; LED<10; LED++){

    if (sensor_value > LED_val_required[LED]){
      CircuitPlayground.setBrightness(default_brightness);
      CircuitPlayground.setPixelColor(LED,RGB[0],RGB[1], RGB[2]);
    }
    else{
      CircuitPlayground.setBrightness(0);
      CircuitPlayground.setPixelColor(LED,RGB[0],RGB[1], RGB[2]);
    }    
  }
}


void setup(){
  Serial.begin(9600);
  CircuitPlayground.begin();
}


void loop(){

  light_sensor_value = CircuitPlayground.lightSensor();

  //if (light_sensor_value < 0){light_sensor_value *= -1;};

  Serial.print("-> light Sensor Value: ");
  Serial.println(light_sensor_value);

  reorderArray(light_sensor_value);
  calculateValues();
  displayLightLevels(light_sensor_value);
}


