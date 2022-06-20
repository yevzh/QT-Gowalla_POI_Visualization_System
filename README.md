# Gowalla Dataset Visualization System

## Final project of CS2309-2-Principles And Problem Solving

### Introduction

The project is the final project of CS2309-2, Shanghai Jiao Tong University. The project aims to realize the visualization of the Gowalla POI dataset. During development, I implemented several functions to analyze the statistics from different dimensions. The main function is to show the spatio-temporal distribution pattern, to compare two different POIs or users, and to predict a given POI's checking-in number in next period and a given user's next interested POI.

### Building and Running

1. Open the Gowalla Dataset Visualization System in Qt Creator
2. Configure: version of Qt :5.9.0, c++ compiler: MinGW 5.3.0 32bit for C
3. Build and run the program in Qt Creator.

### Functionalities

#### 1. Data import

Click on the "open" botton besides the QLineEdit widget, and choose the csv. file which contains the gowalla dataset. And then click on the "load" button and all of the data will be stored in a class.


#### 2. Spatio-temporal Pattern Visualization

After loading the data, click the "display" button and you will see the line chart of number and datetime. You can **tune the parameters on the top spinboxes** to focus on different ranges of data. You can also **choose the time precision in the combobox** to adjust the parameters. 


Then you can move to the second part. You can also tune the parameters above. Click on the "draw" button and you will see a 3D bar chart. There are some small widgets that can change your observing angles and themes and so on. **Remember to click on "clear" before draw another picture!**


#### 3. Comparisons

In the third part, firstly you should choose 2 users and you can also change the time span. Then click on the "compare", you will see the number-time line on the left, and their respective top 10 interested POIs.


In the fourth part, **choose a date and click on the "dipict the DAU" button**, and you will see the top 10 popular POIs on that day. Then you can choose two POIs for comparison.




#### 4.Prediction

In the last function, there are two parts. You can choose one POI, and the system will calculate the expected number of checking-ins on the next 3 days for you.

The other one is to predict the next POI one user might be interested to visit.
