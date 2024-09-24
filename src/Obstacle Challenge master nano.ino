#include <Wire.h>     // Library for I2C communication
#include <Pixy2.h>    // Library for interfacing with Pixy2 camera

Pixy2 pixy;  // Create an object to interact with the Pixy2 camera

#define SLAVE_ADDR 9  // I2C slave address, should match the one on the Arduino Nano or ESP32

void setup() {
  Wire.begin();  // Initialize I2C as master
  Serial.begin(115200);  // Begin serial communication for debugging at a baud rate of 115200
  
  // Initialize Pixy2
  pixy.init();
  Serial.println("Master initialized.");  // Print a message once initialization is done
}

void loop() {
  // Request to get the blocks (objects) detected by Pixy2's color code (CCC) mode
  pixy.ccc.getBlocks();
  
  // If at least one block is detected
  if (pixy.ccc.numBlocks) {
    // Extract the first block detected (index 0) as an example
    int x = pixy.ccc.blocks[0].m_x;  // Get the x-coordinate of the block
    int y = pixy.ccc.blocks[0].m_y;  // Get the y-coordinate of the block
    int signature = pixy.ccc.blocks[0].m_signature;  // Get the signature (color ID) of the block

    // Identify in which region of the camera's view the block is located based on coordinates
    char region = getRegion(x, y);

    // Send the region and signature to the I2C slave device (another Arduino, Raspberry Pi, etc.)
    Wire.beginTransmission(SLAVE_ADDR);  // Begin communication with the slave device
    Wire.write(region);         // Send the region information
    Wire.write(signature);      // Send the block's signature
    Wire.endTransmission();     // End communication

    // Display on the Serial Monitor what is being sent (for debugging)
    Serial.print("Sending region: ");
    Serial.print(region);
    Serial.print(", Signature: ");
    Serial.println(signature);

    delay(100);  // Wait for 100 milliseconds before reading new blocks
  }
}

// Function to determine the region based on the x and y coordinates of the detected block
char getRegion(int x, int y) {
  // Define regions based on predefined boundaries:
  // Regions are assigned letters (A to I) based on position in the Pixy2's field of view.
  
  // Left-Top region: Alpha
  if (x >= 0 && x <= 52 && y >= 0 && y <= 69) return 'A';    
  
  // Left-Bottom region: Delta
  if (x >= 0 && x <= 52 && y > 69 && y <= 138) return 'D';   
    
  // Left-Bottom-Extended region: Eta
  if (x >= 0 && x <= 52 && y > 138) return 'E';              
  
  // Center-Top region: Beta
  if (x > 52 && x <= 252 && y >= 0 && y <= 69) return 'B';   
    
  // Center-Bottom region: Epsilon
  if (x > 52 && x <= 252 && y > 69 && y <= 138) return 'E';   
  
  // Center-Bottom-Extended region: Theta
  if (x > 52 && x <= 252 && y > 138) return 'T';              
  
  // Right-Top region: Gamma
  if (x > 252 && x <= 316 && y >= 0 && y <= 69) return 'G';   
  
  // Right-Bottom region: Zeta
  if (x > 252 && x <= 316 && y > 69 && y <= 138) return 'Z';  
  
  // Right-Bottom-Extended region: Iota
  if (x > 252 && x <= 316 && y > 138) return 'I';             
  
  // If the block is outside the defined regions, return 'U' (Unknown)
  return 'U';
}
