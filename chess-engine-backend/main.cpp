#include "Driver.h"

#include <iostream>

int main() {
  size_t i = 0;
  Driver d;
  while (true) {
    std::cerr << "Waiting for message..." << std::endl;
    json msg;
    std::cin >> msg;
    d.process_msg(msg);
  }
}

/*
 * Represents the entry point of the engine.
 *
 * ENGINE: A cpp program which has the sole purpose of running the current chess
 * game and playing moves.
 *
 * CLIENT: An electron desktop application, this will do everything else. This
 * includes creating an instance of an engine per game.
 *
 * COMMUNICATION: IPC between the client and engine happens through (UNIX)
 * pipes, via JSON messages. An invariant is that the messaging will occur
 * through taking turns (nothing asynchronous). The client will ask the engine
 * to do something.The engine will then attempt to do it and send a response
 * back in JSON and so on ...
 *
 * The engine reads a "message" in a loop, a "message" is just a JSON object
 * that is required to have a "operation" key. This key determines what the
 * engine will do.
 */