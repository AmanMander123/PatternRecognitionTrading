#Pattern Recognition Trading Experiment

##Purpose
The purpose of this experiment is to test the accuracy of a currency trading strategy based on the premise that patterns repeat themselves. 

##Background
This program reads the historical prices and converts them into percent changes from one period to the next. The program takes the percent change patterns for a given interval and stores them on memory. The current pattern is fed into the program and is compared to the previous patterns. If there a match, the future prediction is determined based on the direction that the price had gone the last time it encountered a similar pattern.

##Method
There were 2 variables that were altered and tested:

1. The interval length of how many points to store in a pattern. This ranged from 10 minutes to 115 minutes in 5 minute increments.

2. How far into the future to make the prediction. The cases tested here were 1 minute, 5 minutes, 15 minutes, 30 minutes and 60 minutes.

##Results
As expected, the accuracy of this strategy fluctuates around 50% for different variations in the two variables, thus not profiable in its current form.

##Note
This experiment is still on-going. Further details and in-depth analysis of the results will be added soon.
The accuracy plot is available in the repository. 
