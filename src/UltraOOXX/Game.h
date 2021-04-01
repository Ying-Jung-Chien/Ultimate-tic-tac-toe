#pragma once

#include <UltraOOXX/UltraBoard.h>
#include <UltraOOXX/Wrapper/AI.h>
#include <GUI/GUIInterface.h>

#include <iostream>
#include <cassert>
#include <chrono>
#include <cstdarg>
#include <future>
#include <type_traits>

extern bool stopnwait;

namespace TA
{
    class UltraOOXX
    {
    public:
        UltraOOXX(
            std::chrono::milliseconds runtime_limit = std::chrono::milliseconds(100000)
        ):
            m_runtime_limit(runtime_limit),
            m_P1(nullptr),
            m_P2(nullptr),
            MainBoard()
        {
            gui = new ASCII;
        }

        void setPlayer1(AIInterface *ptr) { assert(checkAI(ptr)); m_P1 = ptr; }
        void setPlayer2(AIInterface *ptr) { assert(checkAI(ptr)); m_P2 = ptr; }

        int round;
        int valid_x, valid_y, valid_x2, valid_y2;
        int mode_choose, mode_P1, mode_P2;   // 0 : Offense; 1 : standard; 2 : user
        int score_x, score_o;

        void run()
        {   
            int one_more;
            
            gui->title();
            score_x = 0;
            score_o = 0;
            do {
                gui->round_load(score_o, score_x);
                
                // choose the mode
                std::cout << "Please choose the mode:\n\n";
                std::cout << "  1. AI vs. AI    : Random AI vs. Standard AI\n";
                std::cout << "  2. AI vs. AI    : Must-Win AI vs. Standard AI\n";
                std::cout << "  3. AI vs. AI    : Standard AI vs. Standard AI\n\n";
                std::cout << "  4. Human vs. AI : Manual mode vs. Standard AI\n";
                std::cout << "  5. AI vs. Human : Standard AI vs. Manual mode\n\n";
                std::cout << "Please enter the number from 1 to 5: ";
                std::cin >> mode_choose;
                while (mode_choose < 1 || mode_choose > 5) {
                    std::cout << "Invalid mode!! Please choose the mode from 1 to 5\n";
                    std::cout << "Please enter the number from 1 to 5: ";
                    std::cin >> mode_choose;
                }
                
                // set the mode
                if (mode_choose == 1) {
                    mode_P1 = 3;
                    mode_P2 = 1;
                }
                else if (mode_choose == 2) {
                    mode_P1 = 0;
                    mode_P2 = 1;
                }
                else if (mode_choose == 3) {
                    mode_P1 = 1;
                    mode_P2 = 1;
                }
                else if (mode_choose == 4) {
                    mode_P1 = 2;
                    mode_P2 = 1;
                }
                else if (mode_choose == 5) {
                    mode_P1 = 1;
                    mode_P2 = 2;
                }
                
                round = 0;
               
                if( !prepareState() ) return ;

                //Todo: Play Game
                //putToGui("Hello world %d\n", 123);
                updateGuiGame();

                while (!checkGameover()) {          
                    round++;
                    //TODO
                
                    //end
                    AIInterface *first = nullptr;
                    AIInterface *second = nullptr;
                    BoardInterface::Tag tag = BoardInterface::Tag::O;

                    //TODO
                    if(round%2==1){
                        first = m_P1;
                        second = m_P2;
                        tag = BoardInterface::Tag::O;
                    }
                    else if(round%2==0){
                        first = m_P2;
                        second = m_P1;
                        tag = BoardInterface::Tag::X;
                    }

                    std::cout<<"Round:"<<round<<" Player:"<<(round%2? "O":"X")<<"\n";

                    if (!playOneRound(first, tag, second)) {
                        std::cout<<"This is the end?\n";
                    }
                    updateGuiGame();

                    //if(stopnwait) system("read -p 'Press Enter to continue...' var");
                
                    //testing
                    printf( "\033[H\033[J" );
                    //end
                }
                
                
                gui->update_score(score_o, score_x);
                
                std::cout << "Do you want to play one more round?\n";
                std::cout << "0: yes; 1: no\n";
                std::cout << "Please enter 0 or 1: ";
                
                std::cin >> one_more;
                while (one_more != 0 && one_more != 1) {
                    std::cout << "Invalid number!! Please choose 0 or 1\n";
                    std::cout << "Please enter 0 or 1: ";
                    std::cin >> one_more;
                }
                
                gui->round_destroy(); // I add it
                MainBoard.reset();
                MainBoard.setWinTag(TA::BoardInterface::Tag::None);
            } while (one_more == 0);
            gui->game_destroy(); // I add it
        } 

   private:
        void updateGuiGame()
        {
            gui->updateGame(MainBoard);
        }

