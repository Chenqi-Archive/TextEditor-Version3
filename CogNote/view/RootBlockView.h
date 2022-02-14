#pragma once

#include "BlockView.h"


BEGIN_NAMESPACE(WndDesign)


class RootBlockView : public BlockView {
private:
	friend class BlockView;

public:
	RootBlockView();

	// selection
private:
	void BeginSelect();
	void DoSelect(Point point);

	// message
private:
	virtual void OnMouseMsg(MouseMsg msg) override;
	virtual void OnNotifyMsg(NotifyMsg msg) override;
};


END_NAMESPACE(WndDesign)