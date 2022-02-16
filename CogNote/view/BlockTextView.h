#pragma once

#include "WndDesign/window/wnd_traits.h"
#include "WndDesign/figure/text_block.h"
#include "WndDesign/common/unicode_helper.h"


BEGIN_NAMESPACE(WndDesign)

class BlockView;


class BlockTextView : public WndType<Assigned, Auto> {
public:
	BlockTextView(BlockView& block_view);
	~BlockTextView() {}

	// context
private:
	BlockView& block_view;

	// text
private:
	using HitTestInfo = TextBlockHitTestInfo;
private:
	std::wstring text;
	TextBlock text_block;
	WordBreakIterator word_break_iterator;
private:
	size_t GetCharacterLength(size_t text_position) { return GetUTF16CharLength(text[text_position]); }
private:
	void TextUpdated();
private:
	void SetText(std::wstring str) { text.assign(std::move(str)); TextUpdated(); }
	void AppendText(std::wstring str) { text.append(str); TextUpdated(); }
	void InsertText(size_t pos, wchar ch) { text.insert(pos, 1, ch); TextUpdated(); }
	void InsertText(size_t pos, std::wstring str) { text.insert(pos, str); TextUpdated(); }
	void ReplaceText(size_t begin, size_t length, wchar ch) { text.replace(begin, length, 1, ch); TextUpdated(); }
	void ReplaceText(size_t begin, size_t length, std::wstring str) { text.replace(begin, length, str); TextUpdated(); }
	void DeleteText(size_t begin, size_t length) { text.erase(begin, length); TextUpdated(); }

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
	static void BlinkCaret();
public:
	static void ActivateCaret();
private:
	void SetCaretFocus(Rect region);
	void SetCaret(const HitTestInfo& info);
	void SetCaret(Point point);
public:
	void SetCaret(size_t text_position);
private:
	void MoveCaretLeft();
	void MoveCaretRight();
	void MoveCaretUp();
	void MoveCaretDown();
	void MoveCaretHome();
	void MoveCaretEnd();

	// selection
private:
	static void RedrawSelectionRegion();
public:
	static void ClearSelection();
private:
	void UpdateSelectionRegion(size_t begin, size_t end);
	void SelectWord();
	void SelectAll();
public:
	void DoSelect(Point point);

	// input
private:
	void Insert(wchar ch);
	void Delete(bool is_backspace);
	void OnImeBegin();
	void OnImeString();
	void OnImeEnd();

	// clipboard
private:
	void Cut();
	void Copy();
	void Paste();

	// message
private:
	virtual void OnMouseMsg(MouseMsg msg) override;
	virtual void OnKeyMsg(KeyMsg msg) override;
	virtual void OnNotifyMsg(NotifyMsg msg) override;
};


END_NAMESPACE(WndDesign)