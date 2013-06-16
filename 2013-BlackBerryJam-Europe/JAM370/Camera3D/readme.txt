This is a proof-of-concept test to experiment with getting the camera viewfinder rendered onto an opengl cube.
my disclaimer is that I have never used OpenGL before in my life :)

It sort of works, but has some problems...
-cube textures are improperly mapped with respect to the cube normals, etc.  this results in the texture being mapped as mirrored triangles instead of continuous squares.
-colour space is mismatched.
-I have to do a memcpy() which is suboptimal, and wastes cpu cycles because GL textures do not support strided bitmaps.
-This can probably be optimized by using EglImage according to Sean Paul Taylor
-Also, seems like this only works on Dev Alpha B, as it is trying to use a 720x720 resolution, which is not supported on Dev Alpha A.  Change video vf resolution to 480x640 or some other supported resolution if using that target.
