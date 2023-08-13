# Ocean Cleanup Game

Swim around the ocean, catch fish, remove rubbish and make sure you dont drown!

Demo Video

## Running

Download the latest release or compile the program using visual studio. Then run the exe located within the build folders.

## Idea

The idea for the game came from my interest in the enviroment and keeping our world clean. As such i thought a game like this would prove a good tool to engage people with the idea of how our waste ends up in the sea.

## Structure

The code base is split into a couple key parts, each with a specialisation that draws upon other elements.

### Window & Render

Code to manage the creation of the GL window, along with loading and rendering assets. The code here is abstracted away from most of the GL work going on behind the scenese. This enables a less cluttered code base.

### Asset & Mesh

Containing the actual code for the loading and drawing of assets. We break the process into multiple steps, loading, building and rendering. This allows for each step to be performed and then any alterations to be made.

In here is the majority of our heavy lifiting of GL functions.

### Game

Here we have code related to user input and actions in the world. This includes moving fish, collecting items and highlighting assets.

### Additional Libraries

[simdjson/simdjson: Parsing gigabytes of JSON per second](https://github.com/simdjson/simdjson) - Used to Parse GLTF JSON at a high rate.




