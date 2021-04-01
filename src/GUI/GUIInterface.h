#pragma once

#include <cstdio>
#include <string>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <vector>
#include <set>

#include <UltraOOXX/UltraBoard.h>

// I add them
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#define SCREEN_W 1100    // Display (screen) width
#define SCREEN_H 800    // Display (screen) height

namespace TA
{
    class GUIInterface
    {
    public:
        virtual void title() = 0;
        virtual void appendText(std::string str) = 0;
        virtual void updateGame(UltraBoard b) = 0;
        virtual void game_destroy() = 0;
        virtual void round_load(int num_o, int num_x) = 0;
        virtual void round_destroy() = 0;
        virtual void update_score(int num_o, int num_x) = 0;
    };

    using std::printf;
    #define ESC "\033"
    class ASCII: public GUIInterface
    {
        ALLEGRO_DISPLAY* game_display;
        ALLEGRO_BITMAP* my_title;
        ALLEGRO_BITMAP* back;
        ALLEGRO_BITMAP* back2;
        ALLEGRO_BITMAP* game_board;
        ALLEGRO_BITMAP* ultra_game_board;
        ALLEGRO_BITMAP* oo;
        ALLEGRO_BITMAP* xx;
        ALLEGRO_BITMAP* record;
        
        ALLEGRO_BITMAP* number_zero;
        ALLEGRO_BITMAP* number_one;
        ALLEGRO_BITMAP* number_two;
        ALLEGRO_BITMAP* number_three;
        ALLEGRO_BITMAP* number_four;
        ALLEGRO_BITMAP* number_five;
        ALLEGRO_BITMAP* number_six;
        ALLEGRO_BITMAP* number_seven;
        ALLEGRO_BITMAP* number_eight;
        ALLEGRO_BITMAP* number_nine;
        
        //ALLEGRO_SAMPLE* main_bgm;
        //ALLEGRO_SAMPLE_INSTANCE* main_bgm_instance;
        //ALLEGRO_SAMPLE_ID main_bgm_id;
        
        int corner_x, corner_y; // the coordinate of left-right corner of game_board
        int corner_x2, corner_y2;
        // the delta of x and y of two symbols in the small board
        int delta_x_small, delta_y_small; 
        // the delta of x and y of two symbols in the large board
        int delta_x_big, delta_y_big;   
    
    
        const static int GRAPH_HIGHT = 7+15;
        const static int GRAPH_WIDTH = 80;
        
        const static int TEXT_HIGHT = 10;
        const static int TEXT_WIDTH = 80;

        std::string m_preparedText;
        std::string m_textbuf;

        void cls()
        {
            printf( ESC "[H" ESC "[J" );
        }

        void gotoxy(int y, int x)
        {
            printf( ESC "\033[%d;%df", y, x);
        }

        void updateTextBuf()
        {
            std::stringstream ss(m_textbuf);
            const std::string ban(TEXT_WIDTH, '-');
            std::string tmp, last;

            m_preparedText.clear();
            for(int L=0 ; L < TEXT_HIGHT - 2; ++L)
            {
                if( last == "" )
                    getline(ss, last);

                tmp = last.substr(0, std::min((size_t)TEXT_WIDTH, last.size()) );
    
                if( tmp.size() == last.size() )
                    last = "";
                else 
                    last = last.substr(TEXT_WIDTH);
    
                m_preparedText = tmp + "\n" + m_preparedText;
            }
            m_textbuf = m_textbuf.substr(0, TEXT_HIGHT * TEXT_WIDTH);
            m_preparedText =  ban + "\n" + m_preparedText + ban;
        }

        void showText()
        {
            gotoxy(GRAPH_HIGHT+1, 0);
            printf( ESC "[J" );
            gotoxy(GRAPH_HIGHT+1, 0);
            puts(m_preparedText.c_str());
            gotoxy(GRAPH_HIGHT+TEXT_HIGHT+1, 0);
            std::fflush(stdout);
        }
        
