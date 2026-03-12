// Coded by amoenux on 2026-03-10

#include <iostream>
#include <random>
#include <chrono>
#include <thread>

const uint faces=6;

const u_int32_t h=4; // HEIGHT
const u_int32_t w=5; // WIDTH
const u_int32_t d=3; // DEPTH

const u_int64_t TOTAL_CELLS=(u_int64_t) h* (u_int64_t) w * (u_int64_t) d;

const u_int32_t h_1=h-1;
const u_int32_t w_1=w-1;
const u_int32_t d_1=d-1;

const u_int32_t h_2=h-2;
const u_int32_t w_2=w-2;
const u_int32_t d_2=d-2;

bool cells1Front[h][w]{};
bool cells1Back[h][w]{};
bool cells1Left[h][d]{};
bool cells1Right[h][d]{};
bool cells1Top[d][w]{};
bool cells1Bot[d][w]{};

bool cells2Front[h][w]{};
bool cells2Back[h][w]{};
bool cells2Left[h][d]{};
bool cells2Right[h][d]{};
bool cells2Top[d][w]{};
bool cells2Bot[d][w]{};

bool oscillator=true;
int neighbour_count;

void generate_random_start(float density, unsigned seed = 0){
    // Optional use of a non-zero seed
    if (seed==0){
        std::random_device rd;
        seed = rd();
    }
    srand(seed);
    std::cout<<seed<<"\n";

    int threshhold = density * RAND_MAX;

    
    for (int row=0;row<h;row++){
        for (int col=0;col<w;col++){
            cells1Front[row][col]=(rand()<threshhold);
            cells1Back[row][col]=(rand()<threshhold);
        }
    }
    for (int row=0;row<h;row++){
        for (int col=0;col<d;col++){
            cells1Left[row][col]=(rand()<threshhold);
            cells1Right[row][col]=(rand()<threshhold);
        }
    }
    for (int row=0;row<d;row++){
        for (int col=0;col<w;col++){
            cells1Top[row][col]=(rand()<threshhold);
            cells1Bot[row][col]=(rand()<threshhold);
        }
    }
}

void cout_cells(){
    if (oscillator){
        for (int row=0;row<d;row++){
            for (int col=0;col<w;col++){
                std::cout<<(cells1Top[row][col] ? '#' : '.');
            }
            std::cout<<"\n";
        }
        for (int col=0;col<w;col++){
            std::cout<<"_";
        }
        std::cout<<"\n";
        for (int row=0;row<h;row++){
            for (int col=0;col<w;col++){
                std::cout<<(cells1Front[row][col] ? '#' : '.');
            }
            std::cout<<"|";
            for (int col=0;col<d;col++){
                std::cout<<(cells1Right[row][col] ? '#' : '.');
            }
            std::cout<<"|";
            for (int col=0;col<w;col++){
                std::cout<<(cells1Back[row][col] ? '#' : '.');
            }
            std::cout<<"|";
            for (int col=0;col<d;col++){
                std::cout<<(cells1Left[row][col] ? '#' : '.');
            }
            std::cout<<"\n";
        }
        for (int col=0;col<w;col++){
            std::cout<<"_";
        }
        std::cout<<"\n";
        for (int row=0;row<d;row++){
            for (int col=0;col<w;col++){
                std::cout<<(cells1Bot[row][col] ? '#' : '.');
            }
            std::cout<<"\n";
        }
    }
    else{
        for (int row=0;row<d;row++){
            for (int col=0;col<w;col++){
                std::cout<<(cells2Top[row][col] ? '#' : '.');
            }
            std::cout<<"\n";
        }
        for (int col=0;col<w;col++){
            std::cout<<"_";
        }
        std::cout<<"\n";
        for (int row=0;row<h;row++){
            for (int col=0;col<w;col++){
                std::cout<<(cells2Front[row][col] ? '#' : '.');
            }
            std::cout<<"|";
            for (int col=0;col<d;col++){
                std::cout<<(cells2Right[row][col] ? '#' : '.');
            }
            std::cout<<"|";
            for (int col=0;col<w;col++){
                std::cout<<(cells2Back[row][col] ? '#' : '.');
            }
            std::cout<<"|";
            for (int col=0;col<d;col++){
                std::cout<<(cells2Left[row][col] ? '#' : '.');
            }
            std::cout<<"\n";
        }
        for (int col=0;col<w;col++){
            std::cout<<"_";
        }
        std::cout<<"\n";
        for (int row=0;row<d;row++){
            for (int col=0;col<w;col++){
                std::cout<<(cells2Bot[row][col] ? '#' : '.');
            }
            std::cout<<"\n";
        }
    }
    std::cout<<"\n";
}

