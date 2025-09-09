#ifndef BUILD_SETTINGS_HPP
#define BUILD_SETTINGS_HPP

#define CONTROL_BAR_WIDTH 300
#define WINDOW_WIDTH_START 1100
#define WINDOW_HEIGHT_START 900
#define FOV_HEIGHT_DEGREES_START 75.0
#define LATITUDE_START -0.00732939 // start camera facing at Orion Constellation
#define LONGITUDE_START 1.45937
#define LONGITUDE_SCROLL_RATE 0.4
#define LATITUDE_SCROLL_RATE 0.4
#define VSYNC_START false
#define FPS_REFRESH_TIME 1.0
#define MAX_R_S 5.0
#define MAX_R_CAMERA 100.0
#define INITIAL_R_CAMERA 20.0
#define MIN_LOG10_TIMESTEP_SCALE -4.0
#define MAX_LOG10_TIMESTEP_SCALE 1.0
#define INITIAL_LOG10_TIMESTEP_SCALE -2.0
#define MAX_MAXSTEPS 10000
#define INITIAL_MAXSTEPS 1500
#define UI_ITEM_SPACING 10
#define UI_PADDING 10

#define VERTEX_SHADER_PATH "shaders/vertex_shader.glsl"
#define FRAGMENT_SHADER_PATH "shaders/fragment_shader.glsl"
//#define STARMAP_PATH "resources/TychoSkymapII.t5_08192x04096.jpg"
#define STARMAP_PATH "resources/TychoSkymapII.t3_04096x02048.jpg"
//#define STARMAP_PATH "resources/TychoSkymapII.t5_04096x02048.jpg"
//#define STARMAP_PATH "resources/checkerboard.jpg"

#endif