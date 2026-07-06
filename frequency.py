#!/usr/bin/env python

#import subprocess
import time
from time import sleep
#import os
import sys
from os import system


iss_doppler_passes = {
	88: [
	{"time_sec": 1, "doppler_434_khz": 9987, "doppler_144_khz": 3329},
	{"time_sec": 2, "doppler_434_khz": 9987, "doppler_144_khz": 3329},
	{"time_sec": 3, "doppler_434_khz": 9987, "doppler_144_khz": 3329},
	{"time_sec": 4, "doppler_434_khz": 9987, "doppler_144_khz": 3329},
	{"time_sec": 5, "doppler_434_khz": 9987, "doppler_144_khz": 3329},
	{"time_sec": 6, "doppler_434_khz": 9987, "doppler_144_khz": 3329},
	{"time_sec": 7, "doppler_434_khz": 9987, "doppler_144_khz": 3329},
	{"time_sec": 8, "doppler_434_khz": 9987, "doppler_144_khz": 3329},
	{"time_sec": 9, "doppler_434_khz": 9987, "doppler_144_khz": 3329},
	{"time_sec": 10, "doppler_434_khz": 9987, "doppler_144_khz": 3329},
	{"time_sec": 11, "doppler_434_khz": 9987, "doppler_144_khz": 3329},
	{"time_sec": 12, "doppler_434_khz": 9986, "doppler_144_khz": 3329},
	{"time_sec": 13, "doppler_434_khz": 9986, "doppler_144_khz": 3329},
	{"time_sec": 14, "doppler_434_khz": 9986, "doppler_144_khz": 3329},
	{"time_sec": 15, "doppler_434_khz": 9986, "doppler_144_khz": 3329},
	{"time_sec": 16, "doppler_434_khz": 9986, "doppler_144_khz": 3329},
	{"time_sec": 17, "doppler_434_khz": 9985, "doppler_144_khz": 3328},
	{"time_sec": 18, "doppler_434_khz": 9985, "doppler_144_khz": 3328},
	{"time_sec": 19, "doppler_434_khz": 9985, "doppler_144_khz": 3328},
	{"time_sec": 20, "doppler_434_khz": 9985, "doppler_144_khz": 3328},
	{"time_sec": 21, "doppler_434_khz": 9984, "doppler_144_khz": 3328},
	{"time_sec": 22, "doppler_434_khz": 9984, "doppler_144_khz": 3328},
	{"time_sec": 23, "doppler_434_khz": 9984, "doppler_144_khz": 3328},
	{"time_sec": 24, "doppler_434_khz": 9984, "doppler_144_khz": 3328},
	{"time_sec": 25, "doppler_434_khz": 9983, "doppler_144_khz": 3328},
	{"time_sec": 26, "doppler_434_khz": 9983, "doppler_144_khz": 3328},
	{"time_sec": 27, "doppler_434_khz": 9983, "doppler_144_khz": 3328},
	{"time_sec": 28, "doppler_434_khz": 9982, "doppler_144_khz": 3327},
	{"time_sec": 29, "doppler_434_khz": 9982, "doppler_144_khz": 3327},
	{"time_sec": 30, "doppler_434_khz": 9982, "doppler_144_khz": 3327},
	{"time_sec": 31, "doppler_434_khz": 9981, "doppler_144_khz": 3327},
	{"time_sec": 32, "doppler_434_khz": 9981, "doppler_144_khz": 3327},
	{"time_sec": 33, "doppler_434_khz": 9980, "doppler_144_khz": 3327},
	{"time_sec": 34, "doppler_434_khz": 9980, "doppler_144_khz": 3327},
	{"time_sec": 35, "doppler_434_khz": 9979, "doppler_144_khz": 3326},
	{"time_sec": 36, "doppler_434_khz": 9979, "doppler_144_khz": 3326},
	{"time_sec": 37, "doppler_434_khz": 9978, "doppler_144_khz": 3326},
	{"time_sec": 38, "doppler_434_khz": 9978, "doppler_144_khz": 3326},
	{"time_sec": 39, "doppler_434_khz": 9977, "doppler_144_khz": 3326},
	{"time_sec": 40, "doppler_434_khz": 9977, "doppler_144_khz": 3326},
	{"time_sec": 41, "doppler_434_khz": 9976, "doppler_144_khz": 3325},
	{"time_sec": 42, "doppler_434_khz": 9975, "doppler_144_khz": 3325},
	{"time_sec": 43, "doppler_434_khz": 9975, "doppler_144_khz": 3325},
	{"time_sec": 44, "doppler_434_khz": 9974, "doppler_144_khz": 3325},
	{"time_sec": 45, "doppler_434_khz": 9973, "doppler_144_khz": 3324},
	{"time_sec": 46, "doppler_434_khz": 9973, "doppler_144_khz": 3324},
	{"time_sec": 47, "doppler_434_khz": 9972, "doppler_144_khz": 3324},
	{"time_sec": 48, "doppler_434_khz": 9972, "doppler_144_khz": 3324},
	{"time_sec": 49, "doppler_434_khz": 9971, "doppler_144_khz": 3324},
	{"time_sec": 50, "doppler_434_khz": 9970, "doppler_144_khz": 3323},
	{"time_sec": 51, "doppler_434_khz": 9969, "doppler_144_khz": 3323},
	{"time_sec": 52, "doppler_434_khz": 9969, "doppler_144_khz": 3323},
	{"time_sec": 53, "doppler_434_khz": 9968, "doppler_144_khz": 3323},
	{"time_sec": 54, "doppler_434_khz": 9967, "doppler_144_khz": 3322},
	{"time_sec": 55, "doppler_434_khz": 9966, "doppler_144_khz": 3322},
	{"time_sec": 56, "doppler_434_khz": 9966, "doppler_144_khz": 3322},
	{"time_sec": 57, "doppler_434_khz": 9964, "doppler_144_khz": 3321},
	{"time_sec": 58, "doppler_434_khz": 9963, "doppler_144_khz": 3321},
	{"time_sec": 59, "doppler_434_khz": 9963, "doppler_144_khz": 3321},
	{"time_sec": 60, "doppler_434_khz": 9962, "doppler_144_khz": 3321},
	{"time_sec": 61, "doppler_434_khz": 9961, "doppler_144_khz": 3320},
	{"time_sec": 62, "doppler_434_khz": 9959, "doppler_144_khz": 3320},
	{"time_sec": 63, "doppler_434_khz": 9959, "doppler_144_khz": 3320},
	{"time_sec": 64, "doppler_434_khz": 9958, "doppler_144_khz": 3319},
	{"time_sec": 65, "doppler_434_khz": 9957, "doppler_144_khz": 3319},
	{"time_sec": 66, "doppler_434_khz": 9955, "doppler_144_khz": 3318},
	{"time_sec": 67, "doppler_434_khz": 9954, "doppler_144_khz": 3318},
	{"time_sec": 68, "doppler_434_khz": 9953, "doppler_144_khz": 3318},
	{"time_sec": 69, "doppler_434_khz": 9952, "doppler_144_khz": 3317},
	{"time_sec": 70, "doppler_434_khz": 9951, "doppler_144_khz": 3317},
	{"time_sec": 71, "doppler_434_khz": 9950, "doppler_144_khz": 3317},
	{"time_sec": 72, "doppler_434_khz": 9949, "doppler_144_khz": 3316},
	{"time_sec": 73, "doppler_434_khz": 9948, "doppler_144_khz": 3316},
	{"time_sec": 74, "doppler_434_khz": 9946, "doppler_144_khz": 3315},
	{"time_sec": 75, "doppler_434_khz": 9945, "doppler_144_khz": 3315},
	{"time_sec": 76, "doppler_434_khz": 9943, "doppler_144_khz": 3314},
	{"time_sec": 77, "doppler_434_khz": 9942, "doppler_144_khz": 3314},
	{"time_sec": 78, "doppler_434_khz": 9941, "doppler_144_khz": 3314},
	{"time_sec": 79, "doppler_434_khz": 9939, "doppler_144_khz": 3313},
	{"time_sec": 80, "doppler_434_khz": 9938, "doppler_144_khz": 3313},
	{"time_sec": 81, "doppler_434_khz": 9937, "doppler_144_khz": 3312},
	{"time_sec": 82, "doppler_434_khz": 9936, "doppler_144_khz": 3312},
	{"time_sec": 83, "doppler_434_khz": 9933, "doppler_144_khz": 3311},
	{"time_sec": 84, "doppler_434_khz": 9932, "doppler_144_khz": 3311},
	{"time_sec": 85, "doppler_434_khz": 9931, "doppler_144_khz": 3310},
	{"time_sec": 86, "doppler_434_khz": 9929, "doppler_144_khz": 3310},
	{"time_sec": 87, "doppler_434_khz": 9927, "doppler_144_khz": 3309},
	{"time_sec": 88, "doppler_434_khz": 9925, "doppler_144_khz": 3308},
	{"time_sec": 89, "doppler_434_khz": 9924, "doppler_144_khz": 3308},
	{"time_sec": 90, "doppler_434_khz": 9923, "doppler_144_khz": 3308},
	{"time_sec": 91, "doppler_434_khz": 9920, "doppler_144_khz": 3307},
	{"time_sec": 92, "doppler_434_khz": 9918, "doppler_144_khz": 3306},
	{"time_sec": 93, "doppler_434_khz": 9917, "doppler_144_khz": 3306},
	{"time_sec": 94, "doppler_434_khz": 9915, "doppler_144_khz": 3305},
	{"time_sec": 95, "doppler_434_khz": 9912, "doppler_144_khz": 3304},
	{"time_sec": 96, "doppler_434_khz": 9911, "doppler_144_khz": 3304},
	{"time_sec": 97, "doppler_434_khz": 9909, "doppler_144_khz": 3303},
	{"time_sec": 98, "doppler_434_khz": 9907, "doppler_144_khz": 3302},
	{"time_sec": 99, "doppler_434_khz": 9906, "doppler_144_khz": 3302},
	{"time_sec": 100, "doppler_434_khz": 9903, "doppler_144_khz": 3301},
	{"time_sec": 101, "doppler_434_khz": 9901, "doppler_144_khz": 3300},
	{"time_sec": 102, "doppler_434_khz": 9899, "doppler_144_khz": 3300},
	{"time_sec": 103, "doppler_434_khz": 9897, "doppler_144_khz": 3299},
	{"time_sec": 104, "doppler_434_khz": 9894, "doppler_144_khz": 3298},
	{"time_sec": 105, "doppler_434_khz": 9892, "doppler_144_khz": 3297},
	{"time_sec": 106, "doppler_434_khz": 9890, "doppler_144_khz": 3297},
	{"time_sec": 107, "doppler_434_khz": 9888, "doppler_144_khz": 3296},
	{"time_sec": 108, "doppler_434_khz": 9884, "doppler_144_khz": 3295},
	{"time_sec": 109, "doppler_434_khz": 9882, "doppler_144_khz": 3294},
	{"time_sec": 110, "doppler_434_khz": 9880, "doppler_144_khz": 3293},
	{"time_sec": 111, "doppler_434_khz": 9878, "doppler_144_khz": 3293},
	{"time_sec": 112, "doppler_434_khz": 9874, "doppler_144_khz": 3291},
	{"time_sec": 113, "doppler_434_khz": 9872, "doppler_144_khz": 3291},
	{"time_sec": 114, "doppler_434_khz": 9870, "doppler_144_khz": 3290},
	{"time_sec": 115, "doppler_434_khz": 9868, "doppler_144_khz": 3289},
	{"time_sec": 116, "doppler_434_khz": 9864, "doppler_144_khz": 3288},
	{"time_sec": 117, "doppler_434_khz": 9861, "doppler_144_khz": 3287},
	{"time_sec": 118, "doppler_434_khz": 9859, "doppler_144_khz": 3286},
	{"time_sec": 119, "doppler_434_khz": 9857, "doppler_144_khz": 3286},
	{"time_sec": 120, "doppler_434_khz": 9852, "doppler_144_khz": 3284},
	{"time_sec": 121, "doppler_434_khz": 9850, "doppler_144_khz": 3283},
	{"time_sec": 122, "doppler_434_khz": 9847, "doppler_144_khz": 3282},
	{"time_sec": 123, "doppler_434_khz": 9845, "doppler_144_khz": 3282},
	{"time_sec": 124, "doppler_434_khz": 9842, "doppler_144_khz": 3281},
	{"time_sec": 125, "doppler_434_khz": 9837, "doppler_144_khz": 3279},
	{"time_sec": 126, "doppler_434_khz": 9834, "doppler_144_khz": 3278},
	{"time_sec": 127, "doppler_434_khz": 9832, "doppler_144_khz": 3277},
	{"time_sec": 128, "doppler_434_khz": 9829, "doppler_144_khz": 3276},
	{"time_sec": 129, "doppler_434_khz": 9823, "doppler_144_khz": 3274},
	{"time_sec": 130, "doppler_434_khz": 9821, "doppler_144_khz": 3274},
	{"time_sec": 131, "doppler_434_khz": 9818, "doppler_144_khz": 3273},
	{"time_sec": 132, "doppler_434_khz": 9815, "doppler_144_khz": 3272},
	{"time_sec": 133, "doppler_434_khz": 9812, "doppler_144_khz": 3271},
	{"time_sec": 134, "doppler_434_khz": 9806, "doppler_144_khz": 3269},
	{"time_sec": 135, "doppler_434_khz": 9803, "doppler_144_khz": 3268},
	{"time_sec": 136, "doppler_434_khz": 9800, "doppler_144_khz": 3267},
	{"time_sec": 137, "doppler_434_khz": 9796, "doppler_144_khz": 3265},
	{"time_sec": 138, "doppler_434_khz": 9790, "doppler_144_khz": 3263},
	{"time_sec": 139, "doppler_434_khz": 9787, "doppler_144_khz": 3262},
	{"time_sec": 140, "doppler_434_khz": 9783, "doppler_144_khz": 3261},
	{"time_sec": 141, "doppler_434_khz": 9780, "doppler_144_khz": 3260},
	{"time_sec": 142, "doppler_434_khz": 9773, "doppler_144_khz": 3258},
	{"time_sec": 143, "doppler_434_khz": 9769, "doppler_144_khz": 3256},
	{"time_sec": 144, "doppler_434_khz": 9766, "doppler_144_khz": 3255},
	{"time_sec": 145, "doppler_434_khz": 9762, "doppler_144_khz": 3254},
	{"time_sec": 146, "doppler_434_khz": 9759, "doppler_144_khz": 3253},
	{"time_sec": 147, "doppler_434_khz": 9751, "doppler_144_khz": 3250},
	{"time_sec": 148, "doppler_434_khz": 9747, "doppler_144_khz": 3249},
	{"time_sec": 149, "doppler_434_khz": 9743, "doppler_144_khz": 3248},
	{"time_sec": 150, "doppler_434_khz": 9739, "doppler_144_khz": 3246},
	{"time_sec": 151, "doppler_434_khz": 9731, "doppler_144_khz": 3244},
	{"time_sec": 152, "doppler_434_khz": 9727, "doppler_144_khz": 3242},
	{"time_sec": 153, "doppler_434_khz": 9723, "doppler_144_khz": 3241},
	{"time_sec": 154, "doppler_434_khz": 9719, "doppler_144_khz": 3240},
	{"time_sec": 155, "doppler_434_khz": 9715, "doppler_144_khz": 3238},
	{"time_sec": 156, "doppler_434_khz": 9706, "doppler_144_khz": 3235},
	{"time_sec": 157, "doppler_434_khz": 9701, "doppler_144_khz": 3234},
	{"time_sec": 158, "doppler_434_khz": 9697, "doppler_144_khz": 3232},
	{"time_sec": 159, "doppler_434_khz": 9692, "doppler_144_khz": 3231},
	{"time_sec": 160, "doppler_434_khz": 9683, "doppler_144_khz": 3228},
	{"time_sec": 161, "doppler_434_khz": 9678, "doppler_144_khz": 3226},
	{"time_sec": 162, "doppler_434_khz": 9673, "doppler_144_khz": 3224},
	{"time_sec": 163, "doppler_434_khz": 9668, "doppler_144_khz": 3223},
	{"time_sec": 164, "doppler_434_khz": 9658, "doppler_144_khz": 3219},
	{"time_sec": 165, "doppler_434_khz": 9653, "doppler_144_khz": 3218},
	{"time_sec": 166, "doppler_434_khz": 9647, "doppler_144_khz": 3216},
	{"time_sec": 167, "doppler_434_khz": 9642, "doppler_144_khz": 3214},
	{"time_sec": 168, "doppler_434_khz": 9637, "doppler_144_khz": 3212},
	{"time_sec": 169, "doppler_434_khz": 9626, "doppler_144_khz": 3209},
	{"time_sec": 170, "doppler_434_khz": 9620, "doppler_144_khz": 3207},
	{"time_sec": 171, "doppler_434_khz": 9614, "doppler_144_khz": 3205},
	{"time_sec": 172, "doppler_434_khz": 9608, "doppler_144_khz": 3203},
	{"time_sec": 173, "doppler_434_khz": 9597, "doppler_144_khz": 3199},
	{"time_sec": 174, "doppler_434_khz": 9590, "doppler_144_khz": 3197},
	{"time_sec": 175, "doppler_434_khz": 9584, "doppler_144_khz": 3195},
	{"time_sec": 176, "doppler_434_khz": 9578, "doppler_144_khz": 3193},
	{"time_sec": 177, "doppler_434_khz": 9572, "doppler_144_khz": 3191},
	{"time_sec": 178, "doppler_434_khz": 9559, "doppler_144_khz": 3186},
	{"time_sec": 179, "doppler_434_khz": 9552, "doppler_144_khz": 3184},
	{"time_sec": 180, "doppler_434_khz": 9545, "doppler_144_khz": 3182},
	{"time_sec": 181, "doppler_434_khz": 9539, "doppler_144_khz": 3180},
	{"time_sec": 182, "doppler_434_khz": 9525, "doppler_144_khz": 3175},
	{"time_sec": 183, "doppler_434_khz": 9518, "doppler_144_khz": 3173},
	{"time_sec": 184, "doppler_434_khz": 9510, "doppler_144_khz": 3170},
	{"time_sec": 185, "doppler_434_khz": 9503, "doppler_144_khz": 3168},
	{"time_sec": 186, "doppler_434_khz": 9488, "doppler_144_khz": 3163},
	{"time_sec": 187, "doppler_434_khz": 9480, "doppler_144_khz": 3160},
	{"time_sec": 188, "doppler_434_khz": 9472, "doppler_144_khz": 3157},
	{"time_sec": 189, "doppler_434_khz": 9464, "doppler_144_khz": 3155},
	{"time_sec": 190, "doppler_434_khz": 9456, "doppler_144_khz": 3152},
	{"time_sec": 191, "doppler_434_khz": 9440, "doppler_144_khz": 3147},
	{"time_sec": 192, "doppler_434_khz": 9432, "doppler_144_khz": 3144},
	{"time_sec": 193, "doppler_434_khz": 9423, "doppler_144_khz": 3141},
	{"time_sec": 194, "doppler_434_khz": 9414, "doppler_144_khz": 3138},
	{"time_sec": 195, "doppler_434_khz": 9396, "doppler_144_khz": 3132},
	{"time_sec": 196, "doppler_434_khz": 9387, "doppler_144_khz": 3129},
	{"time_sec": 197, "doppler_434_khz": 9378, "doppler_144_khz": 3126},
	{"time_sec": 198, "doppler_434_khz": 9369, "doppler_144_khz": 3123},
	{"time_sec": 199, "doppler_434_khz": 9349, "doppler_144_khz": 3116},
	{"time_sec": 200, "doppler_434_khz": 9339, "doppler_144_khz": 3113},
	{"time_sec": 201, "doppler_434_khz": 9329, "doppler_144_khz": 3110},
	{"time_sec": 202, "doppler_434_khz": 9318, "doppler_144_khz": 3106},
	{"time_sec": 203, "doppler_434_khz": 9308, "doppler_144_khz": 3103},
	{"time_sec": 204, "doppler_434_khz": 9287, "doppler_144_khz": 3096},
	{"time_sec": 205, "doppler_434_khz": 9276, "doppler_144_khz": 3092},
	{"time_sec": 206, "doppler_434_khz": 9265, "doppler_144_khz": 3088},
	{"time_sec": 207, "doppler_434_khz": 9254, "doppler_144_khz": 3085},
	{"time_sec": 208, "doppler_434_khz": 9230, "doppler_144_khz": 3077},
	{"time_sec": 209, "doppler_434_khz": 9218, "doppler_144_khz": 3073},
	{"time_sec": 210, "doppler_434_khz": 9206, "doppler_144_khz": 3069},
	{"time_sec": 211, "doppler_434_khz": 9194, "doppler_144_khz": 3065},
	{"time_sec": 212, "doppler_434_khz": 9182, "doppler_144_khz": 3061},
	{"time_sec": 213, "doppler_434_khz": 9156, "doppler_144_khz": 3052},
	{"time_sec": 214, "doppler_434_khz": 9143, "doppler_144_khz": 3048},
	{"time_sec": 215, "doppler_434_khz": 9130, "doppler_144_khz": 3043},
	{"time_sec": 216, "doppler_434_khz": 9116, "doppler_144_khz": 3039},
	{"time_sec": 217, "doppler_434_khz": 9088, "doppler_144_khz": 3029},
	{"time_sec": 218, "doppler_434_khz": 9074, "doppler_144_khz": 3025},
	{"time_sec": 219, "doppler_434_khz": 9060, "doppler_144_khz": 3020},
	{"time_sec": 220, "doppler_434_khz": 9045, "doppler_144_khz": 3015},
	{"time_sec": 221, "doppler_434_khz": 9015, "doppler_144_khz": 3005},
	{"time_sec": 222, "doppler_434_khz": 8999, "doppler_144_khz": 3000},
	{"time_sec": 223, "doppler_434_khz": 8983, "doppler_144_khz": 2994},
	{"time_sec": 224, "doppler_434_khz": 8967, "doppler_144_khz": 2989},
	{"time_sec": 225, "doppler_434_khz": 8951, "doppler_144_khz": 2984},
	{"time_sec": 226, "doppler_434_khz": 8917, "doppler_144_khz": 2972},
	{"time_sec": 227, "doppler_434_khz": 8900, "doppler_144_khz": 2967},
	{"time_sec": 228, "doppler_434_khz": 8882, "doppler_144_khz": 2961},
	{"time_sec": 229, "doppler_434_khz": 8865, "doppler_144_khz": 2955},
	{"time_sec": 230, "doppler_434_khz": 8828, "doppler_144_khz": 2943},
	{"time_sec": 231, "doppler_434_khz": 8809, "doppler_144_khz": 2936},
	{"time_sec": 232, "doppler_434_khz": 8790, "doppler_144_khz": 2930},
	{"time_sec": 233, "doppler_434_khz": 8770, "doppler_144_khz": 2923},
	{"time_sec": 234, "doppler_434_khz": 8730, "doppler_144_khz": 2910},
	{"time_sec": 235, "doppler_434_khz": 8709, "doppler_144_khz": 2903},
	{"time_sec": 236, "doppler_434_khz": 8688, "doppler_144_khz": 2896},
	{"time_sec": 237, "doppler_434_khz": 8665, "doppler_144_khz": 2888},
	{"time_sec": 238, "doppler_434_khz": 8643, "doppler_144_khz": 2881},
	{"time_sec": 239, "doppler_434_khz": 8597, "doppler_144_khz": 2866},
	{"time_sec": 240, "doppler_434_khz": 8574, "doppler_144_khz": 2858},
	{"time_sec": 241, "doppler_434_khz": 8550, "doppler_144_khz": 2850},
	{"time_sec": 242, "doppler_434_khz": 8526, "doppler_144_khz": 2842},
	{"time_sec": 243, "doppler_434_khz": 8476, "doppler_144_khz": 2825},
	{"time_sec": 244, "doppler_434_khz": 8450, "doppler_144_khz": 2817},
	{"time_sec": 245, "doppler_434_khz": 8423, "doppler_144_khz": 2808},
	{"time_sec": 246, "doppler_434_khz": 8397, "doppler_144_khz": 2799},
	{"time_sec": 247, "doppler_434_khz": 8370, "doppler_144_khz": 2790},
	{"time_sec": 248, "doppler_434_khz": 8314, "doppler_144_khz": 2771},
	{"time_sec": 249, "doppler_434_khz": 8286, "doppler_144_khz": 2762},
	{"time_sec": 250, "doppler_434_khz": 8256, "doppler_144_khz": 2752},
	{"time_sec": 251, "doppler_434_khz": 8226, "doppler_144_khz": 2742},
	{"time_sec": 252, "doppler_434_khz": 8164, "doppler_144_khz": 2721},
	{"time_sec": 253, "doppler_434_khz": 8133, "doppler_144_khz": 2711},
	{"time_sec": 254, "doppler_434_khz": 8101, "doppler_144_khz": 2700},
	{"time_sec": 255, "doppler_434_khz": 8068, "doppler_144_khz": 2689},
	{"time_sec": 256, "doppler_434_khz": 8034, "doppler_144_khz": 2678},
	{"time_sec": 257, "doppler_434_khz": 7965, "doppler_144_khz": 2655},
	{"time_sec": 258, "doppler_434_khz": 7929, "doppler_144_khz": 2643},
	{"time_sec": 259, "doppler_434_khz": 7893, "doppler_144_khz": 2631},
	{"time_sec": 260, "doppler_434_khz": 7856, "doppler_144_khz": 2619},
	{"time_sec": 261, "doppler_434_khz": 7779, "doppler_144_khz": 2593},
	{"time_sec": 262, "doppler_434_khz": 7740, "doppler_144_khz": 2580},
	{"time_sec": 263, "doppler_434_khz": 7699, "doppler_144_khz": 2566},
	{"time_sec": 264, "doppler_434_khz": 7658, "doppler_144_khz": 2553},
	{"time_sec": 265, "doppler_434_khz": 7574, "doppler_144_khz": 2525},
	{"time_sec": 266, "doppler_434_khz": 7530, "doppler_144_khz": 2510},
	{"time_sec": 267, "doppler_434_khz": 7485, "doppler_144_khz": 2495},
	{"time_sec": 268, "doppler_434_khz": 7440, "doppler_144_khz": 2480},
	{"time_sec": 269, "doppler_434_khz": 7394, "doppler_144_khz": 2465},
	{"time_sec": 270, "doppler_434_khz": 7298, "doppler_144_khz": 2433},
	{"time_sec": 271, "doppler_434_khz": 7248, "doppler_144_khz": 2416},
	{"time_sec": 272, "doppler_434_khz": 7198, "doppler_144_khz": 2399},
	{"time_sec": 273, "doppler_434_khz": 7143, "doppler_144_khz": 2381},
	{"time_sec": 274, "doppler_434_khz": 7037, "doppler_144_khz": 2346},
	{"time_sec": 275, "doppler_434_khz": 6982, "doppler_144_khz": 2327},
	{"time_sec": 276, "doppler_434_khz": 6927, "doppler_144_khz": 2309},
	{"time_sec": 277, "doppler_434_khz": 6870, "doppler_144_khz": 2290},
	{"time_sec": 278, "doppler_434_khz": 6812, "doppler_144_khz": 2271},
	{"time_sec": 279, "doppler_434_khz": 6693, "doppler_144_khz": 2231},
	{"time_sec": 280, "doppler_434_khz": 6631, "doppler_144_khz": 2210},
	{"time_sec": 281, "doppler_434_khz": 6568, "doppler_144_khz": 2189},
	{"time_sec": 282, "doppler_434_khz": 6504, "doppler_144_khz": 2168},
	{"time_sec": 283, "doppler_434_khz": 6373, "doppler_144_khz": 2124},
	{"time_sec": 284, "doppler_434_khz": 6305, "doppler_144_khz": 2102},
	{"time_sec": 285, "doppler_434_khz": 6235, "doppler_144_khz": 2078},
	{"time_sec": 286, "doppler_434_khz": 6165, "doppler_144_khz": 2055},
	{"time_sec": 287, "doppler_434_khz": 6020, "doppler_144_khz": 2007},
	{"time_sec": 288, "doppler_434_khz": 5945, "doppler_144_khz": 1982},
	{"time_sec": 289, "doppler_434_khz": 5868, "doppler_144_khz": 1956},
	{"time_sec": 290, "doppler_434_khz": 5791, "doppler_144_khz": 1930},
	{"time_sec": 291, "doppler_434_khz": 5711, "doppler_144_khz": 1904},
	{"time_sec": 292, "doppler_434_khz": 5548, "doppler_144_khz": 1849},
	{"time_sec": 293, "doppler_434_khz": 5464, "doppler_144_khz": 1821},
	{"time_sec": 294, "doppler_434_khz": 5379, "doppler_144_khz": 1793},
	{"time_sec": 295, "doppler_434_khz": 5292, "doppler_144_khz": 1764},
	{"time_sec": 296, "doppler_434_khz": 5113, "doppler_144_khz": 1704},
	{"time_sec": 297, "doppler_434_khz": 5022, "doppler_144_khz": 1674},
	{"time_sec": 298, "doppler_434_khz": 4928, "doppler_144_khz": 1643},
	{"time_sec": 299, "doppler_434_khz": 4834, "doppler_144_khz": 1611},
	{"time_sec": 300, "doppler_434_khz": 4640, "doppler_144_khz": 1547},
	{"time_sec": 301, "doppler_434_khz": 4540, "doppler_144_khz": 1513},
	{"time_sec": 302, "doppler_434_khz": 4438, "doppler_144_khz": 1479},
	{"time_sec": 303, "doppler_434_khz": 4334, "doppler_144_khz": 1445},
	{"time_sec": 304, "doppler_434_khz": 4229, "doppler_144_khz": 1410},
	{"time_sec": 305, "doppler_434_khz": 4016, "doppler_144_khz": 1339},
	{"time_sec": 306, "doppler_434_khz": 3907, "doppler_144_khz": 1302},
	{"time_sec": 307, "doppler_434_khz": 3796, "doppler_144_khz": 1265},
	{"time_sec": 308, "doppler_434_khz": 3684, "doppler_144_khz": 1228},
	{"time_sec": 309, "doppler_434_khz": 3448, "doppler_144_khz": 1149},
	{"time_sec": 310, "doppler_434_khz": 3330, "doppler_144_khz": 1110},
	{"time_sec": 311, "doppler_434_khz": 3212, "doppler_144_khz": 1071},
	{"time_sec": 312, "doppler_434_khz": 3091, "doppler_144_khz": 1030},
	{"time_sec": 313, "doppler_434_khz": 2969, "doppler_144_khz": 990},
	{"time_sec": 314, "doppler_434_khz": 2722, "doppler_144_khz": 907},
	{"time_sec": 315, "doppler_434_khz": 2595, "doppler_144_khz": 865},
	{"time_sec": 316, "doppler_434_khz": 2466, "doppler_144_khz": 822},
	{"time_sec": 317, "doppler_434_khz": 2337, "doppler_144_khz": 779},
	{"time_sec": 318, "doppler_434_khz": 2206, "doppler_144_khz": 735},
	{"time_sec": 319, "doppler_434_khz": 1941, "doppler_144_khz": 647},
	{"time_sec": 320, "doppler_434_khz": 1807, "doppler_144_khz": 602},
	{"time_sec": 321, "doppler_434_khz": 1672, "doppler_144_khz": 557},
	{"time_sec": 322, "doppler_434_khz": 1536, "doppler_144_khz": 512},
	{"time_sec": 323, "doppler_434_khz": 1395, "doppler_144_khz": 465},
	{"time_sec": 324, "doppler_434_khz": 1119, "doppler_144_khz": 373},
	{"time_sec": 325, "doppler_434_khz": 979, "doppler_144_khz": 326},
	{"time_sec": 326, "doppler_434_khz": 839, "doppler_144_khz": 280},
	{"time_sec": 327, "doppler_434_khz": 699, "doppler_144_khz": 233},
	{"time_sec": 328, "doppler_434_khz": 418, "doppler_144_khz": 139},
	{"time_sec": 329, "doppler_434_khz": 277, "doppler_144_khz": 92},
	{"time_sec": 330, "doppler_434_khz": 137, "doppler_144_khz": 46},
	{"time_sec": 331, "doppler_434_khz": -2, "doppler_144_khz": 0},
	{"time_sec": 332, "doppler_434_khz": -143, "doppler_144_khz": -47},
	{"time_sec": 333, "doppler_434_khz": -427, "doppler_144_khz": -142},
	{"time_sec": 334, "doppler_434_khz": -569, "doppler_144_khz": -189},
	{"time_sec": 335, "doppler_434_khz": -709, "doppler_144_khz": -236},
	{"time_sec": 336, "doppler_434_khz": -848, "doppler_144_khz": -282},
	{"time_sec": 337, "doppler_434_khz": -987, "doppler_144_khz": -328},
	{"time_sec": 338, "doppler_434_khz": -1263, "doppler_144_khz": -420},
	{"time_sec": 339, "doppler_434_khz": -1400, "doppler_144_khz": -466},
	{"time_sec": 340, "doppler_434_khz": -1537, "doppler_144_khz": -512},
	{"time_sec": 341, "doppler_434_khz": -1672, "doppler_144_khz": -557},
	{"time_sec": 342, "doppler_434_khz": -1806, "doppler_144_khz": -601},
	{"time_sec": 343, "doppler_434_khz": -2072, "doppler_144_khz": -690},
	{"time_sec": 344, "doppler_434_khz": -2204, "doppler_144_khz": -734},
	{"time_sec": 345, "doppler_434_khz": -2335, "doppler_144_khz": -778},
	{"time_sec": 346, "doppler_434_khz": -2471, "doppler_144_khz": -823},
	{"time_sec": 347, "doppler_434_khz": -2599, "doppler_144_khz": -866},
	{"time_sec": 348, "doppler_434_khz": -2851, "doppler_144_khz": -950},
	{"time_sec": 349, "doppler_434_khz": -2975, "doppler_144_khz": -991},
	{"time_sec": 350, "doppler_434_khz": -3098, "doppler_144_khz": -1032},
	{"time_sec": 351, "doppler_434_khz": -3220, "doppler_144_khz": -1073},
	{"time_sec": 352, "doppler_434_khz": -3458, "doppler_144_khz": -1152},
	{"time_sec": 353, "doppler_434_khz": -3574, "doppler_144_khz": -1191},
	{"time_sec": 354, "doppler_434_khz": -3693, "doppler_144_khz": -1230},
	{"time_sec": 355, "doppler_434_khz": -3807, "doppler_144_khz": -1268},
	{"time_sec": 356, "doppler_434_khz": -3921, "doppler_144_khz": -1306},
	{"time_sec": 357, "doppler_434_khz": -4141, "doppler_144_khz": -1380},
	{"time_sec": 358, "doppler_434_khz": -4249, "doppler_144_khz": -1416},
	{"time_sec": 359, "doppler_434_khz": -4356, "doppler_144_khz": -1451},
	{"time_sec": 360, "doppler_434_khz": -4460, "doppler_144_khz": -1486},
	{"time_sec": 361, "doppler_434_khz": -4563, "doppler_144_khz": -1520},
	{"time_sec": 362, "doppler_434_khz": -4763, "doppler_144_khz": -1587},
	{"time_sec": 363, "doppler_434_khz": -4861, "doppler_144_khz": -1620},
	{"time_sec": 364, "doppler_434_khz": -4956, "doppler_144_khz": -1651},
	{"time_sec": 365, "doppler_434_khz": -5050, "doppler_144_khz": -1683},
	{"time_sec": 366, "doppler_434_khz": -5142, "doppler_144_khz": -1713},
	{"time_sec": 367, "doppler_434_khz": -5323, "doppler_144_khz": -1774},
	{"time_sec": 368, "doppler_434_khz": -5410, "doppler_144_khz": -1803},
	{"time_sec": 369, "doppler_434_khz": -5497, "doppler_144_khz": -1832},
	{"time_sec": 370, "doppler_434_khz": -5582, "doppler_144_khz": -1860},
	{"time_sec": 371, "doppler_434_khz": -5665, "doppler_144_khz": -1888},
	{"time_sec": 372, "doppler_434_khz": -5825, "doppler_144_khz": -1941},
	{"time_sec": 373, "doppler_434_khz": -5904, "doppler_144_khz": -1967},
	{"time_sec": 374, "doppler_434_khz": -5981, "doppler_144_khz": -1993},
	{"time_sec": 375, "doppler_434_khz": -6056, "doppler_144_khz": -2018},
	{"time_sec": 376, "doppler_434_khz": -6203, "doppler_144_khz": -2067},
	{"time_sec": 377, "doppler_434_khz": -6275, "doppler_144_khz": -2091},
	{"time_sec": 378, "doppler_434_khz": -6344, "doppler_144_khz": -2114},
	{"time_sec": 379, "doppler_434_khz": -6413, "doppler_144_khz": -2137},
	{"time_sec": 380, "doppler_434_khz": -6479, "doppler_144_khz": -2159},
	{"time_sec": 381, "doppler_434_khz": -6609, "doppler_144_khz": -2202},
	{"time_sec": 382, "doppler_434_khz": -6676, "doppler_144_khz": -2225},
	{"time_sec": 383, "doppler_434_khz": -6738, "doppler_144_khz": -2245},
	{"time_sec": 384, "doppler_434_khz": -6798, "doppler_144_khz": -2265},
	{"time_sec": 385, "doppler_434_khz": -6858, "doppler_144_khz": -2285},
	{"time_sec": 386, "doppler_434_khz": -6974, "doppler_144_khz": -2324},
	{"time_sec": 387, "doppler_434_khz": -7031, "doppler_144_khz": -2343},
	{"time_sec": 388, "doppler_434_khz": -7086, "doppler_144_khz": -2361},
	{"time_sec": 389, "doppler_434_khz": -7139, "doppler_144_khz": -2379},
	{"time_sec": 390, "doppler_434_khz": -7191, "doppler_144_khz": -2396},
	{"time_sec": 391, "doppler_434_khz": -7292, "doppler_144_khz": -2430},
	{"time_sec": 392, "doppler_434_khz": -7342, "doppler_144_khz": -2447},
	{"time_sec": 393, "doppler_434_khz": -7390, "doppler_144_khz": -2463},
	{"time_sec": 394, "doppler_434_khz": -7437, "doppler_144_khz": -2478},
	{"time_sec": 395, "doppler_434_khz": -7483, "doppler_144_khz": -2494},
	{"time_sec": 396, "doppler_434_khz": -7573, "doppler_144_khz": -2524},
	{"time_sec": 397, "doppler_434_khz": -7617, "doppler_144_khz": -2538},
	{"time_sec": 398, "doppler_434_khz": -7659, "doppler_144_khz": -2552},
	{"time_sec": 399, "doppler_434_khz": -7701, "doppler_144_khz": -2566},
	{"time_sec": 400, "doppler_434_khz": -7742, "doppler_144_khz": -2580},
	{"time_sec": 401, "doppler_434_khz": -7821, "doppler_144_khz": -2606},
	{"time_sec": 402, "doppler_434_khz": -7860, "doppler_144_khz": -2619},
	{"time_sec": 403, "doppler_434_khz": -7898, "doppler_144_khz": -2632},
	{"time_sec": 404, "doppler_434_khz": -7935, "doppler_144_khz": -2644},
	{"time_sec": 405, "doppler_434_khz": -7971, "doppler_144_khz": -2656},
	{"time_sec": 406, "doppler_434_khz": -8041, "doppler_144_khz": -2680},
	{"time_sec": 407, "doppler_434_khz": -8076, "doppler_144_khz": -2691},
	{"time_sec": 408, "doppler_434_khz": -8109, "doppler_144_khz": -2702},
	{"time_sec": 409, "doppler_434_khz": -8142, "doppler_144_khz": -2713},
	{"time_sec": 410, "doppler_434_khz": -8205, "doppler_144_khz": -2734},
	{"time_sec": 411, "doppler_434_khz": -8236, "doppler_144_khz": -2745},
	{"time_sec": 412, "doppler_434_khz": -8266, "doppler_144_khz": -2755},
	{"time_sec": 413, "doppler_434_khz": -8296, "doppler_144_khz": -2765},
	{"time_sec": 414, "doppler_434_khz": -8325, "doppler_144_khz": -2774},
	{"time_sec": 415, "doppler_434_khz": -8381, "doppler_144_khz": -2793},
	{"time_sec": 416, "doppler_434_khz": -8408, "doppler_144_khz": -2802},
	{"time_sec": 417, "doppler_434_khz": -8435, "doppler_144_khz": -2811},
	{"time_sec": 418, "doppler_434_khz": -8462, "doppler_144_khz": -2820},
	{"time_sec": 419, "doppler_434_khz": -8487, "doppler_144_khz": -2828},
	{"time_sec": 420, "doppler_434_khz": -8539, "doppler_144_khz": -2846},
	{"time_sec": 421, "doppler_434_khz": -8563, "doppler_144_khz": -2854},
	{"time_sec": 422, "doppler_434_khz": -8587, "doppler_144_khz": -2862},
	{"time_sec": 423, "doppler_434_khz": -8610, "doppler_144_khz": -2869},
	{"time_sec": 424, "doppler_434_khz": -8633, "doppler_144_khz": -2877},
	{"time_sec": 425, "doppler_434_khz": -8678, "doppler_144_khz": -2892},
	{"time_sec": 426, "doppler_434_khz": -8700, "doppler_144_khz": -2899},
	{"time_sec": 427, "doppler_434_khz": -8721, "doppler_144_khz": -2906},
	{"time_sec": 428, "doppler_434_khz": -8742, "doppler_144_khz": -2913},
	{"time_sec": 429, "doppler_434_khz": -8762, "doppler_144_khz": -2920},
	{"time_sec": 430, "doppler_434_khz": -8802, "doppler_144_khz": -2933},
	{"time_sec": 431, "doppler_434_khz": -8822, "doppler_144_khz": -2940},
	{"time_sec": 432, "doppler_434_khz": -8841, "doppler_144_khz": -2946},
	{"time_sec": 433, "doppler_434_khz": -8860, "doppler_144_khz": -2953},
	{"time_sec": 434, "doppler_434_khz": -8878, "doppler_144_khz": -2959},
	{"time_sec": 435, "doppler_434_khz": -8915, "doppler_144_khz": -2971},
	{"time_sec": 436, "doppler_434_khz": -8932, "doppler_144_khz": -2977},
	{"time_sec": 437, "doppler_434_khz": -8949, "doppler_144_khz": -2982},
	{"time_sec": 438, "doppler_434_khz": -8966, "doppler_144_khz": -2988},
	{"time_sec": 439, "doppler_434_khz": -8982, "doppler_144_khz": -2993},
	{"time_sec": 440, "doppler_434_khz": -9015, "doppler_144_khz": -3004},
	{"time_sec": 441, "doppler_434_khz": -9030, "doppler_144_khz": -3009},
	{"time_sec": 442, "doppler_434_khz": -9045, "doppler_144_khz": -3014},
	{"time_sec": 443, "doppler_434_khz": -9061, "doppler_144_khz": -3020},
	{"time_sec": 444, "doppler_434_khz": -9075, "doppler_144_khz": -3024},
	{"time_sec": 445, "doppler_434_khz": -9104, "doppler_144_khz": -3034},
	{"time_sec": 446, "doppler_434_khz": -9118, "doppler_144_khz": -3039},
	{"time_sec": 447, "doppler_434_khz": -9132, "doppler_144_khz": -3043},
	{"time_sec": 448, "doppler_434_khz": -9146, "doppler_144_khz": -3048},
	{"time_sec": 449, "doppler_434_khz": -9172, "doppler_144_khz": -3057},
	{"time_sec": 450, "doppler_434_khz": -9185, "doppler_144_khz": -3061},
	{"time_sec": 451, "doppler_434_khz": -9197, "doppler_144_khz": -3065},
	{"time_sec": 452, "doppler_434_khz": -9210, "doppler_144_khz": -3069},
	{"time_sec": 453, "doppler_434_khz": -9222, "doppler_144_khz": -3073},
	{"time_sec": 454, "doppler_434_khz": -9246, "doppler_144_khz": -3081},
	{"time_sec": 455, "doppler_434_khz": -9258, "doppler_144_khz": -3085},
	{"time_sec": 456, "doppler_434_khz": -9270, "doppler_144_khz": -3089},
	{"time_sec": 457, "doppler_434_khz": -9281, "doppler_144_khz": -3093},
	{"time_sec": 458, "doppler_434_khz": -9292, "doppler_144_khz": -3097},
	{"time_sec": 459, "doppler_434_khz": -9313, "doppler_144_khz": -3104},
	{"time_sec": 460, "doppler_434_khz": -9324, "doppler_144_khz": -3107},
	{"time_sec": 461, "doppler_434_khz": -9334, "doppler_144_khz": -3111},
	{"time_sec": 462, "doppler_434_khz": -9344, "doppler_144_khz": -3114},
	{"time_sec": 463, "doppler_434_khz": -9354, "doppler_144_khz": -3117},
	{"time_sec": 464, "doppler_434_khz": -9374, "doppler_144_khz": -3124},
	{"time_sec": 465, "doppler_434_khz": -9383, "doppler_144_khz": -3127},
	{"time_sec": 466, "doppler_434_khz": -9393, "doppler_144_khz": -3130},
	{"time_sec": 467, "doppler_434_khz": -9402, "doppler_144_khz": -3133},
	{"time_sec": 468, "doppler_434_khz": -9411, "doppler_144_khz": -3136},
	{"time_sec": 469, "doppler_434_khz": -9429, "doppler_144_khz": -3142},
	{"time_sec": 470, "doppler_434_khz": -9438, "doppler_144_khz": -3145},
	{"time_sec": 471, "doppler_434_khz": -9446, "doppler_144_khz": -3148},
	{"time_sec": 472, "doppler_434_khz": -9454, "doppler_144_khz": -3151},
	{"time_sec": 473, "doppler_434_khz": -9470, "doppler_144_khz": -3156},
	{"time_sec": 474, "doppler_434_khz": -9478, "doppler_144_khz": -3159},
	{"time_sec": 475, "doppler_434_khz": -9486, "doppler_144_khz": -3161},
	{"time_sec": 476, "doppler_434_khz": -9494, "doppler_144_khz": -3164},
	{"time_sec": 477, "doppler_434_khz": -9501, "doppler_144_khz": -3166},
	{"time_sec": 478, "doppler_434_khz": -9516, "doppler_144_khz": -3171},
	{"time_sec": 479, "doppler_434_khz": -9523, "doppler_144_khz": -3174},
	{"time_sec": 480, "doppler_434_khz": -9530, "doppler_144_khz": -3176},
	{"time_sec": 481, "doppler_434_khz": -9537, "doppler_144_khz": -3178},
	{"time_sec": 482, "doppler_434_khz": -9551, "doppler_144_khz": -3183},
	{"time_sec": 483, "doppler_434_khz": -9557, "doppler_144_khz": -3185},
	{"time_sec": 484, "doppler_434_khz": -9564, "doppler_144_khz": -3187},
	{"time_sec": 485, "doppler_434_khz": -9570, "doppler_144_khz": -3189},
	{"time_sec": 486, "doppler_434_khz": -9583, "doppler_144_khz": -3194},
	{"time_sec": 487, "doppler_434_khz": -9589, "doppler_144_khz": -3196},
	{"time_sec": 488, "doppler_434_khz": -9595, "doppler_144_khz": -3198},
	{"time_sec": 489, "doppler_434_khz": -9601, "doppler_144_khz": -3200},
	{"time_sec": 490, "doppler_434_khz": -9607, "doppler_144_khz": -3202},
	{"time_sec": 491, "doppler_434_khz": -9619, "doppler_144_khz": -3206},
	{"time_sec": 492, "doppler_434_khz": -9625, "doppler_144_khz": -3208},
	{"time_sec": 493, "doppler_434_khz": -9630, "doppler_144_khz": -3209},
	{"time_sec": 494, "doppler_434_khz": -9636, "doppler_144_khz": -3211},
	{"time_sec": 495, "doppler_434_khz": -9642, "doppler_144_khz": -3213},
	{"time_sec": 496, "doppler_434_khz": -9652, "doppler_144_khz": -3217},
	{"time_sec": 497, "doppler_434_khz": -9658, "doppler_144_khz": -3219},
	{"time_sec": 498, "doppler_434_khz": -9663, "doppler_144_khz": -3220},
	{"time_sec": 499, "doppler_434_khz": -9668, "doppler_144_khz": -3222},
	{"time_sec": 500, "doppler_434_khz": -9678, "doppler_144_khz": -3225},
	{"time_sec": 501, "doppler_434_khz": -9683, "doppler_144_khz": -3227},
	{"time_sec": 502, "doppler_434_khz": -9687, "doppler_144_khz": -3228},
	{"time_sec": 503, "doppler_434_khz": -9692, "doppler_144_khz": -3230},
	{"time_sec": 504, "doppler_434_khz": -9697, "doppler_144_khz": -3232},
	{"time_sec": 505, "doppler_434_khz": -9706, "doppler_144_khz": -3235},
	{"time_sec": 506, "doppler_434_khz": -9710, "doppler_144_khz": -3236},
	{"time_sec": 507, "doppler_434_khz": -9715, "doppler_144_khz": -3238},
	{"time_sec": 508, "doppler_434_khz": -9719, "doppler_144_khz": -3239},
	{"time_sec": 509, "doppler_434_khz": -9727, "doppler_144_khz": -3242},
	{"time_sec": 510, "doppler_434_khz": -9732, "doppler_144_khz": -3243},
	{"time_sec": 511, "doppler_434_khz": -9736, "doppler_144_khz": -3245},
	{"time_sec": 512, "doppler_434_khz": -9740, "doppler_144_khz": -3246},
	{"time_sec": 513, "doppler_434_khz": -9747, "doppler_144_khz": -3248},
	{"time_sec": 514, "doppler_434_khz": -9751, "doppler_144_khz": -3250},
	{"time_sec": 515, "doppler_434_khz": -9755, "doppler_144_khz": -3251},
	{"time_sec": 516, "doppler_434_khz": -9759, "doppler_144_khz": -3252},
	{"time_sec": 517, "doppler_434_khz": -9763, "doppler_144_khz": -3254},
	{"time_sec": 518, "doppler_434_khz": -9770, "doppler_144_khz": -3256},
	{"time_sec": 519, "doppler_434_khz": -9773, "doppler_144_khz": -3257},
	{"time_sec": 520, "doppler_434_khz": -9777, "doppler_144_khz": -3258},
	{"time_sec": 521, "doppler_434_khz": -9780, "doppler_144_khz": -3259},
	{"time_sec": 522, "doppler_434_khz": -9787, "doppler_144_khz": -3262},
	{"time_sec": 523, "doppler_434_khz": -9791, "doppler_144_khz": -3263},
	{"time_sec": 524, "doppler_434_khz": -9794, "doppler_144_khz": -3264},
	{"time_sec": 525, "doppler_434_khz": -9797, "doppler_144_khz": -3265},
	{"time_sec": 526, "doppler_434_khz": -9800, "doppler_144_khz": -3266},
	{"time_sec": 527, "doppler_434_khz": -9807, "doppler_144_khz": -3268},
	{"time_sec": 528, "doppler_434_khz": -9810, "doppler_144_khz": -3269},
	{"time_sec": 529, "doppler_434_khz": -9813, "doppler_144_khz": -3270},
	{"time_sec": 530, "doppler_434_khz": -9816, "doppler_144_khz": -3271},
	{"time_sec": 531, "doppler_434_khz": -9822, "doppler_144_khz": -3273},
	{"time_sec": 532, "doppler_434_khz": -9825, "doppler_144_khz": -3274},
	{"time_sec": 533, "doppler_434_khz": -9827, "doppler_144_khz": -3275},
	{"time_sec": 534, "doppler_434_khz": -9830, "doppler_144_khz": -3276},
	{"time_sec": 535, "doppler_434_khz": -9833, "doppler_144_khz": -3277},
	{"time_sec": 536, "doppler_434_khz": -9838, "doppler_144_khz": -3279},
	{"time_sec": 537, "doppler_434_khz": -9841, "doppler_144_khz": -3280},
	{"time_sec": 538, "doppler_434_khz": -9843, "doppler_144_khz": -3280},
	{"time_sec": 539, "doppler_434_khz": -9846, "doppler_144_khz": -3281},
	{"time_sec": 540, "doppler_434_khz": -9851, "doppler_144_khz": -3283},
	{"time_sec": 541, "doppler_434_khz": -9853, "doppler_144_khz": -3284},
	{"time_sec": 542, "doppler_434_khz": -9856, "doppler_144_khz": -3285},
	{"time_sec": 543, "doppler_434_khz": -9858, "doppler_144_khz": -3285},
	{"time_sec": 544, "doppler_434_khz": -9861, "doppler_144_khz": -3286},
	{"time_sec": 545, "doppler_434_khz": -9865, "doppler_144_khz": -3288},
	{"time_sec": 546, "doppler_434_khz": -9867, "doppler_144_khz": -3288},
	{"time_sec": 547, "doppler_434_khz": -9870, "doppler_144_khz": -3289},
	{"time_sec": 548, "doppler_434_khz": -9872, "doppler_144_khz": -3290},
	{"time_sec": 549, "doppler_434_khz": -9876, "doppler_144_khz": -3291},
	{"time_sec": 550, "doppler_434_khz": -9878, "doppler_144_khz": -3292},
	{"time_sec": 551, "doppler_434_khz": -9880, "doppler_144_khz": -3293},
	{"time_sec": 552, "doppler_434_khz": -9882, "doppler_144_khz": -3293},
	{"time_sec": 553, "doppler_434_khz": -9886, "doppler_144_khz": -3295},
	{"time_sec": 554, "doppler_434_khz": -9888, "doppler_144_khz": -3295},
	{"time_sec": 555, "doppler_434_khz": -9890, "doppler_144_khz": -3296},
	{"time_sec": 556, "doppler_434_khz": -9892, "doppler_144_khz": -3297},
	{"time_sec": 557, "doppler_434_khz": -9894, "doppler_144_khz": -3297},
	{"time_sec": 558, "doppler_434_khz": -9898, "doppler_144_khz": -3299},
	{"time_sec": 559, "doppler_434_khz": -9899, "doppler_144_khz": -3299},
	{"time_sec": 560, "doppler_434_khz": -9901, "doppler_144_khz": -3300},
	{"time_sec": 561, "doppler_434_khz": -9903, "doppler_144_khz": -3300},
	{"time_sec": 562, "doppler_434_khz": -9906, "doppler_144_khz": -3301},
	{"time_sec": 563, "doppler_434_khz": -9908, "doppler_144_khz": -3302},
	{"time_sec": 564, "doppler_434_khz": -9910, "doppler_144_khz": -3303},
	{"time_sec": 565, "doppler_434_khz": -9911, "doppler_144_khz": -3303},
	{"time_sec": 566, "doppler_434_khz": -9913, "doppler_144_khz": -3304},
	{"time_sec": 567, "doppler_434_khz": -9916, "doppler_144_khz": -3305},
	{"time_sec": 568, "doppler_434_khz": -9918, "doppler_144_khz": -3305},
	{"time_sec": 569, "doppler_434_khz": -9919, "doppler_144_khz": -3306},
	{"time_sec": 570, "doppler_434_khz": -9921, "doppler_144_khz": -3306},
	{"time_sec": 571, "doppler_434_khz": -9923, "doppler_144_khz": -3307},
	{"time_sec": 572, "doppler_434_khz": -9925, "doppler_144_khz": -3308},
	{"time_sec": 573, "doppler_434_khz": -9926, "doppler_144_khz": -3308},
	{"time_sec": 574, "doppler_434_khz": -9928, "doppler_144_khz": -3309},
	{"time_sec": 575, "doppler_434_khz": -9930, "doppler_144_khz": -3309},
	{"time_sec": 576, "doppler_434_khz": -9932, "doppler_144_khz": -3310},
	{"time_sec": 577, "doppler_434_khz": -9933, "doppler_144_khz": -3310},
	{"time_sec": 578, "doppler_434_khz": -9934, "doppler_144_khz": -3311},
	{"time_sec": 579, "doppler_434_khz": -9936, "doppler_144_khz": -3311},
	{"time_sec": 580, "doppler_434_khz": -9938, "doppler_144_khz": -3312},
	{"time_sec": 581, "doppler_434_khz": -9939, "doppler_144_khz": -3312},
	{"time_sec": 582, "doppler_434_khz": -9940, "doppler_144_khz": -3313},
	{"time_sec": 583, "doppler_434_khz": -9942, "doppler_144_khz": -3313},
	{"time_sec": 584, "doppler_434_khz": -9944, "doppler_144_khz": -3314},
	{"time_sec": 585, "doppler_434_khz": -9945, "doppler_144_khz": -3314},
	{"time_sec": 586, "doppler_434_khz": -9946, "doppler_144_khz": -3315},
	{"time_sec": 587, "doppler_434_khz": -9947, "doppler_144_khz": -3315},
	{"time_sec": 588, "doppler_434_khz": -9949, "doppler_144_khz": -3316},
	{"time_sec": 589, "doppler_434_khz": -9950, "doppler_144_khz": -3316},
	{"time_sec": 590, "doppler_434_khz": -9951, "doppler_144_khz": -3316},
	{"time_sec": 591, "doppler_434_khz": -9952, "doppler_144_khz": -3317},
	{"time_sec": 592, "doppler_434_khz": -9953, "doppler_144_khz": -3317},
	{"time_sec": 593, "doppler_434_khz": -9955, "doppler_144_khz": -3318},
	{"time_sec": 594, "doppler_434_khz": -9956, "doppler_144_khz": -3318},
	{"time_sec": 595, "doppler_434_khz": -9957, "doppler_144_khz": -3318},
	{"time_sec": 596, "doppler_434_khz": -9958, "doppler_144_khz": -3319},
	{"time_sec": 597, "doppler_434_khz": -9960, "doppler_144_khz": -3319},
	{"time_sec": 598, "doppler_434_khz": -9961, "doppler_144_khz": -3320},
	{"time_sec": 599, "doppler_434_khz": -9961, "doppler_144_khz": -3320},
	{"time_sec": 600, "doppler_434_khz": -9962, "doppler_144_khz": -3320},
	{"time_sec": 601, "doppler_434_khz": -9963, "doppler_144_khz": -3320},
	{"time_sec": 602, "doppler_434_khz": -9965, "doppler_144_khz": -3321},
	{"time_sec": 603, "doppler_434_khz": -9965, "doppler_144_khz": -3321},
	{"time_sec": 604, "doppler_434_khz": -9966, "doppler_144_khz": -3321},
	{"time_sec": 605, "doppler_434_khz": -9967, "doppler_144_khz": -3322},
	{"time_sec": 606, "doppler_434_khz": -9968, "doppler_144_khz": -3322},
	{"time_sec": 607, "doppler_434_khz": -9969, "doppler_144_khz": -3322},
	{"time_sec": 608, "doppler_434_khz": -9970, "doppler_144_khz": -3323},
	{"time_sec": 609, "doppler_434_khz": -9970, "doppler_144_khz": -3323},
	{"time_sec": 610, "doppler_434_khz": -9972, "doppler_144_khz": -3323},
	{"time_sec": 611, "doppler_434_khz": -9972, "doppler_144_khz": -3323},
	{"time_sec": 612, "doppler_434_khz": -9973, "doppler_144_khz": -3324},
	{"time_sec": 613, "doppler_434_khz": -9974, "doppler_144_khz": -3324},
	{"time_sec": 614, "doppler_434_khz": -9974, "doppler_144_khz": -3324},
	{"time_sec": 615, "doppler_434_khz": -9975, "doppler_144_khz": -3324},
	{"time_sec": 616, "doppler_434_khz": -9976, "doppler_144_khz": -3325},
	{"time_sec": 617, "doppler_434_khz": -9976, "doppler_144_khz": -3325},
	{"time_sec": 618, "doppler_434_khz": -9977, "doppler_144_khz": -3325},
	{"time_sec": 619, "doppler_434_khz": -9978, "doppler_144_khz": -3325},
	{"time_sec": 620, "doppler_434_khz": -9978, "doppler_144_khz": -3325},
	{"time_sec": 621, "doppler_434_khz": -9979, "doppler_144_khz": -3326},
	{"time_sec": 622, "doppler_434_khz": -9979, "doppler_144_khz": -3326},
	{"time_sec": 623, "doppler_434_khz": -9980, "doppler_144_khz": -3326},
	{"time_sec": 624, "doppler_434_khz": -9981, "doppler_144_khz": -3326},
	{"time_sec": 625, "doppler_434_khz": -9981, "doppler_144_khz": -3326},
	{"time_sec": 626, "doppler_434_khz": -9982, "doppler_144_khz": -3327},
	{"time_sec": 627, "doppler_434_khz": -9982, "doppler_144_khz": -3327},
	{"time_sec": 628, "doppler_434_khz": -9983, "doppler_144_khz": -3327},
	{"time_sec": 629, "doppler_434_khz": -9983, "doppler_144_khz": -3327},
	{"time_sec": 630, "doppler_434_khz": -9984, "doppler_144_khz": -3327},
	{"time_sec": 631, "doppler_434_khz": -9984, "doppler_144_khz": -3327},
	{"time_sec": 632, "doppler_434_khz": -9985, "doppler_144_khz": -3328},
	{"time_sec": 633, "doppler_434_khz": -9985, "doppler_144_khz": -3328},
	{"time_sec": 634, "doppler_434_khz": -9985, "doppler_144_khz": -3328},
	{"time_sec": 635, "doppler_434_khz": -9986, "doppler_144_khz": -3328},
	{"time_sec": 636, "doppler_434_khz": -9986, "doppler_144_khz": -3328},
	{"time_sec": 637, "doppler_434_khz": -9986, "doppler_144_khz": -3328},
	{"time_sec": 638, "doppler_434_khz": -9987, "doppler_144_khz": -3328},
	{"time_sec": 639, "doppler_434_khz": -9987, "doppler_144_khz": -3328},
	{"time_sec": 640, "doppler_434_khz": -9987, "doppler_144_khz": -3328},
	{"time_sec": 641, "doppler_434_khz": -9988, "doppler_144_khz": -3329},
	{"time_sec": 642, "doppler_434_khz": -9988, "doppler_144_khz": -3329},
	{"time_sec": 643, "doppler_434_khz": -9988, "doppler_144_khz": -3329},
	{"time_sec": 644, "doppler_434_khz": -9988, "doppler_144_khz": -3329},
	{"time_sec": 645, "doppler_434_khz": -9988, "doppler_144_khz": -3329},
	{"time_sec": 646, "doppler_434_khz": -9989, "doppler_144_khz": -3329},
	{"time_sec": 647, "doppler_434_khz": -9989, "doppler_144_khz": -3329},
	{"time_sec": 648, "doppler_434_khz": -9989, "doppler_144_khz": -3329},
	{"time_sec": 649, "doppler_434_khz": -9989, "doppler_144_khz": -3329},
	{"time_sec": 650, "doppler_434_khz": -9990, "doppler_144_khz": -3329},
	{"time_sec": 651, "doppler_434_khz": -9990, "doppler_144_khz": -3329},
	{"time_sec": 652, "doppler_434_khz": -9990, "doppler_144_khz": -3329},
	{"time_sec": 653, "doppler_434_khz": -9990, "doppler_144_khz": -3329},
	{"time_sec": 654, "doppler_434_khz": -9990, "doppler_144_khz": -3329},
	{"time_sec": 655, "doppler_434_khz": -9990, "doppler_144_khz": -3329},
	{"time_sec": 656, "doppler_434_khz": -9990, "doppler_144_khz": -3329},
	{"time_sec": 657, "doppler_434_khz": -9990, "doppler_144_khz": -3329},
	{"time_sec": 658, "doppler_434_khz": -9990, "doppler_144_khz": -3329},
	{"time_sec": 659, "doppler_434_khz": -9990, "doppler_144_khz": -3329},
	{"time_sec": 660, "doppler_434_khz": -9990, "doppler_144_khz": -3329},
	{"time_sec": 661, "doppler_434_khz": -9990, "doppler_144_khz": -3329}
	]			
}

