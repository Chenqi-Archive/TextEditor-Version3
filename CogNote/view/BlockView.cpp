#include "BlockView.h"
#include "BlockTextView.h"
#include "BlockListView.h"
#include "RootBlockView.h"

#include "widget/DropDownFrame.h"


BEGIN_NAMESPACE(WndDesign)


BlockView::BlockView(RootBlockView& root, BlockView& parent) : PairView{
	new DropDownFrame{
		text_view = new BlockTextView(*this)
	},
	list_view = new BlockListView()
}, root(root), parent(parent) {

}

void BlockView::BeginSelect(BlockView& child) {
	selection_begin = &child == this ? -1 : list_view->GetChildData(child);
	if (&parent != this) {
		parent.BeginSelect(*this);
	} else {
		static_cast<RootBlockView&>(parent).BeginSelect();
	}
}

void BlockView::SelectTextView(Point point) {
	text_view->DoSelect(point_zero + (point - ConvertDescendentPoint(*text_view, point_zero)));
}

void BlockView::SelectSelf() {
	parent.SelectChildRange(parent.selection_begin);
}

void BlockView::SelectChildRange(size_t index) {
	list_view->SelectChildRange(selection_begin, index);
}

void BlockView::DoSelect(Point point) {
	if (selection_begin == -1) {
		if (HitTest(point) != list_view) { 
			SelectTextView(point); 
		} else {
			SelectSelf();
		}
	} else {
		if (HitTest(point) != list_view) {
			SelectSelf();
		} else {
			BlockView& child = static_cast<BlockView&>(*list_view->HitTest(point));
			size_t index = list_view->GetChildData(child);
			if (selection_begin == index) {
				child.DoSelect(point);
			} else {
				SelectChildRange(index);
			}
		}
	}
}


END_NAMESPACE(WndDesign)