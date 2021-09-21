#ifndef _ANIMATION_MANAGER_H_
#define _ANIMATION_MANAGER_H_

/*!
 * @file animation_mgr.h
 * @author João Guilherme Lopes Alves da Costa (joguicosta@hotmail.com)
 * @brief Class that defines the animation manager.
 * @version 1.0
 * @date 2021-08-05
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include <iostream> ///< To use cout and cerr.
#include <string> ///< To use string ans its methods.
#include <cctype> ///< To use tolower.
#include <sstream> ///< To handling the stringstream and ostringstream.
#include <fstream> ///< To handling files (ifstream).
#include <set> ///< To salve all categories of the data.
#include <map> ///< To match a category with a color.
#include <vector> ///< To use vector and its methods.
#include <algorithm> ///< To swap elements in sort.
#include <thread> ///< To pause the current thread for a few ms.

#include "../lib/text_color.h"
#include "bar_chart.h"

/**
 * @namespace bcr contains all the classes used in the bar chart race.
 */
namespace bcr {
    //* This the main class that represents the animation manager.
    class AnimationManager {
        //* Enumeration to determine the state of the program.
        enum AppState {
            START = 0, //!< The initial state.
            END, //!< Finishing the program.
            WELCOME, //!< Welcome message.
            READING, //!< Reading the input file.
            RACING, //!< Animating the bar charts.
        };
        //* Struct to store the options of the program passed by command line.
        struct RunningOpt {
            size_t fps{24}; //!< Quantity of fps per second.
            size_t n_bars{5}; //!< Number of bars in the animation.
            std::string data_filename; //!< Name of data file.
            std::string exe_filename; //!< Name of executable file.
        };

        //== Public methods
        public:
            /**
             * @brief Prints out the syntax to run the program and the error message.
             * @param error The error message who will be displayed.
             */
            void usage(std::string error);

            /**
             * @brief Prints out the welcoming message.
             */
            void welcome_message(void);

            /**
             * @brief Sort a bar chart in descending order according to the value of the bars
             * (it's used Selection sort).
             * @param bc The bar chart that will be sorted (vector of BarItem).
             * @return std::vector<BarChart::BarItem> The bar chart (vector of bars) in descending order.
             */
            std::vector<BarChart::BarItem> sort_bc(std::vector<BarChart::BarItem> bc) {
                for (size_t i{0}; i < bc.size() - 1; i++) {
                    auto pos_max = i;
                    for (size_t j{i+1}; j < bc.size(); j++) {
                        if (bc[j].value > bc[pos_max].value) {
                            pos_max = j;
                        }
                    }
                    std::swap(bc[i], bc[pos_max]);
                }
                return bc;
            }

            /**
             * @brief Get the axis x from a collection of bars.
             * @param n_bars The number of bars that will be printed.
             * @param bis The bars that compose the bar chart.
             * @return std::ostringstream the axis x that will be displayed.
             */
            std::ostringstream get_axis(size_t n_bars, std::vector<BarChart::BarItem> bis) const;
            
            /**
             * @brief Display the current bar chart.
             */
            void display_bc(void);
            
            /**
             * @brief Read the data file and store the informations.
             * @param file_name The name of file that will be read.
             */
            void read_input_file(std::string file_name);

            /**
             * @brief Prints out the summary of the data, after reading.
             */
            void summary(void);

            /**
             * @brief Starts the program according to the arguments passed by command line.
             * @param argc Number of arguments (inputs) of the program execution.
             * @param argv Pointer to the given arguments (inputs).
             */
            void initialize(int argc, char *argv[]);

            /**
             * @brief Check if the program is still running.
             * @return true if the program has finished (arrived in the END state).
             * @return false otherwise.
             */
            bool ended(void);

            /**
             * @brief Processes an event depending on the state of the program
             */
            void process_event(void);

            /**
             * @brief Update the current state of the program.
             * In case of RACING state, until there are bar charts
             * should move though the database.
             */
            void update(void);

            /**
             * @brief Sends information to the standard output, depending of the current stage.
             */
            void render(void);

            /**
             * @brief Clean the terminal before render the next bar chart.
             */
            void clear_screen(void);

        //== Private attributes
        private:
            RunningOpt opt; ///< The options/arguments passed by command line.
            AppState app_state; ///< State of the program.
            Database data_base; ///< The data of the file that will be displayed.
            std::set<std::string> cat; ///< All categories of the data.
            std::map<std::string, Color::value_t> cat_color; ///< The color of the different categories in the chart.
    };
}

#endif