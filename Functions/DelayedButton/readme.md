This foundamental function is used to design a delayed button input so that after a required time pressed, a control signal will be sent to control external devices, such as: Control lights on/off.  Locker open/close.

parameters declearation:

      Input:
      
            button :         Button Input Signal     
            Reset:           Highest external control
            
      Output:
      
            OutputSignal:    Output Controled signal (connect to LED or Locker or Relay)
            CurrentState:    Current state indicator, HIGH means WORKING, LOW means IDEL
            
      Variables:
        
            debounceDelay:    The amount of time to delay (second)
