/*!
 * @file animation_mgr.cpp
 * @author João Guilherme Lopes Alves da Costa (joguicosta@hotmail.com)
 * @brief Implementation of the animation manager class.
 * @version 1.0
 * @date 2021-08-05
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include "animation_mgr.h"

/*!
 * @namespace bcr contains all the classes used in the bar chart race.
 */
namespace bcr {
    //============[ AnimationManager METHODS ]===============//

    void AnimationManager::usage(std::string error) {
        // Print the error message.
        if (error != "")
            std::cerr << Color::tcolor(error, Color::RED, Color::BOLD) << std::endl;
        // Print the guide to run the program correctly.
        std::cerr << "\nUsage: " << Color::tcolor("$ ", Color::BRIGHT_GREEN, Color::REGULAR);
        std::cerr << Color::tcolor(opt.exe_filename, Color::BRIGHT_GREEN, Color::REGULAR);
        std::cerr << Color::tcolor(" [<options>] <input_data_file>\n", Color::BRIGHT_GREEN, Color::REGULAR);
        std::cerr << "  Bar Chart Race options:\n";
        std::cerr << "    -h  Print this help text.\n";
        std::cerr << "    -b  <num> Max # of bars in a single char.\n";
        std::cerr << "                Valid range is [1,15]. Default values is 5.\n";
        std::cerr << "    -f  <num> Animation speed in fps (frames per second).\n";
        std::cerr << "                Valid range is [1,24]. Default value is 24.\n";
        exit(1);
    }

    void AnimationManager::welcome_message(void) {
        std::cout << "\n===================================================\n";
        std::cout << "      Welcome to the Bar Chart Race, v1.0\n";
        std::cout << "  Copyright © 2021, João Guilherme L. A. da Costa\n";
        std::cout << "===================================================\n\n";
    }
    
    void AnimationManager::read_input_file(std::string file_name) {
        std::cout << Color::tcolor(">>> Preparing to read input file \"", Color::YELLOW, Color::REGULAR);
        std::cout << Color::tcolor(file_name, Color::YELLOW, Color::REGULAR);
        std::cout << Color::tcolor("\"...\n", Color::YELLOW, Color::REGULAR);
        
        std::string line;
        std::ifstream data_file;
        data_file.open(file_name);

        std::cout << Color::tcolor("\n>>> Processing data, please wait.", Color::YELLOW, Color::REGULAR) << std::flush;
        
        if (not data_file.is_open()) {
            std::string err("\n>>> ERROR! We didn't can found/open the file. This file probably doesn't exist.");
            usage(err);
        }
        
        //* [1] Read the file header to get the title, the category label, and source information.
        std::getline(data_file, line);
        if (data_file.fail()) {
            std::string err("\n>>> ERROR! We didn't can read the file. It isn't a data file.");
            usage(err);
        }
        data_base.set_title(line);
        std::getline(data_file, line);
        if (data_file.fail()) {
            std::string err("\n>>> ERROR! We couldn't read the file correctly, the file is corrupt.");
            usage(err);
        }
        data_base.set_label(line);
        std::getline(data_file, line);
        if (data_file.fail()) {
            std::string err("\n>>> ERROR! We couldn't read the file correctly, the file is corrupt.");
            usage(err);
        }
        data_base.set_source_info(line);

        size_t n_bars;
        std::string arg;
        std::stringstream tokenizer;
        //* [2] Read the Bar Charts. While there is line to read.
        while (not data_file.eof()) {
            std::getline(data_file, line);
            tokenizer.clear();
            tokenizer << line;
            // [2.1] Read a single integer n_bars: the total number of bars for the current bar chart.
            if (tokenizer >> n_bars) {
                // [2.2] Instantiate an empty BarChart object with smart pointer.
                std::shared_ptr<BarChart> bc {new BarChart()};
                // [2.3] Fixed loop to read n_bars lines from the input file.
                std::string time, dummy;
                std::vector<BarChart::BarItem> bis(n_bars);
                for (size_t i{0}; i < n_bars; i++) {
                    std::getline(data_file, line); //< time_stamp, label, other_related_info, value, category
                    if (data_file.fail()) {
                        std::string err("\n>>> ERROR! We couldn't read the file correctly, the file is corrupt.");
                        usage(err);
                    }
                    std::stringstream ss(line);
                    // time_stamp
                    std::getline(ss, arg, ',');
                    if (ss.fail()) {
                        std::string err("\n>>> ERROR! We couldn't read the file correctly, the file is corrupt.");
                        usage(err);
                    }
                    tokenizer.clear();
                    time = arg;
                    // label
                    std::getline(ss, arg, ',');
                    if (ss.fail()) {
                        std::string err("\n>>> ERROR! We couldn't read the file correctly, the file is corrupt.");
                        usage(err);
                    }
                    tokenizer.clear();
                    bis[i].label = arg;
                    // other_related_info/dummy
                    std::getline(ss, arg, ',');
                    if (ss.fail()) {
                        std::string err("\n>>> ERROR! We couldn't read the file correctly, the file is corrupt.");
                        usage(err);
                    }
                    tokenizer.clear();
                    dummy = arg;
                    // value
                    std::getline(ss, arg, ',');
                    if (ss.fail()) {
                        std::string err("\n>>> ERROR! We couldn't read the file correctly, the file is corrupt.");
                        usage(err);
                    }
                    tokenizer.clear();
                    tokenizer << arg;
                    tokenizer >> bis[i].value;
                    // category
                    std::getline(ss, arg, '\n');
                    if (ss.fail()) {
                        std::string err("\n>>> ERROR! We couldn't read the file correctly, the file is corrupt.");
                        usage(err);
                    }
                    tokenizer.clear();
                    bis[i].category = arg;
                    // Try to insert a new category in a std::set.
                    cat.insert(bis[i].category);
                    ss.clear();
                }
                // [2.4] Store the time_stamp of the last bar as the overall bc's time stamp.
                // And store the n_bars as the number of bars of the bar chart.
                bc->set_timestamp(time);
                // [2.5] Sort the bars of bc object, from highest bar value to the lowest bar value.
                std::vector<BarChart::BarItem> bc_sorted = sort_bc(bis);
                for (size_t i{0}; i < n_bars; i++) {
                    bc->add_new_bar(bc_sorted[i]);
                }
                // [2.6] Store the current (sorted) bc object into the Database object.
                data_base.add_new_barchart(bc);
            }
        }
        //* [3] Instantiate a map with category and its color.
        //* If the number of categories is less than or equal to 14.
        if (cat.size() <= 14) {
            for (size_t i{0}; i < cat.size(); i++) {
                auto str_cat = *next(cat.begin(), i);
                cat_color[str_cat] = Color::color_list[i];
            }
        }
        data_file.close();
        std::cout << Color::tcolor("\n>>> Input file sucessfuly read.\n", Color::GREEN, Color::BOLD);
    }

