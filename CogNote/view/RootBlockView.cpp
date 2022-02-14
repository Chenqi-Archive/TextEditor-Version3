#include "RootBlockView.h"
#include "BlockTextView.h"


BEGIN_NAMESPACE(WndDesign)


RootBlockView::RootBlockView() : BlockView(*this, *this) {}

void RootBlockView::BeginSelect() {
	SetCapture();
}

void RootBlockView::DoSelect(Point point) {
	if (selection_begin == -1) {
		SelectTextView(point);
	} else {
		BlockView::DoSelect(point);
	}
}

void RootBlockView::OnMouseMsg(MouseMsg msg) {
	switch (msg.type) {
	case MouseMsg::Move: DoSelect(msg.point); break;
	case MouseMsg::LeftUp: ReleaseCapture(); break;
	}
}

void RootBlockView::OnNotifyMsg(NotifyMsg msg) {
	switch (msg) {
	case NotifyMsg::MouseEnter: BlockTextView::ActivateCaret(); break;
	}
}


END_NAMESPACE(WndDesign)