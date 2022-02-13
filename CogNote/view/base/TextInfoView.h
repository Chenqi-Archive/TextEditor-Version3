#pragma once

#include "WndDesign/window/wnd_traits.h"
#include "WndDesign/figure/text_block.h"


BEGIN_NAMESPACE(WndDesign)


class TextInfoView : public WndType<Assigned, Assigned> {
public:
	using Style = TextBlockStyle;

public:
	TextInfoView(const Style& style) : style(style), text_block(style, L"") {}
	~TextInfoView() {}

	// text layout
protected:
	const Style& style;
	TextBlock text_block;
public:
	void UpdateText(const std::wstring& text);

	// paint
protected:
	virtual void OnDraw(FigureQueue& figure_queue, Rect draw_region) override;
};


END_NAMESPACE(WndDesign)