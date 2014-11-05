/*
 * =====================================================================================
 *       Filename:  window_manager.hpp
 *    Description:  
 *        Created:  2014-10-24 10:12
 *         Author:  Tiago Lobato Gimenes        (tlgimenes@gmail.com)
 * =====================================================================================
 */

////////////////////////////////////////////////////////////////////////////////////////

#ifndef WINDOW_MANAGER_HPP
#define WINDOW_MANAGER_HPP

////////////////////////////////////////////////////////////////////////////////////////

class WindowManager 
{
    public:
        WindowManager();

        /**
         * Main loop 
         * */
        virtual void main_loop(){};

        /**
         * Returns the windowID of this windowManager 
         * */
        virtual const int operator()() const;

        /**
         * Returns the windowID of this WindowManager
         * */
        virtual int& operator()();

    protected:
        /* Window ID of this window manager */
        int _windowID;
};

////////////////////////////////////////////////////////////////////////////////////////

#endif /* !WINDOW_MANAGER_HPP */

////////////////////////////////////////////////////////////////////////////////////////

