// Global definitions

#define MESSAGE_TYPE _SERIAL   // Communication interface
#define FADE_BY 4            // Controls fade speed between colours
#define EFFECT_NAME_CHARS 16 // Number of chars allowed for effect mode - 1
#define ARGS_NUM 8           // Number of additional effect arguments allowed - 1

// ------------------ SERIAL ------------------ //
#define SERIAL_BEGIN 254     // Message start byte
#define SERIAL_TERMINATE 255 // Message end byte
#define SERIAL_TIMEOUT 1000  // Serial timeout in ms
#define BAUDRATE 9600   // Serial communication speed
// -------------------------------------------- //
