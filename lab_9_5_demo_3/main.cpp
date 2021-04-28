

#include "mbed.h"
#include "mbed_rpc.h"

/**
 *  This example program has been updated to use the RPC implementation in the new mbed libraries.
 *  This example demonstrates using RPC over serial
**/
//RpcDigitalOut myled1(LED1,"myled1");
//RpcDigitalOut myled2(LED2,"myled2");
//RpcDigitalOut myled3(LED3,"myled3");
DigitalOut myled1(LED1);
DigitalOut myled2(LED2);
DigitalOut myled3(LED3);
BufferedSerial pc(USBTX, USBRX);
void LEDControl(Arguments *in, Reply *out);
void flip(Arguments *in, Reply *out);

RPCFunction rpcLED(&LEDControl, "LEDControl");
RPCFunction flipLED(&flip, "FLIP");
double x, y;

void flip(Arguments *in, Reply *out){
    //printf("111\n");
    int x = in->getArg<int>();
    if(x==0){
        myled1=0;
        myled2=0;
        myled3=0;
    }else if(x==1){
        myled2=0;
        myled3=0;
        for(int i=0;i<100;i=i+1){
            myled1=1;
            ThisThread::sleep_for(1s);
            myled1=0;
            ThisThread::sleep_for(1s);
        }
    }else if(x==2){
        myled1=0;
        myled3=0;
        for(int i=0;i<100;i=i+1){
            myled2=1;
            ThisThread::sleep_for(1s);
            myled2=0;
            ThisThread::sleep_for(1s);
        }
    }else if(x==3){
        myled1=0;
        myled2=0;
        for(int i=0;i<100;i=i+1){
            myled3=1;
            ThisThread::sleep_for(1s);
            myled3=0;
            ThisThread::sleep_for(1s);
        }
    }
    
}

int main() {
    //The mbed RPC classes are now wrapped to create an RPC enabled version - see RpcClasses.h so don't add to base class

    // receive commands, and send back the responses
    char buf[256], outbuf[256];

    FILE *devin = fdopen(&pc, "r");
    FILE *devout = fdopen(&pc, "w");
    //flip(1);

    while(1) {
        memset(buf, 0, 256);
        for (int i = 0; ; i++) {
            char recv = fgetc(devin);
            if (recv == '\n') {
                printf("\r\n");
                break;
            }
            buf[i] = fputc(recv, devout);
        }
        //Call the static call method on the RPC class
        RPC::call(buf, outbuf);
        printf("%s\r\n", outbuf);
    }
}

// Make sure the method takes in Arguments and Reply objects.
void LEDControl (Arguments *in, Reply *out)   {
    bool success = true;
//for(int i=0;i<=100;i=i+1){
    // In this scenario, when using RPC delimit the two arguments with a space.
    x = in->getArg<double>();
    //y = in->getArg<double>();

    // Have code here to call another RPC function to wake up specific led or close it.
    char buffer[200], outbuf[256];
    char strings[20];
    int led = x;
    //int on = y;
    
    //sprintf(strings, "/myled%d/write 0", led);
    //printf("11123\n");
    sprintf(strings, "/FLIP/run %d", led);
    strcpy(buffer, strings);
    RPC::call(buffer, outbuf);
    if (success) {
        out->putData(buffer);
    } else {
        out->putData("Failed to execute LED control.");
    }
    /*ThisThread::sleep_for(1s);
    sprintf(strings, "/myled%d/write 1", led);
    strcpy(buffer, strings);
    RPC::call(buffer, outbuf);
    if (success) {
        out->putData(buffer);
    } else {
        out->putData("Failed to execute LED control.");
    }
    ThisThread::sleep_for(1s);*/
//}

}
