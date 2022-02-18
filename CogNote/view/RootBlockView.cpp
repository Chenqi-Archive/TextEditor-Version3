#include "RootBlockView.h"
#include "BlockTextView.h"
#include "BlockListView.h"


BEGIN_NAMESPACE(WndDesign)

BEGIN_NAMESPACE(Anonymous)

// drag and drop
bool drag_drop = false;
ref_ptr<BlockView> drag_drop_src = nullptr;
ref_ptr<BlockView> drag_drop_dest = nullptr;

END_NAMESPACE(Anonymous)


RootBlockView::RootBlockView() : BlockView(*this, L"Root") { root = this; }

void RootBlockView::BeginSelect() {
	SetCapture();
}

void RootBlockView::DoSelect(Point point) {
	if (IsSelfSelectionBegin()) {
		text_view->DoSelect(ConvertPointToTextView(point));
	} else {
		BlockView::DoSelect(point);
	}
}

void RootBlockView::BeginDragDrop(BlockView& block_view) {
	drag_drop = true; drag_drop_src = &block_view; drag_drop_dest = nullptr;
	SetCapture();
}

void RootBlockView::CancelDragDrop() {
	drag_drop = false;
	ReleaseCapture();
}

void RootBlockView::DoDragDrop(Point point) {
	if (drag_drop_src != nullptr) {
		ref_ptr<BlockView> drag_drop_dest_curr = BlockView::DoDragDrop(point);
		if (drag_drop_dest_curr != nullptr) { drag_drop_dest = drag_drop_dest_curr; }
	}
}

void RootBlockView::FinishDragDrop() {
	if (drag_drop_dest != nullptr && drag_drop_src != nullptr) {
		drag_drop_dest->FinishDragDrop(*drag_drop_src);
	}
	CancelDragDrop();
}

void RootBlockView::OnMouseMsg(MouseMsg msg) {
	switch (msg.type) {
	case MouseMsg::Move: drag_drop ? DoDragDrop(msg.point) : DoSelect(msg.point); break;
	case MouseMsg::LeftUp: if (drag_drop) { FinishDragDrop(); } else { ReleaseCapture(); } break;
	}
}

void RootBlockView::OnNotifyMsg(NotifyMsg msg) {
	switch (msg) {
	case NotifyMsg::MouseEnter: BlockTextView::ActivateCaret(); break;
	}
}


END_NAMESPACE(WndDesign)