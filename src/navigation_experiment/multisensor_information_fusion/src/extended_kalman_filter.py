#!/usr/bin/env python
# coding:utf-8
'''
author:chen_lee

'''
import numpy as np

class EKF:
    def __init__(self, initial_state):
        #initial parameter
        num_states = 5
        self.dt = 1.0/50.0
        self.P = np.diag([0.21,0.20,0.32,0.0003,0.003])
        s_uwb = 0.5 * 0.2 * self.dt ** 2
        s_yaw = 0.1 * self.dt
        s_v = 0.2 * self.dt
        s_yaw_v = 0.83 * self.dt
        self.Q = np.diag([s_uwb**2,s_uwb**2,s_yaw**2,s_v**2,s_yaw_v**2])  #过程噪声协方差
        var_uwb = 0.5
        var_v = 0.1#1.0
        var_yaw_v = 0.2
        self.R = np.matrix([[var_uwb**2,0.0,0.0,0.0],
                       [0.0,var_uwb**2,0.0,0.0],
                       [0.0,0.0,var_v**2,0.0],
                       [0.0,0.0,0.0,var_yaw_v**2]]) #测量噪声协方差
        self.I = np.eye(num_states)
        #initial state x,y,yaw,dx,yaw_v
        self.x = np.matrix([[initial_state[0],initial_state[1],
                             initial_state[2],initial_state[3],
                             initial_state[4]]]).T	
    
    #estimate state
    '''
        about state function see the paper for details
    '''
    def state_estimate(self,x):
        if x[4] == 0:
            x[4] = 0.000001
        x[0] = x[0] + (x[3]/x[4]) * (np.sin(x[4]*self.dt+x[2]) - np.sin(x[2]))
        x[1] = x[1] + (x[3]/x[4]) * (-np.cos(x[4]*self.dt+x[2]) + np.cos(x[2]))
        x[2] = (x[2] + x[4]*self.dt) % (2.0*np.pi)
        x[3] = x[3]
        x[4] = x[4]
        return x
    
    #update the A matrix
    def A_matrix(self,x):
        a13 = float((x[3]/x[4]) * (np.cos(x[4]*self.dt+x[2]) 
                                       - np.cos(x[2])))
        a14 = float((1.0/x[4]) * (np.sin(x[4]*self.dt+x[2]) 
                                       - np.sin(x[2])))
        a15 = float((self.dt*x[3]/x[4])*np.cos(x[4]*self.dt+x[2]) 
            - (x[3]/x[4]**2)*(np.sin(x[4]*self.dt+x[2]) - np.sin(x[2])))
        a23 = float((x[3]/x[4]) * (np.sin(x[4]*self.dt+x[2]) 
                                   - np.sin(x[2])))
        a24 = float((1.0/x[4]) * (-np.cos(x[4]*self.dt+x[2]) 
                                  + np.cos(x[2])))
        a25 = float((self.dt*x[3]/x[4])*np.sin(x[4]*self.dt+x[2]) 
            - (x[3]/x[4]**2)*(-np.cos(x[4]*self.dt+x[2]) + np.cos(x[2])))
        A = np.matrix([[1.0, 0.0, a13, a14, a15],
                       [0.0,1.0, a23, a24, a25],
                       [0.0, 0.0, 1.0, 0.0, self.dt],
                       [0.0, 0.0, 0.0, 1.0, 0.0],
                       [0.0, 0.0, 0.0, 0.0, 1.0]])
        return A
 
    def H_matrix(self):
        H = np.matrix([[1,0.0,0.0,0.0,0.0],
                       [0.0,1.0,0.0,0.0,0.0],
                       [0.0,0.0,0.0,1.0,0.0],
                       [0.0,0.0,0.0,0.0,1.0]])
        return H

    #extend kalman filter
    '''
        input:raw_x,raw_y,raw_v,raw_yaw_v
        output:x,y,yaw
        state: x,y,yaw,v,yaw_v correspond to x[0-4]
    '''
    def extended_kalman_filter(self,measurement):
        #time update
        x_pri = self.state_estimate(self.x)
        A = self.A_matrix(x_pri)
        P_pri = A*self.P*A.T + self.Q
        
        H = self.H_matrix()
        K = (self.P*H.T) * np.linalg.inv( H * P_pri * H.T + self.R)
        Z = np.matrix([[measurement[0],measurement[1],
                        measurement[2],measurement[3]]]).T
        self.x = x_pri + K * ( Z - H * x_pri)
        self.P = (self.I - K * H) * P_pri

        optimal_estimate = [float(self.x[0]),float(self.x[1]),
                            float(self.x[2]),float(self.x[3]),
                            float(self.x[4])]
        return optimal_estimate

if __name__ == '__main__':
    initial = [1,2,3,4,5]
    ekf = EKF(initial)
    measurement = [1,2,3,4]
    ekf.extended_kalman_filter(measurement)
