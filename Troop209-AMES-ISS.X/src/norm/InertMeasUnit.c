#include "system.h"
#include "nesi.h"
#include "PIC24_I2C2_R7.h"
#include "Gyroscope.h"
#include "math.h"
#include "LSM9DS0.h"
#include "InertMeasUnit.h"


const char GYR_MaxSend = 64        ;     // Max number of bytes you can send in one message
const char GYR_MaxRead = 64      ;     // Max number of bytes you can continuous read (Register size)

const char GYR_MinRegAddr  = 0x00 ;   // Lowest register address accessable from host
const char GYR_MaxRegAdsr  = 0x3F  ;   // Highest register address accessable from host

char    ACL_rbuf[16]               ;  // Read buffer
char    MAG_rbuf[16]               ;  // Read buffer
char    GYR_rbuf[16]               ;  // Read buffer

#define MAG_LPF_FACTOR  0.4
#define ACC_LPF_FACTOR  0.1

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//
//               wORK Time
//
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//
//
//

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// G Tutorial 3 :
/*
	This program  reads the angles from the accelerometer and gyroscope
	on a BerryIMU connected to a Raspberry Pi.
	http://ozzmaker.com/


    Copyright (C) 2014  Mark Williams

*/


// NRM #define DT 0.02         // [s/loop] loop period. 20ms
// NRM #define AA 0.97         // complementary filter constant

#define A_GAIN 0.0573      // [deg/LSB]
#define G_GAIN 0.070     // [deg/s/LSB]
#define RAD_TO_DEG 57.29578
#define M_PI 3.14159265358979323846

#define DT 0.02         // [s/loop] loop period. 20ms
#define AA 0.97         // complementary filter constant

//Used by Kalman Filters
float Q_angle  =  0.01;
float Q_gyro   =  0.0003;
float R_angle  =  0.01;
float x_bias = 0;
float y_bias = 0;
float XP_00 = 0, XP_01 = 0, XP_10 = 0, XP_11 = 0;
float YP_00 = 0, YP_01 = 0, YP_10 = 0, YP_11 = 0;
float KFangleX = 0.0;
float KFangleY = 0.0;

// NRM float kalmanFilterX(float accAngle, float gyroRate);
// NRM float kalmanFilterY(float accAngle, float gyroRate);

/*
void  INThandler(int sig)
{
        signal(sig, SIG_IGN);
        exit(0);
}
*/
/*
int mymillis()
{
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return (tv.tv_sec) * 1000 + (tv.tv_usec)/1000;
}

int timeval_subtract(struct timeval *result, struct timeval *t2, struct timeval *t1)
{
    long int diff = (t2->tv_usec + 1000000 * t2->tv_sec) - (t1->tv_usec + 1000000 * t1->tv_sec);
    result->tv_sec = diff / 1000000;
    result->tv_usec = diff % 1000000;
    return (diff<0);
}
*/

