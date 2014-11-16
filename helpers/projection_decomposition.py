#! /usr/bin/env python
# -*- coding: utf-8 -*-
# vim:fenc=utf-8
#
# Copyright © 2014 zip76 <zip76@BugMachine2>
#
# Distributed under terms of the MIT license.

"""
Decomposition of the projection matrix
"""

import cv2
import numpy as np

cameraMatrix1 = np.identity(3,np.float)
rotMatrix1 = np.identity(3,np.float)
transVect1 = np.array([0, 0, 0, 0], np.float)

projMatrix1 = np.array([[1.008625769,-.1231000000,-2.726496908,100.7731396],
                      [.8361681672,4.182806666,.09730552466,157.7821517],
                      [.001940031162,-.0002500000000,.0004294973553,.5091827970]], np.float)

cv2.decomposeProjectionMatrix(projMatrix1, cameraMatrix1, rotMatrix1, transVect1)
print "cameraMatrix: ", cameraMatrix1
print "rotMatrix", rotMatrix1
print "transVect", transVect1

cameraMatrix2 = np.identity(3,np.float)
rotMatrix2 = np.identity(3,np.float)
transVect2 = np.array([0, 0, 0, 0], np.float)

projMatrix2 = np.array([[1.426624453,-.1471000000,-2.539954861,78.3047630],
                       [.7976452886,4.150586666,.1303686090,143.3641217],
                       [.001903601260,-.0001466666667,.0006532670189,.5012476834]], np.float)

cv2.decomposeProjectionMatrix(projMatrix2, cameraMatrix2, rotMatrix2, transVect2)
print "cameraMatrix: ", cameraMatrix2
print "rotMatrix", rotMatrix2
print "transVect", transVect2
