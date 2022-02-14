#pragma once

#include "WndDesign/window/wnd_traits.h"
#include "WndDesign/figure/text_block.h"


BEGIN_NAMESPACE(WndDesign)

class BlockView;


class BlockTextView : public WndType<Assigned, Auto> {
private:
	friend class BlockView;
	friend class RootBlockView;

private:
	BlockTextView(BlockView& block_view);
	~BlockTextView() {}

	// BlockView
private:
	BlockView& block_view;

	// text
private:
	using HitTestInfo = TextBlockHitTestInfo;
private:
	std::wstring text;
	TextBlock text_block;
private:
	void TextUpdated();

	// layout
private:
	float width = 0.0f;
private:
	Size UpdateLayout();
private:
	virtual Size OnSizeRefUpdate(Size size_ref) override;

	// paint
private:
	virtual void OnDraw(FigureQueue& figure_queue, Rect draw_region) override;

	// caret
private:
	friend void BlinkCaret();
private:
	static void RedrawCaretRegion();
	static void ShowCaret();
	static void HideCaret();
	static void ActivateCaret();
	static void BlinkCaret();
private:
	void SetCaretFocus(Rect region);
	void SetCaret(const HitTestInfo& info);
	void SetCaret(Point point);

	// selection
private:
	static void RedrawSelectionRegion();
private:
	void UpdateSelectionRegion();
	void DoSelect(Point point);

	// message
private:
	virtual void OnMouseMsg(MouseMsg msg) override;
	virtual void OnKeyMsg(KeyMsg msg) override;
	virtual void OnNotifyMsg(NotifyMsg msg) override;
};


END_NAMESPACE(WndDesign)