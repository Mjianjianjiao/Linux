#pragma once 
#include <iostream>
#include <ncurses.h>
#include <curses.h>
#define SIZE 1024
class window{
    public:
      window();
      ~window();
      void PutStringToWindow(WINDOW* w, int y, int x, std::string &message);
      void GetStringFromWindow(WINDOW* w, std::string &message);
      void draw_header();
      void draw_output();
      void draw_friend_list();
      void draw_input();       
   
      WINDOW* GetHeader();
      WINDOW* GetOutput();
      WINDOW* GetInput();
      WINDOW* GetFriendlist();
    
    
    
    
    
    
    private:
      WINDOW *header;
      WINDOW *output;
      WINDOW *input;
      WINDOW *friendlist;
};
