#include "project.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
//PWM_SERVO_1 is Azimuth P12[7]
//PWM_SERVO_2 is arm2 P12[2]
//PWM_SERVO_3 gripper P13[0]
//PWM_SERVO_4 arm1 P13[1]

int main(void)
{   int cv1=1500;int dir1=0;
    int cv2=1500;int dir2=0;
    int cv3=1500;int dir3=0;
    int cv4=1500;int dir4=0;
    int counter=0;
    char pos1[5],pos2[5],pos3[5],pos4[5];
    __enable_irq(); /* Enable global interrupts. */
    
    UART_Start();
    setvbuf(stdin, NULL, _IONBF, 0);
    PWM_SERVO_1_Start();
    PWM_SERVO_2_Start();
    PWM_SERVO_3_Start();
    PWM_SERVO_4_Start();
    char c;
    //int pinReadValue=0;
    /* Place your initialization/startup code here (e.g. MyInst_Start()) */

    for(;;)
    { //if (pinReadValue==1){
            //Cy_GPIO_Write(LED5_Red_0_PORT,LED5_Red_0_NUM,0);
            //CyDelay(200);
            //Cy_GPIO_Write(LED5_Red_0_PORT,LED5_Red_0_NUM,1);
            //CyDelay(200);}
      //else CyDelay(500);
     //pinReadValue =  Cy_GPIO_Read( SW2_PORT, SW2_NUM );
        
        counter++;
        
        
        
        if (dir1==0){if (cv1<2000) {cv1++;CyDelay(1);} else dir1=1;}
        else{ if (cv1>1000){cv1--; CyDelay(1);} else dir1=0;}
        PWM_SERVO_1_SetCompare0(cv1);
       
        
        if (dir2==0){if (cv2<1800) {cv2++;CyDelay(1);} else dir2=1;}
        else{ if (cv2>1200){cv2--; CyDelay(1);} else dir2=0;}
        PWM_SERVO_2_SetCompare0(cv2);
        
        
        //if (dir3==0){if (cv3<2000) {cv3++;CyDelay(1);} else dir3=1;}
        //else{ if (cv3>1400){cv3--; CyDelay(1);} else dir3=0;}
        PWM_SERVO_3_SetCompare0(cv3);
       
        
        if (dir4==0){if (cv4<2000) {cv4++;CyDelay(1);} else dir4=1;}
        else{ if (cv4>1500){cv4--; CyDelay(1);} else dir4=0;}
        PWM_SERVO_4_SetCompare0(cv4);
        itoa(cv1,pos1,10);
        itoa(cv2,pos2,10);
        itoa(cv3,pos3,10);
        itoa(cv4,pos4,10);
        //count++;
        //if (count>1000)
        //{
        //char *mes=malloc(40*sizeof(char));
        //itoa(cv1,pos1,10);
        //strcat(mes, pos1);
        //strcat(mes, "|");
        //itoa(cv2,pos2,10);
        //strcat(mes, pos2);
       // strcat(mes, "|");
        //itoa(cv3,pos3,10);
        //strcat(mes, pos3);
        //strcat(mes, "|");
        //itoa(cv4,pos4,10);
        //strcat(mes, pos4);
        //strcat(mes, "|");
        //UART_PutString(strcat(mes,"\r\n"));count=0;
        //free(mes);
    //}
     
        if (counter>200)
        {   c=getchar();
            if(c)
            {
            switch (c)
            {case '?':
                printf("%s|%s|%s|%s\r\n",pos1,pos2,pos3,pos4);break;
            case 's':
                //increase gripper angle
                cv3+=100;
                printf("increase gripper angle\r\n");break;
            case 'z':
                //decrease gripper angle
                cv3-=100;
                printf("decrease gripper angle\r\n");break;
                
            }
            counter=0;
            }
        }
        
}
}

