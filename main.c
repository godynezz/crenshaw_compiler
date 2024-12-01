#include "./cradle.h"

int main() {
    // a small terminal calculator
    // kinda feels like bc

    Init();

    while (look != '.') {
        switch (look) {
            case '?':
                Input();
                break;
            case '!':
                Output();
                break;
            default:
                Assignment();
                break;
        }
        NewLine();
    }

    return 0;
}