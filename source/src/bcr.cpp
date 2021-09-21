/*!
 * @file bcr.cpp
 * @author João Guilherme Lopes Alves da Costa (joguicosta@hotmail.com)
 * @brief This program implements The Bar Chart Race.
 * The basic action of the bcr system is to read an input file 
 * with some data progressing over a period of time and display 
 * them in sequence, creating an illusion of animation.
 * @version 1.0
 * @date 2021-08-05
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include <cstdlib> ///< EXIT_SUCCESS

#include "animation_mgr.h"

int main(int argc, char *argv[]) {
    bcr::AnimationManager am;

    am.initialize(argc, argv);

    // Keep running until the animation ends or an error occurs.
    while (not am.ended()) {
        am.process_event();
        am.update();
        am.render();
    }

    return EXIT_SUCCESS;
}