int gyro3_main(int argc, char *argv[])
{

	float rate_gyr_y = 0.0;      // [deg/s]
	float rate_gyr_x = 0.0;      // [deg/s]
	float rate_gyr_z = 0.0;      // [deg/s]

	int  accRaw[3];
    int  magRaw[3];
	int  gyrRaw[3];



	float gyroXangle = 0.0;
	float gyroYangle = 0.0;
	float gyroZangle = 0.0;
	float AccYangle = 0.0;
	float AccXangle = 0.0;
	float CFangleX = 0.0;
	float CFangleY = 0.0;

    float accXnorm;
    float accYnorm;
    float pitch;
    float roll;
    float magXcomp;
    float magYcomp;
    
    float scaledMag[3];

	int oldXMagRawValue = 0;
	int oldYMagRawValue = 0;
	int oldZMagRawValue = 0;
	int oldXAccRawValue = 0;
	int oldYAccRawValue = 0;
	int oldZAccRawValue = 0;

    int magXmax = 0;
    int magYmax = 0;
    int magZmax = 0;
    int magXmin = 0;
    int magYmin = 0;
    int magZmin = 0;


	// NRM int startInt  = mymillis();
	// struct  timeval tvBegin, tvEnd, tvDiff;
    // NRM long tvBegin, tvEnd, tvDiff;    // NRM change from struct timeval

    // NRM     signal(SIGINT, INThandler);

	enableIMU();

	// NRM gettimeofday(&tvBegin, NULL);


	//while(1)
	{
	// NRM startInt = mymillis();


	
	readACC(accRaw);
	readMAG(magRaw);
    readGYR(gyrRaw);

	//Convert Gyro raw to degrees per second
	rate_gyr_x = (float) gyrRaw[0]  * G_GAIN;
	rate_gyr_y = (float) gyrRaw[1]  * G_GAIN;
	rate_gyr_z = (float) gyrRaw[2]  * G_GAIN;



	//Calculate the angles from the gyro
	gyroXangle+=rate_gyr_x*DT;
	gyroYangle+=rate_gyr_y*DT;
	gyroZangle+=rate_gyr_z*DT;


	//Convert Accelerometer values to degrees
	AccXangle = (float) (atan2(accRaw[1],accRaw[2])+M_PI)*RAD_TO_DEG;
	AccYangle = (float) (atan2(accRaw[2],accRaw[0])+M_PI)*RAD_TO_DEG;

    		//Apply low pass filter to reduce noise
		magRaw[0] =  magRaw[0]  * MAG_LPF_FACTOR + oldXMagRawValue*(1 - MAG_LPF_FACTOR);
		magRaw[1] =  magRaw[1]  * MAG_LPF_FACTOR + oldYMagRawValue*(1 - MAG_LPF_FACTOR);
		magRaw[2] =  magRaw[2]  * MAG_LPF_FACTOR + oldZMagRawValue*(1 - MAG_LPF_FACTOR);
		accRaw[0] =  accRaw[0]  * ACC_LPF_FACTOR + oldXAccRawValue*(1 - ACC_LPF_FACTOR);
		accRaw[1] =  accRaw[1]  * ACC_LPF_FACTOR + oldYAccRawValue*(1 - ACC_LPF_FACTOR);
		accRaw[2] =  accRaw[2]  * ACC_LPF_FACTOR + oldZAccRawValue*(1 - ACC_LPF_FACTOR);


		oldXMagRawValue = magRaw[0];
		oldYMagRawValue = magRaw[1];
		oldZMagRawValue = magRaw[2];
		oldXAccRawValue = accRaw[0];
		oldYAccRawValue = accRaw[1];
		oldZAccRawValue = accRaw[2];



                //Apply hard iron calibration
                magRaw[0] -= (magXmin + magXmax) /2 ;
                magRaw[1] -= (magYmin + magYmax) /2 ;
                magRaw[2] -= (magZmin + magZmax) /2 ;

                //Apply soft iron calibration
                scaledMag[0]  = (float)(magRaw[0] - magXmin) / (magXmax - magXmin) * 2 - 1;
                scaledMag[1]  = (float)(magRaw[1] - magYmin) / (magYmax - magYmin) * 2 - 1;
                scaledMag[2]  = (float)(magRaw[2] - magZmin) / (magZmax - magZmin) * 2 - 1;


        //Change the rotation value of the accelerometer to -/+ 180 and move the Y axis '0' point to up.
        //Two different pieces of code are used depending on how your IMU is mounted.
        //If IMU is upside down
	// NRM / *
        if (AccXangle >180)
                AccXangle -= (float)360.0;

        AccYangle-=90;
        if (AccYangle >180)
                AccYangle -= (float)360.0;
	// NRM * /

        //If IMU is up the correct way, use these lines
        AccXangle -= (float)180.0;
	if (AccYangle > 90)
	        AccYangle -= (float)270;
	else
		AccYangle += (float)90;

        		//If your IMU is upside down, comment out the two lines below which we correct the tilt calculation
		//accRaw[0] = -accRaw[0];
		//accRaw[1] = -accRaw[1];

		//Compute heading
	        float heading = 180 * atan2(magRaw[1],magRaw[0])/M_PI;

		//Convert heading to 0 - 360
        	if(heading < 0)
	  	      heading += 360;

		//Normalize accelerometer raw values.
                accXnorm = accRaw[0]/sqrt(accRaw[0] * accRaw[0] + accRaw[1] * accRaw[1] + accRaw[2] * accRaw[2]);
                accYnorm = accRaw[1]/sqrt(accRaw[0] * accRaw[0] + accRaw[1] * accRaw[1] + accRaw[2] * accRaw[2]);

		//Calculate pitch and roll
		pitch = asin(accXnorm);
		roll = -asin(accYnorm/cos(pitch));

		//Calculate the new tilt compensated values
		magXcomp = magRaw[0]*cos(pitch)+magRaw[02]*sin(pitch);
		magYcomp = magRaw[0]*sin(roll)*sin(pitch)+magRaw[1]*cos(roll)-magRaw[2]*sin(roll)*cos(pitch);


		//Calculate heading
		heading = 180*atan2(magYcomp,magXcomp)/M_PI;

                //Convert heading to 0 - 360
		if(heading < 0)
		      heading += 360;


	//Kalman Filter
// NRM 	float kalmanX = kalmanFilterX(AccXangle, rate_gyr_x);
// NRM 	float kalmanY = kalmanFilterY(AccYangle, rate_gyr_y);
	// NRM printf ("\033[22;31mkalmanX %7.3f  \033[22;36mkalmanY %7.3f\t\e[m",kalmanX,kalmanY);

	//Complementary filter used to combine the accelerometer and gyro values.
	CFangleX=AA*(CFangleX+rate_gyr_x*DT) +(1 - AA) * AccXangle;
	CFangleY=AA*(CFangleY+rate_gyr_y*DT) +(1 - AA) * AccYangle;


	// NRM printf ("GyroX  %7.3f \t AccXangle \e[m %7.3f \t \033[22;31mCFangleX %7.3f\033[0m\t GyroY  %7.3f \t AccYangle %7.3f \t \033[22;36mCFangleY %7.3f\t\033[0m\n",gyroXangle,AccXangle,CFangleX,gyroYangle,AccYangle,CFangleY);
/*
	//Each loop should be at least 20ms.
        while(mymillis() - startInt < (DT*1000))
        {
     // NRM        usleep(100);
        }
 */
	// NRM printf("Loop Time %d\t", mymillis()- startInt);
    }
    return (0)  ;
}



