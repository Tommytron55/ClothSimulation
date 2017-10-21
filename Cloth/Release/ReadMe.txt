Thomas Male

A simple tech demo showing off techniques learnt in class.

The scene consists of a rotating quad, a Cube and a sheet of water. Scissor test is on by default which cuts the sides of your 
view of objects (the skybox will still render as normal). Antianilising is also on by default. All objects have Fog effects on them
which occur when the object moves too far away from the main camera.

-The Rotating quad shows that back face culling is enabled
-The Cube can be moved using controls. It also has a Red stencil outline showing stencil/depth testing.
-The sheet of water is static but shows opacity and blending when objects pass behind it.

controls:

Cube:
- W - Moves cube fowards 
- S - Moves cube Backwards
- A - Moves cube Left 
- D - Moves cube Right

Camera:
- I - Moves camera in direction you are facing 
- K - Moves camera backwards in direction you are facing 
- J - Moves camera left in direction you are facing 
- L - Moves camera right in direction you are facing 

Toggles:
- R - Resets Scene positions
- F - Enables/Disables Antianilising
- T - Enables/Disables Scissor Testing