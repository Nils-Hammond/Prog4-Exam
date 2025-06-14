# Source Control

Github Link: https://github.com/Nils-Hammond/Prog4-Exam

My Engine and Game contain a lot of potentially questionable design choices that I ended up making due to time constraints on account of starting work on my assignment too late. Hopefully there's nothing too egregious.

# Engine

While my work is not the most consistent about when to apply physics, I opted to have a collision system as a service.
I prefered using a service locator pattern over a singleton because I already had a service locator for my Audio, but also because I thought it would be useful for if I had time to implement more robust collision systems.
The only concrete collision system implemented currently is one that does a simple check between every collider, but if I had the time I would've liked to try making one that checks collisions using spatial partitioning, even though that isn't necessary for this scale of project.
Having the collision system as a service could also allow to swap between a 3D collision system instead of 2D as well which is cool.

The collision systems all rely on a collider component to simulate collisions with. The colliders themselves have allow for registration with observers. So while collisions (should normally be) done completely seperate from their owning GameObject, any component part of the same GameObject can fetch the collider and register themselves with it to react to collisions.
Colliders also have unique tags for more specific collision handling, created using the same handy sdbm hashing function used for event IDs.

Events in the engine are entirely handled using an observer pattern, with subjects being held as a unique pointer in whatever needs to be observed. Events are sent with a unique ID, along with some event specific data given as an std::any. Observers can therefore use the event ID to determine what to cast the data to, and this keeps specific events completely detached from the engine and entirely user defined.
I can't say I'm 100% sure of the validity of using std::any to send event data, but it's safer than using void pointers which would have been my second choice.

The audio system that I have implemented uses SDL_Mixer since all the other 3rd party implementations also use SDL. It uses an event queue to load up and play sounds, but when pausing or stopping sounds it does it immediately.
I figured this is ok since these other functions don't need to check if a soundfile is valid, if it's already loaded, if it needs to be loaded and all that, but it does mean that you can't play a sound and immediately try to pause it as if you're queueing it up for later as the sound will end up actually being played after the call to pause.

I left the scene/sceneManager mostly as they were in the default Minigin, along with the vector of shared pointers. This is partly due to laziness in trying to resolve undefined class issues when switching to unique pointers but also, while this project doesn't really call for it, there is a case to be made for objects that can persist between scenes.
