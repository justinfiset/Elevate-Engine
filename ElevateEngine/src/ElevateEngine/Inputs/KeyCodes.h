#pragma once

//from glfw3.h
#ifdef RH_PLATFORM_WINDOWS
	#define EE_KEY_SPACE              32
	#define EE_KEY_APOSTROPHE         39  /* ' */
	#define EE_KEY_COMMA              44  /* , */
	#define EE_KEY_MINUS              45  /* - */
	#define EE_KEY_PERIOD             46  /* . */
	#define EE_KEY_SLASH              47  /* / */
	#define EE_KEY_0                  48
	#define EE_KEY_1                  49
	#define EE_KEY_2                  50
	#define EE_KEY_3                  51
	#define EE_KEY_4                  52
	#define EE_KEY_5                  53
	#define EE_KEY_6                  54
	#define EE_KEY_7                  55
	#define EE_KEY_8                  56
	#define EE_KEY_9                  57
	#define EE_KEY_SEMICOLON          59  /* ; */
	#define EE_KEY_EQUAL              61  /* = */
	#define EE_KEY_A                  65
	#define EE_KEY_B                  66
	#define EE_KEY_C                  67
	#define EE_KEY_D                  68
	#define EE_KEY_E                  69
	#define EE_KEY_F                  70
	#define EE_KEY_G                  71
	#define EE_KEY_H                  72
	#define EE_KEY_I                  73
	#define EE_KEY_J                  74
	#define EE_KEY_K                  75
	#define EE_KEY_L                  76
	#define EE_KEY_M                  77
	#define EE_KEY_N                  78
	#define EE_KEY_O                  79
	#define EE_KEY_P                  80
	#define EE_KEY_Q                  81
	#define EE_KEY_R                  82
	#define EE_KEY_S                  83
	#define EE_KEY_T                  84
	#define EE_KEY_U                  85
	#define EE_KEY_V                  86
	#define EE_KEY_W                  87
	#define EE_KEY_X                  88
	#define EE_KEY_Y                  89
	#define EE_KEY_Z                  90
	#define EE_KEY_LEFT_BRACKET       91  /* [ */
	#define EE_KEY_BACKSLASH          92  /* \ */
	#define EE_KEY_RIGHT_BRACKET      93  /* ] */
	#define EE_KEY_GRAVE_ACCENT       96  /* ` */
	#define EE_KEY_WORLD_1            161 /* non-US #1 */
	#define EE_KEY_WORLD_2            162 /* non-US #2 */

	/* Function keys */
	#define EE_KEY_ESCAPE             256
	#define EE_KEY_ENTER              257
	#define EE_KEY_TAB                258
	#define EE_KEY_BACKSPACE          259
	#define EE_KEY_INSERT             260
	#define EE_KEY_DELETE             261
	#define EE_KEY_RIGHT              262
	#define EE_KEY_LEFT               263
	#define EE_KEY_DOWN               264
	#define EE_KEY_UP                 265
	#define EE_KEY_PAGE_UP            266
	#define EE_KEY_PAGE_DOWN          267
	#define EE_KEY_HOME               268
	#define EE_KEY_END                269
	#define EE_KEY_CAPS_LOCK          280
	#define EE_KEY_SCROLL_LOCK        281
	#define EE_KEY_NUM_LOCK           282
	#define EE_KEY_PRINT_SCREEN       283
	#define EE_KEY_PAUSE              284
	#define EE_KEY_F1                 290
	#define EE_KEY_F2                 291
	#define EE_KEY_F3                 292
	#define EE_KEY_F4                 293
	#define EE_KEY_F5                 294
	#define EE_KEY_F6                 295
	#define EE_KEY_F7                 296
	#define EE_KEY_F8                 297
	#define EE_KEY_F9                 298
	#define EE_KEY_F10                299
	#define EE_KEY_F11                300
	#define EE_KEY_F12                301
	#define EE_KEY_F13                302
	#define EE_KEY_F14                303
	#define EE_KEY_F15                304
	#define EE_KEY_F16                305
	#define EE_KEY_F17                306
	#define EE_KEY_F18                307
	#define EE_KEY_F19                308
	#define EE_KEY_F20                309
	#define EE_KEY_F21                310
	#define EE_KEY_F22                311
	#define EE_KEY_F23                312
	#define EE_KEY_F24                313
	#define EE_KEY_F25                314
	#define EE_KEY_KP_0               320
	#define EE_KEY_KP_1               321
	#define EE_KEY_KP_2               322
	#define EE_KEY_KP_3               323
	#define EE_KEY_KP_4               324
	#define EE_KEY_KP_5               325
	#define EE_KEY_KP_6               326
	#define EE_KEY_KP_7               327
	#define EE_KEY_KP_8               328
	#define EE_KEY_KP_9               329
	#define EE_KEY_KP_DECIMAL         330
	#define EE_KEY_KP_DIVIDE          331
	#define EE_KEY_KP_MULTIPLY        332
	#define EE_KEY_KP_SUBTRACT        333
	#define EE_KEY_KP_ADD             334
	#define EE_KEY_KP_ENTER           335
	#define EE_KEY_KP_EQUAL           336
	#define EE_KEY_LEFT_SHIFT         340
	#define EE_KEY_LEFT_CONTROL       341
	#define EE_KEY_LEFT_ALT           342
	#define EE_KEY_LEFT_SUPER         343 // Ex: Windows key
	#define EE_KEY_RIGHT_SHIFT        344
	#define EE_KEY_RIGHT_CONTROL      345
	#define EE_KEY_RIGHT_ALT          346
	#define EE_KEY_RIGHT_SUPER        347
	#define EE_KEY_MENU               348
#endif