    void AnimationManager::summary(void) {
        std::ostringstream oss;
        oss << "\n>>> We have \"" << data_base.get_n_charts() << "\" charts"
            << ", each with a maximum of \"" << opt.n_bars << "\" bars.\n"
            << "\n>>> Animation speed is: " << opt.fps << ".\n"
            << ">>> Title: " << data_base.get_title() << "\n"
            << ">>> Value is: " << data_base.get_label() << "\n"
            << ">>> Source: " << data_base.get_source_info() << "\n"
            << ">>> # of categories found: " << cat.size() << "\n";
        std::cout << Color::tcolor(oss.str(), Color::YELLOW, Color::REGULAR);
    }

    std::ostringstream AnimationManager::get_axis(size_t n_bars, std::vector<BarChart::BarItem> bis) const {
        std::ostringstream oss;
        size_t last{n_bars - 1}; ///< The last bar that will be printed (lower value).
        size_t low_value = (bis[last].value / 10) * 10; ///< The minimum value (after 0) that will be represented in the bar (rounds down).
        size_t high_value = ((bis[0].value / 10) * 10) + 10; ///< The maximum value that will be represented in the bar (rounds up).
        size_t aux_value{low_value}; ///< The smaller value that will be jumped.
        size_t aux_pos_1, aux_pos_2, jumps;
        size_t increment = (high_value - low_value) / 5; ///< The increment of the value in each position in the axis.
        
        // If the bar value with the max value is bigger than 0.
        if (bis[0].value > 0) {
            size_t low_pos = (low_value * MAX_BAR_LEN) / bis[0].value; ///< The position of the smallest value proportional to the size of the largest bar.
            //* Set - and +
            oss << "+";
            // Display the range (0, min_value]
            if (low_value > 0) {
                for (size_t i{1}; i < low_pos-1; i++) {
                    oss << "-";
                }
                oss << "+";
            }
            // Print the 5 '+'.
            for (size_t i{0}; i < 5; i++) {
                aux_value += increment;
                aux_pos_1 = (aux_value * MAX_BAR_LEN) / bis[0].value;
                aux_pos_2 = ((aux_value - increment) * MAX_BAR_LEN) / bis[0].value;
                if (aux_pos_1 - aux_pos_2 == 0)
                    jumps = 0;
                else
                    jumps = aux_pos_1 - aux_pos_2 - 1;
                // Print (less_value, bigger_value]
                for (size_t j{0}; j < jumps; j++) {
                    oss << "-";
                }
                oss << "+";
            }
            // Print the rest of the axis.
            oss << std::setfill('-') << std::setw(MAX_BAR_LEN * 2 - aux_pos_1) << ">" << "\n";
            oss << std::setfill(' ');
            //* Set numbers
            aux_value = low_value;
            // Display the first position (0)
            if (low_value > 0) {
                oss << "0";
            }
            oss << std::setw(low_pos) << aux_value;
            for (size_t i{0}; i < 5; i++) {
                aux_value += increment;
                aux_pos_1 = (aux_value * MAX_BAR_LEN) / bis[0].value;
                aux_pos_2 = ((aux_value - increment) * MAX_BAR_LEN) / bis[0].value;
                if (aux_pos_1 - aux_pos_2 == 0)
                    jumps = 0;
                else
                    jumps = aux_pos_1 - aux_pos_2 - 1;
                oss << std::setw(jumps+1) << aux_value;
            }
            oss << "\n";
        }
        else {
            oss << "+>\n" << "0";
        }
        oss << Color::tcolor(data_base.get_label(), Color::YELLOW, Color::BOLD);

        return oss;
    }
    
