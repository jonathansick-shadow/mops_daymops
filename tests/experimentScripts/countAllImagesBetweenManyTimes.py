#!/usr/bin/env python

PAIRS = [[49628.599999999999, 49630.004836],
         [49629.599999999999, 49632.005601],
         [49630.599999999999, 49637.027544],
         [49631.599999999999, 49637.027544],
         [49636.599999999999, 49639.055422],
         [49638.599999999999, 49640.216554],
         [49639.599999999999, 49641.244384],
         [49640.599999999999, 49643.035438],
         [49642.599999999999, 49644.151072],
         [49643.599999999999, 49646.059302],
         [49645.599999999999, 49648.05513],
         [49647.599999999999, 49650.062052],
         [49649.599999999999, 49652.178147],
         [49651.599999999999, 49654.064485],
         [49653.599999999999, 49658.173659],
         [49657.599999999999, 49661.164986],
         [49663.599999999999, 49667.034011],
         [49665.599999999999, 49667.034011],
         [49666.599999999999, 49671.059597],
         [49670.599999999999, 49672.147568],
         [49671.599999999999, 49673.105967],
         [49672.599999999999, 49675.020546],
         [49674.599999999999, 49679.01921],
         [49678.599999999999, 49680.024829],
         [49679.599999999999, 49681.025532],
         [49680.599999999999, 49682.02528],
         [49689.599999999999, 49693.070653]]


from countImagesBetweenObsTime import *

import MySQLdb as db
import sys

DB_USER = "jmyers"
DB_PASS = "jmyers"
DB_HOST = "localhost"

DIAS_DB = "mops_noDeepAstromError"
DIAS_TABLE = "fullerDiaSource"
OPSIM_DB = "opsim_3_61"
OPSIM_TABLE = "output_opsim3_61"


if __name__ == "__main__":
    conn = db.connect(host=DB_HOST, user=DB_USER, passwd=DB_PASS)
    cursor = conn.cursor()
    for pair in PAIRS:
        nImg = numImagesBetween(cursor, pair[0], pair[1])
        nDia = numDiasBetween(cursor, pair[0], pair[1])
        print pair[0], pair[1], nImg, nDia