/*
  float kalmanFilterX(float accAngle, float gyroRate)
  {
    float  y, S;
    float K_0, K_1;


    KFangleX += DT * (gyroRate - x_bias);

    XP_00 +=  - DT * (XP_10 + XP_01) + Q_angle * DT;
    XP_01 +=  - DT * XP_11;
    XP_10 +=  - DT * XP_11;
    XP_11 +=  + Q_gyro * DT;

    y = accAngle - KFangleX;
    S = XP_00 + R_angle;
    K_0 = XP_00 / S;
    K_1 = XP_10 / S;

    KFangleX +=  K_0 * y;
    x_bias  +=  K_1 * y;
    XP_00 -= K_0 * XP_00;
    XP_01 -= K_0 * XP_01;
    XP_10 -= K_1 * XP_00;
    XP_11 -= K_1 * XP_01;

    return KFangleX;
  }


  float kalmanFilterY(float accAngle, float gyroRate)
  {
    float  y, S;
    float K_0, K_1;


    KFangleY += DT * (gyroRate - y_bias);

    YP_00 +=  - DT * (YP_10 + YP_01) + Q_angle * DT;
    YP_01 +=  - DT * YP_11;
    YP_10 +=  - DT * YP_11;
    YP_11 +=  + Q_gyro * DT;

    y = accAngle - KFangleY;
    S = YP_00 + R_angle;
    K_0 = YP_00 / S;
    K_1 = YP_10 / S;

    KFangleY +=  K_0 * y;
    y_bias  +=  K_1 * y;
    YP_00 -= K_0 * YP_00;
    YP_01 -= K_0 * YP_01;
    YP_10 -= K_1 * YP_00;
    YP_11 -= K_1 * YP_01;

    return KFangleY;
  }
*/

