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
//  std::string tips = "Please Enter# ";
//  std::string message;
//  int y, x;
//  while(1){
//    getmaxyx(w.GetInput(), y, x);
//    w.PutStringToWindow(w.GetInput(), 1, 1, tips);
//    w.GetStringFromWindow(w.GetInput(), message);
//    w.draw_input();
//  }
//












//





  
//  std::string title = "====ChatRoom====";
//  int i = 1;
//  int y, x;
//  int dir = 0;
//    while(1){
//   //  w.draw_header();//重新绘制，消除上次的字符
//      getmaxyx(w.GetHeader(), y, x);
//       w.PutStringToWindow(w.GetHeader(), y/2, i, title);
// 
//       if(i >=  (x - title.size() - 3)){
//             dir = 1;
//      }
//
//      if(i <= 4){
//        dir = 0;
//      }
//
//      if(dir == 0 ){
//        i++;
//      }else if(dir == 1) 
//        i--;
//
//      usleep(33000);
//    }
//  return 0;
//}
