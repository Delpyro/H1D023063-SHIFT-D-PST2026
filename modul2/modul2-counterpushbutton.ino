// ============================== PIN ==============================
const int segmentPins[8] = {7, 6, 5, 11, 10, 8, 9, 4};
// a b c d e f g dp

// Sesuaikan dengan kabel hijau di gambar (terhubung ke pin 2)
const int btnUp = 2; 

// ============================== DATA ==============================
// CC: 1 = ON, 0 = OFF
byte digitPattern[16][8] = {
  {1,1,1,1,1,1,0,0}, //0
  {0,1,1,0,0,0,0,0}, //1
  {1,1,0,1,1,0,1,0}, //2
  {1,1,1,1,0,0,1,0}, //3
  {0,1,1,0,0,1,1,0}, //4
  {1,0,1,1,0,1,1,0}, //5
  {1,0,1,1,1,1,1,0}, //6
  {1,1,1,0,0,0,0,0}, //7
  {1,1,1,1,1,1,1,0}, //8
  {1,1,1,1,0,1,1,0}, //9
  {1,1,1,0,1,1,1,0}, //A
  {0,0,1,1,1,1,1,0}, //b
  {1,0,0,1,1,1,0,0}, //C
  {0,1,1,1,1,0,1,0}, //d
  {1,0,0,1,1,1,1,0}, //E
  {1,0,0,0,1,1,1,0}  //F
};

int currentDigit = 0;
bool lastUpState = HIGH;

// ============================== FUNCTION ==============================
void displayDigit(int num)
{
  for(int i=0; i<8; i++)
  {
    // Menggunakan ! karena Seven Segment kamu kemungkinan Common Anode
    digitalWrite(segmentPins[i], !digitPattern[num][i]);
  }
}

// ============================== SETUP ==============================
void setup()
{
  for(int i=0; i<8; i++)
  {
    pinMode(segmentPins[i], OUTPUT);
  }

  pinMode(btnUp, INPUT_PULLUP);
  displayDigit(currentDigit);
}

// ============================== LOOP ==============================
void loop()
{
  bool upState = digitalRead(btnUp);

  // Edge detection: mendeteksi saat tombol ditekan (HIGH ke LOW)
  if(lastUpState == HIGH && upState == LOW)
  {
    delay(50); // Debounce lebih pendek agar responsif
    currentDigit++;
    if(currentDigit > 15) currentDigit = 0;
    displayDigit(currentDigit);
    
    // Tunggu sampai tombol dilepas agar tidak looping terus
    while(digitalRead(btnUp) == LOW); 
    delay(50);
  }

  lastUpState = upState;
}