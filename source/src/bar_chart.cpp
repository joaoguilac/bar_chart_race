/*!
 * @file bar_chart.cpp
 * @author João Guilherme Lopes Alves da Costa (joguicosta@hotmail.com)
 * @brief Implementation of the bar chart class and data base class.
 * @version 1.0
 * @date 2021-08-05
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include "bar_chart.h"

/*!
 * @namespace bcr contains all the classes used in the bar chart race.
 */
namespace bcr {
    //============[ BarChart METHODS ]===============//

    void BarChart::add_new_bar(BarItem new_bar) {
        bars.push_back(new_bar);
    }
    void BarChart::set_timestamp(std::string time) {
        bc_timestamp = time;
    }

    std::vector<BarChart::BarItem> BarChart::get_bars(void) {
        return bars;
    }
    size_t BarChart::get_n_bars(void) {
        return bars.size();
    }
    std::string BarChart::get_timestamp(void) const {
        return "Time Stamp: " + bc_timestamp;
    }
    
    //============[ End BarChart class ]===============//

    //============[ Database METHODS ]===============//

    void Database::add_new_barchart(std::shared_ptr<BarChart> new_bc) {
        data_set.push_back(new_bc);
    }
    void Database::set_current_bc(std::size_t index) {
        current_bc = index;
    }
    void Database::set_title(std::string title) {
        main_title = title;
    }
    void Database::set_label(std::string label) {
        chart_label = label;
    }
    void Database::set_source_info(std::string source_info) {
        chart_source_info = source_info;
    }
    
    std::shared_ptr<BarChart> Database::get_chart(void) {
        return data_set[current_bc];
    }
    std::size_t Database::get_n_charts(void) {
        return data_set.size();
    }
    std::size_t Database::get_current_bc(void) const {
        return current_bc;
    }
    std::string Database::get_title(void) const {
        return main_title;
    }
    std::string Database::get_label(void) const {
        return chart_label;
    }
    std::string Database::get_source_info(void) const {
        return chart_source_info;
    }

    //============[ End Database class ]===============//

} // namespace bcr
