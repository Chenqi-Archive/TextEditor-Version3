#include "BlockTextView.h"
#include "BlockView.h"

#include "WndDesign/message/timer.h"
#include "WndDesign/figure/shape.h"


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

// message
bool is_mouse_down = false;

END_NAMESPACE(Anonymous)


BlockTextView::BlockTextView(BlockView& block_view) : text(L"Input"), text_block(style, text), block_view(block_view) {
	cursor = Cursor::Text;
}

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

void BlockTextView::ActivateCaret() {
	if (caret_state != CaretState::Hide) {
		if (!caret_timer.IsSet()) {
			caret_timer.Set(caret_blink_period);
		}
		caret_blink_time = 0;
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
	SetCaretFocus(Rect(point, size));
}

void BlockTextView::SetCaret(Point point) {
	SetCaret(text_block.HitTestPoint(point));
	selection_begin = caret_position;
}

void BlockTextView::RedrawSelectionRegion() {
	caret_focus->Redraw(selection_region_union);
}

void BlockTextView::UpdateSelectionRegion() {
	selection_info = HasSelection() ?
		text_block.HitTestTextRange(selection_range_begin, selection_range_end - selection_range_begin) :
		std::vector<HitTestInfo>();
	RedrawSelectionRegion();
	selection_region_union = region_empty;
	for (auto& it : selection_info) {
		selection_region_union = selection_region_union.Union(it.geometry_region);
	}
	RedrawSelectionRegion();
}

void BlockTextView::DoSelect(Point point) {
	SetFocus();
	SetCaret(text_block.HitTestPoint(point));
	selection_range_begin = selection_begin;
	selection_range_end = caret_position;
	if (selection_range_end < selection_range_begin) { std::swap(selection_range_begin, selection_range_end); }
	UpdateSelectionRegion();
}

void BlockTextView::OnMouseMsg(MouseMsg msg) {
	switch (msg.type) {
	case MouseMsg::LeftDown: is_mouse_down = true; SetCaret(msg.point); ShowCaret(); break;
	case MouseMsg::Move: if (is_mouse_down) { is_mouse_down = false; block_view.BeginSelect(); HideCaret(); } break;
	case MouseMsg::LeftUp: if (is_mouse_down) { is_mouse_down = false; SetFocus(); ShowCaret(); } break;
	}
}

void BlockTextView::OnKeyMsg(KeyMsg msg) {

}

void BlockTextView::OnNotifyMsg(NotifyMsg msg) {
	switch (msg) {
	case NotifyMsg::MouseLeave: is_mouse_down = false; break;
	case NotifyMsg::LoseFocus: HideCaret(); break;
	}
}


END_NAMESPACE(WndDesign)