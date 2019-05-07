#include "pathlist.h"

#include <stdexcept>

template <typename Enc, typename outT>
PathList<Enc, outT>::PathList(const Enc &enc) {
    Path<Enc, outT> p={0.0, {}, enc};

    add_path(p);
}

template <typename Enc, typename outT>
void PathList<Enc, outT>::add_path(const Path<Enc, outT> &path){
    auto it=d_paths.begin();

    for(;it!=d_paths.end() && it->metric > path.metric;it++)
        { /* iterate through list */ }

    d_paths.insert(it, path);
}

template <typename Enc, typename outT>
Path<Enc, outT> PathList<Enc, outT>::pop_begin(){
    if(d_paths.empty())
        throw std::runtime_error("Path List is empty. ");

    Path<Enc, outT> p=*d_paths.begin(); //puffer first element
    d_paths.pop_front();            //remove first element
    return p;
}

template <typename Enc, typename outT>
void PathList<Enc, outT>::remove_paths(long min_output_sz){
    if(d_paths.empty())
        return;

    auto it=d_paths.begin();
    while(it!=d_paths.end()){
        if(it->output.size() < min_output_sz)
            it=d_paths.erase(it);
        else
            it++;
    }
}

template <typename Enc, typename outT>
void PathList<Enc, outT>::remove_paths(double min_metric){
    while(d_paths.size()){
        if(d_paths.back().metric >= min_metric)
            break;
        d_paths.pop_back();
    }
}

template struct Path<ConvolutionalEncoder, char>;
template class PathList<ConvolutionalEncoder, char>;
