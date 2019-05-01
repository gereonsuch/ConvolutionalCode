/******************************************************************************
 *                                                                            *
 * MIT License                                                                *
 *                                                                            *
 * Copyright 2019 Gereon Such                                                 *
 *                                                                            *
 * Permission is hereby granted, free of charge, to any person obtaining      *
 * a copy of this software and associated documentation files (the            *
 * "Software"), to deal in the Software without restriction, including        *
 * without limitation the rights to use, copy, modify, merge, publish,        *
 * distribute, sublicense, and/or sell copies of the Software, and to         *
 * permit persons to whom the Software is furnished to do so, subject         *
 * to the following conditions:                                               *
 *                                                                            *
 * The above copyright notice and this permission notice shall be             *
 * included in all copies or substantial portions of the Software.            *
 *                                                                            *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,            *
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES            *
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND                   *
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS        *
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN         *
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN          *
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE. *
 ******************************************************************************/

#include "convolutionalencoder.h"

#include <time.h>
#include <cstdlib>
#include <iostream>


void print_bits(std::vector<char> &vec, unsigned long linebreak=64, char one='#', char zero='.'){
    for(unsigned long k=0;k<vec.size();k++){
        if(k%linebreak==0) std::cout << std::endl;
        std::cout << (vec[k]!=0? one : zero );
    }
    std::cout << std::endl;
}

int main(int argc, char *argv[])
{
    //create encoder and annihilation polynom encoder
    ConvolutionalEncoder enc({37,11}); // this is Generator={100101, 1011}
    ConvolutionalEncoder anni({enc.get_annihilator(),0}); //set the annihilator to 10010011011, see get_annihilator docstring for more info

    //create random information bits
    srand (time(NULL));
    int len=512;
    std::vector<char> info;
    info.reserve(len);
    for(int i=0;i<len;i++)
        info.push_back(rand()%2);

    std::cout << std::endl << "Information bits: ";
    print_bits(info);

    //encode info bits
    std::vector<char> encoded;
    enc(encoded, info);

    std::cout << std::endl << "Encoded Bits: ";
    print_bits(encoded);

    //apply annihilation polynom
    std::vector<char> annihilated;
    anni(annihilated, encoded);

    std::cout << std::endl << "Applied annihilation polynom(" << anni.get_generator() [0] <<"): ";
    print_bits(annihilated);

    std::cout << std::endl << "Uneven columns of annihilated data";
    std::vector<char> col_2=takeskip(annihilated,1,1,1);
    print_bits(col_2);

    std::cout << std::endl << "If all 0, the annihilator matches the code and the bit offset is matched(=1). " << std::endl << std::endl;
}
