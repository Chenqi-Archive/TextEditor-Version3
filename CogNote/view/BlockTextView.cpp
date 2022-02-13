#include "BlockTextView.h"


BEGIN_NAMESPACE(WndDesign)

BEGIN_NAMESPACE(Anonymous)

struct Style : public TextBlockStyle {
	Style() {
		paragraph.line_height(25px);
		font.family(L"Calibri", L"Segoe UI Emoji", L"DengXian").size(20);
	}
}style;

END_NAMESPACE(Anonymous)


BlockTextView::BlockTextView() : text(L"Input"), text_block(style, text) {}

void BlockTextView::TextUpdated() {
	text_block.SetText(style, text);
	SizeUpdated(UpdateLayout());
	Redraw(region_infinite);
}

Size BlockTextView::UpdateLayout() {
	text_block.UpdateSizeRef(Size(width, length_max));
	return Size(width, text_block.GetSize().height);
}

Size BlockTextView::OnSizeRefUpdate(Size size_ref) {
	width = size_ref.width;
	return UpdateLayout();
}

void BlockTextView::OnDraw(FigureQueue& figure_queue, Rect draw_region) {
	figure_queue.add(point_zero, new TextBlockFigure(text_block, style.font._color));
}


END_NAMESPACE(WndDesign)