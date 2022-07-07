// TODO: Make this readme better.

# notOsu

Another open source clone of the popular circle clicking rhythm game, Osu!
Written in C++, it tries to offer high performance and all QOL features you could think of. Written with hindsight in
mind and designed to be expandable, notOsu is the perfect platform for not only playing, but experiments too.

## Building

Simply build with CMake.

### Commands:

```
mkdir build
cd build
cmake ..
```

### Dependencies:

```
GLFW3
OpenGL
```

## How to play

Just like the game it's cloning, the main point of notOsu is to click circles which appear on you screen at the right
time to the rhythm of music. There are three types of gameplay objects.

The simplest of these are the humble notes. They appear as circles on screen To hit a note, simply move your cursor over
it and press either mouse buttons or the keys ```X``` or ```Z```. You will know when to press by looking at the approach
circle. This circle will appear around the note and will close down on it.

If you manage to hit one successfully, a rating will appear telling you how close you were. You will also gain some
points. The more on time you are, the more points and accuracy.

Another type of object is the slider. Sliders are rather complicated. They appear as notes followed by a trail. To
successfully play a slider, you must press down same as you would do with a note, however the circle will begin moving.
You must then follow it with your cursor whilst still holding down. Only after the slider completes may you release the
button. Note that even if you miss the starting circle, you can still hold your button while tracking it with your
cursor to earn some points.

The last type of object in the game is the spinner. They appear as large circles in the middle of your screen. Once a
spinner appears, you must press down on any button and spin your cursor around the center of the spinner as fast as you
can! After a while, the spinner will fill up, and you may stop spinning, you will be notified of this. However, if you
keep spinning you may get some bonus points.

There are two main scoring systems in the game. The point score, which is displayed as a number and a accuracy score,
displayed as a percentage. The point score is the sum of all points you've gained while playing a map. You may earn
these points by successfully completing any of the gameplay elements. NotOsu is a combo game, try to hit long streaks of
accurate hits. Each element you play successfully increases a score multiplier. The accuracy score is a percentage. It
is the average accuracy you've shown throughout the match. These two scores are very intertwined, as higher accuracy
means more points! 
