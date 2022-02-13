#pragma once

#include "WndDesign/frame/WndFrame.h"


BEGIN_NAMESPACE(WndDesign)


class RootFrame : public WndFrame, public LayoutType<Assigned, Auto> {
public:
	RootFrame();
};


END_NAMESPACE(WndDesign)