/*
void  readBlock(char command, char size, char *data) 
{ 
    int result = i2c_smbus_read_i2c_block_data(file, command, size, data); 
    if (result != size) 
    { 
        printf("Failed to read block from I2C."); 
        exit(1); 
    } 
} 
*/
/*
void selectDevice(int file, int addr) 
{ 
        if (ioctl(file, I2C_SLAVE, addr) < 0) { 
"		 printf(""Failed to select I2C device.""); "
        } 
} 
*/


// int ACL_ReadValues (void)
int readACC(int  *a) 
{       int ACL_stat  = 0 ;
        const char  ACL_RdValLS1     = 1                   ;
        char        ACL_RdValPR1[1]  = {OUT_X_L_A+0x80}    ;    // ACC data Register Set
        const char  ACL_RdValLR1     = 6                  ;

        char ACCblock[6]; 
        //NRM    selectDevice(file,ACC_ADDRESS); 
        // NRM   readBlock(0x80 | OUT_X_L_A, sizeof(block), block); 
        ACL_stat= i2c2_TalkToDevice(ACC_Addr, ACL_RdValLS1, ACL_RdValPR1, ACL_RdValLR1, ACCblock)    ;
    

        *a     = (Int16)(ACCblock[0] | ACCblock[1] << 8); 
        *(a+1) = (Int16)(ACCblock[2] | ACCblock[3] << 8); 
        *(a+2) = (Int16)(ACCblock[4] | ACCblock[5] << 8); 

 return (ACL_stat)     ;
} 

//     int MAG_ReadValues (void)
int readMAG(int  *m) 
{       int MAG_stat  = 0 ;
        const char  MAG_RdValLS1     = 1                   ;
        char        MAG_RdValPR1[1]  = {OUT_X_L_M+0x80}    ;    // MAG data Register Set
        const char  MAG_RdValLR1     = 6                  ;

        char MAGblock[6]; 
        //NRM    selectDevice(file,MAG_ADDRESS); 
        // NRM   readBlock(0x80 | OUT_X_L_M, sizeof(block), block); 
        MAG_stat= i2c2_TalkToDevice(MAG_Addr, MAG_RdValLS1, MAG_RdValPR1, MAG_RdValLR1, MAGblock)    ;
  
        *m     = (Int16)(MAGblock[0] | MAGblock[1] << 8); 
        *(m+1) = (Int16)(MAGblock[2] | MAGblock[3] << 8); 
        *(m+2) = (Int16)(MAGblock[4] | MAGblock[5] << 8); 

        return (MAG_stat)     ;
} 

// int GYR_ReadValues (void)
int readGYR(int *g) 
{       int GYR_stat  = 0 ;
        const char  GYR_RdValLS1     = 1                   ;
        char        GYR_RdValPR1[1]  = {OUT_X_L_G+0x80}    ;    // GYR data Register Set
        const char  GYR_RdValLR1     = 6                  ;

        char GYRblock[6]; 
        //NRM    selectDevice(file,GYR_ADDRESS); 
        // NRM readBlock(0x80 | OUT_X_L_G, sizeof(block), block); 
        GYR_stat= i2c2_TalkToDevice(GYR_Addr, GYR_RdValLS1, GYR_RdValPR1, GYR_RdValLR1, GYRblock)    ;
 

        *g     = (Int16)(GYRblock[0] | GYRblock[1] << 8); 
        *(g+1) = (Int16)(GYRblock[2] | GYRblock[3] << 8); 
        *(g+2) = (Int16)(GYRblock[4] | GYRblock[5] << 8); 
        
        return (GYR_stat)     ;
} 

/*
void writeAccReg(char reg, char value) 
{ 
    //NRM selectDevice(file,ACC_ADDRESS); 
  //NRM int result = i2c_smbus_write_byte_data(file, reg, value); 
    if (result == -1) 
    { 
        // NRM printf ("Failed to write byte to I2C Acc."); 
        // NRM exit(1); 
    } 
} 
*/
/*
void writeMagReg(char reg, char value) 
{ 
    //NRM selectDevice(file,MAG_ADDRESS); 
  //NRM int result = i2c_smbus_write_byte_data(file, reg, value); 
    if (result == -1) 
    { 
        // NRM printf("Failed to write byte to I2C Mag."); 
        // NRM exit(1); 
     } 
 } 
*/


