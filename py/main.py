
import matplotlib.pyplot as plt
import numpy as np 

class Pwm():
    '''Generate senoidal reference for sPWM '''
    def __init__(self,top):
        self._ICR=top
        self._vecSin =np.zeros(self._ICR,dtype=int)
    
    def gera_pwm(self):
        for i in range(0,int(self._ICR)):
            self._vecSin[i]=int(np.sin(2*np.pi*i/self._ICR)*self._ICR)+self._ICR
            i=i+1
        return self._vecSin
        


if __name__=='__main__':
    vec=np.array(Pwm(3200).gera_pwm())
    plt.plot(vec)
    plt.show()
    vec=np.array(Pwm().gera_pwm())
    plt.plot(vec)
    #plt.show()
    #print(vec)

