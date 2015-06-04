# Details #

This function will start generating a tone for a specified duration, or until [noTone()](FDnotone.md) is called.

# Use #

tone(frequency,duration) **or** tone(frequency)
  * Arguments:
    * _unsigned int_ frequency:
      * the frequency of the tone
    * _unsigned long_ duration:
      * The duration to play the tone in ms
      * If no duration is given the tone will be output until [noTone()](FDnotone.md) is called.