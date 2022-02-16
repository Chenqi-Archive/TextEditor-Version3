#include "BlockView.h"
#include "BlockTextView.h"
#include "BlockListView.h"
#include "RootBlockView.h"

#include "widget/DropDownFrame.h"

#include "WndDesign/frame/PaddingFrame.h"


BEGIN_NAMESPACE(WndDesign)


BlockView::BlockView(RootBlockView& root, BlockView& parent, std::wstring text) : PairView{
	new DropDownFrame{
		text_view = new BlockTextView(*this, text)
	},
	new PaddingFrame{
		Padding(10px, 0, 0, 0),
		list_view = new BlockListView(*this)
	}
}, root(root), parent(parent) {
}

void BlockView::SetCaret(size_t pos) { text_view->SetCaret(pos); }

void BlockView::ClearSelection() {
	BlockListView::ClearSelection();
	BlockTextView::ClearSelection();
}

bool BlockView::IsSelfSelectionBegin() {
	return list_view->selection_begin == -1;
}

void BlockView::BeginSelect(BlockView& child) {
	list_view->selection_begin = &child == this ? -1 : list_view->GetChildIndex(child);
	if (!IsRootBlock()) {
		parent.BeginSelect(*this);
	} else {
		static_cast<RootBlockView&>(parent).BeginSelect();
	}
}

void BlockView::SelectSelf() {
	if (IsRootBlock()) { return; }
	parent.SelectChild(*this);
}

void BlockView::SelectChild(BlockView& child) {
	list_view->SelectChild(child);
}

void BlockView::SelectTextView(Point point) {
	text_view->DoSelect(point_zero + (point - ConvertDescendentPoint(*text_view, point_zero)));
}

void BlockView::SelectListView(Point point) {
	list_view->DoSelect(point_zero + (point - ConvertDescendentPoint(*list_view, point_zero)));
}

void BlockView::DoSelect(Point point) {
	if (IsSelfSelectionBegin()) {
		if (HitTestTextView(point)) {
			SelectTextView(point);
		} else {
			SelectSelf();
		}
	} else {
		if (HitTestTextView(point)) {
			SelectSelf();
		} else {
			SelectListView(point);
		}
	}
}

void BlockView::InsertNewFront() {
	list_view->InsertNewFront();
}

void BlockView::InsertAfter(BlockView& child, std::vector<std::wstring> text, size_t caret_pos) {
	list_view->InsertAfter(child, text, caret_pos);
}

void BlockView::InsertAfterSelf(std::vector<std::wstring> text, size_t caret_pos) {
	IsRootBlock() ? list_view->InsertAt(0, text, caret_pos) : parent.InsertAfter(*this, text, caret_pos);
}


END_NAMESPACE(WndDesign)