        // I add it
        // load resized and check if it is failed
        ALLEGRO_BITMAP *load_bitmap_resized(const char *filename, int w, int h) {
	        ALLEGRO_BITMAP* loaded_bmp = al_load_bitmap(filename);
	        if (!loaded_bmp)
		        std::cout << "failed to load image:" << filename << '\n';
	        ALLEGRO_BITMAP *resized_bmp = al_create_bitmap(w, h);
	        ALLEGRO_BITMAP *prev_target = al_get_target_bitmap();

	        //if (!resized_bmp)
		    //    game_abort("failed to create bitmap when creating resized image: %s", filename);
	        al_set_target_bitmap(resized_bmp);
	        al_draw_scaled_bitmap(loaded_bmp, 0, 0,
		        al_get_bitmap_width(loaded_bmp),
		        al_get_bitmap_height(loaded_bmp),
		        0, 0, w, h, 0);
	        al_set_target_bitmap(prev_target);
	        al_destroy_bitmap(loaded_bmp);

	        //game_log("resized image: %s", filename);

	        return resized_bmp;
        }

        

    public:
        virtual void title() override
        {
            cls();
            puts(
R"( _   _ _ _             _____  _______   ____   __
| | | | | |           |  _  ||  _  \ \ / /\ \ / /
| | | | | |_ _ __ __ _| | | || | | |\ V /  \ V /
| | | | | __| '__/ _` | | | || | | |/   \  /   \
| |_| | | |_| | | (_| \ \_/ /\ \_/ / /^\ \/ /^\ \
 \___/|_|\__|_|  \__,_|\___/  \___/\/   \/\/   \/
)");
            
            // I add them:
            al_init();
            al_init_image_addon();
            
            game_display = al_create_display(SCREEN_W, SCREEN_H);
            al_clear_to_color(al_map_rgb(100, 100, 100));   // set background to white
            
            // image
            back = load_bitmap_resized("picture/back.png", SCREEN_W, SCREEN_H);
            back2 = load_bitmap_resized("picture/back.png", 150, 70);
            my_title = load_bitmap_resized("picture/title2.PNG", 302, 50);
            game_board = load_bitmap_resized("picture/board2.png", SCREEN_H, SCREEN_H);
            ultra_game_board = load_bitmap_resized("picture/ultra_board2.png", 190, SCREEN_H / 3);
            //oo = load_bitmap_resized("picture/oo.png", SCREEN_H / 20, SCREEN_H / 20);
            //xx = load_bitmap_resized("picture/xx3.png", SCREEN_H / 20, SCREEN_H / 20);
            record = load_bitmap_resized("picture/record.PNG", 231 * 2 / 3, 76 * 2 / 3);
            
            number_zero = load_bitmap_resized("picture/number0.png", 90, 67.5);
            number_one = load_bitmap_resized("picture/number1.png", 90, 67.5);
            number_two = load_bitmap_resized("picture/number2.png", 90, 67.5);
            number_three = load_bitmap_resized("picture/number3.png", 90, 67.5);
            number_four = load_bitmap_resized("picture/number4.png", 90, 67.5);
            number_five = load_bitmap_resized("picture/number5.png", 90, 67.5);
            number_six = load_bitmap_resized("picture/number6.png", 90, 67.5);
            number_seven = load_bitmap_resized("picture/number7.png", 90, 67.5);
            number_eight = load_bitmap_resized("picture/number8.png", 90, 67.5);
            number_nine = load_bitmap_resized("picture/number9.png", 90, 67.5);
            
            al_draw_bitmap(back, 0, 0, 0);
            //al_draw_bitmap(game_board, 0, 0, 0);
            //al_draw_bitmap(ultra_game_board, SCREEN_H, 525, 0); 
            //al_draw_bitmap(my_title, SCREEN_W - 310, 0, 0);
            al_flip_display();
            
            corner_x2 = 844;
            corner_y2 = 550;
            //al_draw_bitmap(oo, corner_x2, corner_y2, 0);
            
            // initialize coordinate
            corner_x = 140;
            corner_y = 100;
            delta_x_small = 52.5;
            delta_y_small = 60;
            delta_x_big = 32.5;
            delta_y_big = 42.5;
            
            //al_draw_bitmap(oo, corner_x2 + delta_x_small, corner_y2, 0);
            //al_draw_bitmap(oo, corner_x2, corner_y2 + delta_y_small, 0);
        }
        
