#! /usr/bin/env python
# -*- coding: utf-8 -*-
# vim:fenc=utf-8
#
# Copyright © 2014 zip76 <zip76@BugMachine2>
#
# Distributed under terms of the MIT license.

"""
Homography tester
"""
import numpy as np

b = np.array([100, 12, 1, 0]);
a = np.array([[194.553,1.40942,-22358.4,0],
              [-2.78234,199.262,-5346.03,0],
              [-0.0159356,0.00464647,198.541,0]]);

def homography_for_matrix(a,b):
    s = np.dot(a,b);
    s = s/s[2];
    return s[0], s[1];

def read_file(file):
    f = open(file, "r");
    A = [];
    while True:
        try:
            a = np.array([[float(f.readline()), float(f.readline()), float(f.readline()), float(f.readline())],
                          [float(f.readline()), float(f.readline()), float(f.readline()), float(f.readline())],
                          [float(f.readline()), float(f.readline()), float(f.readline()), float(f.readline())]
                         ]);
            A.append(a);
        except:
            break;
    f.close();
    
    return A;

def homography(A,b):
    count = 0;
    for a in A:
        x,y = homography_for_matrix(a,b);
        if x >= 0 and y >= 0:
            count = count + 1;

    return count;

A = read_file("homography.txt");

width=512;
height=512;

for j in range(0, height):
    for i in range(0, width):
        count = homography(A,np.array([i,j,1,0]));
        if count == 0:
            print "no correspondency at (", i, ",", j, ")";

