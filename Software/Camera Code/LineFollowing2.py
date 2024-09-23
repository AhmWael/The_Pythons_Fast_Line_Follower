# Black Grayscale Line Following Example
#
# Making a line following robot requires a lot of effort. This example script
# shows how to do the machine vision part of the line following robot. You
# can use the output from this script to drive a differential drive robot to
# follow a line. This script just generates a single turn value that tells
# your robot to go left or right.
#
# For this script to work properly you should point the camera at a line at a
# 45 or so degree angle. Please make sure that only the line is within the
# camera's field of view.

import sensor, image, time, math
from pyb import UART
from pyb import LED
'''
red_led = LED(1)
green_led = LED(2)
blue_led = LED(3)
red_led.on()
green_led.on()
blue_led.on()
'''
uart = UART(3, 115200)

# Tracks a black line. Use [(128, 255)] for a tracking a white line.
GRAYSCALE_THRESHOLD = [(0, 120)]
mainROI = (0,20,160,82)
# Each roi is (x, y, w, h). The line detection algorithm will try to find the
# centroid of the largest blob in each roi. The x position of the centroids
# will then be averaged with different weights where the most weight is assigned
# to the roi near the bottom of the image and less to the next roi and so on.
ROIS = [ # [ROI, weight]
        (20, 0, 120, 65, 0.7), # You'll need to tweak the weights for your app
        #(20,  35, 120, 20, 0.3), # depending on how your robot is setup.
        #(20,   10, 120, 20, 0.1)
       ]

# Compute the weight divisor (we're computing this so you don't have to make weights add to 1).
weight_sum = 0
for r in ROIS: weight_sum += r[4] # r[4] is the roi weight.

# Camera setup...
sensor.reset() # Initialize the camera sensor.
sensor.set_pixformat(sensor.GRAYSCALE) # use grayscale.
sensor.set_framesize(sensor.QQVGA) # use QQVGA for speed.
sensor.skip_frames(time = 2000) # Let new settings take affect.
sensor.set_auto_gain(False) # must be turned off for color tracking
sensor.set_auto_whitebal(False) # must be turned off for color tracking
clock = time.clock() # Tracks FPS.
#sensor.set_auto_exposure(False,exposure_us=5000)
sensor.skip_frames(time = 500) # Let new settings take affect.
first_cx=0
middle_cx=0
while(True):
    clock.tick() # Track elapsed milliseconds between snapshots().
    img = sensor.snapshot() # Take a picture and return the image.
    img.gamma_corr(gamma=1.3,contrast=2.3,brightness=-0.5)
    img.crop(roi= mainROI)
    centroid_sum = 0
    num_found=0
    last_exist=False
    last_cx=0
    region_width=0
    for r in ROIS:
        blobs = img.find_blobs(GRAYSCALE_THRESHOLD, roi=r[0:4], merge=True,pixels_threshold=100) # r[0:4] is roi tuple.

        if blobs:
            # Find the blob with the most pixels.
            largest_blob = max(blobs, key=lambda b: b.pixels())
            print(largest_blob.w())
            region_width=largest_blob.w()
            # Draw a rect around the blob.
            img.draw_rectangle(largest_blob.rect(),color=0)
            img.draw_cross(largest_blob.cx(),
                           largest_blob.cy())
            num_found+=1
            centroid_sum += largest_blob.cx() * r[4] # r[4] is the roi weight.
            if r[4]==0.1:
                first_cx=largest_blob.cx()
            elif r[4]==0.3:
                middle_cx=largest_blob.cx()
            elif r[4]==0.7:
                last_cx=largest_blob.cx()
                last_exist=True


    center_pos = (centroid_sum / weight_sum) # Determine center of line.

    # Convert the center_pos to a deflection angle. We're using a non-linear
    # operation so that the response gets stronger the farther off the line we
    # are. Non-linear operations are good to use on the output of algorithms
    # like this to cause a response "trigger".
    deflection_angle = 0

    # The 80 is from half the X res, the 60 is from half the Y res. The
    # equation below is just computing the angle of a triangle where the
    # opposite side of the triangle is the deviation of the center position
    # from the center and the adjacent side is half the Y res. This limits
    # the angle output to around -45 to 45. (It's not quite -45 and 45).
    deflection_angle = -math.atan((center_pos-80)/41)

    # Convert angle in radians to degrees.
    deflection_angle = math.degrees(deflection_angle)

    #img.draw_rectangle(20, 60, 120, 20, color = 0, thickness = 2, fill = False)
    #img.draw_rectangle(20,  35, 120, 20, color = 0, thickness = 2, fill = False)
    img.draw_rectangle(20,   0, 120, 65, color = 0, thickness = 2, fill = False)
    # Now you have an angle telling you how much to turn the robot by which
    # incorporates the part of the line nearest to the robot and parts of
    # the line farther away from the robot for a better prediction.
    #print("Turn Angle: %d" % int(deflection_angle))
    if last_exist:
        print("Diff 1: " +str(last_cx-first_cx) )
    else:
        print("Diff 2: " +str(first_cx-80) )
    '''
    if(num_found>1) :
        uart.write(str(int(deflection_angle))+'\n')
        #uart.write("5\n")
        print("num:",num_found)
    else:
        uart.write("0\n")
    '''
    diff=(last_cx-80)
    '''
    if last_exist:
        uart.write(str((last_cx-first_cx))+'\n')
    else:
        uart.write(str((first_cx-80))+'\n')
    '''
    if region_width>60:
        uart.write(str(45)+'\n')
        print("Curve Detected!")
    elif region_width<30:
        uart.write(str(0)+'\n')
        print("Speed Up!")
    else:
        uart.write(str(20)+'\n')
        print("Normal Line")
    #uart.write(str(int(-deflection_angle))+'\n')
    #time.sleep_ms(100)
    #print(clock.fps()) # Note: Your OpenMV Cam runs about half as fast while
    # connected to your computer. The FPS should increase once disconnected.
