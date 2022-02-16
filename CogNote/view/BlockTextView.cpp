#include "BlockTextView.h"
#include "BlockView.h"

#include "WndDesign/message/timer.h"
#include "WndDesign/message/ime.h"
#include "WndDesign/message/mouse_tracker.h"
#include "WndDesign/figure/shape.h"
#include "WndDesign/system/clipboard.h"

#include "../common/string_helper.h"


BEGIN_NAMESPACE(WndDesign)

void BlinkCaret() { BlockTextView::BlinkCaret(); }

BEGIN_NAMESPACE(Anonymous)

// style
struct Style : public TextBlockStyle {
	Style() {
		paragraph.line_height(25px);
		font.family(L"Calibri", L"Segoe UI Emoji", L"DengXian").size(20);
	}
}style;

// caret
constexpr float caret_width = 1.0f;
constexpr Color caret_color = Color::DimGray;

enum class CaretState { Hide, BlinkHide, BlinkShow, Show };
constexpr uint caret_blink_period = 500;  // 500ms
constexpr uint caret_blink_expire_time = 20000;  // 20s

CaretState caret_state = CaretState::Hide;
Timer caret_timer = Timer([&]() { BlinkCaret(); });
ushort caret_blink_time = 0;

ref_ptr<BlockTextView> caret_focus = nullptr;
size_t caret_position = 0;
Rect caret_region = region_empty;

bool IsCaretVisible() { return caret_state == CaretState::Show || caret_state == CaretState::BlinkShow; }

// selection
constexpr Color selection_color = Color(Color::DimGray, 0x7f);

size_t selection_begin = 0;
size_t selection_range_begin = 0;
size_t selection_range_end = 0;

std::vector<TextBlockHitTestInfo> selection_info;
Rect selection_region_union;

bool HasSelection() { return selection_range_end > selection_range_begin; }

// ime
size_t ime_composition_begin = 0;
size_t ime_composition_end = 0;

// message
bool is_ctrl_down = false;
MouseTracker mouse_tracker;

END_NAMESPACE(Anonymous)


BlockTextView::BlockTextView(BlockView& block_view, std::wstring text) :
	block_view(block_view), text(text), text_block(style, text) {
	cursor = Cursor::Text;
	ime.Enable(*this);
	word_break_iterator.SetText(text);
}

void BlockTextView::TextUpdated() {
	text_block.SetText(style, text);
	word_break_iterator.SetText(text);
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
	if (caret_focus == this && IsCaretVisible()) {
		figure_queue.add(caret_region.point, new Rectangle(caret_region.size, caret_color));
	}
	if (caret_focus == this && HasSelection()) {
		for (auto& it : selection_info) {
			Rect& region = it.geometry_region; if (region.Intersect(draw_region).IsEmpty()) { continue; }
			figure_queue.add(region.point, new Rectangle(region.size, selection_color));
		}
	}
}

void BlockTextView::RedrawCaretRegion() {
	caret_focus->Redraw(caret_region);
}

void BlockTextView::ShowCaret() {
	if (!IsCaretVisible()) { RedrawCaretRegion(); }
	caret_state = CaretState::Show;
	ActivateCaret();
}

void BlockTextView::HideCaret() {
	if (caret_state != CaretState::Hide) {
		if (IsCaretVisible()) { RedrawCaretRegion(); }
		caret_state = CaretState::Hide;
	}
}

void BlockTextView::BlinkCaret() {
	caret_blink_time += caret_blink_period;
	if (caret_blink_time >= caret_blink_expire_time) {
		caret_state = CaretState::Show;
		caret_timer.Stop();
		return;
	}
	switch (caret_state) {
	case CaretState::Hide: caret_timer.Stop(); return;
	case CaretState::Show:
	case CaretState::BlinkShow: caret_state = CaretState::BlinkHide; break;
	case CaretState::BlinkHide: caret_state = CaretState::BlinkShow; break;
	}
	RedrawCaretRegion();
}

void BlockTextView::ActivateCaret() {
	if (caret_state != CaretState::Hide) {
		if (!caret_timer.IsSet()) {
			caret_timer.Set(caret_blink_period);
		}
		caret_blink_time = 0;
	}
}

void BlockTextView::SetCaretFocus(Rect region) {
	if (IsCaretVisible()) { RedrawCaretRegion(); }
	caret_focus = this; caret_region = region;
}

