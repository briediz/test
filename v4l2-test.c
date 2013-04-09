
#include <sys/types.h> 
#include <sys/stat.h> 
#include <sys/ioctl.h> 
#include <sys/mman.h> 
#include <linux/videodev2.h> 
#include <fcntl.h> 
#include <unistd.h> 
#include <errno.h> 
#include <stdio.h>

int deviceHandle; 
int err;

void closedev() 
{ 
  err = close(deviceHandle); 
  if (deviceHandle == -1) 
      printf("could not close device /dev/video\n"); 
  else 
      printf("close device /dev/video\n"); 
}

int main () 
{
  
  char* deviceName = "/dev/video0";
  deviceHandle = open (deviceName, O_RDWR); 
  if (deviceHandle == -1) 
        printf("could not open device %s\n", deviceName);
  else 
        printf("open device %s\n", deviceName);


  struct v4l2_capability capability; // this structure is filled out by the ioctl call 
  if (ioctl (deviceHandle, VIDIOC_QUERYCAP, &capability) != -1) 
    printf("ioctl VIDIOC_QUERYCAP %s\n", deviceName);
  else 
    { 
      printf("failed ioctl VIDIOC_QUERYCAP %s\n", deviceName);
      closedev(); 
      return; 
    }


  if ((capability.capabilities & V4L2_CAP_VIDEO_CAPTURE) != 0) 
  { 
    printf("this device can capture video to memory\n"); 
  } 
  else 
  { 
    printf("this device CANNOT capture video to memory\n"); 
  }

  // enumerat devices 
  /*struct v4l2_input input; 
  if (ioctl (deviceHandle, VIDIOC_ENUMINPUT, &input) != -1) 
    printf("ioctl VIDIOC_ENUMINPUT /dev/video found input.index %d\n", input.index); 
  else 
  { 
    printf("failed ioctl VIDIOC_ENUMINPUT /dev/video\n"); 
    perror(""); 
    closedev(); 
    return; 
  }*/

  struct v4l2_input queryChannel; 
  int i = 0; 
  while (i < 5/*input.index*/) 
  { 
          queryChannel.index = i; 
          if (ioctl (deviceHandle, VIDIOC_ENUMINPUT, &queryChannel) != -1) 
          {       // ioctl success, queryChannel contains information about this channel 
                  printf ("%d. %s %d\n", queryChannel.index, queryChannel.name, queryChannel.type ); 
          } 
          else 
          {       printf("failed ioctl %s queryChannel\n", deviceName);
          } 
          ++ i; 
  }

  struct v4l2_input input; 
  memset (&input, 0, sizeof (input)); 
  int index;

  if (-1 == ioctl (deviceHandle, VIDIOC_G_INPUT, &index)) { 
          perror ("VIDIOC_G_INPUT"); 
  }

  memset (&input, 0, sizeof (input)); 
  input.index = index;

  if (-1 == ioctl (deviceHandle, VIDIOC_ENUMINPUT, &input)) { 
          perror ("VIDIOC_ENUMINPUT"); 
  }

  printf ("Current input - index %d name %s std 0x%lx\n", input.index, input.name, input.std);

  int width;
  int height;
  struct v4l2_format captureWindow;
  memset (&captureWindow, 0, sizeof (captureWindow));
  if (ioctl (deviceHandle, VIDIOC_G_FMT, &captureWindow) == -1)
  {
	  	  perror ("VIDIOC_G_FMT");
  } else {
	  width = captureWindow.fmt.pix.width;
	  height = captureWindow.fmt.pix.height;
	  printf ("height %d width %d \n", width, height);
  }


	struct v4l2_format captureWindow;
	memset (&captureWindow, 0, sizeof (captureWindow));
	if (ioctl (deviceHandle, VIDIOC_QUERYBUF, &captureWindow) == -1)
	{
		perror ("VIDIOC_QUERYBUF");
	} else {
		printf ("height %d width %d \n", width, height);
	}




/* 
  // set input to 1 
  index = 1;

  if (-1 == ioctl (deviceHandle, VIDIOC_S_INPUT, &index)) { 
          perror ("VIDIOC_S_INPUT"); 
          
  } 
  memset (&input, 0, sizeof (input)); 
  input.index = index;

  if (-1 == ioctl (deviceHandle, VIDIOC_ENUMINPUT, &input)) { 
          perror ("VIDIOC_ENUMINPUT"); 
  }

  printf ("Current input: %s\n", input.name); 
*/     



  closedev(); 
  
  return; 
}


