FaceFilter sample

This sample is written in C++ and uses the Native C Camera API and libscreen to implement a video recorder which uses read/write filter access to the camera buffers to implement a blur effect over top of the first face detected in the scene.
The blur effect will appear in the recorded video.
Event-mode access to the buffers is shown, since event-mode is the only interface for read/write access.  The usual callback mechanism or event-based mode can be used for access to read-only buffers.
I'm also showing how to query supported viewfinder resolutions in this sample and select an appropriate size for the task.
Since this is mainly a visual demo, I am aiming for a 720p resolution, as 1080p is downsized to fit on the screen anyways.
Also, registering for resource management notification is demonstrated as well as how to clean-up when the notification arrives.