    void AnimationManager::display_bc(void) {
        std::shared_ptr<BarChart> bc = data_base.get_chart();
        std::vector<BarChart::BarItem> bis = bc->get_bars();
        std::ostringstream oss;
        size_t n_bars;
        // Display the titles.
        size_t len_main_title = (MAX_BAR_LEN*2 - data_base.get_title().size())/2;
        size_t len_timestamp = (MAX_BAR_LEN*2 - bc->get_timestamp().size())/2;
        oss << std::setw(len_main_title) << "" << data_base.get_title() << "\n\n";
        oss << std::setw(len_timestamp) << "" << bc->get_timestamp() << "\n\n";
        std::cout << Color::tcolor(oss.str(), Color::BLUE, Color::BOLD);
        oss.clear();
        // Display the bars.
        if (bc->get_n_bars() < opt.n_bars) {
            n_bars = bc->get_n_bars();
        }
        else {
            n_bars = opt.n_bars;
        }
        for (size_t i{0}; i < n_bars; i++) {
            if (bis[i].value > 0) {
                size_t bar_len = (bis[i].value * MAX_BAR_LEN) / bis[0].value;
                for (size_t j{0}; j < bar_len; j++) {
                    if (cat.size() <= 14)
                        std::cout << Color::tcolor("█", cat_color[bis[i].category], Color::REGULAR);
                    else
                        std::cout << Color::tcolor("█", Color::WHITE, Color::REGULAR);
                }
            }
            if (cat.size() <= 14)
                std::cout << " " << Color::tcolor(bis[i].label, cat_color[bis[i].category], Color::BOLD) << " [" << bis[i].value << "]\n\n";
            else
                std::cout << " " << Color::tcolor(bis[i].label, Color::WHITE, Color::BOLD) << " [" << bis[i].value << "]\n\n";
            oss.clear();
        }
        // Display x axis.
        oss = get_axis(n_bars, bis);
        std::cout << oss.str();
        oss.clear();
        // Display source info.
        std::cout << "\n\n" << Color::tcolor(data_base.get_source_info(), Color::WHITE, Color::BOLD);
        // Display legend.
        if (cat.size() <= 14) {
            std::cout << "\n";
            std::map<std::string, Color::value_t>::iterator it;
            for (it=cat_color.begin(); it!=cat_color.end(); ++it) {
                std::cout << Color::tcolor("█", it->second, Color::REGULAR) << ": "
                    << Color::tcolor(it->first, it->second, Color::BOLD) << " ";
            }
        }
        std::cout << "\n\n";
    }

