#pragma once
// mata_SDK Version 5

//////////////////////////////////// Window options
// window name
constexpr const char* WINDOW_NAME = "mata_SDK";

// window size
constexpr int WINDOW_WIDTH = 1200;
constexpr int WINDOW_HEIGHT = 800;

// fullscreen option
constexpr bool FULL_SCREEN_OPTION = false;



//////////////////////////////////// SDK options
// intro screen option
constexpr bool ENABLE_INTRO_SCREEN = true;

// frame limits option
// If this option is set to 0, the system will run at maximum framerate.
constexpr int FRAME_LIMITS = 0;

// When enabled, the GPU computes matrices. When disabled, the CPU computes matrices.
constexpr bool USE_COMPUTE_SHADER = false;



//////////////////////////////////// Development options
// console window option
constexpr bool SHOW_CONSOLE = true;

// FPS indicator option
constexpr bool SHOW_FPS = true;

// show boundbox option
#define SHOW_BOUND_BOX

// Program termination options when developing a program
// When this option is activated, pressing ESC will immediately terminate the program.
#define ENABLE_DEV_EXIT



//////////////////////////////////// Scene Options
// scene layer option
// 'EOL' should be typed at last
enum Layer
{ LAYER1, LAYER2, LAYER3, EOL };

// Specifies the size of the deletion location buffer
constexpr int DELETE_LOCATION_BUFFER_SIZE = 500;



//////////////////////////////////// Sound Options
// Max sound channel option
constexpr int MAX_CHANNEL_SIZE = 32;

// FFT Size option
constexpr int FFT_SIZE = 1024;



//////////////////////////////////// MathUtil Options
//When the relevant threshold is reached in MathUtil::Lerp(), the calculation is stopped immediately to optimize the amount of calculation.
//When set to 0.0, calculation amount optimization is not performed.
constexpr double LERP_THRESHOLD = 0.0001;



//////////////////////////////////// Text Options
// Specifies the maximum length of string that TextUtil can process.
constexpr int MAX_TEXT_LENGTH = 1024;



//////////////////////////////////// File security Options
// Application version
constexpr float APPLICATION_VERSION = 1.0;

// Data file security option
// When this option is activated, the data file is saved as an encrypted file.
// When this option is disabled, the data file is saved as an xml file.
constexpr bool USE_FILE_SECURITY = false;

// AES, IV for FileUtil
// Recommend that you make this part private or 
// modify it with the default password before distributing
constexpr unsigned char AES_KEY[16]
{ "1234567890" };

constexpr unsigned char IV_KEY[16]
{ "1234567890" };