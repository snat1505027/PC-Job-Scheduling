# PC Job Scheduling Algorithm: YDS

## This is an implementation of offline YDS algorithm with C++ from scratch.

* YDS is a scheduling algorithm for dynamic speed scaling processors which minimizes the total energy consumption.
* It was named after and developed by Frances Yao, Alan Demers and Scott Shenker.
* There is both an online and an offline version of the algorithm.
* Yao, Demers and Shenker make two simplifying assumptions. 
    * There is no upper bound on the allowed processor speed. Hence a feasible schedule always exists. 
    * The processor has a continuous spectrum of speeds.
* Pre-emption of jobs is allowed, i.e. the processing of a job may be stopped and resumed later.

## Algorithm pseudocode

![Alt text](https://github.com/snat1505027/PC-Job-Scheduling/blob/master/yds.PNG?raw=true "Title")
