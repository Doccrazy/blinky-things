#include <npNeoPixel.h>
#include <npMeteor.h>

//-------------------------------------------------------------------------------------
unsigned int Delay = 100;

// define three meteors
// npMeteor(red, green, blue, length, decay, random, delay, vneostrip, keepBackground)
npMeteor Meteor1(0x30, 0xAA, 0x33, 1, 184, true, Delay, vNeo1, false);
npMeteor Meteor2(0x30, 0xAA, 0x33, 1, 184, true, Delay, vNeo2, false);
npMeteor Meteor3(0x30, 0xAA, 0x33, 1, 184, true, Delay, vNeo3, false);
npMeteor Meteor4(0x30, 0xAA, 0x33, 1, 184, true, Delay, vNeo4, false);

byte counter=1;
byte *cm;

//-------------------------------------------------------------------------------------
void run_meteor ()
{
  Meteor1.update();
  Meteor2.update();
  Meteor3.update();
  Meteor4.update();
  
  if (counter%2==0)
  {
    cm=Wheel(counter);
    Meteor1.changeColor(*cm, *(cm+1), *(cm+2));
    Meteor2.changeColor(*cm, *(cm+1), *(cm+2));
    Meteor3.changeColor(*cm, *(cm+1), *(cm+2));
    Meteor4.changeColor(*cm, *(cm+1), *(cm+2));
    counter=counter+5;
  }
  
  if (Meteor1.hasFinished()) {
    Meteor1.restart();
    counter++;
  }
  if (Meteor2.hasFinished()) Meteor2.restart();   
  if (Meteor3.hasFinished()) Meteor3.restart();   
  if (Meteor4.hasFinished()) Meteor4.restart();
}