def update_doppler():

	try:
		global start_time
		global tx_doppler_freq_hz
		global rx_doppler_freq_hz
		global rxpl_value
		global txpl_value
		global sq
		global doppler_table
		global mode
		global tx
		print("update_doppler")
		try:
			relative_time = (time.perf_counter() - start_time) % 661
		except:
			start_time = time.perf_counter()
			relative_time = (time.perf_counter() - start_time) % 661			
		index = int(relative_time/10)
		print(f"relative time: {relative_time:.1f} seconds after AOS is index: {index}")
		table_row = doppler_table[index]
		rx_doppler_shift_hz = table_row["doppler_434_khz"] * 1000
		if (mode == 'e'):
			tx_doppler_shift_hz = table_row["doppler_144_khz"] * 1000
		else:
			tx_doppler_shift_hz = rx_doppler_shift_hz 
		tx_doppler_freq_hz = tx_doppler_start_hz + tx_doppler_shift_hz
		print(f"Tx Doppler shift: {tx_doppler_freq_hz:.0f}")
		rx_doppler_freq_hz = rx_doppler_start_hz + rx_doppler_shift_hz
		print(f"Rx Doppler shift: {rx_doppler_freq_hz:.0f}")
		
	except:
		print("update_doppler failed")

try:	
	tx_value = '0'
	rx_value = '0'
	sq = '0'
	tx = '434.9000'	
	rx = '435.0000'
	txr = '144.9000'
	sim_mode = False
	sim_config = False
	doppler_mode = "off"
	debug_mode = True

	try:
		file = open("/home/pi/CubeSatSim/.mode")
		mode = file.read(1)
	except:
		mode = 'f'
		if (debug_mode == 1):
			print("Can't open .mode file, defaulting to FSK")
	file.close()
	print("Mode is: ")
	print(mode)
	
	try:
		file = open("/home/pi/CubeSatSim/sim.cfg")
		config = file.readline().split()		
		if len(config) > 6:
			txf = float(config[6])
			tx = "{:.4f}".format(txf)
			if (mode == 'e'):
				txrf = (txf - 290.0) # - 0.1 # Cross Band Repeater mode transmit frequency in 2m band
				txr = "{:.4f}".format(txrf)
				print("Transmit frequency: ",txr)
			else:
				print("Transmit frequency: ",tx)
		if len(config) > 7:
			rxf = float(config[7])
			rx = "{:.4f}".format(rxf)
			print(rx)
		if len(config) > 9:
			rxpl = float(config[9])
