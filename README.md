# LUINT
Lua machine environment written in C++

LUINT is an environment where you can place "machines" that connect with one another. You can play with them and do whatever you want: Maybe an operating system written entirely in Lua, a complex network or anything else you can imagine of.

## Basic Concepts
Machines always are inside *networks*. When networks have more than one machine in them, machines from within the same network can send information to each other via **packets**. Packets are containers of data that have a name and a machine ID assigned to them.

Processing Units work with *events*. Events are, literally, exactly equal to packets. The only difference between "events" and "packets" is that when talking of events we're normally referring to data that the Processing Unit sends to itself or data from another machine that was sent without a packet to receive it (Data that was sent without asking, basically).

Processing Units run at around 20 ticks per second, which means that they have to "sleep" or yield 20 ticks per second. They can't execute continuously: They have to allow for the rest of the program and machines to work. If a Processing Unit takes too much to yield, it might freeze the program (or at least, for now).

## Machines
As of version 0.1, there are 5 machines available:
- **LED**: One of the most basic ones. Turns on or off if it senses a `set_state` packet. Can be used to debug, or just to decorate.
- **Button**: This one sends a `button_pressed` packet across the network when it is pushed, and a `button_released` packet when it is released.
- **PTM Monitor**: Stands for "Persistent Text Monitor". Allows displaying text to it, and nothing else. The text *persists* when the machine using it turns off (kind of like those e-paper displays in e-books).
- **Lua Processing Unit**: The most complex one by far. Allows programming it with Lua. Can interact with other machines using its `computer.send_packet` function.

## Planned Features
- **Keyboard receivers** that relay any keyboard input as `key_pressed` and `key_released` packets.
- **Filesystems** that allow for saving/loading data inside the virtual environment.
- **Sound Output Devices** that allow creating sounds, music, etc.
- **A tutorial inside the environment** or at least an about window that explains everything.

## Contributing
You can help the most opening issues that haven't been found yet. However, if you know C++ and Lua, you can also help me fix any of the issues available.
