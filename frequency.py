#!/usr/bin/env python

#import subprocess
import time
from time import sleep
#import os
import sys
from os import system


iss_doppler_passes = {
	5: [
		{"time_sec": -180, "velocity_kms": -4.00, "doppler_434_khz": 5.79, "obs_434_mhz": 434.9058, "doppler_144_khz": 1.93, "obs_144_mhz": 144.9019},
        {"time_sec": -170, "velocity_kms": -3.89, "doppler_434_khz": 5.64, "obs_434_mhz": 434.9056, "doppler_144_khz": 1.88, "obs_144_mhz": 144.9019},
        {"time_sec": -160, "velocity_kms": -3.78, "doppler_434_khz": 5.48, "obs_434_mhz": 434.9055, "doppler_144_khz": 1.83, "obs_144_mhz": 144.9018},
        {"time_sec": -150, "velocity_kms": -3.67, "doppler_434_khz": 5.32, "obs_434_mhz": 434.9053, "doppler_144_khz": 1.77, "obs_144_mhz": 144.9018},
        {"time_sec": -140, "velocity_kms": -3.54, "doppler_434_khz": 5.14, "obs_434_mhz": 434.9051, "doppler_144_khz": 1.71, "obs_144_mhz": 144.9017},
        {"time_sec": -130, "velocity_kms": -3.42, "doppler_434_khz": 4.96, "obs_434_mhz": 434.9050, "doppler_144_khz": 1.65, "obs_144_mhz": 144.9017},
        {"time_sec": -120, "velocity_kms": -3.28, "doppler_434_khz": 4.76, "obs_434_mhz": 434.9048, "doppler_144_khz": 1.59, "obs_144_mhz": 144.9016},
        {"time_sec": -110, "velocity_kms": -3.14, "doppler_434_khz": 4.55, "obs_434_mhz": 434.9046, "doppler_144_khz": 1.52, "obs_144_mhz": 144.9015},
        {"time_sec": -100, "velocity_kms": -2.99, "doppler_434_khz": 4.33, "obs_434_mhz": 434.9043, "doppler_144_khz": 1.44, "obs_144_mhz": 144.9014},
        {"time_sec": -90, "velocity_kms": -2.83, "doppler_434_khz": 4.10, "obs_434_mhz": 434.9041, "doppler_144_khz": 1.37, "obs_144_mhz": 144.9014},
        {"time_sec": -80, "velocity_kms": -2.66, "doppler_434_khz": 3.86, "obs_434_mhz": 434.9039, "doppler_144_khz": 1.29, "obs_144_mhz": 144.9013},
        {"time_sec": -70, "velocity_kms": -2.48, "doppler_434_khz": 3.60, "obs_434_mhz": 434.9036, "doppler_144_khz": 1.20, "obs_144_mhz": 144.9012},
        {"time_sec": -60, "velocity_kms": -2.29, "doppler_434_khz": 3.32, "obs_434_mhz": 434.9033, "doppler_144_khz": 1.11, "obs_144_mhz": 144.9011},
        {"time_sec": -50, "velocity_kms": -2.09, "doppler_434_khz": 3.03, "obs_434_mhz": 434.9030, "doppler_144_khz": 1.01, "obs_144_mhz": 144.9010},
        {"time_sec": -40, "velocity_kms": -1.87, "doppler_434_khz": 2.71, "obs_434_mhz": 434.9027, "doppler_144_khz": 0.90, "obs_144_mhz": 144.9009},
        {"time_sec": -30, "velocity_kms": -1.63, "doppler_434_khz": 2.37, "obs_434_mhz": 434.9024, "doppler_144_khz": 0.79, "obs_144_mhz": 144.9008},
        {"time_sec": -20, "velocity_kms": -1.33, "doppler_434_khz": 1.93, "obs_434_mhz": 434.9019, "doppler_144_khz": 0.64, "obs_144_mhz": 144.9006},
        {"time_sec": -10, "velocity_kms": -0.84, "doppler_434_khz": 1.22, "obs_434_mhz": 434.9012, "doppler_144_khz": 0.41, "obs_144_mhz": 144.9004},
        {"time_sec": 0, "velocity_kms": 0.00, "doppler_434_khz": 0.00, "obs_434_mhz": 434.9000, "doppler_144_khz": 0.00, "obs_144_mhz": 144.9000},
        {"time_sec": 10, "velocity_kms": 0.84, "doppler_434_khz": -1.22, "obs_434_mhz": 434.8988, "doppler_144_khz": -0.41, "obs_144_mhz": 144.8996},
        {"time_sec": 20, "velocity_kms": 1.33, "doppler_434_khz": -1.93, "obs_434_mhz": 434.8981, "doppler_144_khz": -0.64, "obs_144_mhz": 144.8994},
        {"time_sec": 30, "velocity_kms": 1.63, "doppler_434_khz": -2.37, "obs_434_mhz": 434.8976, "doppler_144_khz": -0.79, "obs_144_mhz": 144.8992},
        {"time_sec": 40, "velocity_kms": 1.87, "doppler_434_khz": -2.71, "obs_434_mhz": 434.8973, "doppler_144_khz": -0.90, "obs_144_mhz": 144.8991},
        {"time_sec": 50, "velocity_kms": 2.09, "doppler_434_khz": -3.03, "obs_434_mhz": 434.8970, "doppler_144_khz": -1.01, "obs_144_mhz": 144.8990},
        {"time_sec": 60, "velocity_kms": 2.29, "doppler_434_khz": -3.32, "obs_434_mhz": 434.8967, "doppler_144_khz": -1.11, "obs_144_mhz": 144.8989},
        {"time_sec": 70, "velocity_kms": 2.48, "doppler_434_khz": -3.60, "obs_434_mhz": 434.8964, "doppler_144_khz": -1.20, "obs_144_mhz": 144.8988},
        {"time_sec": 80, "velocity_kms": 2.66, "doppler_434_khz": -3.86, "obs_434_mhz": 434.8961, "doppler_144_khz": -1.29, "obs_144_mhz": 144.8987},
        {"time_sec": 90, "velocity_kms": 2.83, "doppler_434_khz": -4.10, "obs_434_mhz": 434.8959, "doppler_144_khz": -1.37, "obs_144_mhz": 144.8986},
        {"time_sec": 100, "velocity_kms": 2.99, "doppler_434_khz": -4.33, "obs_434_mhz": 434.8957, "doppler_144_khz": -1.44, "obs_144_mhz": 144.8986},
        {"time_sec": 110, "velocity_kms": 3.14, "doppler_434_khz": -4.55, "obs_434_mhz": 434.8954, "doppler_144_khz": -1.52, "obs_144_mhz": 144.8985},
        {"time_sec": 120, "velocity_kms": 3.28, "doppler_434_khz": -4.76, "obs_434_mhz": 434.8952, "doppler_144_khz": -1.59, "obs_144_mhz": 144.8984},
        {"time_sec": 130, "velocity_kms": 3.42, "doppler_434_khz": -4.96, "obs_434_mhz": 434.8950, "doppler_144_khz": -1.65, "obs_144_mhz": 144.8983},
        {"time_sec": 140, "velocity_kms": 3.54, "doppler_434_khz": -5.14, "obs_434_mhz": 434.8949, "doppler_144_khz": -1.71, "obs_144_mhz": 144.8983},
        {"time_sec": 150, "velocity_kms": 3.67, "doppler_434_khz": -5.32, "obs_434_mhz": 434.8947, "doppler_144_khz": -1.77, "obs_144_mhz": 144.8982},
        {"time_sec": 160, "velocity_kms": 3.78, "doppler_434_khz": -5.48, "obs_434_mhz": 434.8945, "doppler_144_khz": -1.83, "obs_144_mhz": 144.8982},
        {"time_sec": 170, "velocity_kms": 3.89, "doppler_434_khz": -5.64, "obs_434_mhz": 434.8944, "doppler_144_khz": -1.88, "obs_144_mhz": 144.8981},
        {"time_sec": 180, "velocity_kms": 4.00, "doppler_434_khz": -5.79, "obs_434_mhz": 434.8942, "doppler_144_khz": -1.93, "obs_144_mhz": 144.8981}
	],
    20: [
        {"time_sec": -180, "velocity_kms": -5.81, "doppler_434_khz": 8.43, "obs_434_mhz": 434.9084, "doppler_144_khz": 2.81, "obs_144_mhz": 144.9028},
        {"time_sec": -170, "velocity_kms": -5.68, "doppler_434_khz": 8.24, "obs_434_mhz": 434.9082, "doppler_144_khz": 2.75, "obs_144_mhz": 144.9027},
        {"time_sec": -160, "velocity_kms": -5.55, "doppler_434_khz": 8.05, "obs_434_mhz": 434.9081, "doppler_144_khz": 2.68, "obs_144_mhz": 144.9027},
        {"time_sec": -150, "velocity_kms": -5.39, "doppler_434_khz": 7.82, "obs_434_mhz": 434.9078, "doppler_144_khz": 2.61, "obs_144_mhz": 144.9026},
        {"time_sec": -140, "velocity_kms": -5.22, "doppler_434_khz": 7.57, "obs_434_mhz": 434.9076, "doppler_144_khz": 2.52, "obs_144_mhz": 144.9025},
        {"time_sec": -130, "velocity_kms": -5.02, "doppler_434_khz": 7.28, "obs_434_mhz": 434.9073, "doppler_144_khz": 2.43, "obs_144_mhz": 144.9024},
        {"time_sec": -120, "velocity_kms": -4.81, "doppler_434_khz": 6.98, "obs_434_mhz": 434.9070, "doppler_144_khz": 2.32, "obs_144_mhz": 144.9023},
        {"time_sec": -110, "velocity_kms": -4.57, "doppler_434_khz": 6.63, "obs_434_mhz": 434.9066, "doppler_144_khz": 2.21, "obs_144_mhz": 144.9022},
        {"time_sec": -100, "velocity_kms": -4.30, "doppler_434_khz": 6.24, "obs_434_mhz": 434.9062, "doppler_144_khz": 2.08, "obs_144_mhz": 144.9021},
        {"time_sec": -90, "velocity_kms": -4.00, "doppler_434_khz": 5.80, "obs_434_mhz": 434.9058, "doppler_144_khz": 1.93, "obs_144_mhz": 144.9019},
        {"time_sec": -80, "velocity_kms": -3.68, "doppler_434_khz": 5.34, "obs_434_mhz": 434.9053, "doppler_144_khz": 1.78, "obs_144_mhz": 144.9018},
        {"time_sec": -70, "velocity_kms": -3.32, "doppler_434_khz": 4.82, "obs_434_mhz": 434.9048, "doppler_144_khz": 1.60, "obs_144_mhz": 144.9016},
        {"time_sec": -60, "velocity_kms": -2.92, "doppler_434_khz": 4.24, "obs_434_mhz": 434.9042, "doppler_144_khz": 1.41, "obs_144_mhz": 144.9014},
        {"time_sec": -50, "velocity_kms": -2.50, "doppler_434_khz": 3.63, "obs_434_mhz": 434.9036, "doppler_144_khz": 1.21, "obs_144_mhz": 144.9012},
        {"time_sec": -40, "velocity_kms": -2.04, "doppler_434_khz": 2.96, "obs_434_mhz": 434.9030, "doppler_144_khz": 0.99, "obs_144_mhz": 144.9010},
        {"time_sec": -30, "velocity_kms": -1.56, "doppler_434_khz": 2.26, "obs_434_mhz": 434.9023, "doppler_144_khz": 0.75, "obs_144_mhz": 144.9008},
        {"time_sec": -20, "velocity_kms": -1.05, "doppler_434_khz": 1.52, "obs_434_mhz": 434.9015, "doppler_144_khz": 0.51, "obs_144_mhz": 144.9005},
        {"time_sec": -10, "velocity_kms": -0.53, "doppler_434_khz": 0.77, "obs_434_mhz": 434.9008, "doppler_144_khz": 0.26, "obs_144_mhz": 144.9003},
        {"time_sec": 0, "velocity_kms": 0.00, "doppler_434_khz": -0.00, "obs_434_mhz": 434.9000, "doppler_144_khz": -0.00, "obs_144_mhz": 144.9000},
        {"time_sec": 10, "velocity_kms": 0.53, "doppler_434_khz": -0.77, "obs_434_mhz": 434.8992, "doppler_144_khz": -0.26, "obs_144_mhz": 144.8997},
        {"time_sec": 20, "velocity_kms": 1.05, "doppler_434_khz": -1.52, "obs_434_mhz": 434.8985, "doppler_144_khz": -0.51, "obs_144_mhz": 144.8995},
        {"time_sec": 30, "velocity_kms": 1.56, "doppler_434_khz": -2.26, "obs_434_mhz": 434.8977, "doppler_144_khz": -0.75, "obs_144_mhz": 144.8992},
        {"time_sec": 40, "velocity_kms": 2.04, "doppler_434_khz": -2.96, "obs_434_mhz": 434.8970, "doppler_144_khz": -0.99, "obs_144_mhz": 144.8990},
        {"time_sec": 50, "velocity_kms": 2.50, "doppler_434_khz": -3.63, "obs_434_mhz": 434.8964, "doppler_144_khz": -1.21, "obs_144_mhz": 144.8988},
        {"time_sec": 60, "velocity_kms": 2.92, "doppler_434_khz": -4.24, "obs_434_mhz": 434.8958, "doppler_144_khz": -1.41, "obs_144_mhz": 144.8986},
        {"time_sec": 70, "velocity_kms": 3.32, "doppler_434_khz": -4.82, "obs_434_mhz": 434.8952, "doppler_144_khz": -1.60, "obs_144_mhz": 144.8984},
        {"time_sec": 80, "velocity_kms": 3.68, "doppler_434_khz": -5.34, "obs_434_mhz": 434.8947, "doppler_144_khz": -1.78, "obs_144_mhz": 144.8982},
        {"time_sec": 90, "velocity_kms": 4.00, "doppler_434_khz": -5.80, "obs_434_mhz": 434.8942, "doppler_144_khz": -1.93, "obs_144_mhz": 144.8981},
        {"time_sec": 100, "velocity_kms": 4.30, "doppler_434_khz": -6.24, "obs_434_mhz": 434.8938, "doppler_144_khz": -2.08, "obs_144_mhz": 144.8979},
        {"time_sec": 110, "velocity_kms": 4.57, "doppler_434_khz": -6.63, "obs_434_mhz": 434.8934, "doppler_144_khz": -2.21, "obs_144_mhz": 144.8978},
        {"time_sec": 120, "velocity_kms": 4.81, "doppler_434_khz": -6.98, "obs_434_mhz": 434.8930, "doppler_144_khz": -2.32, "obs_144_mhz": 144.8977},
        {"time_sec": 130, "velocity_kms": 5.02, "doppler_434_khz": -7.28, "obs_434_mhz": 434.8927, "doppler_144_khz": -2.43, "obs_144_mhz": 144.8976},
        {"time_sec": 140, "velocity_kms": 5.22, "doppler_434_khz": -7.57, "obs_434_mhz": 434.8924, "doppler_144_khz": -2.52, "obs_144_mhz": 144.8975},
        {"time_sec": 150, "velocity_kms": 5.39, "doppler_434_khz": -7.82, "obs_434_mhz": 434.8922, "doppler_144_khz": -2.61, "obs_144_mhz": 144.8974},
        {"time_sec": 160, "velocity_kms": 5.55, "doppler_434_khz": -8.05, "obs_434_mhz": 434.8919, "doppler_144_khz": -2.68, "obs_144_mhz": 144.8973},
        {"time_sec": 170, "velocity_kms": 5.68, "doppler_434_khz": -8.24, "obs_434_mhz": 434.8918, "doppler_144_khz": -2.75, "obs_144_mhz": 144.8973},
        {"time_sec": 180, "velocity_kms": 5.81, "doppler_434_khz": -8.43, "obs_434_mhz": 434.8916, "doppler_144_khz": -2.81, "obs_144_mhz": 144.8972}
    ],
    40: [
        {"time_sec": -180, "velocity_kms": -6.67, "doppler_434_khz": 9.68, "obs_434_mhz": 434.9097, "doppler_144_khz": 3.22, "obs_144_mhz": 144.9032},
        {"time_sec": -170, "velocity_kms": -6.60, "doppler_434_khz": 9.57, "obs_434_mhz": 434.9096, "doppler_144_khz": 3.19, "obs_144_mhz": 144.9032},
        {"time_sec": -160, "velocity_kms": -6.52, "doppler_434_khz": 9.46, "obs_434_mhz": 434.9095, "doppler_144_khz": 3.15, "obs_144_mhz": 144.9032},
        {"time_sec": -150, "velocity_kms": -6.43, "doppler_434_khz": 9.33, "obs_434_mhz": 434.9093, "doppler_144_khz": 3.11, "obs_144_mhz": 144.9031},
        {"time_sec": -140, "velocity_kms": -6.32, "doppler_434_khz": 9.17, "obs_434_mhz": 434.9092, "doppler_144_khz": 3.05, "obs_144_mhz": 144.9031},
        {"time_sec": -130, "velocity_kms": -6.19, "doppler_434_khz": 8.98, "obs_434_mhz": 434.9090, "doppler_144_khz": 2.99, "obs_144_mhz": 144.9030},
        {"time_sec": -120, "velocity_kms": -6.04, "doppler_434_khz": 8.76, "obs_434_mhz": 434.9088, "doppler_144_khz": 2.92, "obs_144_mhz": 144.9029},
        {"time_sec": -110, "velocity_kms": -5.86, "doppler_434_khz": 8.50, "obs_434_mhz": 434.9085, "doppler_144_khz": 2.83, "obs_144_mhz": 144.9028},
        {"time_sec": -100, "velocity_kms": -5.65, "doppler_434_khz": 8.20, "obs_434_mhz": 434.9082, "doppler_144_khz": 2.73, "obs_144_mhz": 144.9027},
        {"time_sec": -90, "velocity_kms": -5.39, "doppler_434_khz": 7.82, "obs_434_mhz": 434.9078, "doppler_144_khz": 2.61, "obs_144_mhz": 144.9026},
        {"time_sec": -80, "velocity_kms": -5.08, "doppler_434_khz": 7.37, "obs_434_mhz": 434.9074, "doppler_144_khz": 2.46, "obs_144_mhz": 144.9025},
        {"time_sec": -70, "velocity_kms": -4.72, "doppler_434_khz": 6.85, "obs_434_mhz": 434.9068, "doppler_144_khz": 2.28, "obs_144_mhz": 144.9023},
        {"time_sec": -60, "velocity_kms": -4.28, "doppler_434_khz": 6.21, "obs_434_mhz": 434.9062, "doppler_144_khz": 2.07, "obs_144_mhz": 144.9021},
        {"time_sec": -50, "velocity_kms": -3.77, "doppler_434_khz": 5.47, "obs_434_mhz": 434.9055, "doppler_144_khz": 1.82, "obs_144_mhz": 144.8993}, # manual note context adjustments active
        {"time_sec": -40, "velocity_kms": -3.17, "doppler_434_khz": 4.60, "obs_434_mhz": 434.9046, "doppler_144_khz": 1.53, "obs_144_mhz": 144.9015},
        {"time_sec": -30, "velocity_kms": -2.48, "doppler_434_khz": 3.60, "obs_434_mhz": 434.9036, "doppler_144_khz": 1.20, "obs_144_mhz": 144.9012},
        {"time_sec": -20, "velocity_kms": -1.70, "doppler_434_khz": 2.47, "obs_434_mhz": 434.9025, "doppler_144_khz": 0.82, "obs_144_mhz": 144.9008},
        {"time_sec": -10, "velocity_kms": -0.87, "doppler_434_khz": 1.26, "obs_434_mhz": 434.9013, "doppler_144_khz": 0.42, "obs_144_mhz": 144.9004},
        {"time_sec": 0, "velocity_kms": 0.00, "doppler_434_khz": -0.00, "obs_434_mhz": 434.9000, "doppler_144_khz": -0.00, "obs_144_mhz": 144.9000},
        {"time_sec": 10, "velocity_kms": 0.87, "doppler_434_khz": -1.26, "obs_434_mhz": 434.8987, "doppler_144_khz": -0.42, "obs_144_mhz": 144.8996},
        {"time_sec": 20, "velocity_kms": 1.70, "doppler_434_khz": -2.47, "obs_434_mhz": 434.8975, "doppler_144_khz": -0.82, "obs_144_mhz": 144.8992},
        {"time_sec": 30, "velocity_kms": 2.48, "doppler_434_khz": -3.60, "obs_434_mhz": 434.8964, "doppler_144_khz": -1.20, "obs_144_mhz": 144.8988},
        {"time_sec": 40, "velocity_kms": 3.17, "doppler_434_khz": -4.60, "obs_434_mhz": 434.8954, "doppler_144_khz": -1.53, "obs_144_mhz": 144.8985},
        {"time_sec": 50, "velocity_kms": 3.77, "doppler_434_khz": -5.47, "obs_434_mhz": 434.8945, "doppler_144_khz": -1.82, "obs_144_mhz": 144.8982},
        {"time_sec": 60, "velocity_kms": 4.28, "doppler_434_khz": -6.21, "obs_434_mhz": 434.8938, "doppler_144_khz": -2.07, "obs_144_mhz": 144.8979},
        {"time_sec": 70, "velocity_kms": 4.72, "doppler_434_khz": -6.85, "obs_434_mhz": 434.8932, "doppler_144_khz": -2.28, "obs_144_mhz": 144.8977},
        {"time_sec": 80, "velocity_kms": 5.08, "doppler_434_khz": -7.37, "obs_434_mhz": 434.8926, "doppler_144_khz": -2.46, "obs_144_mhz": 144.8975},
        {"time_sec": 90, "velocity_kms": 5.39, "doppler_434_khz": -7.82, "obs_434_mhz": 434.8922, "doppler_144_khz": -2.61, "obs_144_mhz": 144.8974},
        {"time_sec": 100, "velocity_kms": 0.87, "doppler_434_khz": -8.19, "obs_434_mhz": 434.8918, "doppler_144_khz": -2.73, "obs_144_mhz": 144.8973},
        {"time_sec": 110, "velocity_kms": 1.70, "doppler_434_khz": -8.50, "obs_434_mhz": 434.8915, "doppler_144_khz": -2.83, "obs_144_mhz": 144.8972},
        {"time_sec": 120, "velocity_kms": 2.48, "doppler_434_khz": -8.76, "obs_434_mhz": 434.8912, "doppler_144_khz": -2.92, "obs_144_mhz": 144.8971},
        {"time_sec": 130, "velocity_kms": 3.17, "doppler_434_khz": -8.98, "obs_434_mhz": 434.8910, "doppler_144_khz": -2.99, "obs_144_mhz": 144.8970},
        {"time_sec": 140, "velocity_kms": 3.77, "doppler_434_khz": -9.17, "obs_434_mhz": 434.8908, "doppler_144_khz": -3.05, "obs_144_mhz": 144.8969},
        {"time_sec": 150, "velocity_kms": 4.28, "doppler_434_khz": -9.33, "obs_434_mhz": 434.8907, "doppler_144_khz": -3.11, "obs_144_mhz": 144.8969},
        {"time_sec": 160, "velocity_kms": 4.72, "doppler_434_khz": -9.46, "obs_434_mhz": 434.8905, "doppler_144_khz": -3.15, "obs_144_mhz": 144.8968},
        {"time_sec": 170, "velocity_kms": 5.08, "doppler_434_khz": -9.58, "obs_434_mhz": 434.8904, "doppler_144_khz": -3.19, "obs_144_mhz": 144.8968},
        {"time_sec": 180, "velocity_kms": 5.39, "doppler_434_khz": -9.68, "obs_434_mhz": 434.8903, "doppler_144_khz": -3.22, "obs_144_mhz": 144.8968}
	],
    70: [	
    	{"time_sec": -180, "velocity_kms": -7.00, "doppler_434_khz": 10.15, "obs_434_mhz": 434.9102, "doppler_144_khz": 3.38, "obs_144_mhz": 144.9034},
    	{"time_sec": -170, "velocity_kms": -6.95, "doppler_434_khz": 10.09, "obs_434_mhz": 434.9101, "doppler_144_khz": 3.36, "obs_144_mhz": 144.9034},
    	{"time_sec": -160, "velocity_kms": -6.91, "doppler_434_khz": 10.02, "obs_434_mhz": 434.9100, "doppler_144_khz": 3.34, "obs_144_mhz": 144.9033},
    	{"time_sec": -150, "velocity_kms": -6.86, "doppler_434_khz":  9.95, "obs_434_mhz": 434.9100, "doppler_144_khz": 3.32, "obs_144_mhz": 144.9033},
    	{"time_sec": -140, "velocity_kms": -6.78, "doppler_434_khz":  9.84, "obs_434_mhz": 434.9098, "doppler_144_khz": 3.28, "obs_144_mhz": 144.9033},
    	{"time_sec": -130, "velocity_kms": -6.70, "doppler_434_khz":  9.72, "obs_434_mhz": 434.9097, "doppler_144_khz": 3.24, "obs_144_mhz": 144.9032},
    	{"time_sec": -120, "velocity_kms": -6.62, "doppler_434_khz":  9.60, "obs_434_mhz": 434.9096, "doppler_144_khz": 3.20, "obs_144_mhz": 144.9032},
    	{"time_sec": -110, "velocity_kms": -6.47, "doppler_434_khz":  9.39, "obs_434_mhz": 434.9094, "doppler_144_khz": 3.13, "obs_144_mhz": 144.9031},
    	{"time_sec": -100, "velocity_kms": -6.32, "doppler_434_khz":  9.17, "obs_434_mhz": 434.9092, "doppler_144_khz": 3.05, "obs_144_mhz": 144.9031},
    	{"time_sec": -90, "velocity_kms": -6.17, "doppler_434_khz":  8.95, "obs_434_mhz": 434.9090, "doppler_144_khz": 2.98, "obs_144_mhz": 144.9030},
    	{"time_sec": -80, "velocity_kms": -5.86, "doppler_434_khz":  8.50, "obs_434_mhz": 434.9085, "doppler_144_khz": 2.83, "obs_144_mhz": 144.9028},
    	{"time_sec": -70, "velocity_kms": -5.55, "doppler_434_khz":  8.05, "obs_434_mhz": 434.9081, "doppler_144_khz": 2.68, "obs_144_mhz": 144.9027},
   		{"time_sec": -60, "velocity_kms": -5.24, "doppler_434_khz":  7.60, "obs_434_mhz": 434.9076, "doppler_144_khz": 2.53, "obs_144_mhz": 144.9025},
   		{"time_sec": -50, "velocity_kms": -4.60, "doppler_434_khz":  6.67, "obs_434_mhz": 434.9067, "doppler_144_khz": 2.22, "obs_144_mhz": 144.9022},
   		{"time_sec": -40, "velocity_kms": -3.96, "doppler_434_khz":  5.74, "obs_434_mhz": 434.9057, "doppler_144_khz": 1.91, "obs_144_mhz": 144.9019},
    	{"time_sec": -30, "velocity_kms": -3.32, "doppler_434_khz":  4.82, "obs_434_mhz": 434.9048, "doppler_144_khz": 1.60, "obs_144_mhz": 144.9016},
    	{"time_sec": -20, "velocity_kms": -2.21, "doppler_434_khz":  3.21, "obs_434_mhz": 434.9032, "doppler_144_khz": 1.07, "obs_144_mhz": 144.9011},
    	{"time_sec": -10, "velocity_kms": -1.11, "doppler_434_khz":  1.61, "obs_434_mhz": 434.9016, "doppler_144_khz": 0.53, "obs_144_mhz": 144.9005},
     	{"time_sec": 0, "velocity_kms":  0.00, "doppler_434_khz": -0.00, "obs_434_mhz": 434.9000, "doppler_144_khz": -0.00, "obs_144_mhz": 144.9000},
   		{"time_sec": 10, "velocity_kms":  1.11, "doppler_434_khz": -1.61, "obs_434_mhz": 434.8984, "doppler_144_khz": -0.53, "obs_144_mhz": 144.8995},
  	 	{"time_sec": 20, "velocity_kms":  2.21, "doppler_434_khz": -3.21, "obs_434_mhz": 434.8968, "doppler_144_khz": -1.07, "obs_144_mhz": 144.8989},
    	{"time_sec": 30, "velocity_kms":  3.32, "doppler_434_khz": -4.82, "obs_434_mhz": 434.8952, "doppler_144_khz": -1.60, "obs_144_mhz": 144.8984},
    	{"time_sec": 40, "velocity_kms":  3.96, "doppler_434_khz": -5.74, "obs_434_mhz": 434.8943, "doppler_144_khz": -1.91, "obs_144_mhz": 144.8981},
   		{"time_sec": 50, "velocity_kms":  4.60, "doppler_434_khz": -6.67, "obs_434_mhz": 434.8933, "doppler_144_khz": -2.22, "obs_144_mhz": 144.8978},
   		{"time_sec": 60, "velocity_kms":  5.24, "doppler_434_khz": -7.60, "obs_434_mhz": 434.8924, "doppler_144_khz": -2.53, "obs_144_mhz": 144.8975},
   		{"time_sec": 70, "velocity_kms":  5.55, "doppler_434_khz": -8.05, "obs_434_mhz": 434.8919, "doppler_144_khz": -2.68, "obs_144_mhz": 144.8973},
   		{"time_sec": 80, "velocity_kms":  5.86, "doppler_434_khz": -8.50, "obs_434_mhz": 434.8915, "doppler_144_khz": -2.83, "obs_144_mhz": 144.8972},
   		{"time_sec": 90, "velocity_kms":  6.17, "doppler_434_khz": -8.95, "obs_434_mhz": 434.8910, "doppler_144_khz": -2.98, "obs_144_mhz": 144.8970},
   		{"time_sec": 100, "velocity_kms":  6.32, "doppler_434_khz": -9.17, "obs_434_mhz": 434.8908, "doppler_144_khz": -3.05, "obs_144_mhz": 144.8969},
        {"time_sec": 110, "velocity_kms":  6.47, "doppler_434_khz": -9.39, "obs_434_mhz": 434.8906, "doppler_144_khz": -3.13, "obs_144_mhz": 144.8969},
        {"time_sec": 120, "velocity_kms":  6.62, "doppler_434_khz": -9.60, "obs_434_mhz": 434.8904, "doppler_144_khz": -3.20, "obs_144_mhz": 144.8968},
        {"time_sec": 130, "velocity_kms":  6.70, "doppler_434_khz": -9.72, "obs_434_mhz": 434.8903, "doppler_144_khz": -3.24, "obs_144_mhz": 144.8968},
        {"time_sec": 140, "velocity_kms":  6.78, "doppler_434_khz": -9.84, "obs_434_mhz": 434.8902, "doppler_144_khz": -3.28, "obs_144_mhz": 144.8967},
        {"time_sec": 150, "velocity_kms":  6.86, "doppler_434_khz": -9.95, "obs_434_mhz": 434.8900, "doppler_144_khz": -3.32, "obs_144_mhz": 144.8967},
        {"time_sec": 160, "velocity_kms":  6.91, "doppler_434_khz": -10.02, "obs_434_mhz": 434.8900, "doppler_144_khz": -3.34, "obs_144_mhz": 144.8967},
        {"time_sec": 170, "velocity_kms":  6.95, "doppler_434_khz": -10.09, "obs_434_mhz": 434.8899, "doppler_144_khz": -3.36, "obs_144_mhz": 144.8966},
        {"time_sec": 180, "velocity_kms":  7.00, "doppler_434_khz": -10.15, "obs_434_mhz": 434.8898, "doppler_144_khz": -3.38, "obs_144_mhz": 144.8966}
    ],
    85: [	
        {"time_sec": -180, "velocity_kms": -7.15, "doppler_434_khz": 10.37, "obs_434_mhz": 434.9104, "doppler_144_khz": 3.46, "obs_144_mhz": 144.9035},
        {"time_sec": -170, "velocity_kms": -7.12, "doppler_434_khz": 10.32, "obs_434_mhz": 434.9103, "doppler_144_khz": 3.44, "obs_144_mhz": 144.9034},
        {"time_sec": -160, "velocity_kms": -7.08, "doppler_434_khz": 10.28, "obs_434_mhz": 434.9103, "doppler_144_khz": 3.42, "obs_144_mhz": 144.9034},
        {"time_sec": -150, "velocity_kms": -7.05, "doppler_434_khz": 10.23, "obs_434_mhz": 434.9102, "doppler_144_khz": 3.41, "obs_144_mhz": 144.9034},
        {"time_sec": -140, "velocity_kms": -6.99, "doppler_434_khz": 10.15, "obs_434_mhz": 434.9101, "doppler_144_khz": 3.38, "obs_144_mhz": 144.9034},
        {"time_sec": -130, "velocity_kms": -6.94, "doppler_434_khz": 10.06, "obs_434_mhz": 434.9101, "doppler_144_khz": 3.35, "obs_144_mhz": 144.9034},
        {"time_sec": -120, "velocity_kms": -6.88, "doppler_434_khz":  9.98, "obs_434_mhz": 434.9100, "doppler_144_khz": 3.33, "obs_144_mhz": 144.9033},
        {"time_sec": -110, "velocity_kms": -6.75, "doppler_434_khz":  9.80, "obs_434_mhz": 434.9098, "doppler_144_khz": 3.26, "obs_144_mhz": 144.9033},
        {"time_sec": -100, "velocity_kms": -6.63, "doppler_434_khz":  9.61, "obs_434_mhz": 434.9096, "doppler_144_khz": 3.20, "obs_144_mhz": 144.9032},
        {"time_sec": -90, "velocity_kms": -6.50, "doppler_434_khz":  9.43, "obs_434_mhz": 434.9094, "doppler_144_khz": 3.14, "obs_144_mhz": 144.9031},
        {"time_sec": -80, "velocity_kms": -6.23, "doppler_434_khz":  9.04, "obs_434_mhz": 434.9090, "doppler_144_khz": 3.01, "obs_144_mhz": 144.9030},
        {"time_sec": -70, "velocity_kms": -5.97, "doppler_434_khz":  8.66, "obs_434_mhz": 434.9087, "doppler_144_khz": 2.88, "obs_144_mhz": 144.9029},
        {"time_sec": -60, "velocity_kms": -5.70, "doppler_434_khz":  8.27, "obs_434_mhz": 434.9083, "doppler_144_khz": 2.76, "obs_144_mhz": 144.9028},
        {"time_sec": -50, "velocity_kms": -5.10, "doppler_434_khz":  7.40, "obs_434_mhz": 434.9074, "doppler_144_khz": 2.47, "obs_144_mhz": 144.9025},
        {"time_sec": -40, "velocity_kms": -4.50, "doppler_434_khz":  6.53, "obs_434_mhz": 434.9065, "doppler_144_khz": 2.18, "obs_144_mhz": 144.9022},
        {"time_sec": -30, "velocity_kms": -3.90, "doppler_434_khz":  5.66, "obs_434_mhz": 434.9057, "doppler_144_khz": 1.89, "obs_144_mhz": 144.9019},
        {"time_sec": -20 ,"velocity_kms": -2.60, "doppler_434_khz":  3.77, "obs_434_mhz": 434.9038, "doppler_144_khz": 1.26, "obs_144_mhz": 144.9013},
        {"time_sec": -10 ,"velocity_kms": -1.30, "doppler_434_khz":  1.89, "obs_434_mhz": 434.9019, "doppler_144_khz": 0.63, "obs_144_mhz": 144.9006},
        {"time_sec": 0 ,"velocity_kms":  0.00, "doppler_434_khz": -0.00, "obs_434_mhz": 434.9000, "doppler_144_khz": -0.00, "obs_144_mhz": 144.9000},
        {"time_sec": 10 ,"velocity_kms":  1.30, "doppler_434_khz": -1.89, "obs_434_mhz": 434.8981, "doppler_144_khz": -0.63, "obs_144_mhz": 144.8994},
        {"time_sec": 20 ,"velocity_kms":  2.60, "doppler_434_khz": -3.77, "obs_434_mhz": 434.8962, "doppler_144_khz": -1.26, "obs_144_mhz": 144.8987},
        {"time_sec": 30 ,"velocity_kms":  3.90, "doppler_434_khz": -5.66, "obs_434_mhz": 434.8943, "doppler_144_khz": -1.89, "obs_144_mhz": 144.8981},
        {"time_sec": 40 ,"velocity_kms":  4.50, "doppler_434_khz": -6.53, "obs_434_mhz": 434.8935, "doppler_144_khz": -2.18, "obs_144_mhz": 144.8978},
        {"time_sec": 50 ,"velocity_kms":  5.10, "doppler_434_khz": -7.40, "obs_434_mhz": 434.8926, "doppler_144_khz": -2.47, "obs_144_mhz": 144.8975},
        {"time_sec": 60 ,"velocity_kms":  5.70, "doppler_434_khz": -8.27, "obs_434_mhz": 434.8917, "doppler_144_khz": -2.76, "obs_144_mhz": 144.8972},
        {"time_sec": 70 ,"velocity_kms":  5.97, "doppler_434_khz": -8.66, "obs_434_mhz": 434.8913, "doppler_144_khz": -2.88, "obs_144_mhz": 144.8971},
        {"time_sec": 80 ,"velocity_kms":  6.23, "doppler_434_khz": -9.04, "obs_434_mhz": 434.8910, "doppler_144_khz": -3.01, "obs_144_mhz": 144.8970},
        {"time_sec": 90 ,"velocity_kms":  6.50, "doppler_434_khz": -9.43, "obs_434_mhz": 434.8906, "doppler_144_khz": -3.14, "obs_144_mhz": 144.8969},
        {"time_sec": 100 ,"velocity_kms":  6.63, "doppler_434_khz": -9.61, "obs_434_mhz": 434.8904, "doppler_144_khz": -3.20, "obs_144_mhz": 144.8968},
        {"time_sec": 110 ,"velocity_kms":  6.75, "doppler_434_khz": -9.80, "obs_434_mhz": 434.8902, "doppler_144_khz": -3.26, "obs_144_mhz": 144.8967},
        {"time_sec": 120 ,"velocity_kms":  6.88, "doppler_434_khz": -9.98, "obs_434_mhz": 434.8900, "doppler_144_khz": -3.33, "obs_144_mhz": 144.8967},
        {"time_sec": 130 ,"velocity_kms":  6.94, "doppler_434_khz": -10.06, "obs_434_mhz": 434.8899, "doppler_144_khz": -3.35, "obs_144_mhz": 144.8966},
        {"time_sec": 140 ,"velocity_kms":  6.99, "doppler_434_khz": -10.15, "obs_434_mhz": 434.8899, "doppler_144_khz": -3.38, "obs_144_mhz": 144.8966},
        {"time_sec": 150 ,"velocity_kms":  7.05, "doppler_434_khz": -10.23, "obs_434_mhz": 434.8898, "doppler_144_khz": -3.41, "obs_144_mhz": 144.8966},
        {"time_sec": 160 ,"velocity_kms":  7.08, "doppler_434_khz": -10.28, "obs_434_mhz": 434.8917, "doppler_144_khz": -3.42, "obs_144_mhz": 144.8966},
        {"time_sec": 170 ,"velocity_kms":  7.12, "doppler_434_khz": -10.32, "obs_434_mhz": 434.8897, "doppler_144_khz": -3.44, "obs_144_mhz": 144.8966},
        {"time_sec": 180 ,"velocity_kms":  7.15, "doppler_434_khz": -10.37, "obs_434_mhz": 434.8896, "doppler_144_khz": -3.46, "obs_144_mhz": 144.8965}
    ],
	89: [
        {"time_sec": -180, "velocity_kms": -7.17, "doppler_434_khz": 10.40, "obs_434_mhz": 434.9104, "doppler_144_khz": 3.47, "obs_144_mhz": 144.9035},
        {"time_sec": -170, "velocity_kms": -7.13, "doppler_434_khz": 10.35, "obs_434_mhz": 434.9103, "doppler_144_khz": 3.45, "obs_144_mhz": 144.9034},
        {"time_sec": -160, "velocity_kms": -7.09, "doppler_434_khz": 10.28, "obs_434_mhz": 434.9103, "doppler_144_khz": 3.43, "obs_144_mhz": 144.9034},
        {"time_sec": -150, "velocity_kms": -7.04, "doppler_434_khz": 10.21, "obs_434_mhz": 434.9102, "doppler_144_khz": 3.40, "obs_144_mhz": 144.9034},
        {"time_sec": -140, "velocity_kms": -6.97, "doppler_434_khz": 10.12, "obs_434_mhz": 434.9101, "doppler_144_khz": 3.37, "obs_144_mhz": 144.9034},
        {"time_sec": -130, "velocity_kms": -6.90, "doppler_434_khz": 10.01, "obs_434_mhz": 434.9100, "doppler_144_khz": 3.33, "obs_144_mhz": 144.9033},
        {"time_sec": -120, "velocity_kms": -6.81, "doppler_434_khz": 9.87, "obs_434_mhz": 434.9099, "doppler_144_khz": 3.29, "obs_144_mhz": 144.9033},
        {"time_sec": -110, "velocity_kms": -6.69, "doppler_434_khz": 9.71, "obs_434_mhz": 434.9097, "doppler_144_khz": 3.24, "obs_144_mhz": 144.9032},
        {"time_sec": -100, "velocity_kms": -6.55, "doppler_434_khz": 9.51, "obs_434_mhz": 434.9095, "doppler_144_khz": 3.17, "obs_144_mhz": 144.9032},
        {"time_sec": -90, "velocity_kms": -6.38, "doppler_434_khz": 9.25, "obs_434_mhz": 434.9093, "doppler_144_khz": 3.08, "obs_144_mhz": 144.9031},
        {"time_sec": -80, "velocity_kms": -6.15, "doppler_434_khz": 8.93, "obs_434_mhz": 434.9089, "doppler_144_khz": 2.97, "obs_144_mhz": 144.9030},
        {"time_sec": -70, "velocity_kms": -5.87, "doppler_434_khz": 8.51, "obs_434_mhz": 434.9085, "doppler_144_khz": 2.84, "obs_144_mhz": 144.9028},
        {"time_sec": -60, "velocity_kms": -5.49, "doppler_434_khz": 7.97, "obs_434_mhz": 434.9080, "doppler_144_khz": 2.66, "obs_144_mhz": 144.9027},
        {"time_sec": -50, "velocity_kms": -5.01, "doppler_434_khz": 7.26, "obs_434_mhz": 434.9073, "doppler_144_khz": 2.42, "obs_144_mhz": 144.9024},
        {"time_sec": -40, "velocity_kms": -4.37, "doppler_434_khz": 6.34, "obs_434_mhz": 434.9063, "doppler_144_khz": 2.11, "obs_144_mhz": 144.9021},
        {"time_sec": -30, "velocity_kms": -3.55, "doppler_434_khz": 5.15, "obs_434_mhz": 434.9051, "doppler_144_khz": 1.72, "obs_144_mhz": 144.9017},
        {"time_sec": -20, "velocity_kms": -2.53, "doppler_434_khz": 3.67, "obs_434_mhz": 434.9037, "doppler_144_khz": 1.22, "obs_144_mhz": 144.9012},
        {"time_sec": -10, "velocity_kms": -1.32, "doppler_434_khz": 1.92, "obs_434_mhz": 434.9019, "doppler_144_khz": 0.64, "obs_144_mhz": 144.9006},
        {"time_sec": 0, "velocity_kms": 0.00, "doppler_434_khz": 0.00, "obs_434_mhz": 434.9000, "doppler_144_khz": 0.00, "obs_144_mhz": 144.9000},
        {"time_sec": 10, "velocity_kms": 1.32, "doppler_434_khz": -1.92, "obs_434_mhz": 434.8981, "doppler_144_khz": -0.64, "obs_144_mhz": 144.8994},
        {"time_sec": 20, "velocity_kms": 2.53, "doppler_434_khz": -3.67, "obs_434_mhz": 434.8963, "doppler_144_khz": -1.22, "obs_144_mhz": 144.8988},
        {"time_sec": 30, "velocity_kms": 3.55, "doppler_434_khz": -5.15, "obs_434_mhz": 434.8949, "doppler_144_khz": -1.72, "obs_144_mhz": 144.8983},
        {"time_sec": 40, "velocity_kms": 4.37, "doppler_434_khz": -6.34, "obs_434_mhz": 434.8937, "doppler_144_khz": -2.11, "obs_144_mhz": 144.8979},
        {"time_sec": 50, "velocity_kms": 5.01, "doppler_434_khz": -7.26, "obs_434_mhz": 434.8927, "doppler_144_khz": -2.42, "obs_144_mhz": 144.8976},
        {"time_sec": 60, "velocity_kms": 5.49, "doppler_434_khz": -7.97, "obs_434_mhz": 434.8920, "doppler_144_khz": -2.66, "obs_144_mhz": 144.8973},
        {"time_sec": 70, "velocity_kms": 5.87, "doppler_434_khz": -8.51, "obs_434_mhz": 434.8915, "doppler_144_khz": -2.84, "obs_144_mhz": 144.8972},
        {"time_sec": 80, "velocity_kms": 6.15, "doppler_434_khz": -8.93, "obs_434_mhz": 434.8911, "doppler_144_khz": -2.97, "obs_144_mhz": 144.8970},
        {"time_sec": 90, "velocity_kms": 6.38, "doppler_434_khz": -9.25, "obs_434_mhz": 434.8907, "doppler_144_khz": -3.08, "obs_144_mhz": 144.8969},
        {"time_sec": 100, "velocity_kms": 6.55, "doppler_434_khz": -9.51, "obs_434_mhz": 434.8905, "doppler_144_khz": -3.17, "obs_144_mhz": 144.8968},
        {"time_sec": 110, "velocity_kms": 6.69, "doppler_434_khz": -9.71, "obs_434_mhz": 434.8903, "doppler_144_khz": -3.24, "obs_144_mhz": 144.8968},
        {"time_sec": 120, "velocity_kms": 6.81, "doppler_434_khz": -9.87, "obs_434_mhz": 434.8901, "doppler_144_khz": -3.29, "obs_144_mhz": 144.8967},
        {"time_sec": 130, "velocity_kms": 6.90, "doppler_434_khz": -10.01, "obs_434_mhz": 434.8900, "doppler_144_khz": -3.33, "obs_144_mhz": 144.8967},
        {"time_sec": 140, "velocity_kms": 6.97, "doppler_434_khz": -10.12, "obs_434_mhz": 434.8899, "doppler_144_khz": -3.37, "obs_144_mhz": 144.8966},
        {"time_sec": 150, "velocity_kms": 7.04, "doppler_434_khz": -10.21, "obs_434_mhz": 434.8898, "doppler_144_khz": -3.40, "obs_144_mhz": 144.8966},
        {"time_sec": 160, "velocity_kms": 7.09, "doppler_434_khz": -10.28, "obs_434_mhz": 434.8897, "doppler_144_khz": -3.43, "obs_144_mhz": 144.8966},
        {"time_sec": 170, "velocity_kms": 7.13, "doppler_434_khz": -10.35, "obs_434_mhz": 434.8897, "doppler_144_khz": -3.45, "obs_144_mhz": 144.8966},
        {"time_sec": 180, "velocity_kms": 7.17, "doppler_434_khz": -10.40, "obs_434_mhz": 434.8896, "doppler_144_khz": -3.47, "obs_144_mhz": 144.8965}
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
			relative_time = (time.perf_counter() - start_time) % 370
		except:
			start_time = time.perf_counter()
			relative_time = (time.perf_counter() - start_time) % 370			
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
			if config[13] == 'rig':	
				doppler_mode = 'rig'
				print("rigctl Doppler frequency shift is enabled.")	
			else:
				system("sudo systemctl stop rigctld")
		else:
			system("sudo systemctl stop rigctld")
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
  tx_freq_hz = txf * 1000
  rx_freq_hz = txf * 1000
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
	
  TARGET_PASS = 85           # Maximum elevation profile
  doppler_table = iss_doppler_passes[TARGET_PASS]
  print(f"Pass Max Elevation: {TARGET_PASS}°")
	
  start_time = time.perf_counter() 
  current_index = 0;
  while True:
    relative_time = (time.perf_counter() - start_time) % 370
    index = int(relative_time/10)
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
    sleep(1)

