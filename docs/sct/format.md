[<- Back to the Docs](../index.md)

# The .sct format (from a binary standpoint)
The .sct file format is a level scripting file format designed for the 1999 game '[Silver](https://en.wikipedia.org/wiki/Silver_(video_game))'.

These files typically contain information about characters, items, game events and game variables.
An sct file is mainly composed of a code section and a data section that together tell the game's engine what game functions to call and when to call them, all interpreted by the game's engine in real time.

Here's veranda.sct file if you want to follow along:
[veranda.sct](./veranda.sct) file, you can download.

### Making sense of the .sct binary file
The information presented here is my findings upon investigating the .sct file format (mainly using a Hex editor), trying to make sense of it by guessing, testing and staring at it. Please be aware that some parts are likely to be inaccurate or plain wrong.

In this document, Let us discuss the following:

1. [The binary file structure](#the-binary-file-structure)
2. [Predefined execution rules](#predefined-execution-rules)

## The binary file structure
As an example I'll use the file veranda.sct to describe an .sct's general structure.
A binary .sct file is structured into 5 sections in the following order: (top to bottom)

_(the .sct file presented is read using big endian byte order)_

1. [Header](./header.md)
2. [Code section (scripts)](./code.md)
3. [Scripts offset table](script.md)
4. [Data section](data.md)
5. [Link table](link.md)


## Predefined execution rules
As far as I currently understand it, these rules apply to all .sct files in the game, but are mostly relevant to the level scripts _(meaning the .sct files inside the levels folder having a level's name)_
- first script from the top is the main script, meaning it's the first one to be executed.
- the second script from the top is always executed as an infinite loop.
- the third script from the top is executed when the player exit the room.