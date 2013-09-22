#include "utility.h"
#include "config.h"

bool commandHelp(texture l = texture()){
	if(l.empty()){
		printf(R"(
Type 'help' to see this list.
Type 'help <command>' to find out more about COMMAND.
Type 'help var' to find out more about variables.
Type 'help var <variable>' to find out more about VARIABLE.

M = available in main menu
G = available in-game
O = available as one-letter command on O-hello's turns

hello     M     - say hello to program
speed     M     - test speed of computer
load      M     - load game from file
help      M G   - view this text
menu      M G O - go to main menu
quit      M G O - exit program
new         G O - restart game using current settings
save        G O - save game in file
undo        G   - undo move
redo        G   - redo move
reflect     G   - reflect board (horizontally)
fsearch     G   - fixed depth search
tsearch     G   - time limit search
endsearch   G   - end-game search, gives exact score
wldsearch   G   - end-game search, gives win/loss/draw value
set       M G   - set variable
show      M G   - show variable
reset     M G   - reset variable to default

)");
		return true;
	}
	auto x = l.read();
	if(x == "hello" and l.empty()){
		printf(R"(
hello: hello
    Available in main menu.
    Say hello to the program. The program will respond with a message.

)");
        return true;
    }
    if(x == "speed" and l.empty()){
        printf(R"(
speed: speed
    Available in main menu.
    Test the speed of the computer from the number of loops it can execute in
    one second.

)");
        return true;
    }
    if(x == "load" and l.empty()){
        printf(R"(
load: load <filename>
    Available in main menu.
    Load the game from the file FILENAME.ohl
    See also 'save'

)");
        return true;
    }
    if(x == "help" and l.empty()){
        printf(R"(
help: help [<command>]
      help var [<variable>]
    Available in main menu and in-game.
    Display helpful information about commands. If COMMAND is specified,
    display information about COMMAND. 'help var' displays information about
    variables. If VARIABLE is specified, display information about VARIABLE.

)");
        return true;
    }
    if(x == "menu" and l.empty()){
        printf(R"(
menu: menu
    Available in main menu, in-game, and on O-hello's turns.
    Go to main menu.

)");
        return true;
    }
    if(x == "quit" and l.empty()){
        printf(R"(
quit: quit
    Available in main menu, in-game, and on O-hello's turns.
    Exit the program.

)");
        return true;
    }
    if(x == "new" and l.empty()){
        printf(R"(
new: new
    Available in-game and on O-hello's turns.
    Restart the game using current settings and reset undo data. If the game
    was loaded from file, the game will be reloaded. Otherwise, the game will
    restart from the initial board postition.
    See also 'undo'

)");
        return true;
    }
    if(x == "save" and l.empty()){
        printf(R"(
save: save <filename>
    Available in-game and on O-hello's turns.
    Save the current game state in the file FILENAME.ohl
    See also 'load'

)");
        return true;
    }
    if(x == "undo" and l.empty()){
        printf(R"(
undo: undo [<number>]
      undo all
    Available in-game.
    Undo human's moves (excluding passes) for NUMBER times. If not specified,
    NUMBER is set to 1. 'undo all' undoes every move since the beginning of the
    game.
    See also 'new', 'redo'

)");
        return true;
    }
    if(x == "redo" and l.empty()){
        printf(R"(
redo: redo [<number>]
      redo all
    Available in-game.
    Redo human's moves (excluding passes) for NUMBER times. If not specified,
    NUMBER is set to 1. 'redo all' restores the game to its furthest position.
    See also 'undo'

)");
        return true;
    }
    if(x == "reflect" and l.empty()){
        printf(R"(
reflect: reflect
    Available in-game.
    Reflect the board horizontally.

)");
        return true;
    }
    if(x == "fsearch" and l.empty()){
        printf(R"(
fsearch: fsearch <depth>
    Available in-game.
    Execute a fixed depth search using depth DEPTH (DEPTH is a positive integer)
    Return the search position, score and statistics.
    See also 'tsearch', 'endsearch', 'wldsearch'

)");
        return true;
    }
    if(x == "tsearch" and l.empty()){
        printf(R"(
tsearch: tsearch <time>
    Available in-game.
    Execute a time limit search using limit TIME second(s) (TIME is a positive
    number). Return the search position, score and statistics.
    See also 'fsearch'

)");
        return true;
    }
    if(x == "endsearch" and l.empty()){
            printf(R"(
endsearch: endsearch
    Available in-game.
    Execute a search until end-game positions and return the exact score.
    Currently, its speed is no different that that of fsearch. Return the search
    position, score and statistics.
    See also 'fsearch', 'wldsearch'

)");
        return true;
    }
    if(x == "wldsearch" and l.empty()){
            printf(R"(
wldsearch: wldsearch <depth>
    Available in-game.
    Execute a search until end-game positions and return the win/loss/draw
    value. It first executes a normal search (fsearch) to depth DEPTH before
    attempting win/loss/draw search. This guarantees that the result will not be
    worse than using fsearch with depth DEPTH. Return the search position, score
    and statistics.
    See also 'fsearch, 'endsearch'

)");
        return true;
    }
    if(x == "set" and l.empty()){
        printf(R"(
set: set <variable> [<value>]
    Available in main menu and in-game.
    Set VARIABLE to have the value VALUE. Some values cannot be assigned
    directly. In such cases, 'set <variable>' must be used instead, and you
    will be asked to input VALUE later.
    Type 'help var' to find out more about variables.
    See also 'show', 'reset'

)");
        return true;
    }
    if(x == "show" and l.empty()){
        printf(R"(
show: show <variable>
      show all
    Available in main menu and in-game.
    Display the current value of VARIABLE. 'show all' displays current values
    of all variables.
    Type 'help var' to find out more about variables.
    See also 'set', 'reset'

)");
        return true;
    }
    if(x == "reset" and l.empty()){
        printf(R"(
reset: reset <variable>
       reset all
    Available in main menu and in-game.
    Reset VARIABLE to its default value. 'reset all' resets all variables.
    Type 'help var' to find out more about variables.
    See also 'set', 'show'

)");
        return true;
    }
    if(x == "var"){
        if(l.empty()){
            std::string typeName[] = {"int", "bool", "float", "vint", "texture"};
            printf(R"(
Type 'help var <variable>' to find out more about VARIABLE.

int     : value = integer
bool    : value = on, off
float   : value = real number
vint    : value = multiple integers
texture : value = row x column characters

)");
            printf("%10s %7s   %s\n", "VARIABLE", "TYPE", "DESCRIPTION");
            for(auto var : setting){
                printf("%10s %7s - %s\n", var.first.c_str(), typeName[var.second.type].c_str(), var.second.desc.c_str());
            }
            printf("\n");
            return true;
        }
        x = l.read();
        if(x == "black" and l.empty()){
            printf(R"(
variable: black
type    : texture
    Appearance of black disk. Black always goes first in Othello.
    See also 'white'

)");
            return true;
        }
        if(x == "white" and l.empty()){
            printf(R"(
variable: white
type    : texture
    Appearance of white disk. White always goes second in Othello.
    See also 'black'

)");
            return true;
        }
        if(x == "rotate" and l.empty()){
            printf(R"(
variable: rotate
type    : bool
    Enable (on) or disable (off) the rotation effect. Rotation effect is
    displayed when O-hello is thinking and is displayed every ROTATETIME nodes.
    See also 'rotatetime'

)");
            return true;
        }
        if(x == "rotatetime" and l.empty()){
            printf(R"(
variable: rotatetime
type    : int
    The number of nodes between each display of rotation effect. Rotation effect
    is displayed when O-hello is thinking and can be enabled or disabled
    according to value of ROTATE.
    See also 'rotate'

)");
            return true;
        }
        if(x == "flip" and l.empty()){
            printf(R"(
variable: flip
type    : bool
    Enable (on) or diable (off) disk flipping display. Each frame of disk
    flipping is FLIPTIME second(s) apart and appearance is according to value
    of FLIPLOOK.
    See also 'fliptime', 'fliplook'

)");
            return true;
        }
        if(x == "fliptime" and l.empty()){
            printf(R"(
variable: fliptime
type    : float
    The amount of time (sec) between each frame of disk flipping. Disk flipping
    display can be enabled or disabled according to value of FLIP and appearance
    is according to value of FLIPLOOK.
    See also 'flip', 'fliplook'

)");
            return true;
        }
        if(x == "fliplook" and l.empty()){
            printf(R"(
variable: fliplook
type    : texture
    Appearance of flipping disk. Disk flipping display can be enabled or
    disabled according to value of FLIP and each frame of disk flipping is
    FLIPTIME second(s) apart.
    See also 'flip', 'fliptime'

)");
            return true;
        }
        if(x == "move" and l.empty()){
            printf(R"(
variable: move
type    : bool
    Enable (on) or disable (off) valid move display for current player.
    Appearance of valid move indicator is according to value of MOVELOOK.
    See also 'movelook'

)");
            return true;
        }
        if(x == "movelook" and l.empty()){
            printf(R"(
variable: movelook
type    : texture
    Appearance of valid move indicator. Valid move display can be enabled or
    disabled according to value of MOVE.
    See also 'move'

)");
            return true;
        }
        if(x == "rand" and l.empty()){
            printf(R"(
variable: rand
type    : bool
    If there is more than one move that O-hello finds equally good, ON will
    select one of these moves at random, while OFF will select the first move.
    Currently, this effect can only be seen in the opening moves. This is
    because the current mechanism of search functions can only find one move
    with the best score.
    See also 'parallel'

)");
            return true;
        }
        if(x == "parallel" and l.empty()){
            printf(R"(
variable: parallel
type    : bool
    Allow (on) or not allow (off) O-hello to play the parallel opening.
    If ON, O-hello can play diagonal, perpendicular, and parallel opening.
    If OFF, O-hello can play only diagonal and perpendicular opening.
    O-hello considers all openings equally good.
    See also 'rand'

)");
            return true;
        }
        if(x == "raw" and l.empty()){
            printf(R"(
variable: raw
type    : bool
    Use RAW MODE (on) or DEFAULT MODE (off) to configure O-hello levels.
    In RAW MODE, user can select between mode 0 (random), 1 (fsearch), and 2
    (tsearch), and can input any value for depth and depthperfect. RAW MODE is
    more flexible than DEFAULT mode.
    In DEFAULT MODE, user can select between only mode 1 (fsearch) and 2
    (tsearch), and has to choose from specific values of depth, depthperfect,
    and depthwld.
    DEFAULT MODE is more user-friendly than raw mode.

)");
            return true;
        }
        if(x == "weight" and l.empty()){
            printf(R"(
variable: weight
type    : vint
    Weights of the evaluation function. Each component is as follows:
    wd   - disk difference
    wm   - mobiltiy difference
    wp   - potential mobility difference
    wc   - corner difference
    wxx  - x square difference (adjacent to empty corner)
    wcc  - c square difference (adjacent to empty corner)
    we   - edge configuration
    ws   - stable disk difference
    wcut - cutoff (no. of disks in board) for using end-game weights
    wd1  - \
    wm1  - |
    wp1  - |
    wc1  - | end-game counterparts of above
    wxx1 - |
    wcc1 - |
    we1  - |
    ws1  - /
    wnew - use (1) or not use (0) new csquare
    wf   - final disk difference (game ends)

)");
            return true;
        }
    }
    return false;
}
