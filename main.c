#include "./cradle.h"

int main() {
    // a small terminal calculator
    // kinda feels like bc

    Init();

    while (look != '.') {
        switch (look) {
            case '?':
                Input();
            case '!':
                Output();
            default:
                Assignment();
        }
        NewLine();
    }

    return 0;
}