        bool playOneRound(AIInterface *user, BoardInterface::Tag tag, AIInterface *enemy)
        {
            auto pos = call(&AIInterface::queryWhereToPut, user, MainBoard);
            //TODO
            //std::cout<<pos.first<<"!!!"<<pos.second<<"!!!\n";
            
            enemy->callbackReportEnemy(pos.first,pos.second);
           // MainBoard.set(pos.first, pos.second, tag);
            MainBoard.get(pos.first, pos.second) = tag;
 
            determineBoardWin(pos.first, pos.second, tag);
            determineUltraWin(pos.first, pos.second); 
             
            /*if (round % 2 == 1) {
                valid_x2 = pos.first % 3;
                valid_y2 = pos.second % 3;
                if ((pos.first / 3 != valid_x || pos.second / 3 != valid_y)
                    && round != 1)  // illegal position
                    MainBoard.setWinTag(TA::BoardInterface::Tag::X);  
            }           
            else if (round % 2 == 0) {
                valid_x = pos.first % 3;
                valid_y = pos.second % 3;
                if (pos.first / 3 != valid_x2 || pos.second / 3 != valid_y2)  // illegal position
                    MainBoard.setWinTag(TA::BoardInterface::Tag::O);  
            }*/
            
            //Ido
            //MainBoard.sub(pos.first/3, pos.second/3).determineWin(pos.first%3, pos.second%3);
            //MainBoard.determineWin(pos.first/3, pos.second/3);
        //I_end
            //end
            return true;
        }

        bool checkGameover()
        {
            //TODO
            if(MainBoard.getWinTag() == TA::BoardInterface::Tag::X){
                std::cout<<"X WIN!GAMEOVER!!!\n";
                score_x++;
                updateGuiGame();
                return true;
            }
            else if(MainBoard.getWinTag() == TA::BoardInterface::Tag::O){
                std::cout<<"O WIN!GAMEOVER!!!\n";
                score_o++;
                updateGuiGame();
                return true;
            }
            else if(MainBoard.getWinTag() == TA::BoardInterface::Tag::Tie){
                std::cout<<"Tie!GAMEOVER!!!\n";
                updateGuiGame();
                return true;
            }
            else if(MainBoardfull(MainBoard)){
                std::cout<<"FULL!GAMEOVER!!!\n";
                updateGuiGame();
                return true;
            }
            else return false;
            //end
            /*return true; // Gameover!*/
        }

        bool MainBoardfull() {
            for (int i=0;i<3;++i)
                for (int j=0;j<3;++j)
                    if (!MainBoard.sub(i ,j).full())
                        return false;
            return true;
        }

        bool prepareState()
        {
            call(&AIInterface::init, m_P1, mode_P1);
            call(&AIInterface::init, m_P2, mode_P2);
            return true;
        }

        template<typename Func ,typename... Args, 
            std::enable_if_t< std::is_void<
                    std::invoke_result_t<Func, AIInterface, Args...>
                >::value , int> = 0 >
        void call(Func func, AIInterface *ptr, Args... args)
        {
            std::future_status status;
            auto val = std::async(std::launch::async, func, ptr, args...);
            status = val.wait_for(std::chrono::milliseconds(m_runtime_limit));

            if( status != std::future_status::ready )
            {
                exit(-1);
            }
            val.get();
        }

        template<typename Func ,typename... Args, 
            std::enable_if_t< std::is_void<
                    std::invoke_result_t<Func, AIInterface, Args...>
                >::value == false, int> = 0 >
        auto call(Func func, AIInterface *ptr, Args... args)
            -> std::invoke_result_t<Func, AIInterface, Args...>
        {
            std::future_status status;
            auto val = std::async(std::launch::async, func, ptr, args...);
            status = val.wait_for(std::chrono::milliseconds(m_runtime_limit));

            if( status != std::future_status::ready )
            {
                exit(-1);
            }
            return val.get();
        }

        void putToGui(const char *fmt, ...)
        {
            va_list args1;
            va_start(args1, fmt);
            va_list args2;
            va_copy(args2, args1);
            std::vector<char> buf(1+std::vsnprintf(nullptr, 0, fmt, args1));
            va_end(args1);
            std::vsnprintf(buf.data(), buf.size(), fmt, args2);
            va_end(args2);

            if( buf.back() == 0 ) buf.pop_back();
            gui->appendText( std::string(buf.begin(), buf.end()) );
        }

        bool checkAI(AIInterface *ptr) 
        {
            return ptr->abi() == AI_ABI_VER;
        }

