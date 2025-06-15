#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void loading();
void pacman_animation();

int main(){
    loading();
}


void loading(){
    printf("=========================================================");
    printf("\n||               Welcome to The Arcade!                ||\n");
    printf("=========================================================\n");
    int enter = 100;
    printf("Press Enter to start the game");
    pacman_animation();


}

void pacman_animation() {
        const char *frames[] = {":O...  ", ":C...  "};
    // Animation loop
    for (int i = 0; i < 20; i+=0) {       
          for(int j = 0; j < 53; j++){
            for (int k = 0; k < j; k++){
               printf(" ");
            }

            printf("%s", frames[j % 2]);  // Use k as the mod value
            fflush(stdout);
            usleep(100000); // 0.5 seconds
            printf("\r");

            for (int l = 0; l < 60; l++){
               printf(" ");
                }
                  printf("\r");
               }
                    
                    

    }
}