#                       print(rxpl)
#                       print( "{:.0f}".format(rxpl))
			rxpl_value = "{:.0f}".format(rxpl)
			print(rxpl_value)
		if len(config) > 10:
			txpl = float(config[10])
#                        print(txpl)
#                        print( "{:.0f}".format(txpl))
			txpl_value = "{:.0f}".format(txpl)
			print(txpl_value)
		if len(config) > 13:
			if config[13] == 'sim':		
				doppler_mode = 'sim'
				print("Simulated Doppler frequency shift is enabled.")				
			elif config[13] == 'rig':	
				doppler_mode = 'rig'
				print("rigctl Doppler frequency shift is enabled.")	
			else:
				doppler_mode = 'off'
				print("Doppler frequency shift is disabled.")	
		else:
			doppler_mode = 'off'
			print("Doppler frequency shift is disabled.")	
		print(config)
		print
#		print(callsign)
		print(sq)
#		if sq == '8':
#			print("squelch set to 8, no command input!")
#			no_command = True
#		else:
		no_command = False		
		print(no_command)
	except Exception as e:
		print(f"An error occurred: {e}")		
		if (debug_mode == 1):
			print("Can't read sim.cfg file")	
	file.close()
		  
except Exception as e:
	print(f"An error occurred: {e}")	

