#include "PatchSequenceEnumerator.h"
#include "TriangleSet.h"

// virtual destructor
PatchSequenceEnumerator::~PatchSequenceEnumerator() {
}

DirectPatchSequenceEnumerator::DirectPatchSequenceEnumerator(TriangleSet *tset):
		tset_(*tset),
		current_(0),
		size_(tset->count())
{
}

Triangle* DirectPatchSequenceEnumerator::nextPatch() {
	if (current_>=size_)
		// No next patch available
		return 0;
	
	// Human readable post-increment
	Triangle &t= tset_[current_];
	current_++;
	return &t;
}

void DirectPatchSequenceEnumerator::reset() {
	current_ = 0;
}

IndirectPatchSequenceEnumerator::IndirectPatchSequenceEnumerator():
		current_(0)
{
}

IndirectPatchSequenceEnumerator::~IndirectPatchSequenceEnumerator() {
	TContainer::iterator i;
	for(i= container_.begin(); i!= container_.end(); i++)
		delete *i;
}

void IndirectPatchSequenceEnumerator::add(PatchSequenceEnumerator *pe) {
	container_.push_back(pe);
}

Triangle* IndirectPatchSequenceEnumerator::nextPatch() {
	Triangle *t;

	do {
		if (current_ >= static_cast<int>(container_.size()))
			// No next patch available
			return 0;
	
		t = container_[current_]->nextPatch();
		if (0== t)
			current_++;
	}
		while (0== t);
	
	return t;
}

void IndirectPatchSequenceEnumerator::reset() {
	TContainer::iterator i;
	for(i= container_.begin(); i!= container_.end(); i++)
		(*i)->reset();
	current_ = 0;
}


