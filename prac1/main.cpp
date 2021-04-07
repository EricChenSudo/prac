#include "mbed.h"
// #include "uLCD_4DGL.h"

using namespace std::chrono;

Timer timer_fast, timer_slow;
Timer debounce; // define debounce timer

// uLCD_4DGL uLCD(D1, D0, D2); // serial tx, serial rx, reset pin;
InterruptIn but1(A1);         // decrease the pre_freq
InterruptIn but2(A2);         // let freq = pre_freq (confirm the freq)
InterruptIn but3(A3);         // increase the pre_freq
DigitalOut led3(LED3);
DigitalOut led2(LED2);
DigitalOut led1(LED1);
DigitalIn mybut(USER_BUTTON);
AnalogOut Aout(PA_4);       // set the analog output
//EventQueue printfqueue;
//EventQueue eventqueue;
EventQueue queue;

float test;

void flip()
{
    led3 = !led3;
}

void flip1()
{
    led2 = !led2;
}

void shine()
{
    if (test <= 0.0f) {
        test = 1.0f;
        Aout = 1.0f;
        printf("%f\n\r", test);
    }
    else {
        test = test - 0.1f;
        Aout = test;
        printf("%f\n\r", test);
    }

    return;
}

void long_time_print()
{
    int i;
    for (i = 0; i < 40; ++i) {
        printf("I am printing.\n\r");
    }
    return;
}

void toggle()
{
   if (duration_cast<milliseconds>(debounce.elapsed_time()).count() > 1000)
   {
      //only allow toggle if debounce timer has passed 1s
      queue.call(long_time_print);
      debounce.reset(); //restart timer when the toggle is performed
   }
}




int main(void)
{
    test = 1.0f;
    Aout = 1.0f;
    timer_fast.start();
    timer_slow.start();
    led3 = 1;
    led2 = 1;
    led1 = 1;
    //Thread printfThread(osPriorityNormal);
    //printfThread.start(callback(&printfqueue, &EventQueue::dispatch_forever));
    //Thread eventThread(osPriorityNormal);
    //eventThread.start(callback(&eventqueue, &EventQueue::dispatch_forever));

    debounce.start();



    but2.rise(toggle);
    but1.rise(&flip);
    but3.rise(&flip1);
    // Ticker shine_ticker;
    // shine_ticker.attach(eventqueue.event(&shine), 1s);
    queue.call_every(1s, shine); // can't put < 1
    queue.dispatch();
/*    while (1) {
        if(chrono::duration_cast<chrono::seconds>(timer_fast.elapsed_time()).count() > 0.5){
            led1 = !led1;
            timer_fast.reset();
        }

        if(chrono::duration_cast<chrono::seconds>(timer_slow.elapsed_time()).count() > 1){
            led2 = !led2;
            timer_slow.reset();
        }
    }*/
}
