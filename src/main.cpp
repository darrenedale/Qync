/**
 * @file qync.cpp
 * @author Darren Edale
 * @date April 2020
 * @version 1.1.1
 *
 * @brief Qync main application file.
 *
 * This file implements the main() function that is the application entry point.
 */

#include "application.h"

/**
 * @brief The main entry point for the Qync application.
 *
 * This function instantiates a Qync::Application and initiates its
 * event loop.
 *
 * @return 0 on clean exit, non-0 otherwise.
 */
int main(int argc, char ** argv) {
    using Qync::Application;
	Application app(argc, argv);
	return Application::exec();
}
