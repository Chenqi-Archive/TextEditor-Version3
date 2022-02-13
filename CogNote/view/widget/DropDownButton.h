#pragma once

#include "WndDesign/control/Placeholder.h"
#include "WndDesign/figure/shape.h"


BEGIN_NAMESPACE(WndDesign)


class DropDownButton : public Placeholder<Auto, Assigned> {
private:
	friend class DropDownFrame;

private:
	DropDownButton() : Placeholder(30.0f) {}

	// state
private:
	bool is_hidden = true;
	bool is_mouse_over = false;
	bool is_mouse_down = false;

	// paint
private:
	WndObject::Redraw;
private:
	virtual void OnDraw(FigureQueue& figure_queue, Rect draw_region) override {
		if (!is_mouse_over) { return; }
		figure_queue.Offset(Rect(point_zero, size).Center() - point_zero, [&]() {
			if (is_hidden) {
				figure_queue.add(Point(3.0f, 0.0f), new Line(Vector(-6.0f, -6.0f), 1.0f, Color::Gray));
				figure_queue.add(Point(3.0f, 0.0f), new Line(Vector(-6.0f, 6.0f), 1.0f, Color::Gray));
			} else {
				figure_queue.add(Point(0.0f, 3.0f), new Line(Vector(-6.0f, -6.0f), 1.0f, Color::Gray));
				figure_queue.add(Point(0.0f, 3.0f), new Line(Vector(6.0f, -6.0f), 1.0f, Color::Gray));
			}
		});
	}

	// message
private:
	virtual void OnMouseMsg(MouseMsg msg) override {
		switch (msg.type) {
		case MouseMsg::LeftDown: is_mouse_down = true; break;
		case MouseMsg::LeftUp: if (is_mouse_down == true) { is_mouse_down = false; is_hidden = !is_hidden; Redraw(region_infinite); } break;
		}
	}
	virtual void OnNotifyMsg(NotifyMsg msg) override {
		switch (msg) {
		case NotifyMsg::MouseLeave: is_mouse_down = false; break;
		}
	}
};


END_NAMESPACE(WndDesign)