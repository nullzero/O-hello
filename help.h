#include "utility.h"
#include "config.h"

void commandHelp(texture l){
	if(l.empty()){
		printf(R"(
Type 'help' to see this list.
Type 'help <command>' to find out more about COMMAND.
Type 'help variable' to find out more about variables.

M = available in main menu
G = available in-game
O = available as one-letter command on O-hello's turns

hello   M     - say hello to program
speed   M     - test speed of computer
load    M     - load game from file
help    M G   - view this text
menu    M G O - go to main menu
quit    M G O - exit program
new       G O - restart game using current settings
save      G O - save game in file
undo      G   - undo move
redo      G   - redo move
reflect   G   - reflect board (horizontally)
fsearch   G   - fixed depth search
tsearch   G   - time limit search
set     M G   - set variable
show    M G   - show variable
reset   M G   - reset variable to default

)");
		return;
	}
	auto x = l.read();
	if(x == "hello" and l.empty()){
		printf(R"(
hello: hello
    Available in main menu.
    Say hello to the program. The program will respond with a message.

)");
        return;
    }
    if(x == "speed" and l.empty()){
        printf(R"(
speed: speed
    Available in main menu.
    Test the speed of the computer from the number of loops it can execute in
    one second.

)");
        return;
    }
    if(x == "load" and l.empty()){
        printf(R"(
load: load <filename>
    Available in main menu.
    Load the game from FILENAME. FILENAME refers to the file FILENAME.ohl which
    must be in the same folder as the program.

)");
        return;
    }
    if(x == "help" and l.empty()){
        printf(R"(
help: help [<command>]
      help variable
    Available in main menu and in-game.
    Display helpful information about commands. If COMMAND is specified,
    display information about the command COMMAND. 'help variable' displays
    information about variables in the program.

)");
        return;
    }
    if(x == "menu" and l.empty()){
        printf(R"(
menu: menu
    Available in main menu, in-game, and on O-hello's turns.
    Go to main menu.

)");
        return;
    }
    if(x == "quit" and l.empty()){
        printf(R"(
quit: quit
    Available in main menu, in-game, and on O-hello's turns.
    Exit the program.

)");
        return;
    }
    if(x == "new" and l.empty()){
        printf(R"(
new: new
    Available in-game and on O-hello's turns.
    Restart the game using current settings. If the game is loaded from file,
    the game will be reloaded. Otherwise, the game will restart from the
    initial board but with same settings.

)");
        return;
    }
    if(x == "save" and l.empty()){
        printf(R"(
save: save <filename>
    Available in-game and on O-hello's turns.
    Save the current game state in the file FILENAME.ohl. The file will be
    created in the same folder as the program.

)");
        return;
    }
    if(x == "undo" and l.empty()){
        printf(R"(
undo: undo [<number>]
      undo all
    Available in-game.
    Undo human's moves (excluding passes) for NUMBER times. If not specified,
    NUMBER is set to 1. 'undo all' restarts the game in the same manner as the
    command 'new'.

)");
        return;
    }
    if(x == "redo" and l.empty()){
        printf(R"(
redo: redo [<number>]
      redo all
    Available in-game.
    Redo human's moves (excluding passes) for NUMBER times. If not specified,
    NUMBER is set to 1. 'redo all' restores the game to its furthest position.

)");
        return;
    }
    if(x == "reflect" and l.empty()){
        printf(R"(
reflect: reflect
    Available in-game.
    Reflect the board horizontally.

)");
        return;
    }
    if(x == "fsearch" and l.empty()){
        printf(R"(
fsearch: fsearch <number>
    Available in-game.
    Execute a fixed depth search using depth NUMBER (NUMBER is an integer at
    least 1). Return the search position, score and statistics. 

)");
        return;
    }
    if(x == "tsearch" and l.empty()){
        printf(R"(
tsearch: tsearch <time>
    Available in-game.
    Execute a time limit search using limit TIME second(s) (TIME is a number
    greater than 0). Return the search position, score and statistics. 

)");
        return;
    }
    if(x == "set" and l.empty()){
        printf(R"(
set: set <variable> [<value>]
    Available in main menu and in-game.
    Set VARIABLE to have the value VALUE. Some values cannot be assigned
    directly. In such cases, 'set <variable>' must be used instead, and you
    will be asked to input VALUE later.
                
    Type 'help variable' to find out more about variables.
)");
        return;
    }
    if(x == "show" and l.empty()){
        printf(R"(
show: show <variable>
      show all
    Available in main menu and in-game.
    Display the current value of VARIABLE. 'show all' displays current values
    of all variables.

    Type 'help variable' to find out more about variables.
)");
        return;
    }
    if(x == "reset" and l.empty()){
        printf(R"(
reset: reset <variable>
       reset all
    Available in main menu and in-game.
    Reset VARIABLE to its default value.
    
    Type 'help variable' to find out more about variables.
)");
        return;
    }
    
    if(x == "variable" and l.empty()){
        for(auto var : setting){
            printf("%10s %10d %10s\n", var.first.c_str(), var.second.type, var.second.desc.c_str());
        }
    }    
}