/*
 void writeGyrReg(char reg, char value) 
 { 
    //NRM selectDevice(file,GYR_ADDRESS); 
   //NRM int result = i2c_smbus_write_byte_data(file, reg, value); 
     if (result == -1) 
     { 
         // NRM printf("Failed to write byte to I2C Gyr."); 
         // NRM exit(1); 
     } 
 } 
*/



 int enableIMU() 
 {      int ACL_stat=0 ;
        int MAG_stat=0 ;
        int GYR_stat=0 ;
        const char  ACL_Cfg1L1      = 3                    ;
        char        ACL_Cfg1S1[7]  = {
            CTRL_REG1_XM+0x80, 
            0b01100111,       //  z,y,x axis enabled, continuous update,  100Hz data rate 
            0b00100000}     ; // +/- 16G full scale 
          ACL_stat= i2c2_TalkToDevice(ACC_Addr, ACL_Cfg1L1, ACL_Cfg1S1, 0, NullPtr)    ;    // Reg *0


         //Enable the magnetometer 
        const char  MAG_Cfg1L1     = 4   ; // # Bytes 
        char        MAG_Cfg1S1[4]  = {
            CTRL_REG5_XM+0x80, // Reg 5 with Auto Increment flag 
            0b11110000,        // Temp enable, M data rate = 50Hz 
            0b01100000,        // +/-12gauss 
            0b00000000}   ;    // Continuous-conversion mode 
         
        MAG_stat= i2c2_TalkToDevice(MAG_Addr, MAG_Cfg1L1, MAG_Cfg1S1, 0, NullPtr)    ;    // Reg *0


    	 // Enable Gyro "
        const char  GYR_Cfg1L1     = 3   ; // # Bytes                
        char        GYR_Cfg1S1[3]  = {
        CTRL_REG1_G+0x80, 
        0b00001111, // Normal power mode, all axes enabled 
        0b00110000} ; // Continuos update, 2000 dps full scale 

        GYR_stat= i2c2_TalkToDevice(GYR_Addr, GYR_Cfg1L1, GYR_Cfg1S1, 0, NullPtr)    ;    // Reg *0
        return (ACL_stat+MAG_stat+GYR_stat) ;
 } 
 
 
 int dumpIMU(char *ptr)
 {      int IMU_stat=0 ;
     
        const char IMU_DmpLS1=1    ;    //
        char       IMU_DmpS1[1]={0+0x80};    // start at reg 00 +auto Incr flag
        const char IMU_DmpLR1=64   ;    // go to reg 3F
 
        const char IMU_DmpLS2=1    ;    //
        char       IMU_DmpS2[1]={0X20+0x80};    // start at reg 0X20+auto Incr flag
        const char IMU_DmpLR2=25   ;    // go to reg 3F
        IMU_stat= i2c2_TalkToDevice(ACC_Addr, IMU_DmpLS1, IMU_DmpS1, IMU_DmpLR1, ptr)       ;    // Reg *0
        IMU_stat= i2c2_TalkToDevice(GYR_Addr, IMU_DmpLS2, IMU_DmpS2, IMU_DmpLR2, ptr+64)    ;    // Reg *0
        return (IMU_stat)   ;
 }
 int AMG5_main(int argc, char *argv[])
{ return (0) ;
}

/*
int AMG5_main(int argc, char *argv[])
{
	// int magRaw[3];
	// int accRaw[3];
	float accXnorm,accYnorm,pitch,roll,magXcomp,magYcomp;
        float scaledMag[3];

	int oldXMagRawValue = 0;
	int oldYMagRawValue = 0;
	int oldZMagRawValue = 0;
	int oldXAccRawValue = 0;
	int oldYAccRawValue = 0;
	int oldZAccRawValue = 0;

    int magXmax = 0;
    int magYmax = 0;
    int magZmax = 0;
    int magXmin = 0;
    int magYmin = 0;
    int magZmin = 0;

*/
     // NRM    signal(SIGINT, INThandler);
