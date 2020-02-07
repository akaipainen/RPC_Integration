#if !defined(TREE_BUILDER_HPP)
#define TREE_BUILDER_HPP

// CPP includes
#include <list>

// Root includes
#include <TFile.h>
#include <TTree.h>

// Project includes
#include "event.hpp"
#include "utils.hpp"
#include "event_builder.hpp"

class TreeBuilder {
private:
	EventBuilder event_builder;

	TFile *file;
    TTree *event_tree;

	Event *event;
	Int_t live_time;

public:
	// Constructor. Creates a hit_tree TTree object and initializes branches.
    TreeBuilder();

	// Destructor. Destroys hit_tree pointer.
    ~TreeBuilder();

	// Adds an event to the tree.
	void add_event(Event &event);

	// Writes the tree to an outfile (currently "test.root")
	void write_tree();
};

#endif // TREE_BUILDER_HPP
