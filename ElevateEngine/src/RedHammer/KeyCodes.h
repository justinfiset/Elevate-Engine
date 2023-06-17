#pragma once

//from glfw3.h
#ifdef RH_PLATFORM_WINDOWS
	#define RH_KEY_SPACE              32
	#define RH_KEY_APOSTROPHE         39  /* ' */
	#define RH_KEY_COMMA              44  /* , */
	#define RH_KEY_MINUS              45  /* - */
	#define RH_KEY_PERIOD             46  /* . */
	#define RH_KEY_SLASH              47  /* / */
	#define RH_KEY_0                  48
	#define RH_KEY_1                  49
	#define RH_KEY_2                  50
	#define RH_KEY_3                  51
	#define RH_KEY_4                  52
	#define RH_KEY_5                  53
	#define RH_KEY_6                  54
	#define RH_KEY_7                  55
	#define RH_KEY_8                  56
	#define RH_KEY_9                  57
	#define RH_KEY_SEMICOLON          59  /* ; */
	#define RH_KEY_EQUAL              61  /* = */
	#define RH_KEY_A                  65
	#define RH_KEY_B                  66
	#define RH_KEY_C                  67
	#define RH_KEY_D                  68
	#define RH_KEY_E                  69
	#define RH_KEY_F                  70
	#define RH_KEY_G                  71
	#define RH_KEY_H                  72
	#define RH_KEY_I                  73
	#define RH_KEY_J                  74
	#define RH_KEY_K                  75
	#define RH_KEY_L                  76
	#define RH_KEY_M                  77
	#define RH_KEY_N                  78
	#define RH_KEY_O                  79
	#define RH_KEY_P                  80
	#define RH_KEY_Q                  81
	#define RH_KEY_R                  82
	#define RH_KEY_S                  83
	#define RH_KEY_T                  84
	#define RH_KEY_U                  85
	#define RH_KEY_V                  86
	#define RH_KEY_W                  87
	#define RH_KEY_X                  88
	#define RH_KEY_Y                  89
	#define RH_KEY_Z                  90
	#define RH_KEY_LEFT_BRACKET       91  /* [ */
	#define RH_KEY_BACKSLASH          92  /* \ */
	#define RH_KEY_RIGHT_BRACKET      93  /* ] */
	#define RH_KEY_GRAVE_ACCENT       96  /* ` */
	#define RH_KEY_WORLD_1            161 /* non-US #1 */
	#define RH_KEY_WORLD_2            162 /* non-US #2 */

	/* Function keys */
	#define RH_KEY_ESCAPE             256
	#define RH_KEY_ENTER              257
	#define RH_KEY_TAB                258
	#define RH_KEY_BACKSPACE          259
	#define RH_KEY_INSERT             260
	#define RH_KEY_DELETE             261
	#define RH_KEY_RIGHT              262
	#define RH_KEY_LEFT               263
	#define RH_KEY_DOWN               264
	#define RH_KEY_UP                 265
	#define RH_KEY_PAGE_UP            266
	#define RH_KEY_PAGE_DOWN          267
	#define RH_KEY_HOME               268
	#define RH_KEY_END                269
	#define RH_KEY_CAPS_LOCK          280
	#define RH_KEY_SCROLL_LOCK        281
	#define RH_KEY_NUM_LOCK           282
	#define RH_KEY_PRINT_SCREEN       283
	#define RH_KEY_PAUSE              284
	#define RH_KEY_F1                 290
	#define RH_KEY_F2                 291
	#define RH_KEY_F3                 292
	#define RH_KEY_F4                 293
	#define RH_KEY_F5                 294
	#define RH_KEY_F6                 295
	#define RH_KEY_F7                 296
	#define RH_KEY_F8                 297
	#define RH_KEY_F9                 298
	#define RH_KEY_F10                299
	#define RH_KEY_F11                300
	#define RH_KEY_F12                301
	#define RH_KEY_F13                302
	#define RH_KEY_F14                303
	#define RH_KEY_F15                304
	#define RH_KEY_F16                305
	#define RH_KEY_F17                306
	#define RH_KEY_F18                307
	#define RH_KEY_F19                308
	#define RH_KEY_F20                309
	#define RH_KEY_F21                310
	#define RH_KEY_F22                311
	#define RH_KEY_F23                312
	#define RH_KEY_F24                313
	#define RH_KEY_F25                314
	#define RH_KEY_KP_0               320
	#define RH_KEY_KP_1               321
	#define RH_KEY_KP_2               322
	#define RH_KEY_KP_3               323
	#define RH_KEY_KP_4               324
	#define RH_KEY_KP_5               325
	#define RH_KEY_KP_6               326
	#define RH_KEY_KP_7               327
	#define RH_KEY_KP_8               328
	#define RH_KEY_KP_9               329
	#define RH_KEY_KP_DECIMAL         330
	#define RH_KEY_KP_DIVIDE          331
	#define RH_KEY_KP_MULTIPLY        332
	#define RH_KEY_KP_SUBTRACT        333
	#define RH_KEY_KP_ADD             334
	#define RH_KEY_KP_ENTER           335
	#define RH_KEY_KP_EQUAL           336
	#define RH_KEY_LEFT_SHIFT         340
	#define RH_KEY_LEFT_CONTROL       341
	#define RH_KEY_LEFT_ALT           342
	#define RH_KEY_LEFT_SUPER         343 // Ex: Windows key
	#define RH_KEY_RIGHT_SHIFT        344
	#define RH_KEY_RIGHT_CONTROL      345
	#define RH_KEY_RIGHT_ALT          346
	#define RH_KEY_RIGHT_SUPER        347
	#define RH_KEY_MENU               348
#endif