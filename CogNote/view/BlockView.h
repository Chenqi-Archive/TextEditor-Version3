#pragma once

#include "base/PairView.h"


BEGIN_NAMESPACE(WndDesign)

class RootBlockView;
class BlockTextView;
class BlockListView;


class BlockView : public PairView<Vertical> {
protected:
	BlockView(RootBlockView& root, BlockView& parent, std::wstring text);
public:
	BlockView(BlockView& parent, std::wstring text) : BlockView(parent.root, parent, text) {}

	// context
private:
	RootBlockView& root;
	BlockView& parent;
private:
	bool IsRootBlock() const { return &parent == this; }

	// child
private:
	ref_ptr<BlockTextView> text_view;
	ref_ptr<BlockListView> list_view;

	// layout
private:
	bool HitTestTextView(Point point) { return HitTest(point) == first; }

	// caret
public:
	void SetCaret(size_t pos);

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
	void InsertFront(std::wstring text);
	void InsertAfter(BlockView& child, std::wstring text);
	void InsertAfter(BlockView& child, std::vector<std::wstring> text, size_t caret_pos);
	void InsertAfterSelf(std::wstring text);
	void InsertAfterSelf(std::vector<std::wstring> text, size_t caret_pos);
};


END_NAMESPACE(WndDesign)