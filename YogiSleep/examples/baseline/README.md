
# Baseline Voltage Tests #


|   mA | w/LED | Conditions              |
| ---: | ----- | ----------------------- |
| 27.0 | 40.5  | basic                   |
| 21.6 | 36.1  | pins 2-13 set to output |
| 21.4 | 36.1  | pins 2-13 output & LOW  |
| 18.8 | 33.6  | same + A0-7             |
| 18.7 | 33.0  | and set LOW             |
|      |       |                         |
|  6.6 | 33.0  | YogiSleep               |
|  3.4 | 27.1  | remove LED from nano    |


The baseline recipe is a simple one second LED on and
5 seconds LED off.

|   mA | Recipe     |
| ---: | ---------- |
| 20.5 | empty      |
| 17.4 | baseoutput |
|  6.5 | YogiSleep  |
