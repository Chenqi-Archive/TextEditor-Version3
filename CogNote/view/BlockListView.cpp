#include "BlockListView.h"

#include "WndDesign/figure/shape.h"


BEGIN_NAMESPACE(WndDesign)

BEGIN_NAMESPACE(Anonymous)

// selection
constexpr Color selection_color = Color(Color::DimGray, 0x7f);

ref_ptr<BlockListView> selection_focus = nullptr;
size_t selection_range_begin = 0;
size_t selection_range_end = 0;

Rect selection_region;

bool HasSelection() { return selection_range_end > selection_range_begin; }

END_NAMESPACE(Anonymous)


BlockListView::BlockListView() : ListLayout(0.0f) {}

void BlockListView::OnDraw(FigureQueue& figure_queue, Rect draw_region) {
	ListLayout::OnDraw(figure_queue, draw_region);
	if (selection_focus == this && HasSelection()) {
		figure_queue.add(selection_region.point, new Rectangle(selection_region.size, selection_color));
	}
}

void BlockListView::RedrawSelectionRegion() {
	if (selection_focus != nullptr) { selection_focus->Redraw(selection_region); }
}

void BlockListView::SelectChildRange(size_t begin, size_t end) {
	if (end < begin) { std::swap(begin, end); }
	if (selection_focus == this && selection_range_begin == begin && selection_range_end == end) { return; }
	SetFocus();
	RedrawSelectionRegion();
	selection_focus = this; selection_range_begin = begin; selection_range_end = end;
	selection_region.point.x = 0.0f;
	selection_region.point.y = child_list[selection_range_begin].BeginOffset();
	selection_region.size.width = size.width;
	selection_region.size.height = child_list[selection_range_end - 1].EndOffset() - selection_region.point.y;
	RedrawSelectionRegion();
}

void BlockListView::OnKeyMsg(KeyMsg msg) {

}


END_NAMESPACE(WndDesign)