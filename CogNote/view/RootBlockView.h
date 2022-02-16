#pragma once

#include "BlockView.h"


BEGIN_NAMESPACE(WndDesign)


class RootBlockView : public BlockView {
public:
	RootBlockView();

	// selection
public:
	void BeginSelect();
private:
	void DoSelect(Point point);

	// message
private:
	virtual void OnMouseMsg(MouseMsg msg) override;
	virtual void OnNotifyMsg(NotifyMsg msg) override;
};


END_NAMESPACE(WndDesign)