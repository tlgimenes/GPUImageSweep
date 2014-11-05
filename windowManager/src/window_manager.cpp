/*
 * =====================================================================================
 *       Filename:  window_manager.cpp
 *    Description:  This class is a generic interface containning the structure
 *    of a window manager. To implement and use another window manager, you
 *    need to use heritage
 *        Created:  2014-10-24 10:10
 *         Author:  Tiago Lobato Gimenes        (tlgimenes@gmail.com)
 * =====================================================================================
 */

////////////////////////////////////////////////////////////////////////////////////////

#include "window_manager.hpp"

////////////////////////////////////////////////////////////////////////////////////////

WindowManager::WindowManager()
{
    this->_windowID = 0;
}

////////////////////////////////////////////////////////////////////////////////////////

const int WindowManager::operator()() const
{
    return this->_windowID;
}

////////////////////////////////////////////////////////////////////////////////////////

int& WindowManager::operator()()
{
    return this->_windowID;
}

////////////////////////////////////////////////////////////////////////////////////////
