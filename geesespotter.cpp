#include "geesespotter_lib.h"
#include <iostream>

char *create_board(std::size_t x_dim, std::size_t y_dim){
    int size = x_dim * y_dim;
    char *board = new char[size];
    for(int i = 0; i < size; i++){
        board[i] = 0x00;
    }
    return board;

}
void clean_board(char * board){
    delete[] board;

}
void print_board(char * board, std::size_t x_dim, std::size_t y_dim){
    int counter = 0;
    for(int j = 0; j < (x_dim * y_dim); j++){
        if(board[j] & (1 << 4)){
            std::cout << 'M';
        }
        else if(board[j] & (1 << 5)){
            std::cout << '*';
        }
        else{
            std::cout<< int(board[j]);
        }
        counter++;
        if(counter == x_dim){
            std::cout<< "\n";
            counter = 0;
        }
    }
}

void hide_board(char * board, std::size_t x_dim, std::size_t y_dim){
    for(int j = 0; j < (x_dim * y_dim); j++){
        board[j] |= (1 << 5);
    }  
}

int mark(char * board, std::size_t x_dim, std::size_t y_dim, std::size_t x_loc, std::size_t y_loc){
    if((board[(x_loc + (x_dim * y_loc))] & (1 << 5)) == 0){
        return 2;
    }
    else{
        if((board[(x_loc + (x_dim * y_loc))] & (1 << 4)) == 0){
            (board[(x_loc + (x_dim * y_loc))] |= (1 << 4));
        }
        else{
            (board[(x_loc + (x_dim * y_loc))] ^= (1 << 4));
        }
        return 0;
    }
}

void compute_neighbours(char * board, std::size_t x_dim, std::size_t y_dim){
    int num;
    for(int row = 0; row < y_dim; row++){
        for(int col = 0; col < x_dim; col++){
            int index = ((row * x_dim) + col);
            num = 0;
            if(board[index] != 9){
                if(row > 0){
                    if(board[index - x_dim] == 9){
                        num++;
                    }
                    if(col > 0){
                        if(board[index - x_dim - 1] == 9){
                            num++;
                        }
                    }
                    if(col < x_dim - 1){
                        if(board[index - x_dim + 1] == 9){
                            num++;
                        }
                    }
                }
                if(row < y_dim - 1){
                    if(board[index + x_dim] == 9){
                        num++;
                    }
                    if(col > 0){
                        if(board[index + x_dim - 1] == 9){
                            num++;
                        }
                    }
                    if(col < x_dim - 1){
                        if(board[index + x_dim + 1] == 9){
                            num++;
                        }
                    }
                }
                if(col > 0){
                    if(board[index - 1] == 9){
                        num++;
                    }
                }
                if(col < x_dim - 1){
                    if(board[index + 1] == 9){
                        num++;
                    }
                }
                board[index] += num;
            }
        }
    }
}

bool is_game_won(char * board, std::size_t x_dim, std::size_t y_dim){
    for(int i = 0; i < (x_dim * y_dim); i++){
        if((board[i] & value_mask()) != 9){
            if(board[i] & (0x20)){
                return false;
            }
        }
    }
    return true;
}

int reveal(char * board, std::size_t x_dim, std::size_t y_dim, std::size_t x_loc, std::size_t y_loc){
    int index = x_loc + (x_dim * y_loc);
    if((board[index]^1 << 5) == 9){
        board[index] ^= (1 << 5);
        return 9;
    }
    else if((board[index] & (1 << 5)) && ((board[index] & (1 << 4)) == 0)){
        board[index] ^= (1 << 5);
        if(board[index] == 0){
            if(x_loc > 0){
                board[index - 1] &= ~(1 << 5);
            }
            if(x_loc < x_dim - 1){
                board[index + 1] &= ~(1 << 5);
            }
            if(y_loc > 0){
                board[index - x_dim] &= ~(1 << 5);
                if(x_loc > 0){
                    board[index - x_dim -1] &= ~(1 << 5);
                }
                if(x_loc < x_dim - 1){
                    board[index - x_dim + 1] &= ~(1 << 5);
                }
            }
            if(y_loc < y_dim - 1){
                board[index + x_dim] &= ~(1 << 5);
                if(x_loc > 0){
                    board[index + x_dim -1] &= ~(1 << 5);
                }
                if(x_loc < x_dim - 1){
                    board[index + x_dim + 1] &= ~(1 << 5);
                }
            }
        }
    }
    else if(board[index] & (1 << 4)){
        return 1;
    }
    else if((board[index] & (1 << 5)) == 0){
        return 2;
    }
    return 0;
}