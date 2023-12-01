#include <iostream>

using namespace std;

int main() {
   int A[10] = { 0,0,0,0,0,0,0,0,0,0 };
   int B[5][5] = { 0,2,3,4,5,
                   1,3,2,7,8,
                   2,4,6,1,2,
                   9,3,2,1,6,
                   1,2,5,5,6 };
    int count = 0;
    
    for(int i = 0; i < 5; i++) {
        for(int j = 0; j < 5; j++) {
            if(A[(int)B[i][j]] == 0) {
                count++;
                A[(int)B[i][j]]= 1;
            }
        }
    }
    
   cout << count;
}
