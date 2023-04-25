#include <Adafruit_CircuitPlayground.h>
#include <math.h>

bool lightChangeDetected;
int light_sensor_value;  
int old_avg = 0;
int change_sensitivity = 20;
int default_brightness = 4;
int RGB[3]={255,0,0};

// minimum values needed to turn on LEDs 1-10 (in order)
int LED_val_required[10]={0,10,20,30,40,50,60,70,80,90};

// recorded_values in order from most recent [0] to oldest [5]
int recorded_values[6]={0, 0, 0, 0, 0, 0};


// returns updated array with newest value in & oldest value out
void reorderArray(int s){
  
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


// Calculates min, max, average values in array, & if light change is detected
void calculateValues(){
  int val_avg=0, val_max=0;

  // Iterates over array for max values
  for(int a=0, b=1; a < 5; a++, b++) {
  
    int temp_max = max(recorded_values[a], recorded_values[b]);
    val_max = max(val_max, temp_max);

    //Serial.print("TEMP_max: ");Serial.println(temp_max);
  }

  int val_min = val_max;

  // Iterates over array for min values
  for(int a=0, b=1; a < 5; a++, b++) {

    int temp_min = min(recorded_values[a], recorded_values[b]);
    val_min = min(val_min, temp_min);

    //Serial.print("TEMP_min: ");Serial.println(temp_min);
  }
  
  // Calculates average val from max and min only
  val_avg = ((val_max + val_min) / 2);

  // Environmental Change detected if light value is greater than the average by avg_val + change sensitivity
  // or light value is less than the (avg_val - change sensitivity)
  if (val_avg > old_avg + change_sensitivity || val_avg < old_avg - change_sensitivity)
    lightChangeDetected = true;
  else{
    lightChangeDetected = false;
  }

  // Prints values for debugging purposes
  Serial.println();
  Serial.print("MAX: ");Serial.println(val_max);
  Serial.print("MIN: ");Serial.println(val_min);
  Serial.print("AVG: ");Serial.println(val_avg);
  Serial.print("VAL AVG: ");Serial.print(val_avg);Serial.print("  VS  ");
  Serial.print("Sensitivity: +/-");Serial.println(change_sensitivity);
  Serial.print("NO CHANGE DETECTED IF BETWEEN: ");Serial.print(old_avg + change_sensitivity);Serial.print("  and ");Serial.print(old_avg - change_sensitivity);
  Serial.println();
  Serial.print("LIGHT CHNG: ");Serial.println(lightChangeDetected);
  Serial.println("----------");
  Serial.println();

  old_avg = val_avg;
}


// Displays lights based on whether change is or is not detected
void displayLightLevels(float delayMultiplier = 1){
  int delayT = 100;

  if (lightChangeDetected){
    CircuitPlayground.setBrightness(default_brightness);
    
    Serial.println("->ENABLING LIGHTS!");

    // Turns LEDs 0-9 on/off
    CircuitPlayground.setPixelColor(4,RGB[0],RGB[1], RGB[2]);
    CircuitPlayground.setPixelColor(5,RGB[0],RGB[1], RGB[2]);

    delay(delayT * delayMultiplier);
  }
  else{
    CircuitPlayground.setBrightness(0);

    for(int LED=0; LED<10; LED++){
    CircuitPlayground.setPixelColor(LED,RGB[0],RGB[1], RGB[2]);
    }
    //CircuitPlayground.setPixelColor(LED,RGB[0],RGB[1], RGB[2]);
  }    

}


void setup(){
  lightChangeDetected = false;
  Serial.begin(9600);
  CircuitPlayground.begin();
}


void loop(){

  //Gets Light Value
  light_sensor_value = CircuitPlayground.lightSensor();

  //Formats Value So >= 0
  if (light_sensor_value < 0){light_sensor_value *= -1;};

  //Serial.print("-> light Sensor Value: ");
  //Serial.println(light_sensor_value);

  //FIFO Reorders Array W/ New Value
  reorderArray(light_sensor_value);

  //Calculates int Avg, Min, Max, & boolean LightDectected
  calculateValues();

  //Displays If Change Detected
  displayLightLevels();

  delay(200);
}