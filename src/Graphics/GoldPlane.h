#pragma once

#include <Magnum/Ui/Plane.h>
#include <Magnum/Ui/Label.h>
#include <Magnum/Ui/Anchor.h>
#include <Magnum/Text/Alignment.h>
#include <Magnum/Ui/BasicUserInterface.h>

#include "Events/Events.h"

using namespace Magnum;

struct GoldPlane : Ui::Plane, EventHandler<GoldChangedEvent> {
	explicit GoldPlane(Ui::UserInterface& ui)
		:
		Plane(ui, Ui::Snap::Right | Ui::Snap::Bottom, 0, 50, 10),
	goldLabel_(*this, Ui::Snap::Right | Ui::Snap::Bottom, "Gold!", Text::Alignment::LineCenterIntegral, Ui::Style::Warning) {

	}

	void handle(const GoldChangedEvent& event) override;
	Magnum::Ui::Label goldLabel_;
};
