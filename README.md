# DBSCAN
DBSCAN from scratch -- CMSC 5724 Course Project

## How to use
1. cd to main.cpp
2. g++ main.cpp
3. `./a.out [dataset path] [<float> radius] [<int> threshold]`
eg. `./a.out /Users/Destop/DBSCAN/dataset.txt 2.0 4`
4. REMINDER: `The clusters and outliers output will be in the folder of input dataset.`

## Algorithm
Details please refer to slides from Prof. Tao Yufei [here](http://www.cse.cuhk.edu.hk/~taoyf/course/cmsc5724/spr15/notes/cluster::den.pdf)

## Dataset
Download [here](http://www.cse.cuhk.edu.hk/~taoyf/course/cmsc5724/data/3spiral.txt) (obtained from the data collection [here](http://cs.joensuu.fi/sipu/datasets/)). Each line has the following format: 

x y

which represent the x- and y-coordinates of a point.

## Task

Partition the dataset into 3 clusters.
