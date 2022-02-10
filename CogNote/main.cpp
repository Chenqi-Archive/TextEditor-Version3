#include "view/widget/TextInfoView.h"
#include "view/widget/ListViewFixed.h"
#include "view/widget/SplitView.h"

#include "WndDesign/window/Global.h"
#include "WndDesign/widget/TitleBarFrame.h"
#include "WndDesign/widget/ScrollBox.h"
#include "WndDesign/frame/PaddingFrame.h"


using namespace WndDesign;


struct MainFrameStyle : public TitleBarFrame::Style {
	MainFrameStyle() {
		width.normal(800px).max(100pct);
		height.normal(500px).max(100pct);
		position.setHorizontalCenter().setVerticalCenter();
		border.width(4px).color(Color::CadetBlue);
		title.assign(L"Test");
		title_format.font.size(20px);
	}
};


class MyListView : public ListViewFixed {
public:
	MyListView() : ListViewFixed(30px, 10px) {}

private:
	class MyTextInfoView : public TextInfoView {
	public:
		MyTextInfoView(const TextInfoView::Style& style, size_t number) : TextInfoView(style) {
			UpdateText(L"TextInfoView NO: " + std::to_wstring(number) + L" inserted!");
		}
	private:
		MyListView& GetParent() { return static_cast<MyListView&>(WndObject::GetParent()); }
	private:
		Size size;
	private:
		virtual Size OnSizeRefUpdate(Size size_ref) override { return size = size_ref; }
	private:
		virtual void OnDraw(FigureQueue& figure_queue, Rect draw_region) override {
			TextInfoView::OnDraw(figure_queue, draw_region);
			figure_queue.add(point_zero, new Rectangle(size, 1.0px, Color::Magenta));
		}
	private:
		virtual void OnMouseMsg(MouseMsg msg) override {
			if (msg.type == MouseMsg::LeftDown) { GetParent().EraseChild(*this); }
		}
	};

private:
	struct Style : public TextInfoView::Style {
		Style() {
			font.family(L"Segoe UI", L"DengXian").size(20).color(Color::Moccasin);
		}
	}style;

private:
	size_t number = 0;

private:
	void InsertChild(size_t index) { ListViewFixed::InsertChild(index, new MyTextInfoView(style, number++)); }
	void EraseChild(TextInfoView& child) { ListViewFixed::EraseChild(GetChildData(child), 1); }

private:
	virtual void OnMouseMsg(MouseMsg msg) override {
		if (msg.type == MouseMsg::LeftDown) { InsertChild(msg.point.y < 0 ? 0 : HitTestItem(msg.point.y) + 1); }
	}
};


int main() {
	global.AddWnd(
		new TitleBarFrame{
			MainFrameStyle(),
			new SplitView<Horizontal>{
				SplitLine(10px, Color::Aquamarine, 200px),
				new SplitView<Vertical>{
					SplitLine(5px, Color::BurlyWood, 30pct),
					new ScrollBox{
						new MyListView()
					},
					new ScrollBox{
						new MyListView()
					}
				},
				new ScrollBox{
					new PaddingFrame{
						Padding(50px, 30px),
						new MyListView()
					}
				}
			}
		}
	);
	global.MessageLoop();
}