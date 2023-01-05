#include <string>

#include "GoldPlane.h"

void GoldPlane::handle(const GoldChangedEvent& event) {
	goldLabel_.setText(std::to_string(event.gold_));
}