/*
	//Open the i2c bus
	sprintf(filename, "/dev/i2c-%d", 1);
	file = open(filename, O_RDWR);
	if (file<0) {
        	printf("Unable to open I2C bus!");
                exit(1);
	}
*/
/*
	//Select the magnetomoter
	if (ioctl(file, I2C_SLAVE, MAG_ADDRESS) < 0) {
                printf("Error: Could not select magnetometer\n");
        }
*/
 /*
	enableIMU();

	while(1)
	{
		readMAG(magRaw);
		readACC(accRaw);

		//Apply low pass filter to reduce noise
		magRaw[0] =  magRaw[0]  * MAG_LPF_FACTOR + oldXMagRawValue*(1 - MAG_LPF_FACTOR);
		magRaw[1] =  magRaw[1]  * MAG_LPF_FACTOR + oldYMagRawValue*(1 - MAG_LPF_FACTOR);
		magRaw[2] =  magRaw[2]  * MAG_LPF_FACTOR + oldZMagRawValue*(1 - MAG_LPF_FACTOR);
		accRaw[0] =  accRaw[0]  * ACC_LPF_FACTOR + oldXAccRawValue*(1 - ACC_LPF_FACTOR);
		accRaw[1] =  accRaw[1]  * ACC_LPF_FACTOR + oldYAccRawValue*(1 - ACC_LPF_FACTOR);
		accRaw[2] =  accRaw[2]  * ACC_LPF_FACTOR + oldZAccRawValue*(1 - ACC_LPF_FACTOR);


		oldXMagRawValue = magRaw[0];
		oldYMagRawValue = magRaw[1];
		oldZMagRawValue = magRaw[2];
		oldXAccRawValue = accRaw[0];
		oldYAccRawValue = accRaw[1];
		oldZAccRawValue = accRaw[2];



                //Apply hard iron calibration
                magRaw[0] -= (magXmin + magXmax) /2 ;
                magRaw[1] -= (magYmin + magYmax) /2 ;
                magRaw[2] -= (magZmin + magZmax) /2 ;

                //Apply soft iron calibration
                scaledMag[0]  = (float)(magRaw[0] - magXmin) / (magXmax - magXmin) * 2 - 1;
                scaledMag[1]  = (float)(magRaw[1] - magYmin) / (magYmax - magYmin) * 2 - 1;
                scaledMag[2]  = (float)(magRaw[2] - magZmin) / (magZmax - magZmin) * 2 - 1;

		//If your IMU is upside down, comment out the two lines below which we correct the tilt calculation
		//accRaw[0] = -accRaw[0];
		//accRaw[1] = -accRaw[1];

		//Compute heading
	        float heading = 180 * atan2(magRaw[1],magRaw[0])/M_PI;

		//Convert heading to 0 - 360
        	if(heading < 0)
	  	      heading += 360;


		//Normalize accelerometer raw values.
                accXnorm = accRaw[0]/sqrt(accRaw[0] * accRaw[0] + accRaw[1] * accRaw[1] + accRaw[2] * accRaw[2]);
                accYnorm = accRaw[1]/sqrt(accRaw[0] * accRaw[0] + accRaw[1] * accRaw[1] + accRaw[2] * accRaw[2]);

		//Calculate pitch and roll
		pitch = asin(accXnorm);
		roll = -asin(accYnorm/cos(pitch));

		//Calculate the new tilt compensated values
		magXcomp = magRaw[0]*cos(pitch)+magRaw[02]*sin(pitch);
		magYcomp = magRaw[0]*sin(roll)*sin(pitch)+magRaw[1]*cos(roll)-magRaw[2]*sin(roll)*cos(pitch);


		//Calculate heading
		heading = 180*atan2(magYcomp,magXcomp)/M_PI;

                //Convert heading to 0 - 360
		if(heading < 0)
		      heading += 360;
	}
    return (0)  ;
}
*/