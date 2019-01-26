#include "window.h"
#include  <unistd.h>
window::window(){

  initscr(); //成功之后 COLS LINES 
}
void window::PutStringToWindow(WINDOW* w, int y, int x, std::string &message){
        mvwaddstr(w, y, x, message.c_str());
        wrefresh(w);//刷新
}

void window::GetStringFromWindow(WINDOW* w, std::string &message){

         char buf[SIZE];
         wgetnstr(w, buf, SIZE);
         message = buf;
       }

void window::draw_header(){

  int x = 0;
  int y = 0;
  int h = LINES/5;
  int w = COLS;

  header = newwin(h, w, y, x);
  box(header,'+', '='); //边框
  wrefresh(header);//刷新
}

void window::draw_friend_list(){

  int x = COLS*3/4;
  int y = LINES/5;
  int h = LINES*3/5;
  int w = COLS/4;
   friendlist = newwin(h, w, y, x);

  box(friendlist,0, 0); //边框
  wrefresh(friendlist);//刷新

}

void window::draw_output(){

  int x = 0;
  int y = LINES/5;
  int h = LINES*3/5;
  int w = COLS*3/4;
  output = newwin(h, w, y, x);
  box(output,0, 0); //边框
  wrefresh(output);//刷新
}

void window::draw_input(){

  int x = 0;
  int y = LINES*4/5;
  int h = LINES/5;
  int w = COLS;
  input = newwin(h, w, y, x);
  box(input,0, 0); 
  wrefresh(input);//刷新
}

WINDOW* window::GetHeader(){
  return header;
}

WINDOW* window::GetOutput(){
  return output;
}

WINDOW* window::GetInput(){
  return input;
}

WINDOW* window::GetFriendlist(){
  return friendlist;
}

window::~window(){
  endwin();
}


//int main(){
//  window w;
//  w.draw_header();
//  w.draw_input();
//  w.draw_output();
//  w.draw_friend_list();
//
//
//  }
//












//





  
//  return 0;
//}