        // destroy the bitmap and display : the end of the game
        virtual void game_destroy(void) override
        {
            al_destroy_bitmap(back);
            al_destroy_bitmap(game_board);
            al_destroy_bitmap(ultra_game_board);
            al_destroy_bitmap(my_title);
            al_destroy_bitmap(record);
            al_destroy_bitmap(number_zero);
            al_destroy_bitmap(number_one);
            al_destroy_bitmap(number_two);
            al_destroy_bitmap(number_three);
            al_destroy_bitmap(number_four);
            al_destroy_bitmap(number_five);
            al_destroy_bitmap(number_six);
            al_destroy_bitmap(number_seven);
            al_destroy_bitmap(number_eight);
            al_destroy_bitmap(number_nine);
            //al_destroy_bitmap(oo);
            //al_destroy_bitmap(xx);
            al_destroy_display(game_display);   
        }
        
        virtual void round_destroy(void) override
        {
            al_destroy_bitmap(oo);
            al_destroy_bitmap(xx);            
        }
        
        virtual void round_load(int num_o, int num_x) override
        {
            oo = load_bitmap_resized("picture/oo.png", SCREEN_H / 20, SCREEN_H / 20);
            xx = load_bitmap_resized("picture/xx3.png", SCREEN_H / 20, SCREEN_H / 20);
            al_draw_bitmap(game_board, 0, 0, 0);
            al_draw_bitmap(ultra_game_board, SCREEN_H + 20, 525, 0); 
            al_draw_bitmap(my_title, SCREEN_W - 350, 10, 0);
            al_draw_bitmap(record, SCREEN_W - 270, 180, 0); 
            al_draw_bitmap(oo, SCREEN_W - 300, 280, 0);
            al_draw_bitmap(xx, SCREEN_W - 300, 380, 0);
    
            update_score(num_o, num_x);
            
            al_flip_display();
        }
        
        virtual void update_score(int num_o, int num_x) override
        {
            al_draw_bitmap(back2, SCREEN_W - 200, 260, 0);
            if (num_o % 10 == 0)
                al_draw_bitmap(number_zero, SCREEN_W - 200, 260, 0);
            else if (num_o % 10 == 1)
                al_draw_bitmap(number_one, SCREEN_W - 200, 260, 0);
            else if (num_o % 10 == 2)
                al_draw_bitmap(number_two, SCREEN_W - 200, 260, 0);
            else if (num_o % 10 == 3)
                al_draw_bitmap(number_three, SCREEN_W - 200, 260, 0);
            else if (num_o % 10 == 4)
                al_draw_bitmap(number_four, SCREEN_W - 200, 260, 0);
            else if (num_o % 10 == 5)
                al_draw_bitmap(number_five, SCREEN_W - 200, 260, 0);
            else if (num_o % 10 == 6)
                al_draw_bitmap(number_six, SCREEN_W - 200, 260, 0);
            else if (num_o % 10 == 7)
                al_draw_bitmap(number_seven, SCREEN_W - 200, 260, 0);
            else if (num_o % 10 == 8)
                al_draw_bitmap(number_eight, SCREEN_W - 200, 260, 0);
            else if (num_o % 10 == 9)
                al_draw_bitmap(number_nine, SCREEN_W - 200, 260, 0);
            
            al_draw_bitmap(back2, SCREEN_W - 200, 360, 0);
            if (num_x % 10 == 0)
                al_draw_bitmap(number_zero, SCREEN_W - 200, 360, 0);
            else if (num_x % 10 == 1)
                al_draw_bitmap(number_one, SCREEN_W - 200, 360, 0);            
            else if (num_x % 10 == 2)
                al_draw_bitmap(number_two, SCREEN_W - 200, 360, 0);
            else if (num_x % 10 == 3)
                al_draw_bitmap(number_three, SCREEN_W - 200, 360, 0); 
            else if (num_x % 10 == 4)
                al_draw_bitmap(number_four, SCREEN_W - 200, 360, 0);            
            else if (num_x % 10 == 5)
                al_draw_bitmap(number_five, SCREEN_W - 200, 360, 0);
            else if (num_x % 10 == 6)
                al_draw_bitmap(number_six, SCREEN_W - 200, 360, 0);             
            else if (num_x % 10 == 7)
                al_draw_bitmap(number_seven, SCREEN_W - 200, 360, 0);            
            else if (num_x % 10 == 8)
                al_draw_bitmap(number_eight, SCREEN_W - 200, 360, 0);
            else if (num_x % 10 == 9)
                al_draw_bitmap(number_nine, SCREEN_W - 200, 360, 0); 
            
            al_flip_display();
        }

