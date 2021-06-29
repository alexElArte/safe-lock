#define NOTURN 0
#define LEFT   1
#define RIGTH  2

// Change the password's length as you want
// You shoudn't use less than 4 numbers
const byte psw_length = 6;

/*  THE PASSWORD NEED TO HAVE A SPECIAL FORM
 * 
 * psw[0] < psw[1] > psw[2] < psw[3] > ...
 * or
 * psw[0] > psw[1] < psw[2] > psw[3] < ...
 * 
 * A high number then a low number
 * Or a low number the a high number
 */
const byte psw[psw_length] = {0, 4, 3, 6, 4, 9};
// Other example
//const byte psw[psw_length] = {9, 5, 7, 0, 2, 1};

byte out_pin = LED_BUILTIN;
byte buzzer_pin = 11;
byte channel = A0;

// Direction of the lock
// 0:nothing 1:turn_left 2:turn_right
byte dir = 0;
byte current = 0;
byte last = 0;

byte step = 0;
byte test1, test2;

void setup() {
  // Define the wrong ways
  if (psw[0] < 5) {
    test1 = LEFT;
    test2 = RIGTH;
  } else {
    test1 = RIGTH;
    test2 = LEFT;
  }

  pinMode(out_pin, OUTPUT);
  digitalWrite(out_pin, LOW);
  pinMode(buzzer_pin, OUTPUT);
}

void loop() {

  // Get current position and 
  current = get_pos();
  if (current < last) dir = LEFT;
  else if (current > last) dir = RIGTH;
  else dir = NOTURN;

  if (dir) { // The lock change signal it
    digitalWrite(buzzer_pin, HIGH);
    delay(15);
    digitalWrite(buzzer_pin, LOW);
  }


  // Reset step counter if we turn in the wrong way
  if ((step % 2) == 1 && dir == test1) step = 0;
  if ((step % 2) == 0 && dir == test2) step = 0;

  if (current == psw[step]) step++;

  // When you enter the rigth password
  // this function is activate
  if (step == psw_length) {
    // Digital output enable
    digitalWrite(out_pin, HIGH);

    digitalWrite(buzzer_pin, HIGH);
    delay(150);
    digitalWrite(buzzer_pin, LOW);
    delay(150);
    digitalWrite(buzzer_pin, HIGH);
    delay(150);
    digitalWrite(buzzer_pin, LOW);
    
    // Disable output when we move the lock
    while(current == get_pos()) delay(50);
    digitalWrite(out_pin, LOW);
    // Reset counter
    step = 0;
  }

  last = current; // Save current position  
  delay(10); // Slow down the code
}

byte get_pos() {
  return 9.0 * (float)(analogRead(channel)) / 1023.0 + 0.5;
}
