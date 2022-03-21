Task 1 (done at home)
------
Drawing of the missile and the source code exploration.


Task 2 (TASK 1_2-Y) UPPER PART - color by triangles
------
Draw the upper part of the UFO by **glDrawArrays()**


2a) Prepare the upper part of the UFO to be drawn 

In data.h: 
* define the contents of the interleaved array (coordinates + colors)
  => array ufoVertices[]                                >>> TASK 1_2-1 <<< datah.h: 152

In render_stuff.cpp:
* initialize upper UFO geometry in function initUfoGeometry() >>> TASK 1_2-4 <<< render_stuff.cpp: 303

WHAT TO EDIT (overview):
 -> data.h: 152
 -> render_stuff.cpp: 303


2b) Draw the upper part of the UFO by glDrawArrays() command using 
    the interleaved array (coordinates, colors).

In render_stuff.cpp: to:
* draw UFO geometry in function drawUfo()
	- draw 3 yellow triangles                       >>> TASK 1_2-2 <<< render_stuff.cpp: 110
	- draw 3 magenta triangles                      >>> TASK 1_2-3 <<< render_stuff.cpp: 115

WHAT TO EDIT (overview):
 -> render_stuff.cpp: 110, 115


Task 3 (TASK 1_3-Y) BOTTOM PART - colors by vertices
------
Draw the bottom part of the UFO by **glDrawElements()** command using the
interleaved array (coordinates, colors).

In data.h:
* add contents of the interleaved array for the bottom part (coordinates + 
  colors)                                                >>> TASK 1_3-1 <<< data.h 162
* define contents of the element array that includes indices of all vertices 
  used for drawing                                       >>> TASK 1_3-2 <<< data.h 173

In render_stuff.cpp:
* initialize the element buffer object for bottom UFO geometry 
  in function initUfoGeometry() 					     >>> TASK 1_3-4 <<< render_stuff.cpp: 307
* draw UFO bottom part's geometry in function drawUfo()  >>> TASK 1_3-3 <<< render_stuff.cpp: 121

WHAT TO EDIT (overview):
TASK 3:
 -> render_stuff.cpp: 121, 307
 -> data.h: 162, 173

Improvement
-----------
Use GL_TRIANGLE_FAN instead of GL_TRIANGLES


Notes:
------
For each task write your code between the distinguised lines, as seen below.
    // ======== BEGIN OF SOLUTION - TASK 1_X-Y ======== //
    <<< YOUR CODE >>>
    // ========  END OF SOLUTION - TASK 1_X-Y  ======== //