#include "blocky/blocky.h"

int main(int argc, char *argv[]) {
        /* TODO: Genesis block being overwritten */
        Blockchain* b = make_blockchain();
        display_blockchain(b);
        add_block(make_block("hello world", get_last_hash(b)), &b);
        add_block(make_block("foo bar", get_last_hash(b)), &b);
        add_block(make_block("fizz buzz", get_last_hash(b)), &b);
        display_blockchain(b);
        return 0; 
}

