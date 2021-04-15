#include "GoldPlane.h"

#include <string>

void GoldPlane::handle(const GoldChangedEvent& event) {
	goldLabel_.setText(std::to_string(event.gold_));
}
