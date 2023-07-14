#define NUM_PLAYERS 4         // number of players 
#define RESET_PIN 6           // pin number for the reset button
#define LIGHT_ON LOW          // pin state that turns on the lights 
#define LIGHT_OFF HIGH        // pin state that turns off the lights
#define STATE_ANIMATING 0     // id of animation state
#define STATE_PLAYING 1       // id of playing state
#define ANIMATION_DURATION 50  // how many 10ms intervals will wait before animating to another light

// pins for buttons and lights. make sure that each button matchs its light
int playerPins[NUM_PLAYERS] = { 2,  3,  4,  5 };
int ledPins[NUM_PLAYERS]    = { 8,  10,  9,  7 };

int currentPlayer = -1;       // the player that is currently selected 
int state = STATE_ANIMATING;  // the initial state for the state machine
int animationCounter = ANIMATION_DURATION;  // counter for each animation
int animationPlayer = 0;                    // what player is turn on during animation

void setup() {
  randomSeed(analogRead(0));  // initialize random generator

  // Initialize the buttons
  for (int i = 0; i < NUM_PLAYERS; i++) {
    pinMode(playerPins[i], INPUT_PULLUP);
  }
  pinMode(RESET_PIN, INPUT_PULLUP);

  // Initialize the lights and turn them all off
  for (int i = 0; i < NUM_PLAYERS; i++) {
    pinMode(ledPins[i], OUTPUT);
    digitalWrite(ledPins[i], LIGHT_OFF);
  }

  goToAnimationState();
}

void loop() {
  switch (state) {
    case STATE_ANIMATING: {
      doAnimation();
      break;
    }

    case STATE_PLAYING: {
      doPlay();
      break;
    }
  }
}

// state transitions

void goToAnimationState() {

  // Reset the game
  currentPlayer = -1;
  state = STATE_ANIMATING;

  // Turn off all LEDs
  for (int i = 0; i < NUM_PLAYERS; i++) {
    digitalWrite(ledPins[i], LIGHT_OFF);
  }

  animationCounter = ANIMATION_DURATION;
  animationPlayer = 0;
  digitalWrite(ledPins[animationPlayer], LIGHT_ON);
}

void goToPlayingState(int player) {

  currentPlayer = player;
  
  for (int j = 0; j < NUM_PLAYERS; j++) {
    if (j == player) {
      digitalWrite(ledPins[player], LIGHT_ON);
    } else {
      pinMode(playerPins[j], INPUT_PULLUP); // Disable other player buttons
      digitalWrite(ledPins[j], LIGHT_OFF); // Turn off other LEDs
    }
  }

  state = STATE_PLAYING;
}

// states

void doAnimation() {
  // Check the player buttons
  for (int i = 0; i < NUM_PLAYERS; i++) {
    if (digitalRead(playerPins[i]) == LOW) {
      goToPlayingState(i);
    }
  }

  if (animationCounter > 0) {
    animationCounter--;
  } else {

    digitalWrite(ledPins[animationPlayer], LIGHT_OFF);
    
    int randomPlayer = 0;
    while (randomPlayer == animationPlayer) {
      randomPlayer = random(4);
    } 
    animationPlayer = randomPlayer;

    digitalWrite(ledPins[animationPlayer], LIGHT_ON);
    animationCounter = ANIMATION_DURATION;
  }
  delay(10);
}

void doPlay() {
  // Check the reset button
  if (digitalRead(RESET_PIN) == LOW) {
    goToAnimationState();
  }
}
