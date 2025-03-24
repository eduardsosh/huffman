#include "huffman/encoder.h"

int main() {
    huffman::Encoder encoder_obj;
    encoder_obj.encode("input.txt");
    return 0;
}
