#ifndef CONVOLUTIONALENCODER_H
#define CONVOLUTIONALENCODER_H

#include <vector>
#include "bitmanipulation.h"

class ConvolutionalEncoder
{
public:
    ConvolutionalEncoder() : ConvolutionalEncoder({1}, 0) {}
    ConvolutionalEncoder(const std::vector<unsigned long> &G, unsigned long mem=0){
        set_generator(G);
        set_memory(mem);
    }
    ConvolutionalEncoder(const std::vector<std::vector<char> > &G, const std::vector<char> &mem){
        set_generator(G);
        set_memory(mem);
    }
    ConvolutionalEncoder(const std::vector<std::vector<char> > &G) : ConvolutionalEncoder(G, {0}) {}

    void set_generator(const std::vector<unsigned long> &G);
    void set_generator(const std::vector<std::vector<char> > &G);

    void set_memory(unsigned long mem) {d_mem=mem;}
    void set_memory(const std::vector<char> &mem) {this->set_memory(bitsequence_from_vector(mem));}

    unsigned long get_annihilator();
    std::vector<unsigned long> get_generator() {return d_generator;}

    unsigned long N() {return 1;} //well, it is before  punctioning.
    unsigned long K() {return d_generator.size();}
    unsigned long get_memory();
    unsigned long get_constraintlen(){return this->get_memory()+1;}

    std::vector<char> operator() (char bit);
    void operator() (std::vector<char> &output, char bit);
    void operator() (std::vector<char> &output, const std::vector<char> &bits);

private:
    unsigned long d_mem;
    std::vector<unsigned long> d_generator;
};

#endif // CONVOLUTIONALENCODER_H
