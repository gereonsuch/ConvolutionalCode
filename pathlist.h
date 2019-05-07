#ifndef PATHLIST_H
#define PATHLIST_H

#include <deque>
#include <list>
#include <vector>
#include "convolutionalencoder.h"

template <typename Enc, typename outT>
struct Path{
    double metric;
    std::deque<outT> output;
    Enc enc;
};

template <typename Enc, typename outT>
class PathList
{
    std::list<Path<Enc, outT> > d_paths;
public:
    PathList(const Enc &enc);

    void add_path(const Path<Enc, outT> &path);
    Path<Enc, outT> pop_begin();

    void remove_paths(long min_output_sz);
    void remove_paths(double min_metric);
};

typedef PathList<ConvolutionalEncoder,char> PathListHarddecision;

#endif // PATHLIST_H
