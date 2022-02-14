#pragma once

#include "base/PairView.h"


BEGIN_NAMESPACE(WndDesign)

class RootBlockView;
class BlockTextView;
class BlockListView;


class BlockView : public PairView<Vertical> {
private:
	friend class RootBlockView;
	friend class BlockTextView;

private:
	BlockView(RootBlockView& root, BlockView& parent);

private:
	RootBlockView& root;
	BlockView& parent;

private:
	ref_ptr<BlockTextView> text_view;
	ref_ptr<BlockListView> list_view;

	// selection
private:
	size_t selection_begin = 0;
private:
	void BeginSelect() { BeginSelect(*this); }
	void BeginSelect(BlockView& child);
	void SelectTextView(Point point);
	void SelectSelf();
	void SelectChildRange(size_t index);
	void DoSelect(Point point);
};


END_NAMESPACE(WndDesign)