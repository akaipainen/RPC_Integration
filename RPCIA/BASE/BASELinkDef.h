#ifdef __CINT__
#pragma link C++ class Digit+;
#pragma link C++ class Cluster+;
#pragma link C++ class Track+;
#pragma link C++ class Digit*+;
#pragma link C++ class Cluster*+;
#pragma link C++ class Track*+;
#pragma link C++ class Store<Digit>+;
#pragma link C++ class Store<Cluster>+;
#pragma link C++ class Store<Track>+;
#pragma link C++ class vector<Digit*>+;
#pragma link C++ class vector<Digit*>::iterator-;
#pragma link C++ class vector<Digit*>::const_iterator-;
#pragma link C++ class vector<Cluster*>+;
#pragma link C++ class vector<Cluster*>::iterator-;
#pragma link C++ class vector<Cluster*>::const_iterator-;
#pragma link C++ class vector<Track*>+;
#pragma link C++ class vector<Track*>::iterator-;
#pragma link C++ class vector<Track*>:const_iterator-;
// #pragma link C++ class Iterator<Digit>-;
// #pragma link C++ class Iterator<Cluster>-;
// #pragma link C++ class Iterator<Track>-;
// #pragma link C++ class vector<Digit*>+;

#endif