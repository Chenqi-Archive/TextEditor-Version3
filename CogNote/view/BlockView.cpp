#include "BlockView.h"
#include "BlockTextView.h"
#include "BlockListView.h"
#include "RootBlockView.h"

#include "widget/DropDownFrame.h"

#include "WndDesign/frame/PaddingFrame.h"


BEGIN_NAMESPACE(WndDesign)


BlockView::BlockView(BlockView& parent, std::wstring text) : PairView{
	new DropDownFrame{
		text_view = new BlockTextView(*this, text)
	},
	new PaddingFrame{
		Padding(10px, 0, 0, 0),
		list_view = new BlockListView(*this)
	}
}, root(parent.root), parent(&parent) {
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
	IsRootBlock() ? root->BeginSelect() : parent->BeginSelect(*this);
}

void BlockView::SelectSelf() {
	IsRootBlock() ? void() : parent->SelectChild(*this);
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

void BlockView::InsertFront(std::wstring text) {
	list_view->InsertFront(text);
}

void BlockView::InsertBack(std::vector<std::unique_ptr<BlockView>> block_view_list) {
	list_view->InsertBack(std::move(block_view_list));
}

void BlockView::InsertAfter(BlockView& child, std::wstring text) {
	list_view->InsertAfter(child, text);
}

void BlockView::InsertAfter(BlockView& child, std::vector<std::wstring> text, size_t caret_pos) {
	list_view->InsertAfter(child, text, caret_pos);
}

void BlockView::InsertAfter(BlockView& child, std::vector<std::unique_ptr<BlockView>> block_view_list) {
	list_view->InsertAfter(child, std::move(block_view_list));
}

void BlockView::InsertAfterSelf(std::wstring text) {
	IsRootBlock() ? list_view->InsertFront(text) : parent->InsertAfter(*this, text);
}

void BlockView::InsertAfterSelf(std::vector<std::wstring> text, size_t caret_pos) {
	IsRootBlock() ? list_view->InsertFront(text, caret_pos) : parent->InsertAfter(*this, text, caret_pos);
}

void BlockView::InsertAfterSelf(std::vector<std::unique_ptr<BlockView>> block_view_list) {
	IsRootBlock() ? void() : parent->InsertAfter(*this, std::move(block_view_list));
}


END_NAMESPACE(WndDesign)