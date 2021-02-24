#ifndef __HEAD_H__
#define __HEAD_H__

#define type 'k'

#define RGB_LED_RED_ON _IO(type,1)
#define RGB_LED_RED_OFF _IO(type,0)

#define ACCESS_WRITE_INT_DATA _IOW(type,0,int)
#define ACCESS_READ_INT_DATA _IOR(type,0,int)

#define ACCESS_WRITE_CHAR_ARRAY _IOW(type,0,char [128])

#endif


