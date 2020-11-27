#include "postmachine.h"

int main()
{
    PostMachine postMachine;
    postMachine.readTape();
    postMachine.readInputFile("C:/Users/Admin/Documents/laboratory/TVP/PostMachine/input.txt");

    string result = postMachine.start();
    cout << result;
    return 0;
}
