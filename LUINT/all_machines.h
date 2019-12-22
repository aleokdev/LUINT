#pragma once
#include "machine.h"
#include "button.h"
#include "keyboard.h"
#include "LED.h"
#include "monitor.h"
#include "processing_unit.h"

#include "machine_list.h"

#define MACHINE_TYPES LUINT::Machines::Button, LUINT::Machines::Keyboard, LUINT::Machines::LED, LUINT::Machines::Monitor, LUINT::Machines::ProcessingUnit

namespace LUINT::Machines
{
	inline LUINT::Machines::List::MachineList<MACHINE_TYPES> allMachineTypes;
}