void BlockTextView::SetCaret(const HitTestInfo& info) {
	caret_position = info.text_position;
	Point point = info.geometry_region.point;
	Size size = Size(caret_width, info.geometry_region.size.height);
	if (info.is_trailing_hit) {
		caret_position += info.text_length;
		point.x += info.geometry_region.size.width;
	}
	SetFocus(); SetCaretFocus(Rect(point, size)); ShowCaret();
}

void BlockTextView::SetCaret(Point point) {
	SetCaret(text_block.HitTestPoint(point));
	selection_begin = caret_position;
	ClearSelection();
}

void BlockTextView::SetCaret(size_t text_position) {
	SetCaret(text_block.HitTestTextPosition(text_position));
}

void BlockTextView::MoveCaretLeft() {
	if (HasSelection()) {
		SetCaret(selection_range_begin);
	} else {
		if (caret_position > 0) {
			SetCaret(caret_position - 1);
		} else {
		}
	}
}

void BlockTextView::MoveCaretRight() {
	if (HasSelection()) {
		SetCaret(selection_range_end);
	} else {
		if (caret_position < text.length()) {
			SetCaret(caret_position + GetCharacterLength(caret_position));
		} else {
		}
	}
}

void BlockTextView::MoveCaretUp() {
}

void BlockTextView::MoveCaretDown() {
}

void BlockTextView::MoveCaretHome() {
	SetCaret(0);
}

void BlockTextView::MoveCaretEnd() {
	SetCaret((size_t)-1);
}

void BlockTextView::RedrawSelectionRegion() {
	caret_focus->Redraw(selection_region_union);
}

void BlockTextView::ClearSelection() {
	if (HasSelection()) {
		selection_range_begin = selection_range_end = 0;
		RedrawSelectionRegion();
		selection_info.clear();
		selection_region_union = region_empty;
	}
}

void BlockTextView::UpdateSelectionRegion(size_t begin, size_t end) {
	if (end < begin) { std::swap(begin, end); }
	if (selection_range_begin == begin && selection_range_end == end) { return; }
	HideCaret();
	RedrawSelectionRegion();
	selection_range_begin = begin; selection_range_end = end;
	selection_info.clear(); selection_region_union = region_empty;
	if (HasSelection()) {
		selection_info = text_block.HitTestTextRange(begin, end - begin);
		for (auto& it : selection_info) {
			selection_region_union = selection_region_union.Union(it.geometry_region);
		}
	}
	RedrawSelectionRegion();
}

void BlockTextView::SelectWord() {
	if (caret_position >= text.length()) { return; }
	TextRange word_range = word_break_iterator.Seek(caret_position);
	UpdateSelectionRegion(word_range.left(), word_range.right());
}

void BlockTextView::SelectAll() {
	if (selection_range_begin == 0 && selection_range_end == text.length()) { return block_view.SelectAll(); }
	UpdateSelectionRegion(0, text.length());
}

void BlockTextView::DoSelect(Point point) {
	SetCaret(text_block.HitTestPoint(point));
	UpdateSelectionRegion(selection_begin, caret_position);
}

void BlockTextView::Insert(wchar ch) {
	if (HasSelection()) {
		ReplaceText(selection_range_begin, selection_range_end - selection_range_begin, ch);
		SetCaret(selection_range_begin + 1);
	} else {
		InsertText(caret_position, ch);
		SetCaret(caret_position + 1);
	}
}

void BlockTextView::Insert(std::wstring str) {
	if (HasSelection()) {
		ReplaceText(selection_range_begin, selection_range_end - selection_range_begin, str);
		SetCaret(selection_range_begin + str.length());
	} else {
		InsertText(caret_position, str);
		SetCaret(caret_position + str.length());
	}
}

void BlockTextView::Delete(bool is_backspace) {
	if (HasSelection()) {
		DeleteText(selection_range_begin, selection_range_end - selection_range_begin);
		SetCaret(selection_range_begin);
	} else {
		if (is_backspace) {
			if (caret_position == 0) { return; }
			size_t previous_caret_position = caret_position;
			SetCaret(caret_position - 1);
			DeleteText(caret_position, previous_caret_position - caret_position);
		} else {
			if (caret_position >= text.length()) { return; }
			DeleteText(caret_position, GetCharacterLength(caret_position));
		}
	}
}