        virtual void appendText(std::string str)
        {
            m_textbuf = str + m_textbuf;
            updateTextBuf();
            showText();
        }

        int toPrintChar(BoardInterface::Tag t){
            switch(t) {
                case BoardInterface::Tag::O: return 'O';
                case BoardInterface::Tag::X: return 'X';
                case BoardInterface::Tag::Tie: return 'T';
                default:
                    return ' ';
            }
        }

        virtual void updateGame(UltraBoard b)
        {
            gotoxy(21+1, 0);//gotoxy(7+1, 0);
            const std::string buf(20, ' ');

            std::printf("%s", buf.c_str());
            std::printf(" 012 345 678\n");
            for (int i=0;i<9;++i)
            {
                std::printf("%s%d", buf.c_str(), i);
                // board show
                for (int j=0;j<9;++j)
                {
                    std::putchar(toPrintChar(b.get(i, j))); // old gui
                    if (j == 2 || j == 5) std::putchar('|');    // old gui
                    
                    int offset_x;   // the number of delta_x_big need to be added
                    int offset_y;   // the number of delta_y_big need to be added
                    
                    offset_x = (j / 3) * delta_x_big;   // calculate the offset
                    offset_y = (i / 3) * delta_y_big;
                    
                    if (b.get(i, j) == BoardInterface::Tag::O) { // print 'O'
                        al_draw_bitmap(oo, corner_x + offset_x + delta_x_small * j, 
                            corner_y + offset_y + delta_y_small * i, 0);
                        al_flip_display();
                        al_rest(0.01);
                    }
                    else if (b.get(i, j) == BoardInterface::Tag::X) {   // print 'X'
                        al_draw_bitmap(xx, corner_x + offset_x + delta_x_small * j, 
                            corner_y + offset_y + delta_y_small * i, 0);    
                        al_flip_display(); 
                        al_rest(0.01);               
                    }
                }
                //UltarBoard show
                if(i>=0 && i<3){
                    std::printf("%s", buf.c_str());
                    for (int j=0;j<3;++j)
                    {
                        std::putchar(toPrintChar(b.state(i, j)));   // old gui
                        if (j == 0 || j == 1) std::putchar('|');    // old gui
                        
                        if (b.state(i, j) == BoardInterface::Tag::O) {  // print 'O'
                            al_draw_bitmap(oo, corner_x2 + delta_x_small * j, 
                                corner_y2 + delta_y_small * i, 0); 
                            al_flip_display();                           
                        }
                        else if (b.state(i, j) == BoardInterface::Tag::X) { // print 'X'
                            al_draw_bitmap(xx, corner_x2 + delta_x_small * j, 
                                corner_y2 + delta_y_small * i, 0); 
                            al_flip_display();                           
                        }
                    }
                }

                std::putchar('\n');
                if (i==2 ||i==5) {
                    std::printf("%s", buf.c_str());
                    std::puts(std::string(12,'-').c_str());
                }
            }
            std::cout<<std::endl;
            //ultra board tag
            /*for (int i=0;i<3;++i)
            {
                
            }
            std::cout<<std::endl;
            */
            gotoxy(GRAPH_HIGHT+TEXT_HIGHT+1, 0);
        }
    };
    #undef ESC
}