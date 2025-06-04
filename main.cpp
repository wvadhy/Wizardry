
#include <iostream>
#include <iomanip>
#include <memory>
#include <iterator>
#include <vector>
#include <forward_list>
#include "Room.h"
#include "wordwrap.h"
#include "State.h"
#include "strings.h"


using std::string;
using std::unique_ptr;

string commandBuffer;
State *currentState;

/**
 * Print out the command prompt then read a command into the provided string buffer.
 * @param buffer Pointer to the string buffer to use.
 */
void inputCommand(string *buffer) {
    buffer->clear();
    std::cout << "> ";
    std::getline(std::cin, *buffer);
}

/**
 * Sets up the map.
 */
void initRooms() {

    auto* room_1 = new Room(&r1name, &r1desc);
    auto* room_2 = new Room(&r2name, &r2desc);
    auto* room_3 = new Room(&r3name, &r3desc);
    auto* room_4 = new Room(&r4name, &r4desc);
    auto* room_5 = new Room(&r5name, &r5desc);
    auto* room_6 = new Room(&r6name, &r6desc);

}

/**
 * Sets up the game state.
 */
void initState() {
    currentState = new State(Room::rooms.front());
}

/**
 * std::string to int conversions to allow for the use of switch cases.
*/
 constexpr unsigned int str2int(const char* str, int h = 0)
{
    return !str[h] ? 5381 : (str2int(str, h+1) * 33) ^ str[h];
}


/**
 * The main game loop.
 */
void gameLoop() {
    bool gameOver=false;
    std::vector <string> directions {"north", "n", "south", "s", "east", "e", "west", "w"};
    while (!gameOver) {
        /* Ask for a command. */
        bool commandOk = false;
        inputCommand(&commandBuffer);
        /* The first word of a command would normally be the verb. The first word is the text before the first
         * space, or if there is no space, the whole string. */
        auto endOfVerb = static_cast<uint8_t>(commandBuffer.find(' '));

        /* We could copy the verb to another string but there's no reason to, we'll just compare it in place. */
        /* Command to go north. */
        string direction = commandBuffer;
        bool valid = (std::find(directions.begin(), directions.end(), direction) != directions.end());
        if (valid) {
            commandOk = true; /* Confirm command has been handled */
            Room *room;
            if (direction == "north" || direction == "n")
                room = currentState->getCurrentRoom()->getNorth();
            else if (direction == "south" || direction == "s")
                room = currentState->getCurrentRoom()->getSouth();
            else if (direction == "east" || direction == "e")
                room = currentState->getCurrentRoom()->getSouth();
            else
                room = currentState->getCurrentRoom()->getWest();

            if (room == nullptr) { /* there isn't */
                wrapOut(&badExit);      /* Output the "can't go there" message */
                wrapEndPara();
            } else {                    /* There is */
                currentState->goTo(room); /* Update state to that room - this will also describe it */
            }
        }

//        if ((commandBuffer.compare(0, endOfVerb, "east") == 0) or (commandBuffer.compare(0,  endOfVerb, "e") == 0))
//        {
//            commandOk = true;
//            Room *eastRoom = currentState->getCurrentRoom()->getEast();
//            if(eastRoom == nullptr)
//            {
//                wrapOut(&badExit);
//                wrapEndPara();
//            }
//            else
//            {
//                currentState->goTo(eastRoom);
//            }
//        }
//
//
//        if ((commandBuffer.compare(0, endOfVerb, "west") == 0) or (commandBuffer.compare(0,  endOfVerb, "w") == 0))
//        {
//            commandOk = true;
//            Room *westRoom = currentState->getCurrentRoom()->getWest();
//            if(westRoom == nullptr)
//            {
//                wrapOut(&badExit);
//                wrapEndPara();
//            }
//            else
//            {
//                currentState->goTo(westRoom);
//            }
//        }
//
//        if ((commandBuffer.compare(0, endOfVerb, "south") == 0) or (commandBuffer.compare(0,  endOfVerb, "s") == 0))
//        {
//            commandOk = true;
//            Room *southRoom = currentState->getCurrentRoom()->getSouth();
//            if(southRoom == nullptr)
//            {
//                wrapOut(&badExit);
//                wrapEndPara();
//            }
//            else
//            {
//                currentState->goTo(southRoom);
//            }
//        }

        /* Quit command */
        if ((commandBuffer.compare(0,endOfVerb,"quit") == 0)) {
            commandOk = true;
            gameOver = true;
        }

        /* If commandOk hasn't been set, command wasn't understood, display error message */
        if(!commandOk) {
            wrapOut(&badCommand);
            wrapEndPara();
        }
    }
}


int main() {
    initWordWrap();
    initRooms();
    initState();
    currentState->announceLoc();
    gameLoop();
    return 0;
}