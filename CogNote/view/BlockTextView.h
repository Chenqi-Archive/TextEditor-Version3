#pragma once

#include "WndDesign/window/wnd_traits.h"
#include "WndDesign/figure/text_block.h"


BEGIN_NAMESPACE(WndDesign)


class BlockTextView : public WndType<Assigned, Auto> {
public:
	BlockTextView();
	~BlockTextView() {}

	// text
protected:
	std::wstring text;
	TextBlock text_block;
protected:
	void TextUpdated();

	// layout
protected:
	float width = 0.0f;
protected:
	Size UpdateLayout();
protected:
	virtual Size OnSizeRefUpdate(Size size_ref) override;

	// paint
protected:
	virtual void OnDraw(FigureQueue& figure_queue, Rect draw_region) override;
};


END_NAMESPACE(WndDesign)