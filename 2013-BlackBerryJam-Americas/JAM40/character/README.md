GamePlay3D Character sample

This sample is a modified version of the standard "Character" sample which ships with the GamePlay3D repo.  It has been modified to add a new textured element to the scene at runtime which has the front-facing camera viewfinder applied to it.  This illustrates how the camera's RGB output mode can be used as an in-game texture.  Camera life-cycle and complete best-practices are not implemented here for the sake of clarity.

The approach taken is:
* create a new rectangle in the scene
* create a new texture for the rectangle (note: we need to use BGRA textures, which is the native h/w format.  performance is impacted otherwise)
* open the camera and configure the video viewfinder for RGB mode at a moderate resolution (higher res = lower performance)
* configure the camera viewfinder event to send a pulse to the app each time a new frame is available
* on a separate thread, catch these viewfinder event pulses and read the latest frame from the camera, and queue it up for the rendering pass.
* when the rendering pass happens, upload the new texture to the rectangle
