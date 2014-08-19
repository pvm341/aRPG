
Project Description
=====
A text based console RPG that is heavily influenced by MUDs.

License
=====
MIT License. Copyright 2014 Paul Massey.

Instructions
=====

### MS Windows
~~~~~~~~~~
You will need to use codeblocks or some other IDE.
~~~~~~~~~~

### gcc method
~~~~~~~~~~
$ make
$ ./aRPG
~~~~~~~~~~~~~~~~~

### codeblocks method

1. Install codeblocks
2. It will automatically detect if you have gcc, and if you have g++
3. File -> Open the cbp in the source folder


Commands
=====

| Movement Commands | Parameters  | Description |
| ------------- | ----------- | ----------- |
| n             |             | Go North    |
| e             |             | Go East     |
| s             |             | Go South    |
| w             |             | Go West     |
| u             |             | Go Up       |
| d             |             | Go Down     |

| Interaction Commands | Parameters | Description |
| ------------- | ----------- | ----------- |
| look          |             | Look at room |
| look          | object    | Look at target |
| look          | direction | Look at direction |
| get           | object    | Get the object |
| inv           |             | List inventory |
| drop          | object    | Drop the object |
| sleep         |             | Fall asleep |
| sleep         | time      | Fall asleep until time |

| Game Commands | Parameters | Description |
| ------------- | ----------- | ----------- |
| quit          |             | Quit the game|
| help          |             | List all commands |
| ?             |             | Same as help |
| save          | filename  | Save the gamestate as ... |
| load          | filename  | Load the gamestate | 

| Speedwalk     | Parameters  | Description |
| ------------- | ----------- | ----------- |
| 4n            |             | Go 4 North  |
| 2e4sw         |             | Go 2 East, 4 South, 1 West     |
| s;w;4s        |             | Go South, West, then 4 South    |
| look east; n10wd; scan d;              |             | Look east, go north, then 10 west, then down, then scan down     |

So, you need to use semicolon notation in order to handle command parameters.

Authors
=====
* Paul Massey (https://github.com/pvm341)
* Bertrand Brompton (https://github.com/angelofmystra)

How to contribute
=====

If you discover any bugs, feel free to create an issue on GitHub. Please add as much information as possible to help us fixing the possible bug. We also encourage you to help even more by forking and sending us a pull request.

Contact
=====



History
=====

Troubleshooting
=====

Known Bugs
=====

There are no known bugs!

