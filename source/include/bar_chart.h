#ifndef _BAR_CHART_H_
#define _BAR_CHART_H_

/*!
 * @file bar_chart.h
 * @author João Guilherme Lopes Alves da Costa (joguicosta@hotmail.com)
 * @brief Stores all data from the read file to print the bar charts.
 * @version 1.0
 * @date 2021-08-05
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include <memory> ///< To use shared_ptr (smart pointer).
#include <vector> ///< To use vector and its methods.
#include <iomanip> ///< To use setw and setfill.

#include "../lib/text_color.h"

#define MAX_BAR_LEN 45 ///< The max length of a bar.

/**
 * @namespace bcr contains all the classes used in the bar chart race.
 */
namespace bcr {
    //* This class represents a single bar chart.
    class BarChart {
        //== Public members
        public:
            //* Struct to define a bar of the chart.
            struct BarItem {
                std::string label; //!< The data label.
                size_t value; //!< The value of the data item to be represented by a single bar in a chart.
                std::string category; //!< A category the data item belong to.
            };

            /**
             * @brief Add a bar to compose the bar chart.
             * @param new_bar It's the bar that will be added to the chart.
             */
            void add_new_bar(BarItem new_bar);
            
            /**
             * @brief Set the bc_timestamp object.
             * @param time The period that the bar chart was captured.
             */
            void set_timestamp(std::string time);
            
            /**
             * @brief Get the bars object.
             * @return std::vector<BarChart::BarItem> A collection of bars that compose a bar chart.
             */
            std::vector<BarChart::BarItem> get_bars(void);
            
            /**
             * @brief Get the number of bars of the bar chart.
             * @return size_t Number of bars that compose a bar chart.
             */
            size_t get_n_bars(void);
            
            /**
             * @brief Get the bc_timestamp object.
             * @return std::string The period that the bar chart was captured.
             */
            std::string get_timestamp(void) const;

        //== Private attributes
        private:
            std::vector<BarItem> bars; ///< Collection of bars.
            std::string bc_timestamp; ///< The timestamp the data was captured on.
    };

    //* This class represents all the data in the file. It has all bar charts.
    class Database {
        //== Public methods
        public:
            /**
             * @brief Store a bar chart.
             * @param new_bc A bar chart that will compose the data.
             */
            void add_new_barchart(std::shared_ptr<BarChart> new_bc);
            
            /**
             * @brief Set the current_bc object.
             * @param index A index to the current (whose will be printed) bar chart.
             */
            void set_current_bc(std::size_t index);
            
            /**
             * @brief Set the main_title object.
             * @param title The main title that will be printed on all bar charts.
             */
            void set_title(std::string title);
            
            /**
             * @brief Set the chart_label object.
             * @param label The label associated with the value portrayed in the chart.
             */
            void set_label(std::string label);
            
            /**
             * @brief Set the chart_source_info object.
             * @param source_info The data information reference.
             */
            void set_source_info(std::string source_info);
            
            /**
             * @brief Get the current chart of the data_set object.
             * @return std::shared_ptr<BarChart> A smart pointer to the current chart.
             */
            std::shared_ptr<BarChart> get_chart(void);
            
            /**
             * @brief Get the number of bar charts in the data.
             * @return std::size_t The size of data_set object.
             */
            std::size_t get_n_charts(void);
            
            /**
             * @brief Get the current_bc object.
             * @return std::size_t The index of the current bar chart.
             */
            std::size_t get_current_bc(void) const;
            
            /**
             * @brief Get the main_title object.
             * @return std::string The main title of the bar chart.
             */
            std::string get_title(void) const;
            
            /**
             * @brief Get the chart_label object.
             * @return std::string The label associated with the value.
             */
            std::string get_label(void) const;
            
            /**
             * @brief Get the chart_source_info object.
             * @return std::string The data information reference.
             */
            std::string get_source_info(void) const;

        //== Private attributes
        private:
            std::vector<std::shared_ptr<BarChart>> data_set; ///< Collection of charts according time.
            std::size_t current_bc{0}; ///< Which bar chart the program is on.
            std::string main_title; ///< The title of the chart.
            std::string chart_label; ///< The label associated with the value portrayed in the chart.
            std::string chart_source_info; ///< The source of information.
    };
}

#endif