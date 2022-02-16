#include "BlockView.h"
#include "BlockTextView.h"
#include "BlockListView.h"
#include "RootBlockView.h"

#include "widget/DropDownFrame.h"

#include "WndDesign/frame/PaddingFrame.h"


BEGIN_NAMESPACE(WndDesign)


BlockView::BlockView(RootBlockView& root, BlockView& parent) : PairView{
	new DropDownFrame{
		text_view = new BlockTextView(*this)
	},
	new PaddingFrame{
		Padding(10px, 0, 0, 0),
		list_view = new BlockListView(*this)
	}
}, root(root), parent(parent) {
}

void BlockView::SetCaret() { text_view->SetCaret(0); }

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

void BlockView::InsertFront() { list_view->InsertFront(); }


END_NAMESPACE(WndDesign)