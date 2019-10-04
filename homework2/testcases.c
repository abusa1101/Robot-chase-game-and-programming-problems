//TESTCASES
// In this first task, and the first test case for this problem, we need to
// construct a valid Microsoft bitmap image.
DONE
> Comment out full rasterize polygon code basically;

// The second test case for this problem is to draw a white line from (10, 10) to
// (200, 200) on a black background of size 640 by 480
DONE
>Comment out all except this:
>>bresenham(10,10,200,200, &bmp, color);

// The third test case for this problem is to draw a 4 by 4 pixel white square
// centered at (0, 0).
NOT DONE?

> ROUND OFF PROBLEM 

// The fourth test case is to draw a 4 by 4 pixel white square centered at (2, 2).
// Now the whole square should be visible.


// The fifth test case is to draw a 5 by 5 pixel white square centered at (2, 2).


// The whole square should also be visible. If you have trouble with this test,
// look back at how to do the polygon rounding again.


// The sixth test case is to draw a 600 (width – 40) by 440 (height – 40) white
// border centered at the center of the image (640 / 2, 480 / 2). The border’s top
// left pixel will be at (20, 20) and the bottom right corner at (619, 459).