if (doppler_mode == 'off'):
  tx_freq_hz = txf * 1e6
  rx_freq_hz = rxf * 1e6
  print("writing sim.cfg frequency to frequency.txt")	
  with open("/home/pi/CubeSatSim/frequency.txt", "w") as file:
      file.write(f"{tx_freq_hz:.0f} {rx_freq_hz:.0f}")
  while True:
    sleep(1)
        
if (doppler_mode == 'sim'):
  print("Sim mode")
	
  if (mode == 'e'):
    tx_doppler_start_hz = txrf * 1e6
  else:
    tx_doppler_start_hz = txf * 1e6
  tx_doppler_shift_hz = 0
  print(f"Tx center frequency: {tx_doppler_start_hz}")
  rx_doppler_start_hz = rxf * 1e6
  rx_doppler_shift_hz = 0
  print(f"Rx center frequency: {rx_doppler_start_hz}")
	
  TARGET_PASS = 88           # Maximum elevation profile
  doppler_table = iss_doppler_passes[TARGET_PASS]
  print(f"Pass Max Elevation: {TARGET_PASS}°")
	
  start_time = time.perf_counter() 
  current_index = 0;
  while True:
    relative_time = (time.perf_counter() - start_time) % 661
    index = int(relative_time/10) % 37
    if (index > current_index):
      current_index = index
      print(f"relative time: {relative_time:.1f} seconds after AOS is index: {index}")
      table_row = doppler_table[index]
      rx_doppler_shift_hz = table_row["doppler_434_khz"] * 1000
      if (mode == 'e'):
        tx_doppler_shift_hz = table_row["doppler_144_khz"] * 1000
      else:
        tx_doppler_shift_hz = rx_doppler_shift_hz 
      tx_doppler_freq_hz = tx_doppler_start_hz + tx_doppler_shift_hz
      print(f"Tx Doppler shift: {tx_doppler_freq_hz:.0f}")
      rx_doppler_freq_hz = rx_doppler_start_hz + rx_doppler_shift_hz
      print(f"Rx Doppler shift: {rx_doppler_freq_hz:.0f}")

      with open("/home/pi/CubeSatSim/frequency.txt", "w") as file:
        file.write(f"{tx_doppler_freq_hz:.0f} {rx_doppler_freq_hz:.0f}")
    sleep(1)

if (doppler_mode == 'rig'):
  print("rig mode")
  while True:

    system("sudo killall -9 ft857d &>/dev/null")
    system("udo killall -9 socat &>/dev/null")
    system("sudo killall -9 rigctld &>/dev/null")

    print("Starting virtual serial ports /tmp/vttyA and /tmp/vttyB")

    system("socat -d -d PTY,raw,echo=0,link=/tmp/vttyA PTY,raw,echo=0,link=/tmp/vttyB &")

    sleep(1.0)

    print("Starting CubeSatSim FT857 emulation")
    system("/home/pi/FT857d/examples/FT857d/ft857d &")

    sleep(1.0)

    print("Starting rigctl emulating FT857 rig using virtual serial port /tmp/vttyB")
    system("rigctld -m 1022 -r /tmp/vttyB -t 4532 -vv")

    print("rigctld ended")

