#include "BlockView.h"
#include "BlockTextView.h"
#include "BlockListView.h"
#include "RootBlockView.h"

#include "widget/DropDownFrame.h"

#include "WndDesign/frame/PaddingFrame.h"


BEGIN_NAMESPACE(WndDesign)

BEGIN_NAMESPACE(Anonymous)

// drag and drop
bool drag_drop_text = false;

END_NAMESPACE(Anonymous)


BlockView::BlockView(BlockView& parent, std::wstring text) : PairView{
	new DropDownFrame{
		text_view = new BlockTextView(*this, text)
	},
	new PaddingFrame{
		Padding(20px, 5px, 0, 5px),
		list_view = new BlockListView(*this)
	}
}, root(parent.root), parent(&parent) {
}

Point BlockView::ConvertPointToTextView(Point point) {
	return point_zero + (point - ConvertDescendentPoint(*text_view, point_zero));
}

Point BlockView::ConvertPointToListView(Point point) {
	return point_zero + (point - ConvertDescendentPoint(*list_view, point_zero));
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

void BlockView::DoSelect(Point point) {
	if (IsSelfSelectionBegin()) {
		if (HitTestTextView(point)) {
			text_view->DoSelect(ConvertPointToTextView(point));
		} else {
			SelectSelf();
		}
	} else {
		if (HitTestTextView(point)) {
			SelectSelf();
		} else {
			list_view->DoSelect(ConvertPointToListView(point));
		}
	}
}

void BlockView::BeginTextDragDrop() {
	drag_drop_text = true;
	root->BeginDragDrop(*this);
}

void BlockView::BeginListDragDrop() {
	drag_drop_text = false;
	root->BeginDragDrop(*this);
}

ref_ptr<BlockView> BlockView::DoDragDrop(Point point) {
	if (drag_drop_text) {
		if (HitTestTextView(point)) {
			return text_view->DoDragDrop(ConvertPointToTextView(point));
		} else {
			return list_view->DoTextDragDrop(ConvertPointToListView(point));
		}
	} else {
		return list_view->DoDragDrop(ConvertPointToListView(point));
	}
}

void BlockView::FinishDragDrop(BlockView& block_view) {
	drag_drop_text ? text_view->FinishDragDrop(*block_view.text_view) : list_view->FinishDragDrop(*block_view.list_view);
}

void BlockView::CancelDragDrop() {
	root->CancelDragDrop();
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