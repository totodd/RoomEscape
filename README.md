# RoomEscape
room escape functions

[脉搏模块]
input:

inner connections:
  black: GND
  blue: input = LOW
  yellow: no display if LOW


[5分钟倒计时模块]

input:
  1. signal
  2. GND
  -. button
  3. power cable

functions:
    [initial:]
      all displaying zero
    [action:]
      if(signal || button)
        counter start from 5 mins

output:
  1. signal: HIGH when counting is over
  2. GND

inner connections:
  1. black: GND
  2. green: start signal = LOW
  3. yellow: reset signal = LOW
  4. red: 5V output
  5. white: signal when count down = LOW

[指纹模块]
input:
  -. button
  1. power cable

functions:
  [initial: ]
    if(fingerprint saved) scanning fingerprint
    else waiting for saving fingerprint
  [action:]
    if(button)
      clean all saved fingerprint, start waiting

    when setting fingerPrint:
      scan the fingerprint twice, when no light is flashing, setting is successful.
        (can only save one fingerprint)

output:
  1. signal = HIGH
  2. GND

[IC卡单卡模块]
input:

output:
  1. output signal = HIGH
  2. 5v vin
  3. GND
  4. GND
  5. SDA
  6. SCL

[IC总控]
input:
  1. power cable
  -. switch
  -. button

functions:
  - press button to add connected devices
  - pull switch to choose sequence mode or all-in mode

output:
  1. NOUT signal = LOW
  2. GND
  3. --
  4. GND
  5. GND
  6. OUT signal = HIGH

[电子琴]
input:

[抖音10s模块]
input:
  -. button
  1. power cable 12v
  2.
