# Evaluation Guide

## Favorite Model Tests

1) L803, tests the arch function for a simple case with the original 
initial velocity of the ball.

2) L818, tests both inside and inside_portal functions for a few simple cases.
These functions play an important role in the game.

3) L828, tests ball_hits_ type of functions for a few simple cases.

4) L842, test catcher_hits_ type of functions for a few simple cases. 
Although similar to ball_hits_, these functions have additional stuff in 
them that need testing.

5) L856, tests the update function, particularly checks if the score
increases only when the ball first enters the area of the catcher.

## Functional Requirement Hints

1) It can be tested easily by pressing w, a, s, d keys 
when the game is running.

2) The slowing motion caused by the friction and the fact that catcher 
has a max speed can be observed by controlling it during the game. b value 
can be increased inside model.cxx so that the max speed of the catcher is 
lower. I made the catcher faster than what I mentioned in my proposal to
make the testing easier.

3) It can be observed by playing the game.

4) The zones are distributed so that it's hard to predict where 
they are placed. But it's possible to observe that the ball sometimes
follows a straight line and sometimes moves in an arch. The randomness 
of the radius also can be observed from playing the game, but to make sure,
you can look at L681.  

5) It can be observed by playing the game. I changed 2 times
 to 1.5 times but that doesn't make a big difference. Also the max_score
 is set to 4 for testing.
 
6) This can be observed by playing the game. I implemented an additional
function that changes the color value of the catcher inside the model
depending on whether the ball is inside or not. This makes it easy to
detect when the ball is inside. Also can be seen from the unit test 
L866 - L869.

7) This can be observed by playing the game. It might take up to 
a minute before the ball hits a portal. The randomness of the exit
portal can be both observed from the game, but also can look at L683.

8) This can be observed very clearly by trying to pass through
 the portals in different ways. 
 
9) The time is displayed on the right top corner of the screen.

10) Additionally, when the game is over, it can be restarted by
pressing the space bar key.

## Possible bugs

I came across 2 bugs while testing. The first one is if you hold down
the control keys during the game over screen and restart the game,
when you release the keys, catcher starts to move by itself. The second 
one is very unlikely to happen but one time the catcher got stuck in one
of the portals. I couldn't make it do it again.