        //determine win
        void determineUltraWin(int x, int y){
            //std::cout<<"determineWin\n";
            x /= 3;
            y /= 3;
            if(MainBoard.getWinTag() != TA::BoardInterface::Tag::None) return;
            TA::BoardInterface::Tag tmp = MainBoard.state(x,y);
            //std::cout<<tmp<<"!\n";
            int flag = 0;
            if(y == 0 && tmp == MainBoard.state(x, y+1) && tmp == MainBoard.state(x, y+2)) flag = 1;
            if(y == 1 && tmp == MainBoard.state(x, y-1) && tmp == MainBoard.state(x, y+1)) flag = 1;
            if(y == 2 && tmp == MainBoard.state(x, y-1) && tmp == MainBoard.state(x, y-2)) flag = 1;
            if(x == 0 && tmp == MainBoard.state(x+1, y) && tmp == MainBoard.state(x+2, y)) flag = 1;
            if(x == 1 && tmp == MainBoard.state(x-1, y) && tmp == MainBoard.state(x+1, y)) flag = 1;
            if(x == 2 && tmp == MainBoard.state(x-1, y) && tmp == MainBoard.state(x-2, y)) flag = 1;
            if(((x == 0 && y == 0) && tmp == MainBoard.state(1, 1) && tmp == MainBoard.state(2, 2)) || 
            ((x == 1 && y == 1) && tmp == MainBoard.state(0, 0) && tmp == MainBoard.state(2, 2)) ||
            ((x == 2 && y == 2) && tmp == MainBoard.state(0, 0) && tmp == MainBoard.state(1, 1)) ||
            ((x == 0 && y == 2) && tmp == MainBoard.state(1, 1) && tmp == MainBoard.state(2, 0)) ||
            ((x == 1 && y == 1) && tmp == MainBoard.state(0, 2) && tmp == MainBoard.state(2, 0)) ||
            ((x == 2 && y == 0) && tmp == MainBoard.state(0, 2) && tmp == MainBoard.state(1, 1))) flag = 1;
            if(flag &&tmp!=TA::BoardInterface::Tag::Tie){
                MainBoard.setWinTag(tmp);
                 //std::cout<<"ultra tag change "<<t<<"\n";
            }
            else{
                if(MainBoardfull(MainBoard)) MainBoard.setWinTag(TA::BoardInterface::Tag::Tie);
                else MainBoard.setWinTag(TA::BoardInterface::Tag::None);
                 //std::cout<<"ultra tag no change "<<t<<"\n";
            }
            //TA::BoardInterface::Tag temp = MainBoard.getWinTag();
            //std::cout<<temp<<"\n";
        }

        void determineBoardWin(int x, int y, TA::BoardInterface::Tag t){
            TA::Board& tgtboard = MainBoard.sub(x/3, y/3);
            if(tgtboard.getWinTag() != TA::BoardInterface::Tag::None) return;
            std::cout<<"determineWin"<<x<<y<<"\n";

            x %= 3;
            y %= 3;
            TA::BoardInterface::Tag tmp = t;
            //std::cout<<tmp<<"\n";
            int flag = 0;
            if(y == 0 && tmp == tgtboard.state(x, y+1) && tmp == tgtboard.state(x, y+2)) flag = 1;
            if(y == 1 && tmp == tgtboard.state(x, y-1) && tmp == tgtboard.state(x, y+1)) flag = 1;
            if(y == 2 && tmp == tgtboard.state(x, y-1) && tmp == tgtboard.state(x, y-2)) flag = 1;
            if(x == 0 && tmp == tgtboard.state(x+1, y) && tmp == tgtboard.state(x+2, y)) flag = 1;
            if(x == 1 && tmp == tgtboard.state(x-1, y) && tmp == tgtboard.state(x+1, y)) flag = 1;
            if(x == 2 && tmp == tgtboard.state(x-1, y) && tmp == tgtboard.state(x-2, y)) flag = 1;
            if(((x == 0 && y == 0) && tmp == tgtboard.state(1, 1) && tmp == tgtboard.state(2, 2)) || 
            ((x == 1 && y == 1) && tmp == tgtboard.state(0, 0) && tmp == tgtboard.state(2, 2)) ||
            ((x == 2 && y == 2) && tmp == tgtboard.state(0, 0) && tmp == tgtboard.state(1, 1)) ||
            ((x == 0 && y == 2) && tmp == tgtboard.state(1, 1) && tmp == tgtboard.state(2, 0)) ||
            ((x == 1 && y == 1) && tmp == tgtboard.state(0, 2) && tmp == tgtboard.state(2, 0)) ||
            ((x == 2 && y == 0) && tmp == tgtboard.state(0, 2) && tmp == tgtboard.state(1, 1))) flag = 1;
            if(flag &&tmp!=TA::BoardInterface::Tag::Tie){
                tgtboard.setWinTag(tmp);
                //std::cout<<"tag change "<<t<<"\n";
            }
            else{
                // std::cout<<"tag no change "<<t<<"\n";
                if(tgtboard.full()) tgtboard.setWinTag(TA::BoardInterface::Tag::Tie);
                else tgtboard.setWinTag(TA::BoardInterface::Tag::None);
            }
            //TA::BoardInterface::Tag temp= tgtboard.getWinTag();
            //std::cout<<temp<<"!\n";
        }
        bool MainBoardfull(TA::UltraBoard board) const {
            for (int i=0;i<3;++i)
                for (int j=0;j<3;++j)
                    if (!board.sub(i, j).full())
                        return false;
            return true;
        }

        int m_size;
        std::vector<int> m_ship_size;
        std::chrono::milliseconds m_runtime_limit;

        AIInterface *m_P1;
        AIInterface *m_P2;
        GUIInterface *gui;

        UltraBoard MainBoard;
    };
}
