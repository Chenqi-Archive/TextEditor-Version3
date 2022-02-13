#include "TextInfoView.h"


BEGIN_NAMESPACE(WndDesign)


void TextInfoView::UpdateText(const std::wstring& text) {
	text_block.SetText(style, text);
	text_block.UpdateSizeRef(Size(length_max, length_max));
	Redraw(region_infinite);
}

void TextInfoView::OnDraw(FigureQueue& figure_queue, Rect draw_region) {
	figure_queue.add(point_zero, new TextBlockFigure(text_block, style.font._color));
}


END_NAMESPACE(WndDesign)