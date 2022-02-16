#pragma once

#include "base/PairView.h"


BEGIN_NAMESPACE(WndDesign)

class RootBlockView;
class BlockTextView;
class BlockListView;


class BlockView : public PairView<Vertical> {
public:
	BlockView(RootBlockView& root, BlockView& parent);

	// context
private:
	RootBlockView& root;
	BlockView& parent;
private:
	bool IsRootBlock() const { return &parent == this; }
public:
	RootBlockView& GetRoot() const { return root; }

	// child
private:
	ref_ptr<BlockTextView> text_view;
	ref_ptr<BlockListView> list_view;

	// layout
private:
	bool HitTestTextView(Point point) { return HitTest(point) == first; }

	// caret
public:
	void SetCaret();

	// selection
public:
	static void ClearSelection();
protected:
	bool IsSelfSelectionBegin();
private:
	void BeginSelect(BlockView& child);
	void SelectSelf();
	void SelectChild(BlockView& child);
protected:
	void SelectTextView(Point point);
	void SelectListView(Point point);
public:
	void BeginSelect() { BeginSelect(*this); }
	void SelectAll() { SelectSelf(); }
	void DoSelect(Point point);

	// input
public:
	void InsertFront();
};


END_NAMESPACE(WndDesign)