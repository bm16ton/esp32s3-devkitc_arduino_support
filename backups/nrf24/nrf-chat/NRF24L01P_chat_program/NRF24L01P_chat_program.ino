#include<SPI.h>
#include"nRF24L01.h"
#include"RF24.h"
//#include"printf.h"
RF24 radio(9, 10);
const uint64_t pipes[2] = { 0xF0F0F0F0E1LL, 0xF0F0F0F0D2LL };
bool go_to_tx_mode = false;
char input[32] = "";
const int max_payload_size = 32;


char receive_payload[max_payload_size + 1]; // +1 to allow room for a terminating NULL char

void setup() {
  Serial.begin(57600);
//  printf_begin();
  radio.begin();
  radio.setRetries(15, 15);
  radio.openWritingPipe(pipes[1]);
  radio.openReadingPipe(1, pipes[0]);
  radio.startListening();
  radio.printDetails();
  // put your setup code here, to run once:
}


void loop() {
  if (go_to_tx_mode)
  {
    radio.stopListening();
    bool ok = radio.write(input, 32);
    if (ok)
    {
      Serial.printf("ME : %s\n", input);
      go_to_tx_mode = false;
      radio.startListening();
    }
    else
      Serial.print("could not write....\n");
  }
  /*
  if (radio.available())
  {
    char payload[32] = "";
    bool done = false;
    while (!done)
    {
      done = radio.read( payload , 32 );
      printf("HIM : %s\n", payload);
    }
 
  }
  */
     if ( radio.available() )
    {
      // Dump the payloads until we've gotten everything
      uint8_t len;
      bool done = false;
      while (radio.available())
      {
        // Fetch the payload, and see if this was the last one.
        len = radio.getDynamicPayloadSize();
        radio.read( receive_payload, len );

        // Put a zero at the end for easy printing
        receive_payload[len] = 0;

        // Spew it
        Serial.print(F("Got response size="));
        Serial.print(len);
        Serial.print(F(" value="));
        Serial.println(receive_payload);
      }
    }

  if (Serial.available())
  { 
    int len = Serial.readBytesUntil('\n', input, 31);
    input[len] = '\0';
    go_to_tx_mode = true;
  }
 
}
/*
bool radiodone() {
  if (radio.available())
  {
    char payload[32] = "";
    bool done = false;
    while (!done)
    {
      done = radio.read( payload , 32 );
      Serial.printf("HIM : %s\n", payload);
    }
 
  }
}
*/
//  if (Serial.available())
//  { 
//  	char txmsg[32];
//  	char buf[100]= Serial.read();
//    int len= sizeof(buf);
//    end=0;
//    for(int i=0;end!=1;i=i+31)
//    {
//    	if(i<len)&&(len<i+31)
//    	for(j=1;buf[i+j]!='\0';j++)
//    	{
//    		txm[j]=buf[i+j];
//    	}
//    	else
//    	for(j=1;j<31;j++)
//    	{
//    		txm[j]=buf[i+j];
//    	}
//    	if(j==30)
//    	{
//    		txm[31]='\0';
//    	}
//    	else
//    	end=1;
//    	go_to_tx(txm);
    	


  //  }

  //}