    void AnimationManager::initialize(int argc, char *argv[]) {
        std::string exe_name(argv[0]);
        opt.exe_filename = exe_name;
        //* If not only the executable name is passed.
        if (argc > 1) {
            bool has_arguments{true};
            bool passed_df{false};
            //* Process the arguments from the command line.
            for (int i{1}; i < argc; i++) {
                // Transform the arguments to string and lower case.
                std::string str(argv[i]);
                for (size_t j{0}; j < str.length(); j++) {
                    str[j] = std::tolower(str[j]); 
                }
                // Check if no has more arguments.
                if (i+1 == argc) {
                    has_arguments = false;
                }
                // Check if the argument is number of bars.
                if (str == "-b" and has_arguments) {
                    // Transform the value of number of bars to string and lower case.
                    std::string str_value(argv[i+1]);
                    for (size_t j{0}; j < str_value.length(); j++) {
                        str_value[j] = tolower(str_value[j]);
                    }
                    // Transform the value of number of bars to integer and store.
                    try {
                        opt.n_bars = std::stoi(argv[i+1]);
                    }
                    catch(const std::invalid_argument& e) {
                        std::string err("\n>>> ERROR! The number of bars you entered is invalid (invalid argument).");
                        usage(err);
                    }
                    catch(const std::out_of_range& e) {
                        std::string err("\n>>> ERROR! The number of bars you entered is not in the int range (out of range).");
                        usage(err);
                    }
                    if (opt.n_bars < 1 or opt.n_bars > 15) {
                        std::string err("\n>>> ERROR! You entered a number less than 1 or greater than 15 as fps.");
                        usage(err);
                    }
                    i++;
                }
                // Check if the argument is fps.
                else if (str == "-f" and has_arguments) {
                    // Transform the value of fps to string and lower case.
                    std::string str_value(argv[i+1]);
                    for (size_t j{0}; j < str_value.length(); j++) {
                        str_value[j] = tolower(str_value[j]);
                    }
                    // Transform the value of fps to integer and store.
                    try {
                        opt.fps = std::stoi(argv[i+1]);
                    }
                    catch(const std::invalid_argument& e) {
                        std::string err("\n>>> ERROR! The fps you entered is invalid (invalid argument).");
                        usage(err);
                    }
                    catch(const std::out_of_range& e) {
                        std::string err("\n>>> ERROR! The fps you entered is not in the int range (out of range).");
                        usage(err);
                    }
                    if (opt.fps < 1 or opt.fps > 24) {
                        std::string err("\n>>> ERROR! You entered a number less than 1 or greater than 24 as the number of bars.");
                        usage(err);
                    }
                    i++;
                }
                // Check if the argument is help.
                else if (str == "-h") {
                    usage("");
                }
                // Check if the argument is the name of the data file.
                else {
                    opt.data_filename = argv[i];
                    passed_df = true;
                }
            }
            //* Check if was passed the data file.
            if (not passed_df) {
                std::string err("\n>>> ERROR! You have not entered the data file.");
                usage(err);
            }
        }
        else {
            std::string err("\n>>> ERROR! You just entered the executable name.");
            usage(err);
        }
        app_state = AppState::START;
    }

    bool AnimationManager::ended(void) {
        //* Return true if the state of the application is END.
        return app_state == AppState::END;
    }

    void AnimationManager::process_event(void) {
        if (app_state == AppState::WELCOME) {
            // Calls the function that reads the input file.
            read_input_file(opt.data_filename);
        }
        else if (app_state == AppState::READING) {
            // Waits for the user to press enter to start the animation.
            std::string enter{""};
            do {
                std::cout << Color::tcolor(">>> Press enter to begin the animation.\n", Color::YELLOW, Color::REGULAR);
                std::getline(std::cin, enter);
            } while (enter.length() != 0);
        }
        else if (app_state == AppState::RACING) {
            // Pause the execution for a few milliseconds to simulate the animation speed requested by the user in the fps input option.
            std::chrono::milliseconds duration{1000/opt.fps};
            std::this_thread::sleep_for(duration);
        }
    }

    void AnimationManager::update(void) {
        if (app_state == AppState::START) {
            app_state = AppState::WELCOME;
        }
        else if (app_state == AppState::WELCOME) {
            app_state = AppState::READING;
        }
        else if (app_state == AppState::READING) {
            app_state = AppState::RACING;
        }
        else if (app_state == AppState::RACING) {
            if (data_base.get_current_bc() + 1 < data_base.get_n_charts()) {
                // Move though the database, feeding the bar chart with information that will be presented to the user.
                std::size_t next_chart = data_base.get_current_bc() + 1;
                data_base.set_current_bc(next_chart);
            }
            else {
                // There aren't more bar chart, stop the animation.
                app_state = AppState::END;
            }
        }
    }

    void AnimationManager::render(void) {
        if (app_state == AppState::WELCOME) {
            // Display a welcoming message.
            welcome_message();
        }
        else if (app_state == AppState::READING) {
            // Display a summary of information captured from the database.
            summary();
        }
        else if (app_state == AppState::RACING) {
            // Display a single bar chart.
            clear_screen();
            display_bc();
        }
    }
    
    void AnimationManager::clear_screen(){
        //some C++ voodoo here ;D
        #if defined _WIN32
            system("cls");
        #elif defined (LINUX) || defined(gnu_linux) || defined(linux)
            system("clear");
        #elif defined (APPLE)
            system("clear");
        #endif
    }

    //============[ End AnimationManager class ]===============//

} // namespace bcr