void update_state(){
    // Suboptimal if any of the dimensions is 1
    // But then performance impact is negligible

    // Duplicating every array and using an oscillator is a tradeoff of memory for speed
    if (oscillator){
        // FRONT
        // Front: center area
        for (int row=1;row<h_1;row++){
            for (int col=1;col<w_1;col++){
                neighbour_count=(cells1Front[row-1][col-1]+cells1Front[row-1][col]+cells1Front[row-1][col+1]+
                                cells1Front[row][col-1]+                            cells1Front[row][col+1]+
                                cells1Front[row+1][col-1]+cells1Front[row+1][col]+cells1Front[row+1][col+1]);
                cells2Front[row][col]=(neighbour_count==3 || (neighbour_count+cells1Front[row][col])==3);
            }
        }
        // Front: top area
        for (int col=1;col<w_1;col++){
            neighbour_count=(cells1Top[d_1][col-1]+cells1Top[d_1][col]+cells1Top[d_1][col+1]+
                            cells1Front[0][col-1]+                              cells1Front[0][col+1]+
                            cells1Front[1][col-1]+cells1Front[1][col]+cells1Front[1][col+1]);
            cells2Front[0][col]=(neighbour_count==3 || (neighbour_count+cells1Front[0][col])==3);
        }
        // Front: bottom area
        for (int col=1;col<w_1;col++){
            neighbour_count=(cells1Front[h_2][col-1]+cells1Front[h_2][col]+cells1Front[h_2][col+1]+
                            cells1Front[h_1][col-1]+                               cells1Front[h_1][col+1]+
                            cells1Bot[0][col-1]+cells1Bot[0][col]+cells1Bot[0][col+1]);
            cells2Front[h_1][col]=(neighbour_count==3 || (neighbour_count+cells1Front[h_1][col])==3);
        }
        // Front: left area
        for (int row=1;row<h_1;row++){
            neighbour_count=(cells1Left[row-1][d_1]+cells1Front[row-1][0]+cells1Front[row-1][1]+
                                cells1Left[row][d_1]+                            cells1Front[row][1]+
                                cells1Left[row+1][d_1]+cells1Front[row+1][0]+cells1Front[row+1][1]);
            cells2Front[row][0]=(neighbour_count==3 || (neighbour_count+cells1Front[row][0])==3);
        }
        // Front: right area
        for (int row=1;row<h_1;row++){
            neighbour_count=(cells1Front[row-1][w_2]+cells1Front[row-1][w_1]+cells1Right[row-1][0]+
                                cells1Front[row][w_2]+                            cells1Right[row][0]+
                                cells1Front[row+1][w_2]+cells1Front[row+1][w_1]+cells1Right[row+1][0]);
            cells2Front[row][w_1]=(neighbour_count==3 || (neighbour_count+cells1Front[row][w_1])==3);
        }
        // Front: corners ...
        //      tl
        neighbour_count=(                           cells1Top[d_1][0]+cells1Top[d_1][1]+
                            cells1Left[0][d_1]+                     cells1Front[0][1]+
                            cells1Left[1][d_1]+cells1Front[1][0]+cells1Front[1][1]);
        cells2Front[0][0]=(neighbour_count==3 || (neighbour_count+cells1Front[0][0])==3);
        
        //      tr
        neighbour_count=(cells1Top[d_1][w_2]+cells1Top[d_1][w_1]+
                        cells1Front[0][w_2]+                    cells1Right[0][0]+
                        cells1Front[1][w_2]+cells1Front[1][w_1]+cells1Right[1][0]);
        cells2Front[0][w_1]=(neighbour_count==3 || (neighbour_count+cells1Front[0][w_1])==3);

        //      bl
        neighbour_count=(cells1Left[h_2][d_1]+cells1Front[h_2][0]+cells1Front[h_2][1]+
                        cells1Left[h_1][d_1]+                     cells1Front[h_1][1]+
                                                    cells1Bot[0][0]+cells1Bot[0][1]);
        cells2Front[h_1][0]=(neighbour_count==3 || (neighbour_count+cells1Front[h_1][0])==3);

        //      br
        neighbour_count=(cells1Front[h_2][w_2]+cells1Front[h_2][w_1]+cells1Right[h_2][0]+
                        cells1Front[h_1][w_2]+                       cells1Right[h_1][0]+
                        cells1Bot[0][w_2]+      cells1Bot[0][w_1]        );
        cells2Front[h_1][w_1]=(neighbour_count==3 || (neighbour_count+cells1Front[h_1][w_1])==3);

        // BACK
        // Back: center area
        for (int row=1;row<h_1;row++){
            for (int col=1;col<w_1;col++){
                neighbour_count=(cells1Back[row-1][col-1]+cells1Back[row-1][col]+cells1Back[row-1][col+1]+
                                cells1Back[row][col-1]+                            cells1Back[row][col+1]+
                                cells1Back[row+1][col-1]+cells1Back[row+1][col]+cells1Back[row+1][col+1]);
                cells2Back[row][col]=(neighbour_count==3 || (neighbour_count+cells1Back[row][col])==3);
            }
        }

        // Back: top area
        for (int col=1;col<w_1;col++){
            neighbour_count=(cells1Top[0][w_1-col+1]+cells1Top[0][w_1-col]+cells1Top[0][w_1-col-1]+
                            cells1Back[0][col-1]+                            cells1Back[0][col+1]+
                            cells1Back[1][col-1]+cells1Back[1][col]+cells1Back[1][col+1]);
            cells2Back[0][col]=(neighbour_count==3 || (neighbour_count+cells1Back[0][col])==3);
        }

        // Back: bottom area
        for (int col=1;col<w_1;col++){
            neighbour_count=(cells1Back[h_2][col-1]+cells1Back[h_2][col]+cells1Back[h_2][col+1]+
                            cells1Back[h_1][col-1]+                            cells1Back[h_1][col+1]+
                            cells1Bot[d_1][w_1-col+1]+cells1Bot[d_1][w_1-col]+cells1Bot[d_1][w_1-col-1]);
            cells2Back[h_1][col]=(neighbour_count==3 || (neighbour_count+cells1Back[h_1][col])==3);
        }

        // Back: left area
        for (int row=1;row<h_1;row++){
            neighbour_count=(cells1Right[row-1][d_1]+cells1Back[row-1][0]+cells1Back[row-1][1]+
                            cells1Right[row][d_1]+                            cells1Back[row][1]+
                            cells1Right[row+1][d_1]+cells1Back[row+1][0]+cells1Back[row+1][1]);
            cells2Back[row][0]=(neighbour_count==3 || (neighbour_count+cells1Back[row][0])==3);
        }

        // Back: right area
        for (int row=1;row<h_1;row++){
            neighbour_count=(cells1Back[row-1][w_2]+cells1Back[row-1][w_1]+cells1Left[row-1][0]+
                            cells1Back[row][w_2]+                            cells1Left[row][0]+
                            cells1Back[row+1][w_2]+cells1Back[row+1][w_1]+cells1Left[row+1][0]);
            cells2Back[row][w_1]=(neighbour_count==3 || (neighbour_count+cells1Back[row][w_1])==3);
        }

        // Back: corners
        //      tl
        neighbour_count=(           cells1Top[0][w_1]+cells1Top[0][w_2]+
                        cells1Right[0][d_1]+                            cells1Back[0][1]+
                        cells1Right[1][d_1]+cells1Back[1][0]+cells1Back[1][1]);
        cells2Back[0][0]=(neighbour_count==3 || (neighbour_count+cells1Back[0][0])==3);

        //      tr
        neighbour_count=(cells1Top[0][1]+cells1Top[0][0]+
                        cells1Back[0][w_2]+                            cells1Left[0][0]+
                        cells1Back[1][w_2]+cells1Back[1][w_1]+cells1Left[1][0]);
        cells2Back[0][w_1]=(neighbour_count==3 || (neighbour_count+cells1Back[0][w_1])==3);

        //      bl
        neighbour_count=(cells1Right[h_2][d_1]+cells1Back[h_2][0]+cells1Back[h_2][1]+
                        cells1Right[h_1][d_1]+                            cells1Back[h_1][1]+
                                            cells1Bot[d_1][w_1]+cells1Bot[d_1][w_2]);
        cells2Back[h_1][0]=(neighbour_count==3 || (neighbour_count+cells1Back[h_1][0])==3);

        //      br
        neighbour_count=(cells1Back[h_2][w_2]+cells1Back[h_2][w_1]+cells1Left[h_2][0]+
                        cells1Back[h_1][w_2]+                            cells1Left[h_1][0]+
                        cells1Bot[d_1][0]+cells1Bot[d_1][1]           );
        cells2Back[h_1][w_1]=(neighbour_count==3 || (neighbour_count+cells1Back[h_1][w_1])==3);

        // LEFT
        // Left: center area
        for (int row=1;row<h_1;row++){
            for (int col=1;col<d_1;col++){
                neighbour_count=(cells1Left[row-1][col-1]+cells1Left[row-1][col]+cells1Left[row-1][col+1]+
                                cells1Left[row][col-1]+                            cells1Left[row][col+1]+
                                cells1Left[row+1][col-1]+cells1Left[row+1][col]+cells1Left[row+1][col+1]);
                cells2Left[row][col]=(neighbour_count==3 || (neighbour_count+cells1Left[row][col])==3);
            }
        }

        // Left: top area
        for (int col=1;col<d_1;col++){
            neighbour_count=(cells1Top[col-1][0]+cells1Top[col][0]+cells1Top[col+1][0]+
                                cells1Left[0][col-1]+                            cells1Left[0][col+1]+
                                cells1Left[1][col-1]+cells1Left[1][col]+cells1Left[1][col+1]);
                cells2Left[0][col]=(neighbour_count==3 || (neighbour_count+cells1Left[0][col])==3);
        }

        // Left: bottom area
        for (int col=1;col<d_1;col++){
            neighbour_count=(cells1Left[h_2][col-1]+cells1Left[h_2][col]+cells1Left[h_2][col+1]+
                                cells1Left[h_1][col-1]+                            cells1Left[h_1][col+1]+
                                cells1Bot[d-col][0]+cells1Bot[d_1-col][0]+cells1Bot[d_2-col][0]);
            cells2Left[h_1][col]=(neighbour_count==3 || (neighbour_count+cells1Left[h_1][col])==3);
        }

        // Left: left area
        for (int row=1;row<h_1;row++){
            neighbour_count=(cells1Back[row-1][w_1]+cells1Left[row-1][0]+cells1Left[row-1][1]+
                            cells1Back[row][w_1]+                            cells1Left[row][1]+
                            cells1Back[row+1][w_1]+cells1Left[row+1][0]+cells1Left[row+1][1]);
            cells2Left[row][0]=(neighbour_count==3 || (neighbour_count+cells1Left[row][0])==3);
        }

        // Left: right area
        for (int row=1;row<h_1;row++){
            neighbour_count=(cells1Left[row-1][d_2]+cells1Left[row-1][d_1]+cells1Front[row-1][0]+
                            cells1Left[row][d_2]+                            cells1Front[row][0]+
                            cells1Left[row+1][d_2]+cells1Left[row+1][d_1]+cells1Front[row+1][0]);
            cells2Left[row][d_1]=(neighbour_count==3 || (neighbour_count+cells1Left[row][d_1])==3);
        }

        // Left: corners
        //      tl
        neighbour_count=(           cells1Top[0][0]+cells1Top[1][0]+
                        cells1Back[0][w_1]+                            cells1Left[0][1]+
                        cells1Back[1][w_1]+cells1Left[1][0]+cells1Left[1][1]);
        cells2Left[0][0]=(neighbour_count==3 || (neighbour_count+cells1Left[0][0])==3);

        //      tr
        neighbour_count=(cells1Top[d_2][0]+cells1Top[d_1][0]+
                        cells1Left[0][d_2]+                            cells1Front[0][0]+
                        cells1Left[1][d_2]+cells1Left[1][d_1]+cells1Front[1][0]);
        cells2Left[0][d_1]=(neighbour_count==3 || (neighbour_count+cells1Left[0][d_1])==3);

        //      bl
        neighbour_count=(cells1Back[h_2][w_1]+cells1Left[h_2][0]+cells1Left[h_2][1]+
                        cells1Back[h_1][w_1]+                            cells1Left[h_1][1]+
                                    cells1Bot[d_1][0]+cells1Bot[d_2][0]);
        cells2Left[h_1][0]=(neighbour_count==3 || (neighbour_count+cells1Left[h_1][0])==3);

        //      br
        neighbour_count=(cells1Left[h_2][d_2]+cells1Left[h_2][d_1]+cells1Front[h_2][0]+
                        cells1Left[h_1][d_2]+                            cells1Front[h_1][0]+
                        cells1Bot[1][0]+cells1Bot[0][0]           );
        cells2Left[h_1][d_1]=(neighbour_count==3 || (neighbour_count+cells1Left[h_1][d_1])==3);

        // RIGHT
        // Right: center area
        for (int row=1;row<h_1;row++){
            for (int col=1;col<d_1;col++){
                neighbour_count=(cells1Right[row-1][col-1]+cells1Right[row-1][col]+cells1Right[row-1][col+1]+
                                cells1Right[row][col-1]+                            cells1Right[row][col+1]+
                                cells1Right[row+1][col-1]+cells1Right[row+1][col]+cells1Right[row+1][col+1]);
                cells2Right[row][col]=(neighbour_count==3 || (neighbour_count+cells1Right[row][col])==3);
            }
        }

        // Right: top area
        for (int col=1;col<d_1;col++){
            neighbour_count=(cells1Top[d-col][w_1]+cells1Top[d_1-col][w_1]+cells1Top[d_2-col][w_1]+
                            cells1Right[0][col-1]+                            cells1Right[0][col+1]+
                            cells1Right[1][col-1]+cells1Right[1][col]+cells1Right[1][col+1]);
            cells2Right[0][col]=(neighbour_count==3 || (neighbour_count+cells1Right[0][col])==3);
        }
        // Right: bottom area
        for (int col=1;col<d_1;col++){
            neighbour_count=(cells1Right[h_2][col-1]+cells1Right[h_2][col]+cells1Right[h_2][col+1]+
                            cells1Right[h_1][col-1]+                            cells1Right[h_1][col+1]+
                            cells1Bot[col-1][w_1]+cells1Bot[col][w_1]+cells1Bot[col+1][w_1]);
            cells2Right[h_1][col]=(neighbour_count==3 || (neighbour_count+cells1Right[h_1][col])==3);
        }

        // Right: left area
        for (int row=1;row<h_1;row++){
            neighbour_count=(cells1Front[row-1][w_1]+cells1Right[row-1][0]+cells1Right[row-1][1]+
                            cells1Front[row][w_1]+                            cells1Right[row][1]+
                            cells1Front[row+1][w_1]+cells1Right[row+1][0]+cells1Right[row+1][1]);
            cells2Right[row][0]=(neighbour_count==3 || (neighbour_count+cells1Right[row][0])==3);
        }

        // Right: right area
        for (int row=1;row<h_1;row++){
            neighbour_count=(cells1Right[row-1][d_2]+cells1Right[row-1][d_1]+cells1Back[row-1][0]+
                            cells1Right[row][d_2]+                            cells1Back[row][0]+
                            cells1Right[row+1][d_2]+cells1Right[row+1][d_1]+cells1Back[row+1][0]);
            cells2Right[row][d_1]=(neighbour_count==3 || (neighbour_count+cells1Right[row][d_1])==3);
        }

        // Right: corners
        //      tl
        neighbour_count=(           cells1Top[d_1][w_1]+cells1Top[d_2][w_1]+
                        cells1Front[0][w_1]+                            cells1Right[0][1]+
                        cells1Front[1][w_1]+cells1Right[1][0]+cells1Right[1][1]);
        cells2Right[0][0]=(neighbour_count==3 || (neighbour_count+cells1Right[0][0])==3);

        //      tr
        neighbour_count=(cells1Top[1][w_1]+cells1Top[0][w_1]+
                        cells1Right[0][d_2]+                            cells1Back[0][0]+
                        cells1Right[1][d_2]+cells1Right[1][d_1]+cells1Back[1][0]);
        cells2Right[0][d_1]=(neighbour_count==3 || (neighbour_count+cells1Right[0][d_1])==3);

        //      bl
        neighbour_count=(cells1Front[h_2][w_1]+cells1Right[h_2][0]+cells1Right[h_2][1]+
                        cells1Front[h_1][w_1]+                            cells1Right[h_1][1]+
                                    cells1Bot[0][w_1]+cells1Bot[1][w_1]);
        cells2Right[h_1][0]=(neighbour_count==3 || (neighbour_count+cells1Right[h_1][0])==3);

        //      br
        neighbour_count=(cells1Right[h_2][d_2]+cells1Right[h_2][d_1]+cells1Back[h_2][0]+
                        cells1Right[h_1][d_2]+                            cells1Back[h_1][0]+
                        cells1Bot[d_2][w_1]+cells1Bot[d_1][w_1]           );
        cells2Right[h_1][d_1]=(neighbour_count==3 || (neighbour_count+cells1Right[h_1][d_1])==3);

        // TOP
        // Top: center area
        for (int row=1;row<d_1;row++){
            for (int col=1;col<w_1;col++){
                neighbour_count=(cells1Top[row-1][col-1]+cells1Top[row-1][col]+cells1Top[row-1][col+1]+
                                cells1Top[row][col-1]+                            cells1Top[row][col+1]+
                                cells1Top[row+1][col-1]+cells1Top[row+1][col]+cells1Top[row+1][col+1]);
                cells2Top[row][col]=(neighbour_count==3 || (neighbour_count+cells1Top[row][col])==3);
            }
        }

        // Top: top area
        for (int col=1;col<w_1;col++){
            neighbour_count=(cells1Back[0][w-col]+cells1Back[0][w_1-col]+cells1Back[0][w_2-col]+
                            cells1Top[0][col-1]+                            cells1Top[0][col+1]+
                            cells1Top[1][col-1]+cells1Top[1][col]+cells1Top[1][col+1]);
            cells2Top[0][col]=(neighbour_count==3 || (neighbour_count+cells1Top[0][col])==3);
        }

        // Top: bottom area
        for (int col=1;col<w_1;col++){
            neighbour_count=(cells1Top[d_2][col-1]+cells1Top[d_2][col]+cells1Top[d_2][col+1]+
                            cells1Top[d_1][col-1]+                            cells1Top[d_1][col+1]+
                            cells1Front[0][col-1]+cells1Front[0][col]+cells1Front[0][col+1]);
            cells2Top[d_1][col]=(neighbour_count==3 || (neighbour_count+cells1Top[d_1][col])==3);
        }

        // Top: left area
        for (int row=1;row<d_1;row++){
            neighbour_count=(cells1Left[0][row-1]+cells1Top[row-1][0]+cells1Top[row-1][1]+
                            cells1Left[0][row]+                            cells1Top[row][1]+
                            cells1Left[0][row+1]+cells1Top[row+1][0]+cells1Top[row+1][1]);
            cells2Top[row][0]=(neighbour_count==3 || (neighbour_count+cells1Top[row][0])==3);
        }

        // Top: right area
        for (int row=1;row<d_1;row++){
            neighbour_count=(cells1Top[row-1][w_2]+cells1Top[row-1][w_1]+cells1Right[0][d_2-row]+
                            cells1Top[row][w_2]+                            cells1Right[0][d_1-row]+
                            cells1Top[row+1][w_2]+cells1Top[row+1][w_1]+cells1Right[0][d-row]);
            cells2Top[row][w_1]=(neighbour_count==3 || (neighbour_count+cells1Top[row][w_1])==3);
        }

        // Top: corners
        //      tl
        neighbour_count=(           cells1Back[0][w_1]+cells1Back[0][w_2]+
                        cells1Left[0][0]+                            cells1Top[0][1]+
                        cells1Left[0][1]+cells1Top[1][0]+cells1Top[1][1]);
        cells2Top[0][0]=(neighbour_count==3 || (neighbour_count+cells1Top[0][0])==3);

        //      tr
        neighbour_count=(cells1Back[0][1]+cells1Back[0][0]+
                        cells1Top[0][w_2]+                            cells1Right[0][d_1]+
                        cells1Top[1][w_2]+cells1Top[1][w_1]+cells1Right[0][d_2]);
        cells2Top[0][w_1]=(neighbour_count==3 || (neighbour_count+cells1Top[0][w_1])==3);

        //      bl
        neighbour_count=(cells1Left[0][d_2]+cells1Top[d_2][0]+cells1Top[d_2][1]+
                        cells1Left[0][d_1]+                            cells1Top[d_1][1]+
                                    cells1Front[0][0]+cells1Front[0][1]);
        cells2Top[d_1][0]=(neighbour_count==3 || (neighbour_count+cells1Top[d_1][0])==3);

        //      br
        neighbour_count=(cells1Top[d_2][w_2]+cells1Top[d_2][w_1]+cells1Right[0][1]+
                        cells1Top[d_1][w_2]+                            cells1Right[0][0]+
                        cells1Front[0][w_2]+cells1Front[0][w_1]           );
        cells2Top[d_1][w_1]=(neighbour_count==3 || (neighbour_count+cells1Top[d_1][w_1])==3);

        // BOTTOM
        // Bottom: center area
        for (int row=1;row<d_1;row++){
            for (int col=1;col<w_1;col++){
                neighbour_count=(cells1Bot[row-1][col-1]+cells1Bot[row-1][col]+cells1Bot[row-1][col+1]+
                                cells1Bot[row][col-1]+                            cells1Bot[row][col+1]+
                                cells1Bot[row+1][col-1]+cells1Bot[row+1][col]+cells1Bot[row+1][col+1]);
                cells2Bot[row][col]=(neighbour_count==3 || (neighbour_count+cells1Bot[row][col])==3);
            }
        }

        // Bottom: top area
        for (int col=1;col<w_1;col++){
            neighbour_count=(cells1Front[h_1][col-1]+cells1Front[h_1][col]+cells1Front[h_1][col+1]+
                            cells1Bot[0][col-1]+                            cells1Bot[0][col+1]+
                            cells1Bot[1][col-1]+cells1Bot[1][col]+cells1Bot[1][col+1]);
            cells2Bot[0][col]=(neighbour_count==3 || (neighbour_count+cells1Bot[0][col])==3);
        }

        // Bottom: bottom area
        for (int col=1;col<w_1;col++){
            neighbour_count=(cells1Bot[d_2][col-1]+cells1Bot[d_2][col]+cells1Bot[d_2][col+1]+
                            cells1Bot[d_1][col-1]+                            cells1Bot[d_1][col+1]+
                            cells1Back[h_1][w_2-col]+cells1Back[h_1][w_1-col]+cells1Back[h_1][w-col]);
            cells2Bot[d_1][col]=(neighbour_count==3 || (neighbour_count+cells1Bot[d_1][col])==3);
        }

        // Bottom: left area
        for (int row=1;row<d_1;row++){
            neighbour_count=(cells1Left[h_1][d-row]+cells1Bot[row-1][0]+cells1Bot[row-1][1]+
                            cells1Left[h_1][d_1-row]+                            cells1Bot[row][1]+
                            cells1Left[h_1][d_2-row]+cells1Bot[row+1][0]+cells1Bot[row+1][1]);
            cells2Bot[row][0]=(neighbour_count==3 || (neighbour_count+cells1Bot[row][0])==3);
        }

        // Bottom: right area
        for (int row=1;row<d_1;row++){
            neighbour_count=(cells1Bot[row-1][w_2]+cells1Bot[row-1][w_1]+cells1Right[h_1][row-1]+
                            cells1Bot[row][w_2]+                            cells1Right[h_1][row]+
                            cells1Bot[row+1][w_2]+cells1Bot[row+1][w_1]+cells1Right[h_1][row+1]);
            cells2Bot[row][w_1]=(neighbour_count==3 || (neighbour_count+cells1Bot[row][w_1])==3);
        }

        // Bottom: corners
        //      tl
        neighbour_count=(           cells1Front[h_1][0]+cells1Front[h_1][1]+
                        cells1Left[h_1][d_1]+                            cells1Bot[0][1]+
                        cells1Left[h_1][d_2]+cells1Bot[1][0]+cells1Bot[1][1]);
        cells2Bot[0][0]=(neighbour_count==3 || (neighbour_count+cells1Bot[0][0])==3);

        //      tr
        neighbour_count=(cells1Front[h_1][w_2]+cells1Front[h_1][w_1]+
                        cells1Bot[0][w_2]+                            cells1Right[h_1][0]+
                        cells1Bot[1][w_2]+cells1Bot[1][w_1]+cells1Right[h_1][1]);
        cells2Bot[0][w_1]=(neighbour_count==3 || (neighbour_count+cells1Bot[0][w_1])==3);

        //      bl
        neighbour_count=(cells1Left[h_1][1]+cells1Bot[d_2][0]+cells1Bot[d_2][1]+
                        cells1Left[h_1][0]+                            cells1Bot[d_1][1]+
                                    cells1Back[h_1][w_1]+cells1Back[h_1][w_2]);
        cells2Bot[d_1][0]=(neighbour_count==3 || (neighbour_count+cells1Bot[d_1][0])==3);

        //      br
        neighbour_count=(cells1Bot[d_2][w_2]+cells1Bot[d_2][w_1]+cells1Right[h_1][0]+
                        cells1Bot[d_1][w_2]+                            cells1Right[h_1][1]+
                        cells1Back[h_1][1]+cells1Back[h_1][0]           );
        cells2Bot[d_1][w_1]=(neighbour_count==3 || (neighbour_count+cells1Bot[d_1][w_1])==3);
        
    } else {
        // FRONT
        // Front: center area
        for (int row=1;row<h_1;row++){
            for (int col=1;col<w_1;col++){
                neighbour_count=(cells2Front[row-1][col-1]+cells2Front[row-1][col]+cells2Front[row-1][col+1]+
                                cells2Front[row][col-1]+                            cells2Front[row][col+1]+
                                cells2Front[row+1][col-1]+cells2Front[row+1][col]+cells2Front[row+1][col+1]);
                cells1Front[row][col]=(neighbour_count==3 || (neighbour_count+cells2Front[row][col])==3);
            }
        }
        // Front: top area
        for (int col=1;col<w_1;col++){
            neighbour_count=(cells2Top[d_1][col-1]+cells2Top[d_1][col]+cells2Top[d_1][col+1]+
                            cells2Front[0][col-1]+                              cells2Front[0][col+1]+
                            cells2Front[1][col-1]+cells2Front[1][col]+cells2Front[1][col+1]);
            cells1Front[0][col]=(neighbour_count==3 || (neighbour_count+cells2Front[0][col])==3);
        }
        // Front: bottom area
        for (int col=1;col<w_1;col++){
            neighbour_count=(cells2Front[h_2][col-1]+cells2Front[h_2][col]+cells2Front[h_2][col+1]+
                            cells2Front[h_1][col-1]+                               cells2Front[h_1][col+1]+
                            cells2Bot[0][col-1]+cells2Bot[0][col]+cells2Bot[0][col+1]);
            cells1Front[h_1][col]=(neighbour_count==3 || (neighbour_count+cells2Front[h_1][col])==3);
        }
        // Front: left area
        for (int row=1;row<h_1;row++){
            neighbour_count=(cells2Left[row-1][d_1]+cells2Front[row-1][0]+cells2Front[row-1][1]+
                                cells2Left[row][d_1]+                            cells2Front[row][1]+
                                cells2Left[row+1][d_1]+cells2Front[row+1][0]+cells2Front[row+1][1]);
            cells1Front[row][0]=(neighbour_count==3 || (neighbour_count+cells2Front[row][0])==3);
        }
        // Front: right area
        for (int row=1;row<h_1;row++){
            neighbour_count=(cells2Front[row-1][w_2]+cells2Front[row-1][w_1]+cells2Right[row-1][0]+
                                cells2Front[row][w_2]+                            cells2Right[row][0]+
                                cells2Front[row+1][w_2]+cells2Front[row+1][w_1]+cells2Right[row+1][0]);
            cells1Front[row][w_1]=(neighbour_count==3 || (neighbour_count+cells2Front[row][w_1])==3);
        }
        // Front: corners ...
        //      tl
        neighbour_count=(                           cells2Top[d_1][0]+cells2Top[d_1][1]+
                            cells2Left[0][d_1]+                     cells2Front[0][1]+
                            cells2Left[1][d_1]+cells2Front[1][0]+cells2Front[1][1]);
        cells1Front[0][0]=(neighbour_count==3 || (neighbour_count+cells2Front[0][0])==3);
        
        //      tr
        neighbour_count=(cells2Top[d_1][w_2]+cells2Top[d_1][w_1]+
                        cells2Front[0][w_2]+                    cells2Right[0][0]+
                        cells2Front[1][w_2]+cells2Front[1][w_1]+cells2Right[1][0]);
        cells1Front[0][w_1]=(neighbour_count==3 || (neighbour_count+cells2Front[0][w_1])==3);

        //      bl
        neighbour_count=(cells2Left[h_2][d_1]+cells2Front[h_2][0]+cells2Front[h_2][1]+
                        cells2Left[h_1][d_1]+                     cells2Front[h_1][1]+
                                                    cells2Bot[0][0]+cells2Bot[0][1]);
        cells1Front[h_1][0]=(neighbour_count==3 || (neighbour_count+cells2Front[h_1][0])==3);

        //      br
        neighbour_count=(cells2Front[h_2][w_2]+cells2Front[h_2][w_1]+cells2Right[h_2][0]+
                        cells2Front[h_1][w_2]+                       cells2Right[h_1][0]+
                        cells2Bot[0][w_2]+      cells2Bot[0][w_1]        );
        cells1Front[h_1][w_1]=(neighbour_count==3 || (neighbour_count+cells2Front[h_1][w_1])==3);

        // BACK
        // Back: center area
        for (int row=1;row<h_1;row++){
            for (int col=1;col<w_1;col++){
                neighbour_count=(cells2Back[row-1][col-1]+cells2Back[row-1][col]+cells2Back[row-1][col+1]+
                                cells2Back[row][col-1]+                            cells2Back[row][col+1]+
                                cells2Back[row+1][col-1]+cells2Back[row+1][col]+cells2Back[row+1][col+1]);
                cells1Back[row][col]=(neighbour_count==3 || (neighbour_count+cells2Back[row][col])==3);
            }
        }

        // Back: top area
        for (int col=1;col<w_1;col++){
            neighbour_count=(cells2Top[0][w_1-col+1]+cells2Top[0][w_1-col]+cells2Top[0][w_1-col-1]+
                            cells2Back[0][col-1]+                            cells2Back[0][col+1]+
                            cells2Back[1][col-1]+cells2Back[1][col]+cells2Back[1][col+1]);
            cells1Back[0][col]=(neighbour_count==3 || (neighbour_count+cells2Back[0][col])==3);
        }

        // Back: bottom area
        for (int col=1;col<w_1;col++){
            neighbour_count=(cells2Back[h_2][col-1]+cells2Back[h_2][col]+cells2Back[h_2][col+1]+
                            cells2Back[h_1][col-1]+                            cells2Back[h_1][col+1]+
                            cells2Bot[d_1][w_1-col+1]+cells2Bot[d_1][w_1-col]+cells2Bot[d_1][w_1-col-1]);
            cells1Back[h_1][col]=(neighbour_count==3 || (neighbour_count+cells2Back[h_1][col])==3);
        }

        // Back: left area
        for (int row=1;row<h_1;row++){
            neighbour_count=(cells2Right[row-1][d_1]+cells2Back[row-1][0]+cells2Back[row-1][1]+
                            cells2Right[row][d_1]+                            cells2Back[row][1]+
                            cells2Right[row+1][d_1]+cells2Back[row+1][0]+cells2Back[row+1][1]);
            cells1Back[row][0]=(neighbour_count==3 || (neighbour_count+cells2Back[row][0])==3);
        }

        // Back: right area
        for (int row=1;row<h_1;row++){
            neighbour_count=(cells2Back[row-1][w_2]+cells2Back[row-1][w_1]+cells2Left[row-1][0]+
                            cells2Back[row][w_2]+                            cells2Left[row][0]+
                            cells2Back[row+1][w_2]+cells2Back[row+1][w_1]+cells2Left[row+1][0]);
            cells1Back[row][w_1]=(neighbour_count==3 || (neighbour_count+cells2Back[row][w_1])==3);
        }

        // Back: corners
        //      tl
        neighbour_count=(           cells2Top[0][w_1]+cells2Top[0][w_2]+
                        cells2Right[0][d_1]+                            cells2Back[0][1]+
                        cells2Right[1][d_1]+cells2Back[1][0]+cells2Back[1][1]);
        cells1Back[0][0]=(neighbour_count==3 || (neighbour_count+cells2Back[0][0])==3);

        //      tr
        neighbour_count=(cells2Top[0][1]+cells2Top[0][0]+
                        cells2Back[0][w_2]+                            cells2Left[0][0]+
                        cells2Back[1][w_2]+cells2Back[1][w_1]+cells2Left[1][0]);
        cells1Back[0][w_1]=(neighbour_count==3 || (neighbour_count+cells2Back[0][w_1])==3);

        //      bl
        neighbour_count=(cells2Right[h_2][d_1]+cells2Back[h_2][0]+cells2Back[h_2][1]+
                        cells2Right[h_1][d_1]+                            cells2Back[h_1][1]+
                                            cells2Bot[d_1][w_1]+cells2Bot[d_1][w_2]);
        cells1Back[h_1][0]=(neighbour_count==3 || (neighbour_count+cells2Back[h_1][0])==3);

        //      br
        neighbour_count=(cells2Back[h_2][w_2]+cells2Back[h_2][w_1]+cells2Left[h_2][0]+
                        cells2Back[h_1][w_2]+                            cells2Left[h_1][0]+
                        cells2Bot[d_1][0]+cells2Bot[d_1][1]           );
        cells1Back[h_1][w_1]=(neighbour_count==3 || (neighbour_count+cells2Back[h_1][w_1])==3);

        // LEFT
        // Left: center area
        for (int row=1;row<h_1;row++){
            for (int col=1;col<d_1;col++){
                neighbour_count=(cells2Left[row-1][col-1]+cells2Left[row-1][col]+cells2Left[row-1][col+1]+
                                cells2Left[row][col-1]+                            cells2Left[row][col+1]+
                                cells2Left[row+1][col-1]+cells2Left[row+1][col]+cells2Left[row+1][col+1]);
                cells1Left[row][col]=(neighbour_count==3 || (neighbour_count+cells2Left[row][col])==3);
            }
        }

        // Left: top area
        for (int col=1;col<d_1;col++){
            neighbour_count=(cells2Top[col-1][0]+cells2Top[col][0]+cells2Top[col+1][0]+
                                cells2Left[0][col-1]+                            cells2Left[0][col+1]+
                                cells2Left[1][col-1]+cells2Left[1][col]+cells2Left[1][col+1]);
                cells1Left[0][col]=(neighbour_count==3 || (neighbour_count+cells2Left[0][col])==3);
        }

        // Left: bottom area
        for (int col=1;col<d_1;col++){
            neighbour_count=(cells2Left[h_2][col-1]+cells2Left[h_2][col]+cells2Left[h_2][col+1]+
                                cells2Left[h_1][col-1]+                            cells2Left[h_1][col+1]+
                                cells2Bot[d-col][0]+cells2Bot[d_1-col][0]+cells2Bot[d_2-col][0]);
            cells1Left[h_1][col]=(neighbour_count==3 || (neighbour_count+cells2Left[h_1][col])==3);
        }

        // Left: left area
        for (int row=1;row<h_1;row++){
            neighbour_count=(cells2Back[row-1][w_1]+cells2Left[row-1][0]+cells2Left[row-1][1]+
                            cells2Back[row][w_1]+                            cells2Left[row][1]+
                            cells2Back[row+1][w_1]+cells2Left[row+1][0]+cells2Left[row+1][1]);
            cells1Left[row][0]=(neighbour_count==3 || (neighbour_count+cells2Left[row][0])==3);
        }

        // Left: right area
        for (int row=1;row<h_1;row++){
            neighbour_count=(cells2Left[row-1][d_2]+cells2Left[row-1][d_1]+cells2Front[row-1][0]+
                            cells2Left[row][d_2]+                            cells2Front[row][0]+
                            cells2Left[row+1][d_2]+cells2Left[row+1][d_1]+cells2Front[row+1][0]);
            cells1Left[row][d_1]=(neighbour_count==3 || (neighbour_count+cells2Left[row][d_1])==3);
        }

        // Left: corners
        //      tl
        neighbour_count=(           cells2Top[0][0]+cells2Top[1][0]+
                        cells2Back[0][w_1]+                            cells2Left[0][1]+
                        cells2Back[1][w_1]+cells2Left[1][0]+cells2Left[1][1]);
        cells1Left[0][0]=(neighbour_count==3 || (neighbour_count+cells2Left[0][0])==3);

        //      tr
        neighbour_count=(cells2Top[d_2][0]+cells2Top[d_1][0]+
                        cells2Left[0][d_2]+                            cells2Front[0][0]+
                        cells2Left[1][d_2]+cells2Left[1][d_1]+cells2Front[1][0]);
        cells1Left[0][d_1]=(neighbour_count==3 || (neighbour_count+cells2Left[0][d_1])==3);

        //      bl
        neighbour_count=(cells2Back[h_2][w_1]+cells2Left[h_2][0]+cells2Left[h_2][1]+
                        cells2Back[h_1][w_1]+                            cells2Left[h_1][1]+
                                    cells2Bot[d_1][0]+cells2Bot[d_2][0]);
        cells1Left[h_1][0]=(neighbour_count==3 || (neighbour_count+cells2Left[h_1][0])==3);

        //      br
        neighbour_count=(cells2Left[h_2][d_2]+cells2Left[h_2][d_1]+cells2Front[h_2][0]+
                        cells2Left[h_1][d_2]+                            cells2Front[h_1][0]+
                        cells2Bot[1][0]+cells2Bot[0][0]           );
        cells1Left[h_1][d_1]=(neighbour_count==3 || (neighbour_count+cells2Left[h_1][d_1])==3);

        // RIGHT
        // Right: center area
        for (int row=1;row<h_1;row++){
            for (int col=1;col<d_1;col++){
                neighbour_count=(cells2Right[row-1][col-1]+cells2Right[row-1][col]+cells2Right[row-1][col+1]+
                                cells2Right[row][col-1]+                            cells2Right[row][col+1]+
                                cells2Right[row+1][col-1]+cells2Right[row+1][col]+cells2Right[row+1][col+1]);
                cells1Right[row][col]=(neighbour_count==3 || (neighbour_count+cells2Right[row][col])==3);
            }
        }

        // Right: top area
        for (int col=1;col<d_1;col++){
            neighbour_count=(cells2Top[d-col][w_1]+cells2Top[d_1-col][w_1]+cells2Top[d_2-col][w_1]+
                            cells2Right[0][col-1]+                            cells2Right[0][col+1]+
                            cells2Right[1][col-1]+cells2Right[1][col]+cells2Right[1][col+1]);
            cells1Right[0][col]=(neighbour_count==3 || (neighbour_count+cells2Right[0][col])==3);
        }
        // Right: bottom area
        for (int col=1;col<d_1;col++){
            neighbour_count=(cells2Right[h_2][col-1]+cells2Right[h_2][col]+cells2Right[h_2][col+1]+
                            cells2Right[h_1][col-1]+                            cells2Right[h_1][col+1]+
                            cells2Bot[col-1][w_1]+cells2Bot[col][w_1]+cells2Bot[col+1][w_1]);
            cells1Right[h_1][col]=(neighbour_count==3 || (neighbour_count+cells2Right[h_1][col])==3);
        }

        // Right: left area
        for (int row=1;row<h_1;row++){
            neighbour_count=(cells2Front[row-1][w_1]+cells2Right[row-1][0]+cells2Right[row-1][1]+
                            cells2Front[row][w_1]+                            cells2Right[row][1]+
                            cells2Front[row+1][w_1]+cells2Right[row+1][0]+cells2Right[row+1][1]);
            cells1Right[row][0]=(neighbour_count==3 || (neighbour_count+cells2Right[row][0])==3);
        }

        // Right: right area
        for (int row=1;row<h_1;row++){
            neighbour_count=(cells2Right[row-1][d_2]+cells2Right[row-1][d_1]+cells2Back[row-1][0]+
                            cells2Right[row][d_2]+                            cells2Back[row][0]+
                            cells2Right[row+1][d_2]+cells2Right[row+1][d_1]+cells2Back[row+1][0]);
            cells1Right[row][d_1]=(neighbour_count==3 || (neighbour_count+cells2Right[row][d_1])==3);
        }

        // Right: corners
        //      tl
        neighbour_count=(           cells2Top[d_1][w_1]+cells2Top[d_2][w_1]+
                        cells2Front[0][w_1]+                            cells2Right[0][1]+
                        cells2Front[1][w_1]+cells2Right[1][0]+cells2Right[1][1]);
        cells1Right[0][0]=(neighbour_count==3 || (neighbour_count+cells2Right[0][0])==3);

        //      tr
        neighbour_count=(cells2Top[1][w_1]+cells2Top[0][w_1]+
                        cells2Right[0][d_2]+                            cells2Back[0][0]+
                        cells2Right[1][d_2]+cells2Right[1][d_1]+cells2Back[1][0]);
        cells1Right[0][d_1]=(neighbour_count==3 || (neighbour_count+cells2Right[0][d_1])==3);

        //      bl
        neighbour_count=(cells2Front[h_2][w_1]+cells2Right[h_2][0]+cells2Right[h_2][1]+
                        cells2Front[h_1][w_1]+                            cells2Right[h_1][1]+
                                    cells2Bot[0][w_1]+cells2Bot[1][w_1]);
        cells1Right[h_1][0]=(neighbour_count==3 || (neighbour_count+cells2Right[h_1][0])==3);

        //      br
        neighbour_count=(cells2Right[h_2][d_2]+cells2Right[h_2][d_1]+cells2Back[h_2][0]+
                        cells2Right[h_1][d_2]+                            cells2Back[h_1][0]+
                        cells2Bot[d_2][w_1]+cells2Bot[d_1][w_1]           );
        cells1Right[h_1][d_1]=(neighbour_count==3 || (neighbour_count+cells2Right[h_1][d_1])==3);

        // TOP
        // Top: center area
        for (int row=1;row<d_1;row++){
            for (int col=1;col<w_1;col++){
                neighbour_count=(cells2Top[row-1][col-1]+cells2Top[row-1][col]+cells2Top[row-1][col+1]+
                                cells2Top[row][col-1]+                            cells2Top[row][col+1]+
                                cells2Top[row+1][col-1]+cells2Top[row+1][col]+cells2Top[row+1][col+1]);
                cells1Top[row][col]=(neighbour_count==3 || (neighbour_count+cells2Top[row][col])==3);
            }
        }

        // Top: top area
        for (int col=1;col<w_1;col++){
            neighbour_count=(cells2Back[0][w-col]+cells2Back[0][w_1-col]+cells2Back[0][w_2-col]+
                            cells2Top[0][col-1]+                            cells2Top[0][col+1]+
                            cells2Top[1][col-1]+cells2Top[1][col]+cells2Top[1][col+1]);
            cells1Top[0][col]=(neighbour_count==3 || (neighbour_count+cells2Top[0][col])==3);
        }

        // Top: bottom area
        for (int col=1;col<w_1;col++){
            neighbour_count=(cells2Top[d_2][col-1]+cells2Top[d_2][col]+cells2Top[d_2][col+1]+
                            cells2Top[d_1][col-1]+                            cells2Top[d_1][col+1]+
                            cells2Front[0][col-1]+cells2Front[0][col]+cells2Front[0][col+1]);
            cells1Top[d_1][col]=(neighbour_count==3 || (neighbour_count+cells2Top[d_1][col])==3);
        }

        // Top: left area
        for (int row=1;row<d_1;row++){
            neighbour_count=(cells2Left[0][row-1]+cells2Top[row-1][0]+cells2Top[row-1][1]+
                            cells2Left[0][row]+                            cells2Top[row][1]+
                            cells2Left[0][row+1]+cells2Top[row+1][0]+cells2Top[row+1][1]);
            cells1Top[row][0]=(neighbour_count==3 || (neighbour_count+cells2Top[row][0])==3);
        }

        // Top: right area
        for (int row=1;row<d_1;row++){
            neighbour_count=(cells2Top[row-1][w_2]+cells2Top[row-1][w_1]+cells2Right[0][d_2-row]+
                            cells2Top[row][w_2]+                            cells2Right[0][d_1-row]+
                            cells2Top[row+1][w_2]+cells2Top[row+1][w_1]+cells2Right[0][d-row]);
            cells1Top[row][w_1]=(neighbour_count==3 || (neighbour_count+cells2Top[row][w_1])==3);
        }

        // Top: corners
        //      tl
        neighbour_count=(           cells2Back[0][w_1]+cells2Back[0][w_2]+
                        cells2Left[0][0]+                            cells2Top[0][1]+
                        cells2Left[0][1]+cells2Top[1][0]+cells2Top[1][1]);
        cells1Top[0][0]=(neighbour_count==3 || (neighbour_count+cells2Top[0][0])==3);

        //      tr
        neighbour_count=(cells2Back[0][1]+cells2Back[0][0]+
                        cells2Top[0][w_2]+                            cells2Right[0][d_1]+
                        cells2Top[1][w_2]+cells2Top[1][w_1]+cells2Right[0][d_2]);
        cells1Top[0][w_1]=(neighbour_count==3 || (neighbour_count+cells2Top[0][w_1])==3);

        //      bl
        neighbour_count=(cells2Left[0][d_2]+cells2Top[d_2][0]+cells2Top[d_2][1]+
                        cells2Left[0][d_1]+                            cells2Top[d_1][1]+
                                    cells2Front[0][0]+cells2Front[0][1]);
        cells1Top[d_1][0]=(neighbour_count==3 || (neighbour_count+cells2Top[d_1][0])==3);

        //      br
        neighbour_count=(cells2Top[d_2][w_2]+cells2Top[d_2][w_1]+cells2Right[0][1]+
                        cells2Top[d_1][w_2]+                            cells2Right[0][0]+
                        cells2Front[0][w_2]+cells2Front[0][w_1]           );
        cells1Top[d_1][w_1]=(neighbour_count==3 || (neighbour_count+cells2Top[d_1][w_1])==3);

        // BOTTOM
        // Bottom: center area
        for (int row=1;row<d_1;row++){
            for (int col=1;col<w_1;col++){
                neighbour_count=(cells2Bot[row-1][col-1]+cells2Bot[row-1][col]+cells2Bot[row-1][col+1]+
                                cells2Bot[row][col-1]+                            cells2Bot[row][col+1]+
                                cells2Bot[row+1][col-1]+cells2Bot[row+1][col]+cells2Bot[row+1][col+1]);
                cells1Bot[row][col]=(neighbour_count==3 || (neighbour_count+cells2Bot[row][col])==3);
            }
        }

        // Bottom: top area
        for (int col=1;col<w_1;col++){
            neighbour_count=(cells2Front[h_1][col-1]+cells2Front[h_1][col]+cells2Front[h_1][col+1]+
                            cells2Bot[0][col-1]+                            cells2Bot[0][col+1]+
                            cells2Bot[1][col-1]+cells2Bot[1][col]+cells2Bot[1][col+1]);
            cells1Bot[0][col]=(neighbour_count==3 || (neighbour_count+cells2Bot[0][col])==3);
        }

        // Bottom: bottom area
        for (int col=1;col<w_1;col++){
            neighbour_count=(cells2Bot[d_2][col-1]+cells2Bot[d_2][col]+cells2Bot[d_2][col+1]+
                            cells2Bot[d_1][col-1]+                            cells2Bot[d_1][col+1]+
                            cells2Back[h_1][w_2-col]+cells2Back[h_1][w_1-col]+cells2Back[h_1][w-col]);
            cells1Bot[d_1][col]=(neighbour_count==3 || (neighbour_count+cells2Bot[d_1][col])==3);
        }

        // Bottom: left area
        for (int row=1;row<d_1;row++){
            neighbour_count=(cells2Left[h_1][d-row]+cells2Bot[row-1][0]+cells2Bot[row-1][1]+
                            cells2Left[h_1][d_1-row]+                            cells2Bot[row][1]+
                            cells2Left[h_1][d_2-row]+cells2Bot[row+1][0]+cells2Bot[row+1][1]);
            cells1Bot[row][0]=(neighbour_count==3 || (neighbour_count+cells2Bot[row][0])==3);
        }

        // Bottom: right area
        for (int row=1;row<d_1;row++){
            neighbour_count=(cells2Bot[row-1][w_2]+cells2Bot[row-1][w_1]+cells2Right[h_1][row-1]+
                            cells2Bot[row][w_2]+                            cells2Right[h_1][row]+
                            cells2Bot[row+1][w_2]+cells2Bot[row+1][w_1]+cells2Right[h_1][row+1]);
            cells1Bot[row][w_1]=(neighbour_count==3 || (neighbour_count+cells2Bot[row][w_1])==3);
        }

        // Bottom: corners
        //      tl
        neighbour_count=(           cells2Front[h_1][0]+cells2Front[h_1][1]+
                        cells2Left[h_1][d_1]+                            cells2Bot[0][1]+
                        cells2Left[h_1][d_2]+cells2Bot[1][0]+cells2Bot[1][1]);
        cells1Bot[0][0]=(neighbour_count==3 || (neighbour_count+cells2Bot[0][0])==3);

        //      tr
        neighbour_count=(cells2Front[h_1][w_2]+cells2Front[h_1][w_1]+
                        cells2Bot[0][w_2]+                            cells2Right[h_1][0]+
                        cells2Bot[1][w_2]+cells2Bot[1][w_1]+cells2Right[h_1][1]);
        cells1Bot[0][w_1]=(neighbour_count==3 || (neighbour_count+cells2Bot[0][w_1])==3);

        //      bl
        neighbour_count=(cells2Left[h_1][1]+cells2Bot[d_2][0]+cells2Bot[d_2][1]+
                        cells2Left[h_1][0]+                            cells2Bot[d_1][1]+
                                    cells2Back[h_1][w_1]+cells2Back[h_1][w_2]);
        cells1Bot[d_1][0]=(neighbour_count==3 || (neighbour_count+cells2Bot[d_1][0])==3);

        //      br
        neighbour_count=(cells2Bot[d_2][w_2]+cells2Bot[d_2][w_1]+cells2Right[h_1][0]+
                        cells2Bot[d_1][w_2]+                            cells2Right[h_1][1]+
                        cells2Back[h_1][1]+cells2Back[h_1][0]           );
        cells1Bot[d_1][w_1]=(neighbour_count==3 || (neighbour_count+cells2Bot[d_1][w_1])==3);
    }
}


void run_simple_simulation(){
    generate_random_start(0.5,0);

    uint64_t count=0;
    while (count<100){
        cout_cells();
        update_state();
        oscillator= !oscillator;
        count++;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}



int main(){
    run_simple_simulation();
}

// g++ GameOfLife3DSurface.cpp -o GameOfLife3DSurface.o && ./GameOfLife3DSurface.o