void BlockTextView::OnImeBegin() {
	Point ime_position;
	if (HasSelection()) {
		ime_composition_begin = selection_range_begin;
		ime_composition_end = selection_range_end;
		ime_position = selection_info.front().geometry_region.RightBottom();
	} else {
		ime_composition_begin = caret_position;
		ime_composition_end = ime_composition_begin;
		ime_position = caret_region.RightBottom();
	}
	ime.SetPosition(*this, ime_position);
}

void BlockTextView::OnImeString() {
	std::wstring str = ime.GetString();
	ReplaceText(ime_composition_begin, ime_composition_end - ime_composition_begin, str);
	ime_composition_end = ime_composition_begin + str.length();
	SetCaret(ime_composition_begin + ime.GetCursorPosition());
}

void BlockTextView::OnImeEnd() {
	if (caret_position != ime_composition_end) { SetCaret(ime_composition_end); }
}

void BlockTextView::Cut() {
	if (HasSelection()) {
		Copy();
		Delete(false);
	}
}

void BlockTextView::Copy() {
	if (HasSelection()) {
		SetClipboardData(text.substr(selection_range_begin, selection_range_end - selection_range_begin));
	}
}

void BlockTextView::Paste() {
	std::wstring str; GetClipboardData(str);
	std::vector<std::wstring> text = split_string_filtered(str);
	if (text.size() == 1) {
		Insert(text.front());
	} else {
		if (HasSelection()) {
			str = this->text.substr(selection_range_end);
			ReplaceText(selection_range_begin, -1, text.front());
		} else {
			str = this->text.substr(caret_position);
			ReplaceText(caret_position, -1, text.front());
		}
		text.erase(text.begin()); text.back().insert(0, str);
		block_view.InsertAfterSelf(text, str.size());
	}
}

void BlockTextView::OnMouseMsg(MouseMsg msg) {
	switch (mouse_tracker.Track(msg)) {
	case MouseTrackMsg::LeftDown: SetCaret(msg.point); break;
	case MouseTrackMsg::LeftDrag: mouse_tracker.is_mouse_down = false; block_view.BeginSelect(); HideCaret(); break;
	case MouseTrackMsg::LeftDoubleClick: SelectWord(); break;
	case MouseTrackMsg::LeftTripleClick: SelectAll(); break;
	}
}

void BlockTextView::OnKeyMsg(KeyMsg msg) {
	switch (msg.type) {
	case KeyMsg::KeyDown:
		switch (msg.key) {
		case Key::Left: MoveCaretLeft(); break;
		case Key::Right: MoveCaretRight(); break;
		case Key::Up: MoveCaretUp(); break;
		case Key::Down: MoveCaretDown(); break;
		case Key::Home: MoveCaretHome(); break;
		case Key::End: MoveCaretEnd(); break;

		case Key::Enter: block_view.InsertNewFront(); break;

		case Key::Backspace: Delete(true); break;
		case Key::Delete: Delete(false); break;

		case Key::Ctrl: is_ctrl_down = true; break;

		case CharKey('A'): if (is_ctrl_down) { SelectAll(); } break;
		case CharKey('X'): if (is_ctrl_down) { Cut(); } break;
		case CharKey('C'): if (is_ctrl_down) { Copy(); } break;
		case CharKey('V'): if (is_ctrl_down) { Paste(); } break;
		}
		break;
	case KeyMsg::KeyUp:
		switch (msg.key) {
		case Key::Ctrl: is_ctrl_down = false; break;
		}
		break;
	case KeyMsg::Char:
		if (is_ctrl_down) { break; }
		if (!iswcntrl(msg.ch)) { Insert(msg.ch); };
		break;
	case KeyMsg::ImeBegin: OnImeBegin(); break;
	case KeyMsg::ImeString: OnImeString(); break;
	case KeyMsg::ImeEnd: OnImeEnd(); break;
	}
	ActivateCaret();
}

void BlockTextView::OnNotifyMsg(NotifyMsg msg) {
	switch (msg) {
	case NotifyMsg::MouseLeave: mouse_tracker.is_mouse_down = false; break;
	case NotifyMsg::LoseFocus: block_view.ClearSelection(); HideCaret(); break;
	}
}


END_NAMESPACE(WndDesign)