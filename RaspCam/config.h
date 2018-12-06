#ifndef CONFIG_H
#define CONFIG_H

// Window Size
#define D_FRAME_WIDTH	    480
#define D_FRAME_HEIGHT	    320

// the size of picture captured
#define D_CAMEARA_CAPTURE_WIDTH     1024
#define D_CAMEARA_CAPTURE_HEIGHT    768

// the size of picture displayed ( 66 % )
#define D_CAMERA_DISPLAYED_WIDTH   (D_FRAME_WIDTH*3/5)
#define D_CAMERA_DISPLAYED_HEIGHT   (D_FRAME_HEIGHT*3/5)

#define D_CAMERA_SMALL_IMG_WIDTH
#define D_CAMERA_SMALL_IMG_HEIGHT

#define D_CAMERA_POLLING_MSEC   33  // 30 frame


#define D_UI_NUMBER_OF_LOWER_UI_IMGS    5

// network
#define D_NETWORK_SLEEP_MSEC    1


// define types
typedef unsigned char byte;

#define IMAGE_BASE_FOLDER_PATH  "/home/pi/src/Prj/PowerDevelopers/RaspCam/Images/"


#endif // CONFIG_H
